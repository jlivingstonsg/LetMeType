// $Id: Hook.c,v 1.4 2003/01/25 22:00:15 cc Exp $

// Callback functions which are installed in WH_GETMESSAGE and WH_CALLWNDPROC
// by LetMeType. These run in the process space of the monitored applications.


#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#include "..\hook.h"

PMappedMsgFile msgFile = NULL; /* address of shared memory */
static HANDLE hMutexObject = NULL;
static HANDLE hMapObject = NULL;	  /* handle to file mapping */
static UINT timerId = 0;

void FreeResources()
{
	if (timerId) { KillTimer(NULL, timerId); timerId = 0; }
	if (hMutexObject) {	CloseHandle(hMutexObject); hMutexObject = NULL; }
	if (msgFile) { UnmapViewOfFile(msgFile); msgFile = NULL; }
    if (hMapObject) { CloseHandle(hMapObject); hMapObject = NULL; }
}

// Helper function used by HookProc.
BOOL QueueMessage(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam, DWORD time, BOOL *wasEmpty)
{
	BOOL result;
	struct MsgData *msg = NULL;

	// Get mutex object for shared memory.
	if (!hMutexObject) return FALSE;
	result = WaitForSingleObject(hMutexObject, 100);
	if (result == WAIT_TIMEOUT || result == WAIT_FAILED)
		return FALSE;

	msg = &msgFile->msgs[msgFile->msgsHigh];
	msg->hWnd = hWnd;
	msg->id = uiMessage;
	msg->wParam =
		(uiMessage == WM_LBUTTONDOWN || uiMessage == WM_SETFOCUS) ? 
		wParam : LOBYTE(wParam);
	msg->lParam = lParam;
	msg->time = time ? time : timeGetTime();
	GetCaretPos(&msg->caretPos);

	*wasEmpty = msgFile->msgsLow == msgFile->msgsHigh;

	if (++msgFile->msgsHigh == MSGS_SIZE)		// Wrap msgsHigh if upper bound reached.
		msgFile->msgsHigh = 0;

	if (msgFile->msgsHigh == msgFile->msgsLow)	// Queue full?
		msgFile->jammed = TRUE;

	ReleaseMutex(hMutexObject);
	return TRUE;
}

/* Called every now and then to check if LetMeType has been terminated
   without telling us so. */
VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if (msgFile && !msgFile->lmtRunning)
			FreeResources();
}

/* Called when the libary is loaded */
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason) 
    {
		case DLL_PROCESS_ATTACH:
           	hMapObject = 
           		CreateFileMapping((HANDLE) 0xFFFFFFFF, NULL,PAGE_READWRITE, 0,
           						  sizeof(struct MappedMsgFile), MAP_NAME);
           	if (!hMapObject) return FALSE;

            msgFile = (PMappedMsgFile)MapViewOfFile(hMapObject, FILE_MAP_WRITE, 0, 0, 0);
   	        if (!msgFile) {
				CloseHandle(hMapObject); hMapObject = NULL;
               	return FALSE;
			}

			hMutexObject = OpenMutex(SYNCHRONIZE, FALSE, MUTEX_NAME);
			if (!hMutexObject) {
            	UnmapViewOfFile(msgFile); msgFile = NULL;
    	        CloseHandle(hMapObject); hMapObject = NULL;
				return FALSE;
			}

			timerId = SetTimer(NULL, 0, 10000, (TIMERPROC)TimerProc);
			if (!timerId) {
				CloseHandle(hMutexObject); hMutexObject = NULL;
            	UnmapViewOfFile(msgFile); msgFile = NULL;
    	        CloseHandle(hMapObject); hMapObject = NULL;
				return FALSE;
			}

	       	break;

        case DLL_PROCESS_DETACH:
			FreeResources();
            break;

        default:
          break;
	}

    return TRUE;

    UNREFERENCED_PARAMETER(hinstDLL);
    UNREFERENCED_PARAMETER(lpvReserved);
}



/*****************************************************************************\
* HookProc( hWnd, uiMessage, wParam, lParam )
*
* The hook proc for the windows hook being spied on
*
* Arguments:
*    HWND hWnd - window handle for the parent window
*    UINT uiMessage - message number
*    WPARAM wParam - message-dependent
*    LPARAM lParam - message-dependent
\*****************************************************************************/

void WINAPI HookProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam, DWORD time)
{
	static BOOL menuActive = FALSE;
	BOOL passThrough, wasEmpty;
	BYTE keyboardState[256];
	WORD characters;

	// When we encounter a WM_KEYDOWN for a dead key,
	// we have to store information about it, so we
	// can call ToAscii after the next keystroke,
	// to restore the keyboard driver's internal state.
	static BOOL prevWasDeadKey;
	static WPARAM prevWParam;
	static LPARAM prevLParam;
	static BYTE prevKeyboardState[256];

	// Check if LetMeType has terminated without notifying us.
	if (msgFile && !msgFile->lmtRunning) {
		FreeResources();
		return;
	}

	// msgFile full or not existing?
	if (!msgFile || msgFile->jammed) return;

	// Check if we should ignore this message,
	// or if we have to translate it.
	switch (uiMessage)
	{
		case WM_KEYDOWN:
			passThrough = FALSE;

			// Certain WM_KEYDOWN messages have to be passed through.
			if (wParam >= VK_PRIOR && wParam <= VK_DOWN ||
				wParam >= VK_F1 && wParam <= VK_F24)
				passThrough = TRUE;

			switch (wParam)
			{
				case VK_BACK:
				case VK_TAB:
				case VK_RETURN:
				case VK_UP:
				case VK_DOWN:
				case VK_RIGHT:
				case VK_SHIFT:
				case VK_CONTROL:
				case VK_ESCAPE:
					passThrough = TRUE;
			}

			if (!passThrough) 
			{
				// Normal applications call TranslateMessage,
				// which would give us a WM_CHAR corresponding
				// to this message. But some don't, e.g. 
				// Borland Delphi. So we do the translation.
				
				GetKeyboardState(keyboardState);

				switch (ToAscii(wParam, lParam >> 16, keyboardState, &characters, menuActive))
				{
					case -1:
						// This is a dead character. By calling ToAscii,
						// we altered the internal state of the keyboard
						// driver. Calling it again fixes this. See 
						// Knowledge Base article Q99337 for more information.
						ToAscii(wParam, lParam >> 16, keyboardState, &characters, menuActive);
						prevWasDeadKey = TRUE;
						prevWParam = wParam;
						prevLParam = lParam;
						memcpy(prevKeyboardState, keyboardState, 256);
						return;

					case 1:
						uiMessage = WM_CHAR;
						wParam = characters;

						if (prevWasDeadKey)
							ToAscii(prevWParam, prevLParam >> 16, prevKeyboardState, &characters, menuActive);

						break;

					case 2:
						if (!QueueMessage(hWnd, WM_CHAR, characters & 0xff, lParam, time, &wasEmpty))
							return;

						uiMessage = WM_CHAR;
						wParam = characters >> 8;

						if (prevWasDeadKey)
							ToAscii(prevWParam, prevLParam >> 16, prevKeyboardState, &characters, menuActive);
				}
			}

			prevWasDeadKey = FALSE;
			break;

		case WM_LBUTTONDOWN:
		case WM_SETFOCUS:
			break;

		case WM_ENTERMENULOOP:
			menuActive = TRUE;
			return;

		case WM_EXITMENULOOP:
			menuActive = FALSE;
			return;

		default:
			return;
	}

	// While a menu is active, we skip all messages.
	if (menuActive) return;

	if (!QueueMessage(hWnd, uiMessage, wParam, lParam, time, &wasEmpty))
		return;

	// Notify LMT thread.
	PostMessage(msgFile->hMainWnd, WM_KICKIDLE, 0, 0);
}

// Get Message hook function
LRESULT CALLBACK LMTGetMsgProc(INT hc, WPARAM wParam, LPARAM lParam)
{
    PMSG pmsg;

    pmsg = (PMSG)lParam;

    if (hc >= 0 && pmsg && pmsg->hwnd && wParam == PM_REMOVE)
        HookProc(pmsg->hwnd, pmsg->message, pmsg->wParam, pmsg->lParam, pmsg->time);

	if (msgFile)
		return CallNextHookEx(msgFile->hHookGetMsg, hc, wParam, lParam);
	else return 0;
}


// Call Window Proc (Send Message) hook function
LRESULT CALLBACK LMTCallWndProc(INT hc, WPARAM wParam, LPARAM lParam)
{
    PCWPSTRUCT pcwps;

    pcwps = (PCWPSTRUCT)lParam;

    if (hc >= 0 && pcwps && pcwps->hwnd)
        HookProc(pcwps->hwnd, pcwps->message, pcwps->wParam, pcwps->lParam, 0);

    if (msgFile)
		return CallNextHookEx(msgFile->hHookCallWnd, hc, wParam, lParam);
	else return 0;
}
