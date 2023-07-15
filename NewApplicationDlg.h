#if !defined(AFX_NEWAPPLICATIONDLG_H__8421D7DB_3B38_4EF6_ABBC_17BCEFF039D7__INCLUDED_)
#define AFX_NEWAPPLICATIONDLG_H__8421D7DB_3B38_4EF6_ABBC_17BCEFF039D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewApplicationDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CNewApplicationDlg 

class CNewApplicationDlg : public CDialog
{
// Konstruktion
public:
	CNewApplicationDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CNewApplicationDlg)
	enum { IDD = IDD_NEWAPPLICATION };
	CStatic	m_window;
	CEdit	m_filenameControl;
	CString	m_filenameValue;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CNewApplicationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CNewApplicationDlg)
	afx_msg void OnBrowse();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HWND m_lastWnd;
	bool m_bMouseCaptured;
	HCURSOR m_originalCursor;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_NEWAPPLICATIONDLG_H__8421D7DB_3B38_4EF6_ABBC_17BCEFF039D7__INCLUDED_
