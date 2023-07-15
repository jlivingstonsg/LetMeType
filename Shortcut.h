// Shortcut.h : header file
//

// CShortcut
// Written 1996 by Rob Warner
// rhwarner@southeast.net
// http://users.southeast.net/~rhwarner
// Distribute freely, modify to your heart's content <g>
// Let me know if you find it useful, or if you've improved it <vbg>

// To use: You must #include <afxole.h> in your stdafx.h.
//         You must also initialize/uninitialize COM.  One way to do this is
//         in your derived CWinApp::InitInstance(), add the following line:
//
//          ::CoInitialize(NULL);
//
//         And in your derived CWinApp::ExitInstance(), add the following line:
//
//          ::CoUninitialize();
//

/////////////////////////////////////////////////////////////////////////////
// CShortcut

#ifndef _INC_SHORTCUT
#define _INC_SHORTCUT

#include <shlobj.h>

typedef struct tagSHORTCUTSTRUCT {
    CString strPath;
    CString strTarget;
    CString strStartDir;
    CString strDescription;
    CString strIconLocation;
    CString strArgs;
    int     nIconIndex;
    WORD    wHotkey;
    int     nShowCmd;
} SHORTCUTSTRUCT, *LPSHORTCUTSTRUCT;

class 
#ifdef _AFXEXT
AFX_EXT_CLASS
#endif
CShortcut : public CObject
{
// Constructors
public:
    CShortcut();
    CShortcut(LPCTSTR, BOOL bFill = FALSE);
    BOOL Create(LPCTSTR, LPCTSTR, LPCTSTR, LPCTSTR, LPCTSTR, LPCTSTR, int,
        WORD, int);
    BOOL CreateEx(const LPSHORTCUTSTRUCT);

// Attributes
public:
    // Accessors
    CString GetPath()         { return m_strPath;         }
    CString GetTarget()       { return m_strTarget;       }
    CString GetStartDir()     { return m_strStartDir;     }
    CString GetDescription()  { return m_strDescription;  }
    CString GetArguments()    { return m_strArgs;         }
    CString GetIconLocation() { return m_strIconLocation; }
    int     GetIconIndex()    { return m_nIconIndex;      }
    HICON   GetLargeIcon()    { return m_hLargeIcon;      }
    HICON   GetSmallIcon()    { return m_hSmallIcon;      }
    WORD    GetHotkey()       { return m_wHotkey;         }
    int     GetShowCmd()      { return m_nShowCmd;        }

    // Mutators
    void SetPath(LPCTSTR lpsz)           { m_strPath         = lpsz;  m_bDirty = TRUE; }
    void SetTarget(LPCTSTR lpsz)         { m_strTarget       = lpsz;  m_bDirty = TRUE; }
    void SetStartDir(LPCTSTR lpsz)       { m_strStartDir     = lpsz;  m_bDirty = TRUE; }
    void SetArguments(LPCTSTR lpsz)      { m_strArgs         = lpsz;  m_bDirty = TRUE; }
    void SetIconIndex(const int n)       { m_nIconIndex      = n;     m_bDirty = TRUE; }
    void SetHotkey(const WORD w)         { m_wHotkey         = w;     m_bDirty = TRUE; }
    void SetShowCmd(const int n)         { m_nShowCmd        = n;     m_bDirty = TRUE; }
    void SetDescription(LPCTSTR lpsz)    { m_strDescription  = lpsz;  m_bDirty = TRUE; }
    void SetIconLocation(LPCTSTR lpsz)   { m_strIconLocation = lpsz;  m_bDirty = TRUE; }
    void SetLargeIcon(const HICON hIcon) { m_hLargeIcon      = hIcon; m_bDirty = TRUE; }
    void SetSmallIcon(const HICON hIcon) { m_hSmallIcon      = hIcon; m_bDirty = TRUE; }
    
protected:
    BOOL    m_bDirty;
    CString m_strPath;
    CString m_strTarget;
    CString m_strStartDir;
    CString m_strDescription;
    CString m_strIconLocation;
    CString m_strArgs;
    int     m_nIconIndex;
    HICON   m_hLargeIcon;
    HICON   m_hSmallIcon;
    WORD    m_wHotkey;
    int     m_nShowCmd;

// Operations
public:
    BOOL Save();
    BOOL FillData();

// Overrides

// Implementation
public:
    virtual ~CShortcut();

#ifdef _DEBUG
    virtual void Dump(CDumpContext&);
#endif // _DEBUG

protected:
	void ExtractIcons();
    BOOL FileExists(LPCTSTR);
    BOOL TargetExists();
    BOOL FillTarget      (IShellLink* psl);
    BOOL FillIcons       (IShellLink* psl);
    BOOL FillDescription (IShellLink* psl);
    BOOL FillStartDir    (IShellLink* psl);
    BOOL FillArguments   (IShellLink* psl);
    BOOL FillHotkey      (IShellLink* psl);
    BOOL FillShowCmd     (IShellLink* psl);
};

#endif
