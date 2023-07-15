// suggestw.h : header file
// $Id: suggestw.h,v 1.2 2002/05/20 21:34:09 cc Exp $
//

#ifndef __SUGGESTW_H__
#define __SUGGESTW_H__

#pragma warning ( disable : 4181 )

#include "resource.h"

// Pair (CString x WORD)
struct CStringDWORDPair
{
	CString	s;
	DWORD	w;
};
typedef CStringDWORDPair& CStringDWORDPairR;
int AFXAPI SortElements(const CStringDWORDPair p1, const CStringDWORDPairR p2);
void AFXAPI DestructElements(CStringDWORDPair* p, int nCount);
#ifdef _DEBUG
void AFXAPI DumpElements(CDumpContext& dc, const CStringDWORDPair* pElements, int nCount);
#endif
#include "SortedArray.h"

enum SuggestionPosition { POS_UNDEF=-1, POS_CARET, POS_FIXED, POS_CURSOR };

/////////////////////////////////////////////////////////////////////////////
// CSuggestionsDlg dialog

#define MAX_SUGGESTIONS		9

class CSuggestionsDlg : public CDialog
{
public:
	CSuggestionsDlg(CWnd* pParentWnd = NULL);
	~CSuggestionsDlg();

	short SetSuggestions(short maxSuggested, 
		const CSArray<CStringDWORDPair, CStringDWORDPair&>& words, 
		short& height);
	void ChangeFont(CFont* newFont);

// Dialog Data
	//{{AFX_DATA(CSuggestionsDlg)
	enum { IDD = IDD_SUGGEST_WORDS };
	//}}AFX_DATA
	CStatic	m_Static[MAX_SUGGESTIONS];
	COLORREF foreColor, backColor;

	CPoint m_ptInitial;		// used to determine if the user has moved the window
	SuggestionPosition m_nPosition;

	// Set if the user has selected a suggestion by clicking on it.
	// We need this info so we can place the suggestion window at 
	// the same place the next time.
	bool m_bMouseSelection;

// Overrides
	BOOL ShowWindow(int nCmdShow, SuggestionPosition nPosition=POS_UNDEF, int iTimeout=0);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuggestionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush *brush;
	CFont *curFont;
	UINT m_iTimer;
	HWND m_hPrevWnd;

	// Generated message map functions
	//{{AFX_MSG(CSuggestionsDlg)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPosCaret();
	afx_msg void OnPosCursor();
	afx_msg void OnPosFixed();
	afx_msg void OnWord1();
	afx_msg void OnWord2();
	afx_msg void OnWord3();
	afx_msg void OnWord4();
	afx_msg void OnWord5();
	afx_msg void OnWord6();
	afx_msg void OnWord7();
	afx_msg void OnWord8();
	afx_msg void OnWord9();
	//}}AFX_MSG
	afx_msg void OnEnterSizeMove();
	afx_msg void OnExitSizeMove();
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CPositionDlg dialog

class CPositionDlg : public CDialog
{
// Construction
public:
	CPositionDlg(SuggestionPosition nPosition, CWnd* pParent = NULL);   // standard constructor

	SuggestionPosition GetSettings() const { return SuggestionPosition(m_nPosition); }

// Dialog Data
	//{{AFX_DATA(CPositionDlg)
	enum { IDD = IDD_POSITION };
	int m_nPosition;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPositionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPositionDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
