// newpwdlg.h : header file
// $Id: newpwdlg.h,v 1.1.1.1 2002/05/12 14:40:56 cc Exp $
//

#ifndef __NEWPWDLG_H__
#define __NEWPWDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CNewPWDialog dialog

class CNewPWDialog : public CDialog
{
// Construction
public:
	CNewPWDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewPWDialog)
	enum { IDD = IDD_NEWPASSWORD };
	CString	m_description;
	CString	m_word;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewPWDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewPWDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
