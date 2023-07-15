// settings.h : header file
// $Id: settings.h,v 1.2 2002/05/20 21:33:25 cc Exp $
//

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "AutoTDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CPPgSuggestions

class CPPgSuggestions : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPgSuggestions)

// Construction
public:
	CPPgSuggestions();
	void Init(const DocSettings s);

// Dialog Data
	//{{AFX_DATA(CPPgSuggestions)
	enum { IDD = IDD_PROP_SUGGESTIONS };
	CSpinButtonCtrl	m_maxSuggestedSpin;
	CSpinButtonCtrl	m_addedSpin;
	CSpinButtonCtrl	m_givenSpin;
	UINT	m_given;
	UINT	m_added;
	UINT	m_probability;
	UINT	m_speed;
	UINT	m_maxSuggested;
	BOOL	m_single;
	BOOL	m_signal;
	BOOL	m_addSpace;
	BOOL	m_caseSensitive;
	BOOL	m_numpad;
	BOOL	m_bAutoHide;
	UINT	m_iTimeout;
	int		m_iPosition;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPgSuggestions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPgSuggestions)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnInitDialog();
	afx_msg void OnAutohide();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CPPgDB dialog

class CPPgDB : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPgDB)

// Construction
public:
	CPPgDB();
	void Init(const DocSettings s);

// Dialog Data
	//{{AFX_DATA(CPPgDB)
	enum { IDD = IDD_PROP_DB };
	CSliderCtrl	m_sizeSlider;
	CSliderCtrl	m_ageSlider;
	UINT	m_age;
	UINT	m_size;
	BOOL	m_autoSave;
	BOOL	m_fixedVocabulary;
	BOOL	m_ignorePasswords;
	BOOL	m_bAllowNumbers;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPgDB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool IsOverURL(CPoint point) const;

	bool m_bRegistered;

	// Generated message map functions
	//{{AFX_MSG(CPPgDB)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg dialog

class CSettingsDlg : public CPropertySheet
{
// Construction
public:
	CSettingsDlg(const DocSettings s, CWnd* pParent = NULL);
	DocSettings GetSettings();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	DocSettings settings;

protected:

	CPPgSuggestions suggestions;
	CPPgDB db;

	// Generated message map functions
	//{{AFX_MSG(CSettingsDlg)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif
