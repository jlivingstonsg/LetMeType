// ColorDlg.h : header file
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog

class CColorDlg : public CColorDialog
{
	DECLARE_DYNAMIC(CColorDlg)

public:
	CColorDlg(UINT titleId, COLORREF clrInit = 0, DWORD dwFlags = 0, CWnd* pParentWnd = NULL);

protected:
	//{{AFX_MSG(CColorDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	UINT titleId;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
