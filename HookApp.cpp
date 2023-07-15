// Contains the message hooking functions for LetMeType
// $Id: HookApp.cpp,v 1.1.1.1 2002/05/12 14:40:55 cc Exp $

#include "stdafx.h"
#include "AutoType.h"

#include "windows.h"

#include "hook.h"
#include <fstream.h>

typedef VOID (*PREFIXPROC)(char *);

/*****************************************************************************\
* SetSpyHook
*
* Sets the windows hooks used to trap the messages.  After this
* is called with a TRUE for fSet, the messages will start flowing
* through the hook DLL.
*
* Arguments:
*   BOOL fSet - TRUE to hook, FALSE to unhook.
*
* Returns:
*   TRUE if successful.
*
\*****************************************************************************/

BOOL SetSpyHook(BOOL fSet)
{
    HHOOK &hhkGetMessage = theApp.msgFile->hHookGetMsg;
    HHOOK &hhkCallWndProc = theApp.msgFile->hHookCallWnd;

    if (fSet)
    {
        if (!theApp.hmodHook)
            if (!(theApp.hmodHook = LoadLibrary("lmthook.dll")))
            {
				CString msg;
				msg.LoadString(IDS_DLLERR);
				AfxMessageBox(LPCTSTR(msg), MB_OK | MB_ICONEXCLAMATION);
                return FALSE;
            }

        if (!hhkGetMessage)
        {
            if (!(hhkGetMessage = SetWindowsHookEx(WH_GETMESSAGE,
                HOOKPROC(GetProcAddress(theApp.hmodHook, "LMTGetMsgProc")), 
                theApp.hmodHook, 0)))
                return FALSE;
        }

		if (!hhkCallWndProc)
        {
            if (!(hhkCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC,
                HOOKPROC(GetProcAddress(theApp.hmodHook, "LMTCallWndProc")), 
                theApp.hmodHook, 0)))
			{
	            UnhookWindowsHookEx(hhkGetMessage);
    	        hhkGetMessage = NULL;
                return FALSE;
			}
        }
    }
    else
    {
        if (hhkGetMessage) {
            UnhookWindowsHookEx(hhkGetMessage);
            hhkGetMessage = NULL;
        }
        if (hhkCallWndProc) {
            UnhookWindowsHookEx(hhkCallWndProc);
            hhkCallWndProc = NULL;
        }
    }

    return TRUE;
}
