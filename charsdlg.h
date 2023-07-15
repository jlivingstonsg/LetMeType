// charsdlg.h : header file
// $Id: charsdlg.h,v 1.1.1.1 2002/05/12 14:40:55 cc Exp $
//

#ifndef _CHARSDLG_H_
#define _CHARSDLG_H_

#include "AutoTDoc.h"

#define CHARS_COUNT	20

/////////////////////////////////////////////////////////////////////////////
// CCharsDialog dialog

class CCharsDialog : public CDialog
{
// Construction
public:
	CCharsDialog(const DocSettings s, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCharsDialog)
	enum { IDD = IDD_WORDCHARS };
	BOOL	m_digits;
	BOOL	m_brackets;
	//}}AFX_DATA
	BOOL	m_chars[CHARS_COUNT];

	DocSettings GetSettings();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DocSettings settings;

	// Generated message map functions
	//{{AFX_MSG(CCharsDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
