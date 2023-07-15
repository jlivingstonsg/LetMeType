// AutoType.h : main header file for LetMeType
// $Id: AutoType.h,v 1.3 2003/01/25 22:07:53 cc Exp $
//

#ifndef __AUTOTYPE_H__
#define __AUTOTYPE_H__

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include <fstream.h>
#include "hook.h"

typedef DWORD (WINAPI *GetModuleFileNameExProc)(HANDLE, HMODULE, LPTSTR, DWORD);

const short MAX_WORD_LEN = 40;
const short DB_MAXSIZE_UNREG = 100;
const short DB_MAXSIZE = 20000;
const short DB_MAXAGE_UNREG = 14;
const short DB_MAXAGE = 720;

class CSuggestionsDlg;


struct Taboo
{
	WORD hash;
	CString description;
};
BOOL AFXAPI CompareElements(const Taboo* p1, const Taboo* p2);

#include <afxtempl.h>

class CTabooList : public CList<Taboo, Taboo&>
{
public:
	CTabooList();
	~CTabooList();
	void operator=(const CTabooList&);
	POSITION Find(UINT searchValue, POSITION startAfter = NULL) const;
};


/////////////////////////////////////////////////////////////////////////////
// CAutoTypeApp

class CAutoTypeApp : public CWinApp
{
public:
	CAutoTypeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoTypeApp)
	public:
	virtual BOOL InitApplication();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAutoTypeApp)
	afx_msg void OnAppAbout();
	afx_msg void OnOptionsSuggest();
	afx_msg void OnFileSettings();
	afx_msg void OnOptionsPasswords();
	afx_msg void OnFileChars();
	afx_msg void OnHelpHelp();
	afx_msg void OnOptionsCountkeys();
	afx_msg void OnOptionsBackColor();
	afx_msg void OnOptionsFont();
	afx_msg void OnOptionsClipboard();
	afx_msg void OnOptionsLearn();
	afx_msg void OnOptionsApplications();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void ToggleActivation();
	bool FirstInstance();
	void SaveWndPlace();
	void EmptyMsgBuffer();
	bool GetNextMessage(MsgData &msg);
	virtual BOOL OnIdle(LONG lCount);
	void IncrTypedSelf();
	void IncrTypedAuto(short delta);
	int GetWindowModuleFileName(HWND hwnd, LPTSTR lpszFileName, UINT cchFileNameMax);

	WINDOWPLACEMENT wndPlace;
	CSuggestionsDlg* m_dlgSuggestions;

	PMappedMsgFile msgFile;
	HANDLE hMutexObject;
	char namePrefix[20];

	DWORD wmCharCount;			// total count of received WM_CHAR messages
	DWORD autoTypeCount;		// count of suggested and transmitted chars
	CTabooList taboos;			// words to be ignored by AutoType

	HINSTANCE hmodHook;			// handle of HOOK dll
	HANDLE hMapObject;
	bool m_bSuggest, m_bLearn;
	bool m_bCountKeys;
	CString language;
	short typingDelay;
	bool m_bModalDlg;

	COLORREF foreColor, backColor; // colors for suggestion window
	CString fontName;
	int fontSize;

	bool m_bMonitorClipboard, ignoreClipboard;
	HWND nextClipboardViewer;

	OSVERSIONINFO m_osVersion;
	HINSTANCE hPSAPI;
	GetModuleFileNameExProc pGetModuleFileNameEx;

	// Applications to which suggestions should be limited.
	bool m_bPositiveApplications;
	bool m_bNegativeApplications;
	CStringArray m_positiveApplications;
	CStringArray m_negativeApplications;
};

extern CAutoTypeApp theApp;

#endif
