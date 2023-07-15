// pwdlg.cpp : Dialog for defining taboo words.
// $Id: pwdlg.cpp,v 1.1.1.1 2002/05/12 14:40:56 cc Exp $
//

#include "stdafx.h"
#include "AutoType.h"
#include "pwdlg.h"
#include "newpwdlg.h"
#include "crc16.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Collection helpers

BOOL AFXAPI CompareElements(const Taboo* p1, const Taboo* p2)
{
	return p1->hash == p2->hash;
}


/////////////////////////////////////////////////////////////////////////////
// CPasswordsDialog dialog


CPasswordsDialog::CPasswordsDialog(CTabooList &ts)
	: CDialog(CPasswordsDialog::IDD, theApp.m_pMainWnd)
{
	//{{AFX_DATA_INIT(CPasswordsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	taboos = ts;
}

void CPasswordsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswordsDialog)
	DDX_Control(pDX, IDC_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_PWLIST, m_pwList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasswordsDialog, CDialog)
	//{{AFX_MSG_MAP(CPasswordsDialog)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_NEW, OnNew)
	ON_LBN_SELCHANGE(IDC_PWLIST, OnSelchangePwlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPasswordsDialog message handlers

BOOL CPasswordsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for (POSITION i = taboos.GetHeadPosition(); i;)
	{
		Taboo &t = taboos.GetNext(i);
		m_pwList.SetItemData(m_pwList.AddString(t.description), t.hash);
	}
	
	return TRUE;
}

void CPasswordsDialog::OnRemove() 
{
	int lbPos = m_pwList.GetCurSel();
	if (lbPos != LB_ERR)
	{
		Taboo t;
		POSITION i = taboos.Find(m_pwList.GetItemData(lbPos));
		ASSERT(i);
		taboos.RemoveAt(i);
		m_pwList.DeleteString(lbPos);
	}
}

void CPasswordsDialog::OnNew() 
{
	CNewPWDialog dlg(theApp.m_pMainWnd);
	int result = dlg.DoModal();
	Taboo t;
	t.hash = CRC16(LPCTSTR(dlg.m_word));
	if (result == IDOK && t.hash)
	{
		// Check for duplicate
		POSITION i = taboos.Find(t.hash);
		if (i)
		{
			CString msg;
			msg.LoadString(IDS_PWERRMSG);
			CString title;
			title.LoadString(IDS_PWERRTITLE);
			char s[255];
			sprintf(s, msg, taboos.GetAt(i).description);
			MessageBox(s, title, MB_ICONEXCLAMATION | MB_OK);
			return;
		}

		// Add new taboo word
		t.description = dlg.m_description;
		taboos.AddTail(t);
		m_pwList.SetItemData(m_pwList.AddString(t.description), t.hash);
	}
}

void CPasswordsDialog::OnSelchangePwlist() 
{
	m_btnRemove.EnableWindow();
}
