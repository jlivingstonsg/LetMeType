// newpwdlg.cpp : implementation file
// $Id: newpwdlg.cpp,v 1.1.1.1 2002/05/12 14:40:56 cc Exp $
//

#include "stdafx.h"
#include "AutoType.h"
#include "AutoTDoc.h"
#include "newpwdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewPWDialog dialog


CNewPWDialog::CNewPWDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewPWDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewPWDialog)
	m_description = _T("");
	m_word = _T("");
	//}}AFX_DATA_INIT
}

void AFXAPI DDV_IsWordChar(CDataExchange* pDX, CString const& val)
{
	if (pDX->m_bSaveAndValidate)
	{
		for (int i = 0; i < val.GetLength() && theDoc->IsWordChar(val[i]); i++);
		if (i < val.GetLength())
		{
			CString msg;
			msg.Format(IDS_ILLCHAR, val[i]);
			AfxMessageBox(msg, MB_ICONEXCLAMATION);
			msg.Empty();	// prepare for exception
			pDX->Fail();
		}
	}
}

void CNewPWDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewPWDialog)
	DDX_Text(pDX, IDC_DESCRIPTION, m_description);
	DDX_Text(pDX, IDC_WORD, m_word);
	//}}AFX_DATA_MAP
	DDV_IsWordChar(pDX, m_word);
}


BEGIN_MESSAGE_MAP(CNewPWDialog, CDialog)
	//{{AFX_MSG_MAP(CNewPWDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewPWDialog message handlers

BOOL CNewPWDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GotoDlgCtrl(GetDlgItem(IDC_WORD));
	return FALSE;
}
