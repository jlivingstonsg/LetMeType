// ColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "autotype.h"
#include "ColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorDlg

IMPLEMENT_DYNAMIC(CColorDlg, CColorDialog)

CColorDlg::CColorDlg(UINT _titleId, COLORREF clrInit, DWORD dwFlags, CWnd* pParentWnd) :
	CColorDialog(clrInit, dwFlags, pParentWnd)
{
	titleId = _titleId;
}


BEGIN_MESSAGE_MAP(CColorDlg, CColorDialog)
	//{{AFX_MSG_MAP(CColorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CColorDlg::OnInitDialog() 
{
	CColorDialog::OnInitDialog();

	CString s;
	s.LoadString(titleId);
	SetWindowText(s);
	
	return TRUE;
}
