// AutoType.cpp : Defines the class behaviors for the application.
// $Id: AutoType.cpp,v 1.4 2003/01/25 22:07:52 cc Exp $
//

#include <fstream.h>
#include "stdafx.h"
#include "AutoType.h"
#include "mainfrm.h"
#include "AutoTdoc.h"
#include "AutoTvw.h"
#include "settings.h"
#include "charsdlg.h"
#include "pwdlg.h"
#include "crc16.h"
#include "ColorDlg.h"
#include "FileVersion.h"
#include "UrlDlg.h"
#include "ApplicationsDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable: 4800)


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg

class CAboutDlg : public CUrlDlg
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_version;
	CString	m_autoTypeCount;
	CString	m_wmCharCount;
	CString	m_Accesses;
	CString	m_dbSize;
	CString	m_Age;
	CString	m_relAuto;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnReset();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CUrlDlg(CAboutDlg::IDD)
{
	CString c;
	c.LoadString(IDS_CHARACTERS);
	char s[40];
	sprintf(s, "%ld %s", theApp.autoTypeCount, c);
	m_autoTypeCount = s;
	sprintf(s, "%ld %s", theApp.wmCharCount, c);
	m_wmCharCount = s;
	if (theApp.autoTypeCount) {
		sprintf(s, "(%ld%%)", theApp.autoTypeCount*100/(theApp.autoTypeCount+theApp.wmCharCount));
		m_relAuto = s;
	}
	else m_relAuto = "";
	c.LoadString(IDS_WORDS);
	sprintf(s, "%d %s", theDoc->GetWordCount(), c);
	m_dbSize = s;

	char decimalPoint[2];
	VERIFY(GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, decimalPoint, sizeof(decimalPoint)));

	float *stats = theDoc->GetAvgStats();
	c.LoadString(IDS_TIMES);
	sprintf(s, "%.1f %s", stats[0], c);
	if (decimalPoint[0] != '.') {
		char *p = strchr(s, '.');
		if (p) *p = decimalPoint[0];
	}
	m_Accesses = s;

	c.LoadString(IDS_DAYS);
	sprintf(s, "%.1f %s", stats[1], c);
	if (decimalPoint[0] != '.') {
		char *p = strchr(s, '.');
		if (p) *p = decimalPoint[0];
	}
	m_Age = s;

	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CUrlDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_VERSION, m_version);
	DDX_Text(pDX, IDC_autoTypeCount, m_autoTypeCount);
	DDX_Text(pDX, IDC_wmCharCount, m_wmCharCount);
	DDX_Text(pDX, IDC_ACCESSES, m_Accesses);
	DDX_Text(pDX, IDC_DBSIZE, m_dbSize);
	DDX_Text(pDX, IDC_AGE, m_Age);
	DDX_Text(pDX, IDC_RELAUTO, m_relAuto);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CUrlDlg)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CAboutDlg::OnReset() 
{
	theApp.wmCharCount = theApp.autoTypeCount = 0;
	CString c;
	c.LoadString(IDS_CHARACTERS);
	char s[40];
	sprintf(s, "%ld %s", 0, c);
	m_autoTypeCount = m_wmCharCount = s;
	m_relAuto = "";
	UpdateData(FALSE);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Read version number from our executable.
	TCHAR strPathName[_MAX_PATH];
	CString s;
	HMODULE hInst = AfxGetInstanceHandle();
	if (GetModuleFileName(hInst, strPathName, _MAX_PATH)) 
	{
		s.LoadString(AFX_IDS_APP_TITLE);
		CFileVersion fileVersion;
		if (fileVersion.Open(strPathName))
			m_version.SetWindowText(s + " " + fileVersion.GetProductVersion());
	}

	s.LoadString(IDS_URL);
	GetDlgItem(IDC_URL)->SetWindowText(s);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAutoTypeApp

BEGIN_MESSAGE_MAP(CAutoTypeApp, CWinApp)
	//{{AFX_MSG_MAP(CAutoTypeApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_OPTIONS_SUGGEST, OnOptionsSuggest)
	ON_COMMAND(ID_FILE_SETTINGS, OnFileSettings)
	ON_COMMAND(ID_OPTIONS_PASSWORDS, OnOptionsPasswords)
	ON_COMMAND(ID_FILE_CHARS, OnFileChars)
	ON_COMMAND(ID_HELP_HELP, OnHelpHelp)
	ON_COMMAND(ID_OPTIONS_COUNTKEYS, OnOptionsCountkeys)
	ON_COMMAND(ID_OPTIONS_BACKCOLOR, OnOptionsBackColor)
	ON_COMMAND(ID_OPTIONS_FONT, OnOptionsFont)
	ON_COMMAND(ID_OPTIONS_CLIPBOARD, OnOptionsClipboard)
	ON_COMMAND(ID_OPTIONS_LEARN, OnOptionsLearn)
	ON_COMMAND(ID_OPTIONS_APPLICATIONS, OnOptionsApplications)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoTypeApp construction

CAutoTypeApp::CAutoTypeApp()
{
	m_bSuggest = true;
	m_bLearn = true;
	m_bCountKeys = true;
	typingDelay = -1;
	m_dlgSuggestions = NULL;
	hmodHook = NULL;
	m_bModalDlg = false;

	m_bMonitorClipboard = true;
	ignoreClipboard = false;
	nextClipboardViewer = NULL;

	hPSAPI = NULL;
	pGetModuleFileNameEx = NULL;
	m_bPositiveApplications = false;
	m_bNegativeApplications = false;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAutoTypeApp and CAutoTypeDoc object

CAutoTypeApp theApp;
CAutoTypeDoc *theDoc = NULL;

/////////////////////////////////////////////////////////////////////////////
// CAutoTypeApp initialization

BOOL CAutoTypeApp::InitApplication()
{
	CWinApp::InitApplication();

	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = &AfxWndProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = LoadIcon(IDR_MAINFRAME);
	wndcls.hCursor = LoadStandardCursor(IDC_ARROW);
	CBrush br(GetSysColor(COLOR_WINDOW));
	wndcls.hbrBackground = HBRUSH(br);
	wndcls.lpszClassName = "LetMeType";
	
	if (!::RegisterClass(&wndcls)) return FALSE;

	if (!FirstInstance()) return FALSE;

	bool failure = false;
	hMapObject = 
		CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0,  
   						  sizeof(MappedMsgFile), MAP_NAME);
	if (!hMapObject) return FALSE;

	if (!failure)
	{
	   	msgFile = PMappedMsgFile(MapViewOfFile(hMapObject, FILE_MAP_WRITE, 0, 0, 0));
		if (!msgFile) {
			CloseHandle(hMapObject);
			return FALSE;
		}
	}

	if (!failure)
	{
		msgFile->msgsLow = msgFile->msgsHigh = 0;
		msgFile->hMainWnd = NULL;
		msgFile->hHookGetMsg = msgFile->hHookCallWnd = NULL;
		msgFile->lmtRunning = TRUE;

		msgFile->lmtModule = GetModuleHandle(NULL);

		hMutexObject = CreateMutex(NULL, FALSE, MUTEX_NAME);
		if (!hMutexObject)
		{
			UnmapViewOfFile(msgFile);
			CloseHandle(hMapObject);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CAutoTypeApp::InitInstance()
{
	CString s;

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	CoInitialize(NULL);

	language.LoadString(IDS_LANGUAGE);

	SetRegistryKey("ccl Software");
	LoadStdProfileSettings();

	// Retrieve list of taboo words from the registry.
	Taboo t;
	int i = 0;
	char entry[10];
	do
	{
		entry[0] = 'h';
		itoa(i++, entry+1, 10);
		t.hash = GetProfileInt("Taboos", entry, 0);
		if (t.hash) 
		{
			entry[0] = 'd';
			t.description = GetProfileString("Taboos", entry, "");
			taboos.AddTail(t);
		}
	} while (t.hash);

	// Retrieve lists of positive and negative applications.
	i = 0;
	m_positiveApplications.RemoveAll();
	do
	{
		itoa(i++, entry, 10);
		s = GetProfileString("PositiveApplications", entry, "");
		if (!s.IsEmpty())
			m_positiveApplications.Add(s);
	} while (!s.IsEmpty());

	i = 0;
	m_negativeApplications.RemoveAll();
	do
	{
		itoa(i++, entry, 10);
		s = GetProfileString("NegativeApplications", entry, "");
		if (!s.IsEmpty())
			m_negativeApplications.Add(s);
	} while (!s.IsEmpty());

	wmCharCount = atol(GetProfileString("Statistics", "wmCharCount", "0"));
	autoTypeCount = atol(GetProfileString("Statistics", "autoTypeCount", "0"));

	// Retrieve suggestion window settings.
	wndPlace.rcNormalPosition.left = GetProfileInt("Settings", "normalLeft", -1);
	wndPlace.rcNormalPosition.right = GetProfileInt("Settings", "normalRight", -1);
	wndPlace.rcNormalPosition.top = GetProfileInt("Settings", "normalTop", -1);
	wndPlace.rcNormalPosition.bottom = GetProfileInt("Settings", "normalBottom", -1);

	s = GetProfileString("Settings", "foreColor", "");
	if (s.IsEmpty()) foreColor = RGB(0, 0, 0);
	else sscanf(s, "%ld", &foreColor);

	s = GetProfileString("Settings", "backColor", "");
	if (s.IsEmpty()) backColor = RGB(255, 255, 255);
	else sscanf(s, "%ld", &backColor);

	fontName = GetProfileString("Settings", "fontName", "");
	fontSize = GetProfileInt("Settings", "fontSize", 0);

	m_bSuggest = bool(GetProfileInt("Settings", "suggest", true));
	m_bLearn = bool(GetProfileInt("Settings", "learn", true));
	m_bCountKeys = bool(GetProfileInt("Settings", "countKeys", true));
	m_bMonitorClipboard = bool(GetProfileInt("Settings", "monitorClipboard", true));
	m_bPositiveApplications = bool(GetProfileInt("Settings", "positiveApplications", false));
	m_bNegativeApplications = bool(GetProfileInt("Settings", "negativeApplications", false));

	// Parse command line
	char *fname = 0;
	for (i = 0; m_lpCmdLine[i];)
	{
		for (; m_lpCmdLine[i] && m_lpCmdLine[i] == ' '; i++);
		if (m_lpCmdLine[i] == '"') i++;
		if (isalnum(m_lpCmdLine[i]))
		{
			for (int j = i+1; m_lpCmdLine[j] && m_lpCmdLine[j] != ' ' && 
							  m_lpCmdLine[j] != '"'; j++);
			fname = (char *)malloc(j-i+1);
			ASSERT(fname);
			strncpy(fname, m_lpCmdLine+i, j-i);
			fname[j-i] = 0;
		}
		else if (m_lpCmdLine[i] == '/')
		{
			if (!strncmp(m_lpCmdLine+1, "delay=", 6))
				typingDelay = atoi(m_lpCmdLine+7);
		}
		for (; m_lpCmdLine[i] && m_lpCmdLine[i] != ' '; i++);
	}

	// Register the application's document templates. Document templates
	// serve as the connection between documents, frame windows and views.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CAutoTypeDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CAutoTypeView));
	AddDocTemplate(pDocTemplate);

	if (!fname)	OnFileNew();			// Create a new (empty) document
	else {
		theDoc = (CAutoTypeDoc*)OpenDocumentFile(fname);
		free(fname);
		fname = NULL;
		if (!theDoc) OnFileNew();
	}

	EnableShellOpen();

	if (!m_pMainWnd) { ASSERT(0); return FALSE; }

	// Install some stuff and let the user decide what to do
	// in case of failure
	bool success;
	msgFile->hMainWnd = m_pMainWnd->m_hWnd;
	theDoc->timerId = m_pMainWnd->SetTimer(1, TIMER_SPAN, NULL);
	do
	{
		success = true;
		if (!SetSpyHook(TRUE))
		{
			int result = AfxMessageBox(IDS_SPY_HOOK_NOT_SET, 
				MB_ICONSTOP | MB_ABORTRETRYIGNORE);

			if (result == IDABORT) return FALSE;
			else if (result == IDRETRY) success = false;
		}
	} while (!success);
	((CMainFrame*)m_pMainWnd)->TrayMessage(NIM_MODIFY);

	if (!m_bSuggest)
		m_pMainWnd->GetMenu()->CheckMenuItem(ID_OPTIONS_SUGGEST, MF_UNCHECKED);
	if (!m_bLearn)
		m_pMainWnd->GetMenu()->CheckMenuItem(ID_OPTIONS_LEARN, MF_UNCHECKED);
	if (!m_bCountKeys)
		m_pMainWnd->GetMenu()->CheckMenuItem(ID_OPTIONS_COUNTKEYS, MF_UNCHECKED);

	if (m_bMonitorClipboard) {
		ignoreClipboard = true;
		nextClipboardViewer = m_pMainWnd->SetClipboardViewer();
		ignoreClipboard = false;
	}
	else
		m_pMainWnd->GetMenu()->CheckMenuItem(ID_OPTIONS_CLIPBOARD, MF_UNCHECKED);

	// If we are running under WinNT 4.0, or Windows 2000 and later,
	// we have to load psapi.dll and get a pointer to GetModuleFileNameEx.
	m_osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&m_osVersion);
	if (m_osVersion.dwPlatformId == VER_PLATFORM_WIN32_NT && 
		m_osVersion.dwMajorVersion >= 4)
	{
		hPSAPI = LoadLibrary("psapi.dll");
		if (hPSAPI)
			pGetModuleFileNameEx = (GetModuleFileNameExProc)GetProcAddress(hPSAPI, "GetModuleFileNameExA");
	}

	return TRUE;
}

bool CAutoTypeApp::FirstInstance()
{
  
	// Determine if another window with our class name exists...
	CWnd *prevWnd = CWnd::FindWindow("LetMeType", NULL);
	if (prevWnd)
	{
		// if so, does it have any popups?
		CWnd *childWnd = prevWnd->GetLastActivePopup(); 

		// If iconic, restore the main window
		if (prevWnd->IsIconic()) prevWnd->ShowWindow(SW_RESTORE);      

		// Bring the main window to the top
		prevWnd->SetForegroundWindow();

		// If there was an active popup, bring it along too!
		if (prevWnd != childWnd && IsWindow(childWnd->m_hWnd))
			childWnd->SetForegroundWindow(); 

		return false;
	}
	else return true;  // First instance. Proceed as normal.
}

void CAutoTypeApp::IncrTypedSelf()
{
	if (m_bCountKeys) wmCharCount++;
}

void CAutoTypeApp::IncrTypedAuto(short delta)
{
	if (m_bCountKeys) autoTypeCount += delta;
}

static bool SetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL)
{
	if (!lpszValueName)
	{
		if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
			  lpszValue, lstrlen(lpszValue) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			ASSERT(0);
			TRACE("Warning: registration database update failed for key '%s'.\n", lpszKey);
			return false;
		}
		return true;
	}
	else
	{
		HKEY hKey;

		if (::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS)
		{
			LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ,
				(CONST BYTE*)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(TCHAR));

			if(::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
				return true;
		}
		TRACE("Warning: registration database update failed for key '%s'.\n", lpszKey);
		return false;
	}
}

int CAutoTypeApp::GetWindowModuleFileName(HWND hwnd, LPTSTR lpszFileName, UINT cchFileNameMax)
{
	if (m_osVersion.dwPlatformId == VER_PLATFORM_WIN32_NT && 
		m_osVersion.dwMajorVersion >= 4) 
	{
		int result = 0;
		if (pGetModuleFileNameEx)
		{
			DWORD destProcessID;
			GetWindowThreadProcessId(hwnd, &destProcessID);
			HANDLE destProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, destProcessID);
			if (destProcess) 
			{
				HINSTANCE destModule = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
				if (destModule)
					result = pGetModuleFileNameEx(destProcess, destModule, lpszFileName, cchFileNameMax);
				CloseHandle(destProcess);
			}
		}
		return result;
	}
	else
		return GetWindowModuleFileName(hwnd, lpszFileName, cchFileNameMax);
}


/////////////////////////////////////////////////////////////////////////////
// CAutoTypeApp commands

void CAutoTypeApp::SaveWndPlace()
{
	if (m_pMainWnd) m_pMainWnd->GetWindowPlacement(&wndPlace);
}

int CAutoTypeApp::ExitInstance() 
{
	char entry[10];
	ltoa(wmCharCount, entry, 10);
	WriteProfileString("Statistics", "wmCharCount", entry);
	ltoa(autoTypeCount, entry, 10);
	WriteProfileString("Statistics", "autoTypeCount", entry);

	// Store suggestion window settings.
	WriteProfileInt("Settings", "normalLeft", wndPlace.rcNormalPosition.left);
	WriteProfileInt("Settings", "normalRight", wndPlace.rcNormalPosition.right);
	WriteProfileInt("Settings", "normalTop", wndPlace.rcNormalPosition.top);
	WriteProfileInt("Settings", "normalBottom", wndPlace.rcNormalPosition.bottom);

	ltoa(foreColor, entry, 10);
	WriteProfileString("Settings", "foreColor", entry);
	ltoa(backColor, entry, 10);
	WriteProfileString("Settings", "backColor", entry);
	WriteProfileString("Settings", "fontName", fontName);
	WriteProfileInt("Settings", "fontSize", fontSize);
	WriteProfileInt("Settings", "suggest", m_bSuggest);
	WriteProfileInt("Settings", "learn", m_bLearn);
	WriteProfileInt("Settings", "countKeys", m_bCountKeys);
	WriteProfileInt("Settings", "monitorClipboard", m_bMonitorClipboard);
	WriteProfileInt("Settings", "positiveApplications", m_bPositiveApplications);
	WriteProfileInt("Settings", "negativeApplications", m_bNegativeApplications);

	if (m_dlgSuggestions)	{
		m_dlgSuggestions->DestroyWindow();
		delete m_dlgSuggestions;
		m_dlgSuggestions = NULL;
	}

	SetSpyHook(FALSE);
	FreeLibrary(hmodHook);
	FreeLibrary(hPSAPI);
	msgFile->lmtRunning = FALSE;
 	VERIFY(UnmapViewOfFile(msgFile));
    VERIFY(CloseHandle(hMapObject));
	VERIFY(CloseHandle(hMutexObject));

	CoUninitialize();

	return CWinApp::ExitInstance();
}

void CAutoTypeApp::EmptyMsgBuffer()
{
	ASSERT(msgFile);
	if (msgFile->msgsLow == msgFile->msgsHigh)
		return;

	// Get mutex object for shared memory
	DWORD result = WaitForSingleObject(hMutexObject, 0);
	if (result == WAIT_FAILED || result == WAIT_TIMEOUT)
		return;

	msgFile->msgsLow = msgFile->msgsHigh = 0;
	ReleaseMutex(hMutexObject);
}

bool CAutoTypeApp::GetNextMessage(MsgData &msg)
{
	if (!msgFile) return false;

	if (!msgFile->jammed && msgFile->msgsLow == msgFile->msgsHigh)
		return false;

	// Get mutex object for shared memory
	DWORD result = WaitForSingleObject(hMutexObject, 0);
	if (result == WAIT_FAILED || result == WAIT_TIMEOUT)
		return false;

	if (!msgFile->jammed) {
		msg = msgFile->msgs[msgFile->msgsLow];
		if (++msgFile->msgsLow == MSGS_SIZE)
			msgFile->msgsLow = 0;
	}
	else {
		msgFile->msgsLow = msgFile->msgsHigh = 0;
		msgFile->jammed = FALSE;
		theDoc->Abort();
	}

	ReleaseMutex(hMutexObject);
	return true;
}

BOOL CAutoTypeApp::OnIdle(LONG lCount)
{
	MsgData msg;
	while (GetNextMessage(msg))
		if (IsWindow(msg.hWnd) &&
			theApp.m_pMainWnd->m_hWnd != msg.hWnd &&
			!IsChild(theApp.m_pMainWnd->m_hWnd, msg.hWnd))
			theDoc->ProcessMsg(msg);

	while (CWinApp::OnIdle(lCount++));
	return FALSE;
}

void CAutoTypeApp::OnFileSettings() 
{
	if (m_bModalDlg) return;

	m_bModalDlg = true;
	CSettingsDlg dlg(theDoc->GetSettings(), m_pMainWnd);
	dlg.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	if (dlg.DoModal() == IDOK)
		theDoc->ChangeSettings(dlg.GetSettings());
	m_bModalDlg = false;
}

void CAutoTypeApp::OnFileChars() 
{
	CCharsDialog dlg(theDoc->GetSettings(), m_pMainWnd);
	int result = dlg.DoModal();
	if (result == IDOK)
		theDoc->ChangeSettings(dlg.GetSettings());
}


void CAutoTypeApp::OnOptionsPasswords() 
{
	CPasswordsDialog dlg(taboos);
	if (dlg.DoModal() == IDOK)
	{
		taboos = dlg.taboos;
		int j = 0;
		char entry[10];
		for (POSITION i = taboos.GetHeadPosition(); i;)
		{
			Taboo &t = taboos.GetNext(i);
			itoa(j++, entry+1, 10);
			entry[0] = 'h';
			WriteProfileInt("Taboos", entry, t.hash);
			entry[0] = 'd';
			WriteProfileString("Taboos", entry, t.description);
		}
		itoa(j, entry+1, 10);
		entry[0] = 'h';
		WriteProfileInt("Taboos", entry, 0);	// mark end
	}
}

void CAutoTypeApp::OnOptionsApplications() 
{
	CApplicationsDlg dlg(
		m_positiveApplications,
		m_negativeApplications,
		m_bPositiveApplications,
		m_bNegativeApplications);

	if (dlg.DoModal() == IDOK)
	{
		m_positiveApplications.Copy(dlg.m_positiveApplications);
		m_negativeApplications.Copy(dlg.m_negativeApplications);
		m_bPositiveApplications = dlg.m_bPositiveApplications;
		m_bNegativeApplications = dlg.m_bNegativeApplications;

		// Store paths in the registry.
		char key[10];
		for (int i = 0; i < m_positiveApplications.GetSize(); i++)
		{
			itoa(i, key, 10);
			WriteProfileString("PositiveApplications", key, m_positiveApplications[i]);
		}
		// Mark the end.
		itoa(i, key, 10);
		WriteProfileString("PositiveApplications", key, "");

		for (i = 0; i < m_negativeApplications.GetSize(); i++)
		{
			itoa(i, key, 10);
			WriteProfileString("NegativeApplications", key, m_negativeApplications[i]);
		}
		itoa(i, key, 10);
		WriteProfileString("NegativeApplications", key, "");
	}
}

void CAutoTypeApp::OnHelpHelp() 
{
	WinHelp(0, HELP_FINDER);
}

void CAutoTypeApp::OnOptionsSuggest()
{
	m_bSuggest = !m_bSuggest;
	m_pMainWnd->GetMenu()->CheckMenuItem(ID_OPTIONS_SUGGEST, m_bSuggest ? MF_CHECKED : MF_UNCHECKED);
	((CMainFrame*)m_pMainWnd)->TrayMessage(NIM_MODIFY);
}

void CAutoTypeApp::OnOptionsLearn() 
{
	m_bLearn = !m_bLearn;
	m_pMainWnd->GetMenu()->CheckMenuItem(ID_OPTIONS_LEARN, m_bLearn ? MF_CHECKED : MF_UNCHECKED);
	((CMainFrame*)m_pMainWnd)->TrayMessage(NIM_MODIFY);
}

void CAutoTypeApp::OnOptionsCountkeys()
{
	m_bCountKeys = !m_bCountKeys;
	m_pMainWnd->GetMenu()->CheckMenuItem(ID_OPTIONS_COUNTKEYS, m_bCountKeys ? MF_CHECKED : MF_UNCHECKED);
}

void CAutoTypeApp::OnOptionsClipboard()
{
	m_bMonitorClipboard = !m_bMonitorClipboard;
	m_pMainWnd->GetMenu()->CheckMenuItem(ID_OPTIONS_CLIPBOARD, m_bMonitorClipboard ? MF_CHECKED:MF_UNCHECKED);

	if (m_bMonitorClipboard) {
		ignoreClipboard = true;
		nextClipboardViewer = m_pMainWnd->SetClipboardViewer();
		ignoreClipboard = false;
	}
	else {
		m_pMainWnd->ChangeClipboardChain(nextClipboardViewer);
		nextClipboardViewer = NULL;
	}
}

void CAutoTypeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CAutoTypeApp::OnOptionsBackColor() 
{
	CColorDlg dlg(IDS_BACKCOLOR, backColor, CC_SOLIDCOLOR | CC_RGBINIT);
	if (dlg.DoModal() == IDOK) 
		backColor = dlg.GetColor();
}

void CAutoTypeApp::OnOptionsFont() 
{
	CFont *fnt;
	LOGFONT lf;
	fnt = m_dlgSuggestions->m_Static[0].GetFont();
	fnt->GetLogFont(&lf);
	lf.lfWeight = 0;
	CFontDialog dlg(&lf);
	dlg.m_cf.Flags |= CF_ENABLETEMPLATE;
	dlg.m_cf.lpTemplateName = MAKEINTRESOURCE(IDD_FONT);
	dlg.m_cf.hInstance = AfxGetInstanceHandle();
	dlg.m_cf.rgbColors = foreColor;
	if (dlg.DoModal() == IDOK) {
		fontName = dlg.GetFaceName();
		fontSize = dlg.GetSize();
		foreColor = dlg.GetColor();

		fnt = new CFont;
		dlg.GetCurrentFont(&lf);
		fnt->CreateFontIndirect(&lf);
		m_dlgSuggestions->ChangeFont(fnt);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CTabooList

CTabooList::CTabooList()
	: CList<Taboo, Taboo&>()
{
}

CTabooList::~CTabooList()
{
	for (POSITION i = GetHeadPosition(); i;)
	{
		Taboo &t = GetNext(i);
		t.description.Empty();
	}
}

void CTabooList::operator=(const CTabooList& src)
{
	RemoveAll();
	for (POSITION i = src.GetHeadPosition(); i;)
	{
		Taboo t = src.GetNext(i);
		AddTail(t);
	}
}

POSITION CTabooList::Find(UINT searchValue, POSITION startAfter) const
{
	Taboo t;
	t.hash = searchValue;
	return CList<Taboo, Taboo&>::Find(t, startAfter);
}

void CAutoTypeApp::ToggleActivation()
{
	if (m_bLearn && m_bSuggest)
		m_bSuggest = false;
	else if (m_bLearn && !m_bSuggest)
		m_bLearn = false;
	else if (!m_bLearn && !m_bSuggest) 
		m_bSuggest = true;
	else
		m_bLearn = true;

	((CMainFrame*)m_pMainWnd)->TrayMessage(NIM_MODIFY);
}
