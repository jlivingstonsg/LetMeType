// NewApplicationDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "autotype.h"
#include "NewApplicationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CNewApplicationDlg 


CNewApplicationDlg::CNewApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewApplicationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewApplicationDlg)
	m_filenameValue = _T("");
	//}}AFX_DATA_INIT

	m_bMouseCaptured = false;
}


void CNewApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewApplicationDlg)
	DDX_Control(pDX, IDC_WINDOW, m_window);
	DDX_Control(pDX, IDC_FILENAME, m_filenameControl);
	DDX_Text(pDX, IDC_FILENAME, m_filenameValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewApplicationDlg, CDialog)
	//{{AFX_MSG_MAP(CNewApplicationDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_WINDOW, OnWindow)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CNewApplicationDlg 

void CNewApplicationDlg::OnBrowse() 
{
	CString filter;
	filter.LoadString(IDS_EXE_FILTERS);

	CFileDialog dlg(TRUE, NULL, NULL, 
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
		filter);

	if (dlg.DoModal() == IDOK)
		m_filenameControl.SetWindowText(dlg.m_ofn.lpstrFile);
}

void CNewApplicationDlg::OnWindow() 
{
	m_window.SetIcon(theApp.LoadIcon(IDI_WINDOW));
	m_originalCursor = SetCursor(theApp.LoadCursor(IDC_CROSSHAIR));
	m_bMouseCaptured = true;
	m_lastWnd = NULL;
	SetCapture();
}

void CNewApplicationDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bMouseCaptured)
	{
		SetCursor(m_originalCursor);
		m_window.SetIcon(theApp.LoadIcon(IDI_WINDOW_CROSSHAIR));
		ReleaseCapture();
		m_bMouseCaptured = false;

		POINT p;
		char destWndFileName[512];

		GetCursorPos(&p);
		CWnd *wnd = WindowFromPoint(p);

		if (wnd && theApp.GetWindowModuleFileName(wnd->m_hWnd, destWndFileName, sizeof(destWndFileName)))
			m_filenameControl.SetWindowText(destWndFileName);
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CNewApplicationDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bMouseCaptured)
	{
		POINT p;
		char destWndFileName[512];

		GetCursorPos(&p);
		CWnd *wnd = WindowFromPoint(p);

		if (wnd->m_hWnd != m_lastWnd)
		{
			m_lastWnd = wnd->m_hWnd;

			if (wnd && theApp.GetWindowModuleFileName(wnd->m_hWnd, destWndFileName, sizeof(destWndFileName)))
				m_filenameControl.SetWindowText(destWndFileName);
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}
