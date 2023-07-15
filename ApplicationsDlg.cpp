// ApplicationsDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "autotype.h"
#include "ApplicationsDlg.h"
#include "NewApplicationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CApplicationsDlg 


CApplicationsDlg::CApplicationsDlg(
	CStringArray &positiveApplications, 
	CStringArray &negativeApplications,
	bool bPositiveApplications, 
	bool bNegativeApplications
	) : CDialog(CApplicationsDlg::IDD)
{
	//{{AFX_DATA_INIT(CApplicationsDlg)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT

	m_positiveApplications.Copy(positiveApplications);
	m_negativeApplications.Copy(negativeApplications);
	m_bPositiveApplications = bPositiveApplications;
	m_bNegativeApplications = bNegativeApplications;
}


void CApplicationsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CApplicationsDlg)
	DDX_Control(pDX, IDC_NEW, m_new);
	DDX_Control(pDX, IDC_REMOVE, m_remove);
	DDX_Control(pDX, IDC_ALL, m_all);
	DDX_Control(pDX, IDC_POSITIVE, m_positive);
	DDX_Control(pDX, IDC_NEGATIVE, m_negative);
	DDX_Control(pDX, IDC_APPL_LIST, m_applications);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CApplicationsDlg, CDialog)
	//{{AFX_MSG_MAP(CApplicationsDlg)
	ON_BN_CLICKED(IDC_ALL, OnAll)
	ON_BN_CLICKED(IDC_NEGATIVE, OnNegative)
	ON_BN_CLICKED(IDC_NEW, OnNew)
	ON_BN_CLICKED(IDC_POSITIVE, OnPositive)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_LBN_SELCHANGE(IDC_APPL_LIST, OnSelchangeApplList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CApplicationsDlg 

void CApplicationsDlg::OnNew() 
{
	CNewApplicationDlg dlg;

	if (dlg.DoModal() == IDOK && !dlg.m_filenameValue.IsEmpty())
	{
		if (m_bPositiveApplications)
			m_positiveApplications.Add(dlg.m_filenameValue);
		else
			m_negativeApplications.Add(dlg.m_filenameValue);

		m_applications.AddString(dlg.m_filenameValue);
	}
}

void CApplicationsDlg::OnRemove() 
{
	// We call GetSelItems after each removal because
	// the removal might have changed the item indexes.

	int selItems[256], selCount;
	while (selCount = m_applications.GetSelItems(256, selItems))
	{
		// Remove the item from the CStringArray.
		CString itemText;
		m_applications.GetText(selItems[0], itemText);

		CStringArray *applications;
		if (m_bPositiveApplications)
			applications = &m_positiveApplications;
		else
			applications = &m_negativeApplications;

		for (int j = 0; j < applications->GetSize(); j++)
			if (applications->GetAt(j) == itemText)
			{
				applications->RemoveAt(j);
				break;
			}

		// Remove the item from the listbox.
		m_applications.DeleteString(selItems[0]);
	}

	m_remove.EnableWindow(FALSE);
}

void CApplicationsDlg::OnAll() 
{
	m_bPositiveApplications = false;
	m_bNegativeApplications = false;

	m_applications.EnableWindow(FALSE);
	m_new.EnableWindow(FALSE);
	m_remove.EnableWindow(FALSE);
}

void CApplicationsDlg::OnPositive() 
{
	m_bPositiveApplications = true;
	m_bNegativeApplications = false;

	m_applications.ResetContent();
	for (int i = 0; i < m_positiveApplications.GetSize(); i++)
		m_applications.AddString(m_positiveApplications.GetAt(i));

	m_applications.EnableWindow();
	m_new.EnableWindow();
	m_remove.EnableWindow(FALSE);
}

void CApplicationsDlg::OnNegative() 
{
	m_bPositiveApplications = false;
	m_bNegativeApplications = true;

	m_applications.ResetContent();
	for (int i = 0; i < m_negativeApplications.GetSize(); i++)
		m_applications.AddString(m_negativeApplications.GetAt(i));

	m_applications.EnableWindow();
	m_new.EnableWindow();
	m_remove.EnableWindow(FALSE);
}

BOOL CApplicationsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Get applications path from theDoc and check
	// which of the radio buttons should be active.
	CStringArray *applications = NULL;
	if (m_bPositiveApplications)
	{
		applications = &m_positiveApplications;
		m_positive.SetCheck(1);
	}
	else if (m_bNegativeApplications)
	{
		applications = &m_negativeApplications;
		m_negative.SetCheck(1);
	}
	else
	{
		m_all.SetCheck(1);
		m_applications.EnableWindow(FALSE);
		m_new.EnableWindow(FALSE);
	}

	if (applications)
		for (int i = 0; i < applications->GetSize(); i++)
			m_applications.AddString(applications->GetAt(i));
	
	return TRUE;
}

void CApplicationsDlg::OnSelchangeApplList() 
{
	if (m_applications.GetSelCount())
		m_remove.EnableWindow();
	else
		m_remove.EnableWindow(FALSE);
}
