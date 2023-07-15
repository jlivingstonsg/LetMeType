// UrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "autotype.h"
#include "UrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable: 4800)


/////////////////////////////////////////////////////////////////////////////
// CUrlDlg dialog

CUrlDlg::CUrlDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CUrlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUrlDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

bool CUrlDlg::IsOverURL(CPoint point) const
{
	CWnd* pWnd = GetDlgItem(IDC_URL); 
	if (pWnd) {
		ClientToScreen(&point);
		CRect r; pWnd->GetWindowRect(&r);
		return r.PtInRect(point);
	}
	else return false;
}


BEGIN_MESSAGE_MAP(CUrlDlg, CDialog)
	//{{AFX_MSG_MAP(CUrlDlg)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUrlDlg message handlers

BOOL CUrlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString s; s.LoadString(IDS_URL);
	GetDlgItem(IDC_URL)->SetWindowText(s);

	return TRUE;
}

HBRUSH CUrlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH result = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_URL)
		pDC->SetTextColor(URL_COLOR);
	return result;
}

void CUrlDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (IsOverURL(point)) {
		CString url;
		url.LoadString(IDS_URL);
		ShellExecute(NULL, "open", url, NULL, NULL, 0);
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CUrlDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (IsOverURL(point)) {
		HCURSOR cursor = theApp.LoadCursor(IDC_MYHAND);
		SetCursor(cursor);
	}
	else {
		HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(cursor);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}
