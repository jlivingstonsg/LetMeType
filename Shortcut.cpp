// Shortcut.cpp : implementation file
//

#include "stdafx.h"
#include "Shortcut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShortcut
CShortcut::CShortcut()
{
    // Initialize data members
    m_bDirty           = FALSE;
    m_strPath          = _T("");
    m_strTarget        = _T("");
    m_strStartDir      = _T("");
    m_strDescription   = _T("");
    m_strIconLocation  = _T("");
    m_strArgs          = _T("");
    m_nIconIndex       = 0;
    m_wHotkey          = MAKEWORD((BYTE) 0, (BYTE) 0);
    m_nShowCmd         = 0;
    HICON m_hLargeIcon = NULL;
    HICON m_hSmallIcon = NULL;
}

CShortcut::CShortcut(LPCTSTR lpsz, BOOL bFill)
{
    // Initialize data members
    m_bDirty           = FALSE;
    m_strPath          = lpsz;
    m_strTarget        = _T("");
    m_strStartDir      = _T("");
    m_strDescription   = _T("");
    m_strIconLocation  = _T("");
    m_strArgs          = _T("");
    m_nIconIndex       = 0;
    m_nShowCmd         = 0;
    m_wHotkey          = MAKEWORD((BYTE) 0, (BYTE) 0);
    HICON m_hLargeIcon = NULL;
    HICON m_hSmallIcon = NULL;

    // Fill data
    if (bFill)
    {
        FillData();
    }
}

CShortcut::~CShortcut()
{
    ASSERT(m_bDirty == FALSE);
}

#ifdef _DEBUG
void CShortcut::Dump(CDumpContext& dc)
{
    CObject::Dump(dc);

    dc << _T("\nm_bDirty = ")          << m_bDirty
       << _T("\nm_strPath = ")         << m_strPath
       << _T("\nm_strTarget = ")       << m_strTarget
       << _T("\nm_strStartDir = ")     << m_strStartDir
       << _T("\nm_strDescription = ")  << m_strDescription
       << _T("\nm_strIconLocation = ") << m_strIconLocation
       << _T("\nm_strArgs = ")         << m_strArgs
       << _T("\nm_nIconIndex = ")      << m_nIconIndex
       << _T("\nm_wHotkey = ")         << m_wHotkey
       << _T("\nm_nShowCmd = ")        << m_nShowCmd
       << _T("\nm_hLargeIcon = ")      << m_hLargeIcon
       << _T("\nm_hSmallIcon = ")      << m_hSmallIcon
       << _T("\n");
}
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// Create
// Remarks: Creates a shortcut and saves it
// Inputs:  LPCTSTRs containing full path to file name, full path to target,
//          start directory, description, arguments, icon location, an int
//          containing icon index, a WORD containing hot key, and an int
//          containing show command
// Returns: BOOL; TRUE if successful, FALSE if unsuccessful
BOOL CShortcut::Create(LPCTSTR lpszPath,     LPCTSTR lpszTarget, 
                       LPCTSTR lpszStartDir, LPCTSTR lpszDescription, 
                       LPCTSTR lpszArgs,     LPCTSTR lpszIconLocation,
                       int nIconIndex,       WORD    wHotkey,
                       int nShowCmd)
{
    m_bDirty          = TRUE;
    m_strPath         = lpszPath;
    m_strTarget       = lpszTarget;
    m_strStartDir     = lpszStartDir;
    m_strDescription  = lpszDescription;
    m_strArgs         = lpszArgs;
    m_strIconLocation = lpszIconLocation;
    m_nIconIndex      = nIconIndex;
    m_wHotkey         = wHotkey;
    m_nShowCmd        = nShowCmd;

    return Save();
}

/////////////////////////////////////////////////////////////////////////////
// CreateEx
// Remarks: Creates a shortcut and saves it
// Inputs:  A pointer to a SHORTCUTSTRUCT structure containing initialization
//          data
// Returns: BOOL; TRUE if successful, FALSE if unsuccessful
BOOL CShortcut::CreateEx(const LPSHORTCUTSTRUCT s)
{
    m_bDirty          = TRUE;
    m_strPath         = s->strPath;
    m_strTarget       = s->strTarget;
    m_strStartDir     = s->strStartDir;
    m_strDescription  = s->strDescription;
    m_strArgs         = s->strArgs;
    m_strIconLocation = s->strIconLocation;
    m_nIconIndex      = s->nIconIndex;
    m_wHotkey         = s->wHotkey;
    m_nShowCmd        = s->nShowCmd;

    return Save();
}

/////////////////////////////////////////////////////////////////////////////
// FileExists
// Remarks: Determines whether a file exists
// Inputs:  LPCTSTR containing full path to file name
// Returns: BOOL; TRUE if file exists, FALSE if file does not exist
BOOL CShortcut::FileExists(LPCTSTR lpsz)
{
    ASSERT(lpsz != NULL);

    WIN32_FIND_DATA wfd;
    HANDLE hFind = ::FindFirstFile(lpsz, &wfd);
    ::FindClose(hFind);

    // Make sure file exists, and that it's not a directory
    return (hFind != INVALID_HANDLE_VALUE &&
        !(wfd.dwFileAttributes && FILE_ATTRIBUTE_DIRECTORY));
}

/////////////////////////////////////////////////////////////////////////////
// TargetExists
// Remarks: Determines whether the target of the link exists
// Inputs:  None
// Returns: BOOL; TRUE if file exists, FALSE if file does not exist
BOOL CShortcut::TargetExists()
{
    // If we have the target, see whether it exists
    return (m_strTarget == _T("") ? FALSE : FileExists((LPCTSTR) m_strTarget));
}

/////////////////////////////////////////////////////////////////////////////
// FillData
// Remarks: Fills all the data members for the shortcut
// Inputs:  None
// Returns: BOOL; TRUE if successful, FALSE if unsuccessful
BOOL CShortcut::FillData()
{
    BOOL bRet = FALSE;

    // Make sure we have a path
    ASSERT(m_strPath != _T(""));

    IShellLink* psl;
    HRESULT hres;
    LPTSTR lpsz = m_strPath.GetBuffer(MAX_PATH);

    // Create instance for shell link
    hres = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
        IID_IShellLink, (LPVOID*) &psl);
    if (SUCCEEDED(hres))
    {
        // Get a pointer to the persist file interface
        IPersistFile* ppf;
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*) &ppf);
        if (SUCCEEDED(hres))
        {
            // Make sure it's ANSI
            WORD wsz[MAX_PATH];
            ::MultiByteToWideChar(CP_ACP, 0, lpsz, -1, wsz, MAX_PATH);

            // Load shortcut
            hres = ppf->Load(wsz, STGM_READ);
            if (SUCCEEDED(hres))
            {
                // Fill data members
                bRet = (FillTarget(psl)      &&
                        FillStartDir(psl)    &&
                        FillDescription(psl) &&
                        FillArguments(psl)   &&
                        FillIcons(psl)       &&
                        FillHotkey(psl)      &&
                        FillShowCmd(psl));
            }
            ppf->Release();
        }
        psl->Release();
    }
    return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// FillTarget
// Remarks: Fills the m_strTarget data member
// Inputs:  IShellLink*
// Returns: BOOL; TRUE if successful, FALSE if unsuccessful
BOOL CShortcut::FillTarget(IShellLink* psl)
{
    ASSERT(psl != NULL);
    WIN32_FIND_DATA wfd;
    HRESULT hres = psl->GetPath(m_strTarget.GetBuffer(MAX_PATH), MAX_PATH,
        &wfd, SLGP_UNCPRIORITY);
    m_strTarget.ReleaseBuffer();
    return SUCCEEDED(hres);
}

/////////////////////////////////////////////////////////////////////////////
// FillStartDir
// Remarks: Fills the m_strStartDir data member
// Inputs:  IShellLink*
// Returns: BOOL; TRUE if successful, FALSE if unsuccessful
BOOL CShortcut::FillStartDir(IShellLink* psl)
{
    ASSERT(psl != NULL);
    HRESULT hres = psl->GetWorkingDirectory(m_strStartDir.GetBuffer(MAX_PATH),
        MAX_PATH);
    m_strStartDir.ReleaseBuffer();
    return SUCCEEDED(hres);
}

/////////////////////////////////////////////////////////////////////////////
// FillDescription
// Remarks: Fills the m_strDescription data member
// Inputs:  IShellLink*
// Returns: BOOL; TRUE if successful, FALSE if unsuccessful
BOOL CShortcut::FillDescription(IShellLink* psl)
{
    ASSERT(psl != NULL);
    HRESULT hres = psl->GetDescription(m_strDescription.GetBuffer(MAX_PATH),
        MAX_PATH);
    m_strDescription.ReleaseBuffer();
    return SUCCEEDED(hres);
}

/////////////////////////////////////////////////////////////////////////////
// FillArguments
// Remarks: Fills the m_strArgs data member
// Inputs:  IShellLink*
// Returns: BOOL; TRUE if successful, FALSE if unsuccessful
BOOL CShortcut::FillArguments(IShellLink* psl)
{
    ASSERT(psl != NULL);
    HRESULT hres = psl->GetArguments(m_strArgs.GetBuffer(MAX_PATH), MAX_PATH);
    m_strArgs.ReleaseBuffer();
    return SUCCEEDED(hres);
}

/////////////////////////////////////////////////////////////////////////////
// FillIcons
// Remarks: Fills the m_hLargeIcon & m_hSmallIcon data members.  It also fills
//          the m_strIconLocation and m_nIconIndex data members, but the 
//          m_nIconIndex member doesn't always get the correct icon index.
//          If someone can explain this better to me, I'd appreciate it.
// Inputs:  IShellLink*
// Returns: BOOL; TRUE if successful, FALSE if unsuccessful
BOOL CShortcut::FillIcons(IShellLink* psl)
{
    ASSERT(psl != NULL);

    IExtractIcon* pei;
    UINT nFlags;
    BOOL bRet = FALSE;

    HRESULT hres = psl->QueryInterface(IID_IExtractIcon, (LPVOID*) &pei);
    if (SUCCEEDED(hres))
    {
        hres = pei->GetIconLocation(GIL_FORSHELL, 
            m_strIconLocation.GetBuffer(MAX_PATH), MAX_PATH, &m_nIconIndex,
            &nFlags);
        m_strIconLocation.ReleaseBuffer();
        if (hres == NOERROR)
        {
            // Try to extract it through the interface
            hres = pei->Extract(m_strIconLocation.GetBuffer(MAX_PATH),
                m_nIconIndex, &m_hLargeIcon, &m_hSmallIcon, 0);
            m_strIconLocation.ReleaseBuffer();
            if (hres == S_FALSE)
            {
                ExtractIcons();
                bRet = (m_hLargeIcon != NULL && m_hSmallIcon != NULL);
            }
            else
            {
                bRet = TRUE;
            }
        }
        pei->Release();
    }
    return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// ExtractIcons
// Remarks: Extracts the large and small icons using the ::ExtractIcon API
// Inputs:  None
// Returns: void
void CShortcut::ExtractIcons()
{
    // Extract icons
    m_hLargeIcon = ::ExtractIcon(AfxGetInstanceHandle(), 
        (LPCTSTR) m_strIconLocation, m_nIconIndex);
    m_hSmallIcon = ::ExtractIcon(AfxGetInstanceHandle(), 
        (LPCTSTR) m_strIconLocation, m_nIconIndex);
}

/////////////////////////////////////////////////////////////////////////////
// FillHotkey
// Remarks: Fills the m_wHotkey data member
// Inputs:  IShellLink*
// Returns: BOOL; TRUE if successful, FALSE if unsuccessful
BOOL CShortcut::FillHotkey(IShellLink* psl)
{
    ASSERT(psl != NULL);
    HRESULT hres = psl->GetHotkey(&m_wHotkey);
    return SUCCEEDED(hres);
}

/////////////////////////////////////////////////////////////////////////////
// FillShowCmd
// Remarks: Fills the m_nShowCmd data member
// Inputs:  IShellLink*
// Returns: BOOL; TRUE if successful, FALSE if unsuccessful
BOOL CShortcut::FillShowCmd(IShellLink* psl)
{
    ASSERT(psl != NULL);
    HRESULT hres = psl->GetShowCmd(&m_nShowCmd);
    return SUCCEEDED(hres);
}

/////////////////////////////////////////////////////////////////////////////
// Save
// Remarks: Saves the shortcut
// Inputs:  None
// Returns: BOOL; TRUE if successful, FALSE if unsuccessful
BOOL CShortcut::Save()
{
    // Save only if we have to
    if (m_bDirty == FALSE)
    {
        return TRUE;
    }

    ASSERT(m_strPath != _T(""));

    IShellLink* psl;
    HRESULT hres;
    BOOL bRet = FALSE;

    // Create shell link instance
    hres = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
        IID_IShellLink, (LPVOID*) &psl);
    if (SUCCEEDED(hres))
    {
        // Get a pointer to persist file interface
        IPersistFile* ppf;
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*) &ppf);
        if (SUCCEEDED(hres))
        {
            // Convert to ANSI
            WORD wsz[MAX_PATH];
            ::MultiByteToWideChar(CP_ACP, 0, (LPCTSTR) m_strPath, -1, wsz, 
                MAX_PATH);

            // Set attributes of link
            psl->SetPath((LPCTSTR) m_strTarget);
            psl->SetWorkingDirectory((LPCTSTR) m_strStartDir);
            psl->SetIconLocation((LPCTSTR) m_strIconLocation, m_nIconIndex);
            psl->SetDescription((LPCTSTR) m_strDescription);
            psl->SetArguments((LPCTSTR) m_strArgs);
            psl->SetHotkey(m_wHotkey);
            psl->SetShowCmd(m_nShowCmd);

            // Save the updated link
            hres = ppf->Save(wsz, TRUE);
            if (SUCCEEDED(hres))
            {
                bRet = TRUE;
                m_bDirty = FALSE;
            }
            ppf->Release();
        }
        psl->Release();
    }
    return bRet;
}

