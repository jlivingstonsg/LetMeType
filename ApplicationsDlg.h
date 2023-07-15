#if !defined(AFX_APPLICATIONSDLG_H__41818BF2_0DF9_48D1_97FD_7FF85E2A22C4__INCLUDED_)
#define AFX_APPLICATIONSDLG_H__41818BF2_0DF9_48D1_97FD_7FF85E2A22C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ApplicationsDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CApplicationsDlg 

class CApplicationsDlg : public CDialog
{
// Konstruktion
public:
	CApplicationsDlg(
		CStringArray &positiveApplications, 
		CStringArray &negativeApplications,
		bool bPositiveApplications, 
		bool bNegativeApplications);

	// These are redundant copies of the dialog data.
	bool m_bNegativeApplications;
	bool m_bPositiveApplications;
	CStringArray m_negativeApplications;
	CStringArray m_positiveApplications;

// Dialogfelddaten
	//{{AFX_DATA(CApplicationsDlg)
	enum { IDD = IDD_APPLICATIONS };
	CButton	m_new;
	CButton	m_remove;
	CButton	m_all;
	CButton	m_positive;
	CButton	m_negative;
	CListBox	m_applications;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CApplicationsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CApplicationsDlg)
	afx_msg void OnAll();
	afx_msg void OnNegative();
	afx_msg void OnNew();
	afx_msg void OnPositive();
	afx_msg void OnRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeApplList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_APPLICATIONSDLG_H__41818BF2_0DF9_48D1_97FD_7FF85E2A22C4__INCLUDED_
