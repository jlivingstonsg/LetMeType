// $Id: mainfrm.cpp,v 1.4 2003/01/25 22:05:10 cc Exp $
//

#include "stdafx.h"
#include "AutoType.h"
#include "suggestw.h"
#include "mainfrm.h"
#include "AutoTdoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define WM_NOTIFYICON		(WM_APP+100)


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

#define ON_WM_NOTIFYICON() \
	{ WM_NOTIFYICON, 0, 0, 0, AfxSig_vwl, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, long))OnNotifyIcon },

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_APP_RESTORE, OnAppRestore)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_WM_NOTIFYICON()
	ON_WM_DRAWCLIPBOARD()
	ON_WM_CHANGECBCHAIN()
END_MESSAGE_MAP()


// Status line indicators.
static UINT indicators[] =
{
	ID_SEPARATOR
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_iTimer = 0;
	m_bIgnoreTimer = false;
	VERIFY(m_trayMenu.LoadMenu(IDR_POPUP_TRAY));
	m_bTrayMenuShown = false;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;

	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetButtonStyle(6, TBBS_CHECKBOX | TBBS_CHECKGROUP);
	m_wndToolBar.SetButtonStyle(7, TBBS_CHECKBOX | TBBS_CHECKGROUP);
	m_wndToolBar.GetToolBarCtrl().CheckButton(7);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	static bool first = true;
	
	if (first)
	{
		first = false;
		theApp.wndPlace.showCmd = theApp.m_nCmdShow = nCmdShow;

		if (theApp.wndPlace.rcNormalPosition.left == -1) {
			int w = GetSystemMetrics(SM_CXSCREEN);
			int h = GetSystemMetrics(SM_CYSCREEN);
			theApp.wndPlace.rcNormalPosition.left = w-310;
			theApp.wndPlace.rcNormalPosition.right = w-10;
			theApp.wndPlace.rcNormalPosition.top = h-400;
			theApp.wndPlace.rcNormalPosition.bottom = h-50;
		}
		SetWindowPlacement(&theApp.wndPlace);

		TrayMessage(NIM_ADD); 
	}

	if (nCmdShow == SW_MINIMIZE || nCmdShow == SW_SHOWMINNOACTIVE) 
		nCmdShow = SW_HIDE;

	if (!theApp.m_dlgSuggestions)
		theApp.m_dlgSuggestions = new CSuggestionsDlg(GetDesktopWindow());
	CFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_F1)	theApp.WinHelp(0, HELP_CONTENTS);
	else CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.lpszClass = "LetMeType";
	return CFrameWnd::PreCreateWindow(cs);
}

// Display tray icon and tool tip.
BOOL CMainFrame::TrayMessage(DWORD dwMessage)
{
    BOOL result;
	int nIcon, nStatus;

	if (!theApp.m_bLearn && !theApp.m_bSuggest) {
		nIcon = IDI_DEACTIVATED;
		nStatus = IDS_DEACTIVATED;
	}
	else if (!theApp.m_bLearn) {
		nIcon = IDI_NO_LEARN;
		nStatus = IDS_NO_LEARN;
	}
	else if (!theApp.m_bSuggest) {
		nIcon = IDI_NO_SUGGEST;
		nStatus = IDS_NO_SUGGEST;
	}
	else
		nIcon = IDR_MAINFRAME;

	HICON hIcon = HICON(LoadImage(theApp.m_hInstance, MAKEINTRESOURCE(nIcon), IMAGE_ICON, 16, 16, 0));

	NOTIFYICONDATA tnd;
	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = m_hWnd;
	tnd.uID	= 0;

	tnd.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
	tnd.uCallbackMessage = WM_NOTIFYICON;
	tnd.hIcon = hIcon;

	if (theApp.m_bLearn && theApp.m_bSuggest) 
		strcpy(tnd.szTip, "LetMeType");
	else 
	{
		CString s, s2;
		s2.LoadString(nStatus);
		s.Format("LetMeType (%s)", s2);
		strcpy(tnd.szTip, s);
	}

	result = Shell_NotifyIcon(dwMessage, &tnd);

	if (hIcon) DestroyIcon(hIcon);

	return result;
}

// Hide taskbar entry and add tray icon when main window gets minimized;
// remove tray icon when main window is restored.
void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	if (nType == SIZE_MINIMIZED)
		ShowWindow(SW_HIDE);
	else
		CFrameWnd::OnSize(nType, cx, cy);
}

// Restore window or display pop-up menu if tray icon gets clicked.
void CMainFrame::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
		case WM_LBUTTONUP:
			if (!m_iTimer) m_iTimer = SetTimer(2, GetDoubleClickTime()+10, NULL);
			else m_bIgnoreTimer = true;
			break;

		case WM_LBUTTONDBLCLK:
			ShowWindow(SW_RESTORE);
			SetForegroundWindow();
			break;

		case WM_RBUTTONDOWN:
			if (!theApp.m_bModalDlg)
			{
				SetForegroundWindow();

				POINT p;
				GetCursorPos(&p);

				CMenu* popup = m_trayMenu.GetSubMenu(0);
				if (!popup) { ASSERT(0); return; }

				popup->CheckMenuItem(ID_OPTIONS_SUGGEST, theApp.m_bSuggest ? MF_CHECKED : MF_UNCHECKED);
				popup->CheckMenuItem(ID_OPTIONS_LEARN, theApp.m_bLearn ? MF_CHECKED : MF_UNCHECKED);

				popup->TrackPopupMenu(TPM_RIGHTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, p.x, p.y, this);
			}
	}
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent) 
	{
		case 1: 
			theDoc->OnTimer();
			break;

		case 2:
			KillTimer(m_iTimer); m_iTimer = 0;
			if (!m_bIgnoreTimer) theApp.ToggleActivation();
			else m_bIgnoreTimer = false;
	}

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	theDoc->StopSuggesting();
}

void CMainFrame::OnChangeCbChain(HWND hWndRemove, HWND hWndAfter)
{
	if (hWndRemove == theApp.nextClipboardViewer)
		theApp.nextClipboardViewer = hWndAfter;
	else if (theApp.nextClipboardViewer && IsWindow(theApp.nextClipboardViewer))
		::SendMessage(theApp.nextClipboardViewer, WM_CHANGECBCHAIN, WPARAM(hWndRemove), LPARAM(hWndAfter));
}

void CMainFrame::OnDrawClipboard()
{
	if (theApp.m_bLearn && !theApp.ignoreClipboard && IsClipboardFormatAvailable(CF_OEMTEXT))
		theDoc->Paste(false, true);

	if (theApp.nextClipboardViewer && IsWindow(theApp.nextClipboardViewer))
		::SendMessage(theApp.nextClipboardViewer, WM_DRAWCLIPBOARD, 0, 0);
}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	ChangeClipboardChain(theApp.nextClipboardViewer);
	theApp.nextClipboardViewer = NULL;
	TrayMessage(NIM_DELETE); 
}

void CMainFrame::OnAppRestore() 
{
	ShowWindow(SW_RESTORE);
	SetForegroundWindow();
}

// Hide window instead of closing the application.
afx_msg void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if ((nID & 0xFFF0) == SC_CLOSE)
		ShowWindow(SW_HIDE);
	else 
		CFrameWnd::OnSysCommand(nID, lParam);
}

void CMainFrame::OnClose() 
{
	if (m_iTimer) { KillTimer(m_iTimer); m_iTimer = 0; }
	CFrameWnd::OnClose();
}
