// $Id: suggestw.cpp,v 1.3 2003/01/25 22:02:36 cc Exp $
//

#include "stdafx.h"

#include "AutoType.h"
#include "AutoTDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Collection helpers

// Sort pairs in descending order
int AFXAPI SortElements(const CStringDWORDPair p1, const CStringDWORDPairR p2)
{
	if (p1.w < p2.w) return 1;
	else if (p1.w == p2.w) return 0;
	else return -1;
}

void AFXAPI DestructElements(CStringDWORDPair* p, int nCount)
{
	for (int i = 0; i < nCount; i++)
		p[i].s.Empty();
}

#ifdef _DEBUG
void AFXAPI DumpElements(CDumpContext& dc, const CStringDWORDPair* pElements, int nCount)
{
	if (nCount)
	{
		ASSERT(AfxIsValidAddress(pElements, nCount * sizeof(CStringDWORDPair)));

		for (int i = 0; i < nCount; i++)
			dc << pElements[i].s << " " << pElements[i].w << "\n";
	}
}
#endif


/////////////////////////////////////////////////////////////////////////////
// CSuggestionsDlg dialog

CSuggestionsDlg::CSuggestionsDlg(CWnd* pParentWnd)
 : CDialog(CSuggestionsDlg::IDD, pParentWnd)
{
	brush = NULL;
	curFont = NULL;
	m_nPosition = POS_CARET;
	m_iTimer = 0;
	m_hPrevWnd = NULL;
	m_bMouseSelection = false;

	Create(CSuggestionsDlg::IDD, pParentWnd);
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}

CSuggestionsDlg::~CSuggestionsDlg()
{
	if (brush) delete brush;
	if (curFont) delete curFont;
	if (m_iTimer) KillTimer(m_iTimer);
}

void CSuggestionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSuggestionsDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_WORD1, m_Static[0]);
	DDX_Control(pDX, IDC_WORD2, m_Static[1]);
	DDX_Control(pDX, IDC_WORD3, m_Static[2]);
	DDX_Control(pDX, IDC_WORD4, m_Static[3]);
	DDX_Control(pDX, IDC_WORD5, m_Static[4]);
	DDX_Control(pDX, IDC_WORD6, m_Static[5]);
	DDX_Control(pDX, IDC_WORD7, m_Static[6]);
	DDX_Control(pDX, IDC_WORD8, m_Static[7]);
	DDX_Control(pDX, IDC_WORD9, m_Static[8]);
	ASSERT(MAX_SUGGESTIONS == 9);
}

#define ON_WM_ENTERSIZEMOVE() \
	{ WM_ENTERSIZEMOVE, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnEnterSizeMove },
#define ON_WM_EXITSIZEMOVE() \
	{ WM_EXITSIZEMOVE, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnExitSizeMove },

BEGIN_MESSAGE_MAP(CSuggestionsDlg, CDialog)
	//{{AFX_MSG_MAP(CSuggestionsDlg)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_COMMAND(ID_POS_CARET, OnPosCaret)
	ON_COMMAND(ID_POS_CURSOR, OnPosCursor)
	ON_COMMAND(ID_POS_FIXED, OnPosFixed)
	ON_BN_CLICKED(IDC_WORD1, OnWord1)
	ON_BN_CLICKED(IDC_WORD2, OnWord2)
	ON_BN_CLICKED(IDC_WORD3, OnWord3)
	ON_BN_CLICKED(IDC_WORD4, OnWord4)
	ON_BN_CLICKED(IDC_WORD5, OnWord5)
	ON_BN_CLICKED(IDC_WORD6, OnWord6)
	ON_BN_CLICKED(IDC_WORD7, OnWord7)
	ON_BN_CLICKED(IDC_WORD8, OnWord8)
	ON_BN_CLICKED(IDC_WORD9, OnWord9)
	//}}AFX_MSG_MAP
	ON_WM_ENTERSIZEMOVE()
	ON_WM_EXITSIZEMOVE()
END_MESSAGE_MAP()


/** Set the words to be displayed in the suggestion window.
 *	Also determine how wide the window will have to be.
 *
 * @param maxSuggested	Indicates the user controlled limit for suggestions.
 * @param words			Contains the suggestions ordered by probability.
 * @result height		The row height.
 * @result				The necessary window width.
 */
short CSuggestionsDlg::SetSuggestions(short maxSuggested, 
	const CSArray<CStringDWORDPair, CStringDWORDPair&>& words,
	short& height)
{
	static short lastCount = MAX_SUGGESTIONS;
	short nSuggestions = min(maxSuggested, words.GetSize());
	CClientDC dc(&m_Static[0]);
	CFont* fnt = m_Static[0].GetFont();
	dc.SelectObject(fnt);
	LOGFONT lf;
	fnt->GetLogFont(&lf);

	// For large fonts, we reduce the height by 20%-25%
	// to improve the dialog's layout.
	double factor;
	if (abs(lf.lfHeight) <= 10) factor = 1;
	else if (abs(lf.lfHeight) < 20) factor = 0.75;
	else factor = 0.7;
	height = short(float(abs(lf.lfHeight)) * factor);

	char s[MAX_WORD_LEN*2+4];
	short maxWidth = 0;
	for (short i = 0; i < nSuggestions; i++)
	{
		ASSERT(words[i].s.GetLength() < MAX_WORD_LEN*2);
		CString word;
		if (!word.Find('&')) word = words[i].s;
		else
			for (int j = 0; j < words[i].s.GetLength(); j++) {
				word += words[i].s[j];
				if (words[i].s[j] == '&') word += '&';
			}
		sprintf(s, "&%d: %s", i+1, word);
		m_Static[i].SetWindowText(s);

		// Determine suggestion's width.
		CSize size = dc.GetTextExtent(s).cx;
		if (size.cx > maxWidth) maxWidth = short(size.cx);

		// Make static text visible if needed.
		if (i >= lastCount) m_Static[i].ShowWindow(SW_SHOW);
	}
	for (; i < lastCount; i++) m_Static[i].ShowWindow(SW_HIDE);
	lastCount = nSuggestions;

	return maxWidth;
}

/** Change the font used for displaying the suggestions,
 *  and deletes the old font.
 *
 * @param newFont	New font to be used. Deletion is handled
 *					by CSuggestionsDlg.
 */
void CSuggestionsDlg::ChangeFont(CFont* newFont)
{
	for (short i = 0; i < MAX_SUGGESTIONS; i++)
		m_Static[i].SetFont(newFont, FALSE);
	if (curFont) delete curFont;
	curFont = newFont;
}

BOOL CSuggestionsDlg::ShowWindow(int nCmdShow, SuggestionPosition nPosition, int iTimeout)
{
	if (m_iTimer) { KillTimer(m_iTimer); m_iTimer = 0; }
	if (nCmdShow != SW_HIDE && iTimeout) m_iTimer = SetTimer(1, iTimeout, NULL);

	m_hPrevWnd = ::GetForegroundWindow();
	if (nPosition != POS_UNDEF) m_nPosition = nPosition;

	if (nCmdShow != SW_HIDE)
		m_bMouseSelection = false;

	return CDialog::ShowWindow(nCmdShow);
}


/////////////////////////////////////////////////////////////////////////////
// CSuggestionsDlg message handlers

BOOL CSuggestionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (theApp.fontSize)
	{
		CFont* font = new CFont;
		if (font->CreatePointFont(theApp.fontSize, theApp.fontName)) {
			ChangeFont(font);
			curFont = font;
		}
		else delete font;
	}

	return TRUE;
}

/** Special version of OnEraseBkgnd to allow for custom background color.
 *
 * @param pDC		Specifies the device-context object.
 * @result			Nonzero to indicate that the background 
 *					has been erased.
 */
BOOL CSuggestionsDlg::OnEraseBkgnd(CDC* pDC) 
{
	CRect clientRect;
	GetClientRect(&clientRect);
	pDC->FillRect(clientRect, &CBrush(backColor));
	return TRUE;
}

HBRUSH CSuggestionsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
			pDC->SetBkColor(backColor);
			pDC->SetTextColor(foreColor);
			pDC->SetBkMode(TRANSPARENT);
			if (brush) delete brush;
			brush = new CBrush(backColor);
			return *brush;

		default:
			return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}

void CSuggestionsDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if (m_iTimer) { KillTimer(m_iTimer); m_iTimer = 0; }

	POINT p;
	GetCursorPos(&p);

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_SUGGESTIONS));
	CMenu* popup = menu.GetSubMenu(0);
	ASSERT(popup != NULL);

	popup->CheckMenuItem(ID_OPTIONS_SUGGEST, theApp.m_bSuggest ? MF_CHECKED:MF_UNCHECKED);
	popup->CheckMenuItem(ID_OPTIONS_LEARN, theApp.m_bLearn ? MF_CHECKED:MF_UNCHECKED);

	switch (m_nPosition)
	{
		case POS_CARET:
			popup->CheckMenuItem(ID_POS_CARET, MF_CHECKED);
			break;
		case POS_FIXED:
			popup->CheckMenuItem(ID_POS_FIXED, MF_CHECKED);
			break;
		case POS_CURSOR:
			popup->CheckMenuItem(ID_POS_CURSOR, MF_CHECKED);
	}

	popup->TrackPopupMenu(TPM_RIGHTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
		p.x, p.y, this);
}


void CSuggestionsDlg::OnEnterSizeMove()
{
	if (m_iTimer) { KillTimer(m_iTimer); m_iTimer = 0; }
}

// When the user manually moves the window, we ask her what she wants.
void CSuggestionsDlg::OnExitSizeMove() 
{
	CPositionDlg dlg(m_nPosition, this);
	if (dlg.DoModal() == IDOK) m_nPosition = dlg.GetSettings();
	if (IsWindow(m_hPrevWnd)) VERIFY(::SetForegroundWindow(m_hPrevWnd));
}

void CSuggestionsDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1) {
		if (m_iTimer) { KillTimer(m_iTimer); m_iTimer = 0; }
		theDoc->StopSuggesting();
	}

	CDialog::OnTimer(nIDEvent);
}

void CSuggestionsDlg::OnPosCaret() 
{
	m_nPosition = POS_CARET;
}

void CSuggestionsDlg::OnPosCursor() 
{
	m_nPosition = POS_CURSOR;
}

void CSuggestionsDlg::OnPosFixed() 
{
	m_nPosition = POS_FIXED;
}

void CSuggestionsDlg::OnWord1() 
{
	m_bMouseSelection = true;
	theDoc->SendSuggestion(0, false);
}

void CSuggestionsDlg::OnWord2() 
{
	m_bMouseSelection = true;
	theDoc->SendSuggestion(1, false);
}

void CSuggestionsDlg::OnWord3() 
{
	m_bMouseSelection = true;
	theDoc->SendSuggestion(2, false);
}

void CSuggestionsDlg::OnWord4() 
{
	m_bMouseSelection = true;
	theDoc->SendSuggestion(3, false);
}

void CSuggestionsDlg::OnWord5() 
{
	m_bMouseSelection = true;
	theDoc->SendSuggestion(4, false);
}

void CSuggestionsDlg::OnWord6() 
{
	m_bMouseSelection = true;
	theDoc->SendSuggestion(5, false);
}

void CSuggestionsDlg::OnWord7() 
{
	m_bMouseSelection = true;
	theDoc->SendSuggestion(6, false);
}

void CSuggestionsDlg::OnWord8() 
{
	m_bMouseSelection = true;
	theDoc->SendSuggestion(7, false);
}

void CSuggestionsDlg::OnWord9() 
{
	m_bMouseSelection = true;
	theDoc->SendSuggestion(8, false);
}


/////////////////////////////////////////////////////////////////////////////
// CPositionDlg dialog


CPositionDlg::CPositionDlg(SuggestionPosition nPosition, CWnd* pParent /*=NULL*/)
	: CDialog(CPositionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPositionDlg)
	m_nPosition = nPosition;
	//}}AFX_DATA_INIT
}


void CPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPositionDlg)
	DDX_Radio(pDX, IDC_CARET, m_nPosition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPositionDlg, CDialog)
	//{{AFX_MSG_MAP(CPositionDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPositionDlg message handlers
