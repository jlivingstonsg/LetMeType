// charsdlg.cpp : implementation file
// $Id: charsdlg.cpp,v 1.1.1.1 2002/05/12 14:40:55 cc Exp $
//

#include "stdafx.h"
#include "AutoType.h"
#include "charsdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable: 4800)

static const char chars[] = "_-'.:/\\&#$%@*+=^|~×÷";

/////////////////////////////////////////////////////////////////////////////
// CCharsDialog dialog


CCharsDialog::CCharsDialog(const DocSettings s, CWnd* pParent /*=NULL*/)
	: CDialog(CCharsDialog::IDD, pParent)
{
	settings = s;
	//{{AFX_DATA_INIT(CCharsDialog)
	m_digits = s.allowDigits;
	m_brackets = s.allowBrackets;
	//}}AFX_DATA_INIT
	memset(m_chars, 0, sizeof(m_chars));
	for (BYTE i = 0; i < CHARS_COUNT; i++)
		if (strchr(s.wordChars, chars[i]))
			m_chars[i] = TRUE;
}

DocSettings CCharsDialog::GetSettings()
{
	settings.allowDigits = m_digits;
	settings.allowBrackets = m_brackets;
	for (BYTE i = 0, l = 0; i < CHARS_COUNT; i++)
		if (m_chars[i]) settings.wordChars[l++] = chars[i];
	settings.wordChars[l] = 0;

	return settings;
}

void CCharsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharsDialog)
	DDX_Check(pDX, IDC_DIGITS, m_digits);
	DDX_Check(pDX, IDC_BRACKETS, m_brackets);
	//}}AFX_DATA_MAP
	for (BYTE i = 0; i < CHARS_COUNT; i++)
		DDX_Check(pDX, IDC_CHAR1+i, m_chars[i]);
}


BEGIN_MESSAGE_MAP(CCharsDialog, CDialog)
	//{{AFX_MSG_MAP(CCharsDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCharsDialog message handlers
