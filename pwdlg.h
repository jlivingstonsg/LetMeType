// pwdlg.h : header file
// $Id: pwdlg.h,v 1.1.1.1 2002/05/12 14:40:56 cc Exp $
//

/////////////////////////////////////////////////////////////////////////////
// CPasswordsDialog dialog

class CPasswordsDialog : public CDialog
{
// Construction
public:
	CPasswordsDialog(CTabooList&);

// Dialog Data
	//{{AFX_DATA(CPasswordsDialog)
	enum { IDD = IDD_PASSWORDS };
	CButton	m_btnRemove;
	CListBox	m_pwList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasswordsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CTabooList taboos;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPasswordsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnRemove();
	afx_msg void OnNew();
	afx_msg void OnSelchangePwlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
