/* $Id: Hook.h,v 1.4 2003/01/25 22:00:40 cc Exp $ */

#ifndef __HOOK_H__
#define __HOOK_H__

#include <winuser.h>

#define MSGS_SIZE	200
#define WM_KICKIDLE 0x036A

#define MUTEX_NAME	"LMTHookMemFileMutex"
#define MAP_NAME	"LMTHookMemFileMap"

struct MsgData
{
	HWND	hWnd;
	UINT	id;
    WPARAM  wParam;
    LPARAM  lParam;
	DWORD	time;
	POINT	caretPos;
};
typedef struct MsgData *PMsgData;

struct MappedMsgFile
{
	HWND hMainWnd;
	BOOL lmtRunning;
	BOOL jammed;

	// returned by SetWindowsHookEx
	HHOOK hHookGetMsg;
	HHOOK hHookCallWnd;

	HMODULE lmtModule;

	UINT msgsLow, msgsHigh;
	struct MsgData msgs[MSGS_SIZE];
};
typedef struct MappedMsgFile *PMappedMsgFile;

LRESULT CALLBACK LMTGetMsgProc(INT hc, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LMTCallWndProc(INT hc, WPARAM wParam, LPARAM lParam);

// Functions defined in HookApp.cpp
BOOL SetSpyHook(BOOL fSet);

#endif
