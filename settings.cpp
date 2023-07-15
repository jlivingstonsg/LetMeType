// settings.cpp : implementation file
// $Id: settings.cpp,v 1.2 2002/05/20 21:33:25 cc Exp $
//

#include "stdafx.h"
#include "AutoType.h"
#include "settings.h"
#include "UrlDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable: 4800)

/////////////////////////////////////////////////////////////////////////////
// CPPgSuggestions property page

IMPLEMENT_DYNCREATE(CPPgSuggestions, CPropertyPage)

CPPgSuggestions::CPPgSuggestions() : CPropertyPage(CPPgSuggestions::IDD)
{
	//{{AFX_DATA_INIT(CPPgSuggestions)
	m_caseSensitive = FALSE;
	m_numpad = FALSE;
	m_bAutoHide = TRUE;
	m_iTimeout = 0;
	m_iPosition = -1;
	//}}AFX_DATA_INIT
}

void CPPgSuggestions::Init(const DocSettings s)
{
	m_psp.dwFlags |= PSP_HASHELP;
	m_given = s.minGiven;
	m_added = s.minAdded;
	m_probability = s.howCertain;
	m_speed = s.minTypeTime;
	m_single = !s.multiWords;
	m_signal = s.ringBell;
	m_addSpace = s.addSpace;
	m_maxSuggested = s.maxSuggested;
	m_caseSensitive = s.caseSensitive;
	m_numpad = s.numpad;
	m_iTimeout = s.suggestionTimeout;
	m_iPosition = s.suggestPos;
}

void CPPgSuggestions::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPgSuggestions)
	DDX_Control(pDX, IDC_MAXSUGGESTED_SPIN, m_maxSuggestedSpin);
	DDX_Control(pDX, IDC_ADDED_SPIN, m_addedSpin);
	DDX_Control(pDX, IDC_GIVEN_SPIN, m_givenSpin);
	DDX_Text(pDX, IDC_GIVEN, m_given);
	DDV_MinMaxUInt(pDX, m_given, 1, 99);
	DDX_Text(pDX, IDC_ADDED, m_added);
	DDV_MinMaxUInt(pDX, m_added, 1, 99);
	DDX_Text(pDX, IDC_PROBABILITY, m_probability);
	DDV_MinMaxUInt(pDX, m_probability, 0, 100);
	DDX_Text(pDX, IDC_SPEED, m_speed);
	DDV_MinMaxUInt(pDX, m_speed, 0, 2000);
	DDX_Text(pDX, IDC_MAXSUGGESTED, m_maxSuggested);
	DDV_MinMaxUInt(pDX, m_maxSuggested, 1, 9);
	DDX_Check(pDX, IDC_SINGLE, m_single);
	DDX_Check(pDX, IDC_SIGNAL, m_signal);
	DDX_Check(pDX, IDC_SPACE, m_addSpace);
	DDX_Check(pDX, IDC_CASE, m_caseSensitive);
	DDX_Check(pDX, IDC_NUMPAD, m_numpad);
	DDX_Check(pDX, IDC_AUTOHIDE, m_bAutoHide);
	DDX_Text(pDX, IDC_TIMEOUT, m_iTimeout);
	DDV_MinMaxUInt(pDX, m_iTimeout, 0, 60000);
	DDX_CBIndex(pDX, IDC_POSITION, m_iPosition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPgSuggestions, CPropertyPage)
	//{{AFX_MSG_MAP(CPPgSuggestions)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_AUTOHIDE, OnAutohide)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPgSuggestions message handlers


BOOL CPPgSuggestions::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_givenSpin.SetRange(1, 99);
	m_addedSpin.SetRange(1, 99);
	m_maxSuggestedSpin.SetRange(1, 9);

	if (!m_iTimeout) {
		GetDlgItem(IDC_TIMEOUT)->EnableWindow(FALSE);
		m_bAutoHide = FALSE; UpdateData(FALSE);
	}

	return TRUE;
}

void CPPgSuggestions::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_F1)
		theApp.WinHelp(1, HELP_CONTEXT);
	else
		CPropertyPage::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPPgSuggestions::OnAutohide() 
{
	UpdateData();
	GetDlgItem(IDC_TIMEOUT)->EnableWindow(m_bAutoHide);
}


/////////////////////////////////////////////////////////////////////////////
// CPPgDB property page

IMPLEMENT_DYNCREATE(CPPgDB, CPropertyPage)

CPPgDB::CPPgDB() : CPropertyPage(CPPgDB::IDD)
{
	//{{AFX_DATA_INIT(CPPgDB)
	m_bAllowNumbers = FALSE;
	//}}AFX_DATA_INIT
}

void CPPgDB::Init(const DocSettings s)
{
	m_psp.dwFlags |= PSP_HASHELP;
	m_age = s.maxAge;
	m_size = s.maxSize;
	m_autoSave = s.autoSave;
	m_fixedVocabulary = s.fixedVocabulary;
	m_ignorePasswords = s.ignorePasswords;
	m_bAllowNumbers = s.allowNumbers;
}

void CPPgDB::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPgDB)
	DDX_Control(pDX, IDC_SIZE_SLIDER, m_sizeSlider);
	DDX_Control(pDX, IDC_AGE_SLIDER, m_ageSlider);
	DDX_Text(pDX, IDC_AGE, m_age);
	DDV_MinMaxUInt(pDX, m_age, 1, 720);
	DDX_Text(pDX, IDC_DBSIZE, m_size);
	DDV_MinMaxUInt(pDX, m_size, 1, 20000);
	DDX_Check(pDX, IDC_AUTOSAVE, m_autoSave);
	DDX_Check(pDX, IDC_READONLY, m_fixedVocabulary);
	DDX_Check(pDX, IDC_IGNORE_PASSWORDS, m_ignorePasswords);
	DDX_Check(pDX, IDC_ALLOW_NUMBERS, m_bAllowNumbers);
	//}}AFX_DATA_MAP
}

bool CPPgDB::IsOverURL(CPoint point) const
{
	CWnd* pWnd = GetDlgItem(IDC_URL); 
	if (pWnd) {
		ClientToScreen(&point);
		CRect r; pWnd->GetWindowRect(&r);
		return r.PtInRect(point);
	}
	else return false;
}


BEGIN_MESSAGE_MAP(CPPgDB, CPropertyPage)
	//{{AFX_MSG_MAP(CPPgDB)
	ON_WM_KEYDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPgDB message handlers

BOOL CPPgDB::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_ageSlider.SetRange(1, DB_MAXAGE);
	m_ageSlider.SetTicFreq(90);
	m_ageSlider.SetPos(m_age);
	m_sizeSlider.SetRange(1, DB_MAXSIZE);
	m_sizeSlider.SetTicFreq(2000);
	m_sizeSlider.SetPos(m_size);

	return TRUE;
}

void CPPgDB::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	char s[6];
	sprintf(s, "%d", nPos);
	switch (nSBCode)
	{
		case TB_BOTTOM:
		case TB_TOP:
		case TB_LINEDOWN:
		case TB_LINEUP:
		case TB_PAGEDOWN:
		case TB_PAGEUP:
			nPos = ((CSliderCtrl *)pScrollBar)->GetPos();
			sprintf(s, "%d", nPos);
		case TB_THUMBTRACK:
			if (pScrollBar->GetDlgCtrlID() == IDC_AGE_SLIDER) {
				GetDlgItem(IDC_AGE)->SetWindowText(s);
				return;
			}
			else if (pScrollBar->GetDlgCtrlID() == IDC_SIZE_SLIDER) {
				GetDlgItem(IDC_DBSIZE)->SetWindowText(s);
				return;
			}
	}

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPPgDB::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_F1)
		theApp.WinHelp(1, HELP_CONTEXT);
	else
		CPropertyPage::OnKeyDown(nChar, nRepCnt, nFlags);
}

HBRUSH CPPgDB::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH result = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_URL)
		pDC->SetTextColor(URL_COLOR);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg dialog

CSettingsDlg::CSettingsDlg(const DocSettings s, CWnd* pParent)
	: CPropertySheet(IDS_SETTINGS, pParent)
{
	settings = s;

	suggestions.Init(s);
	db.Init(s);
	AddPage(&suggestions);
	AddPage(&db);

	//{{AFX_DATA_INIT(CSettingsDlg)
	//}}AFX_DATA_INIT
}


void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertySheet::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsDlg)
	//}}AFX_DATA_MAP
}

DocSettings CSettingsDlg::GetSettings()
{
	settings.minGiven = suggestions.m_given;
	settings.minAdded = suggestions.m_added;
	settings.howCertain = suggestions.m_probability;
	settings.minTypeTime = suggestions.m_speed;
	settings.multiWords = !suggestions.m_single;
	settings.ringBell = suggestions.m_signal;
	settings.maxSuggested = suggestions.m_maxSuggested;
	settings.addSpace = suggestions.m_addSpace;
	settings.caseSensitive = suggestions.m_caseSensitive;
	settings.numpad = suggestions.m_numpad;
	settings.suggestionTimeout = suggestions.m_bAutoHide ? suggestions.m_iTimeout : 0;
	settings.suggestPos = SuggestionPosition(suggestions.m_iPosition);

	settings.maxAge = db.m_age;
	settings.maxSize = db.m_size;
	settings.autoSave = db.m_autoSave;
	settings.fixedVocabulary = db.m_fixedVocabulary;
	settings.ignorePasswords = db.m_ignorePasswords;
	settings.allowNumbers = db.m_bAllowNumbers;

	return settings;
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CPropertySheet)
	//{{AFX_MSG_MAP(CSettingsDlg)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg message handlers

void CSettingsDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_F1)
		theApp.WinHelp(1, HELP_CONTEXT);
	else
		CPropertySheet::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSettingsDlg::OnHelp() 
{
	theApp.WinHelp(1, HELP_CONTEXT);
}
