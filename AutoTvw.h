// AutoTvw.h : interface of the CAutoTypeView class
// $Id: AutoTvw.h,v 1.1.1.1 2002/05/12 14:40:55 cc Exp $
//
/////////////////////////////////////////////////////////////////////////////

#include <afxcview.h>

#define DAY_SECONDS	(24*60*60)

typedef CArray<int, int> CIntArray;

enum SortBy { SB_ALPHA, SB_OCCURENCES, SB_AGE, SB_LENGTH };


class CAutoTypeView : public CListView
{
protected: // create from serialization only
	CAutoTypeView();
	DECLARE_DYNCREATE(CAutoTypeView)

// Attributes
public:
	CAutoTypeDoc* GetDocument();

// Operations
public:
	BOOL AddItem(int nItem, int nSubItem, const char* strItem,
				 int nImageIndex = -1);
	void AddWord(int wordIdx, const CWordInfo* wordInfo);
	void RemoveWord(int wordIdx);
	void RemoveAll();

	void GetSelected(CIntArray& selected) const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoTypeView)
	public:
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int nextID;	// used for making a unique key in map listData
	SortBy curSort;	// by which criterium the words are currently sorted
	int descending;
	DWORD wndStyle;

	void Sort();
	void ContextMenu(POINT* pos = NULL);

// Generated message map functions
protected:
	//{{AFX_MSG(CAutoTypeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewList();
	afx_msg void OnViewDetails();
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDelete();
	afx_msg void OnEditEdit();
	afx_msg void OnUpdateViewList(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDetails(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEdit(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnViewSort(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AutoTvw.cpp
inline CAutoTypeDoc* CAutoTypeView::GetDocument()
   { return (CAutoTypeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
