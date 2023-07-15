// AutoTvw.cpp : implementation of the CAutoTypeView class
// $Id: AutoTvw.cpp,v 1.1.1.1 2002/05/12 14:40:55 cc Exp $
//

#include "stdafx.h"
#include "AutoType.h"

#include "AutoTdoc.h"
#include "AutoTvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// Map list item data to a word's index in the document.
// The item data is set using the counter nextID, which
// is incremented for each new item.
CMap<int, int, int, int> listData;


/////////////////////////////////////////////////////////////////////////////
// Helpers

const int DESCENDING = 0x80;
int CALLBACK CompareWords(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int idx;
	if (!listData.Lookup(lParam1, idx)) { ASSERT(0); return 0; }
	const CWordInfo* info1 = theDoc->GetInfo(idx);
	if (!listData.Lookup(lParam2, idx)) { ASSERT(0); return 0; }
	const CWordInfo* info2 = theDoc->GetInfo(idx);
	if (!info1 || !info2) { ASSERT(0); return 0; }

	int result;
	switch (lParamSort & 0x7F)
	{
		case SB_ALPHA:
			result = strcmp(info1->word, info2->word);
			break;

		case SB_OCCURENCES:
			result = info1->occurences[0] + info1->occurences[1] - 
				(info2->occurences[0] + info2->occurences[1]);
			break;

		case SB_AGE:
			result = info2->lastUse - info1->lastUse;
			break;

		case SB_LENGTH:
			result = info1->word.GetLength() - info2->word.GetLength();
			break;

		default:
			ASSERT(0);
			return 0;	// dummy
	}

	return lParamSort & 0x80 ? -result : result;
}


/////////////////////////////////////////////////////////////////////////////
// CAutoTypeView

IMPLEMENT_DYNCREATE(CAutoTypeView, CListView)

BEGIN_MESSAGE_MAP(CAutoTypeView, CListView)
	//{{AFX_MSG_MAP(CAutoTypeView)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_LIST, OnViewList)
	ON_COMMAND(ID_VIEW_DETAILS, OnViewDetails)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_DELETE, OnDelete)
	ON_COMMAND(ID_EDIT_EDIT, OnEditEdit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIST, OnUpdateViewList)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILS, OnUpdateViewDetails)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EDIT, OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEdit)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VIEW_SORT_ALPHA, ID_VIEW_SORT_LENGTH, OnViewSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoTypeView construction/destruction

CAutoTypeView::CAutoTypeView()
{
	curSort = SB_ALPHA;
	descending = 0;
	nextID = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CAutoTypeView operations

int CAutoTypeView::AddItem(int nItem, int nSubItem, const char* strItem,
						   int nImageIndex)
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR) strItem;

	if(nImageIndex != -1)
	{
		lvItem.mask |= LVIF_IMAGE;
		lvItem.iImage |= LVIF_IMAGE;
	}

	if(nSubItem == 0)
		return GetListCtrl().InsertItem(&lvItem);
	else
	{
		GetListCtrl().SetItem(&lvItem);
		return nItem;
	}
}

/* -- AddWord --
   Synopsis: 
	Adds a new word to list view.

   Parameters:
	wordIdx			Word's index in the CAutoTypeDoc object.
	wordInfo		Information about the word.
*/
void CAutoTypeView::AddWord(int wordIdx, const CWordInfo* wordInfo)
{
	time_t curTime;
	time(&curTime);

	// Add word's string representation to the list.
	// Additional information is added as subitems to this item.
	CString s = wordInfo->word;
	if (wordInfo->occurences[1] > wordInfo->occurences[0]) {
		char c = char(CharUpper(LPTSTR(BYTE(s[0]))));
		if (c) s.SetAt(0, c);
	}
	int itemIdx = AddItem(wordIdx, 0, s);
	GetListCtrl().SetItemData(itemIdx, nextID);

	s.Format("%d", wordInfo->occurences[0] + wordInfo->occurences[1]);
	AddItem(itemIdx, 1, s);
	s.Format("%d", curTime/DAY_SECONDS - wordInfo->lastUse);
	AddItem(itemIdx, 2, s);
	
	// Update ID map.
	int key, val;
	for (POSITION pos = listData.GetStartPosition(); pos;)
	{
		listData.GetNextAssoc(pos, key, val);
		if (val >= wordIdx) listData[key] = val+1;
	}

	// Add new ID to map.
	listData[nextID++] = wordIdx;
}

void CAutoTypeView::RemoveWord(int wordIdx)
{
	// Update ID map.
	int key, val, thisKey;
	for (POSITION pos = listData.GetStartPosition(); pos;)
	{
		listData.GetNextAssoc(pos, key, val);
		if (val > wordIdx) listData[key] = val-1;
		else if (val == wordIdx) thisKey = key;
	}

	LV_FINDINFO findInfo;
	findInfo.flags = LVFI_PARAM;
	findInfo.lParam = thisKey;
	GetListCtrl().DeleteItem(GetListCtrl().FindItem(&findInfo));

	listData.RemoveKey(thisKey);
}

void CAutoTypeView::RemoveAll()
{
	listData.RemoveAll();
	GetListCtrl().DeleteAllItems();
}

void CAutoTypeView::GetSelected(CIntArray& selected) const
{
	int itemIdx = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	while (itemIdx != -1)
	{
		int wordIdx;
		VERIFY(listData.Lookup(GetListCtrl().GetItemData(itemIdx), wordIdx));
		selected.Add(wordIdx);
		itemIdx = GetListCtrl().GetNextItem(itemIdx, LVNI_SELECTED);
	}
}

void CAutoTypeView::Sort()
{
	BeginWaitCursor();
	GetListCtrl().SortItems(CompareWords, curSort | descending);
	EndWaitCursor();
}


void CAutoTypeView::ContextMenu(POINT* pos)
{
	if (GetListCtrl().GetNextItem(-1, LVNI_SELECTED) == -1) return;

	POINT p;
	if (pos) p = *pos;
	else {
		RECT r;
		GetWindowRect(&r);
		p.x = r.left + (r.right - r.left)/2;
		p.y = r.top + (r.bottom - r.top)/2;
	}

	CMenu menu;
	if (!menu.LoadMenu(IDR_POPUP_LIST)) { ASSERT(0); return; }
	CMenu* popup = menu.GetSubMenu(0);
	if (!popup) { ASSERT(0); return; }

	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, p.x, p.y, this);
}


/////////////////////////////////////////////////////////////////////////////
// CAutoTypeView diagnostics

#ifdef _DEBUG
void CAutoTypeView::AssertValid() const
{
	CListView::AssertValid();
}

void CAutoTypeView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CAutoTypeDoc* CAutoTypeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAutoTypeDoc)));
	return (CAutoTypeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAutoTypeView message handlers

int CAutoTypeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= LVS_REPORT | LVS_SORTASCENDING | LVS_EDITLABELS;
	if (CListView::OnCreate(lpCreateStruct) == -1) return -1;

	wndStyle = LVS_REPORT;
	GetDocument()->view = this;

	CString s;
	s.LoadString(IDS_OCCURENCES);
	short occWidth = GetListCtrl().GetStringWidth(s) + 15;
	GetListCtrl().InsertColumn(1, s, LVCFMT_RIGHT, occWidth);
	s.LoadString(IDS_AGE);
	short ageWidth = GetListCtrl().GetStringWidth(s) + 15;
	GetListCtrl().InsertColumn(2, s, LVCFMT_RIGHT, ageWidth);
	RECT rect;
	GetClientRect(&rect);
	s.LoadString(IDS_WORD);
	short wordWidth = 
		max(GetListCtrl().GetStringWidth(s) + 15,
			rect.right - occWidth - ageWidth - GetSystemMetrics(SM_CXVSCROLL));
	GetListCtrl().InsertColumn(0, s, LVCFMT_LEFT, wordWidth);

	return 0;
}

void CAutoTypeView::OnViewList() 
{
	wndStyle = LVS_LIST;
	ModifyStyle(LVS_REPORT, LVS_LIST);
	CMenu* menu = theApp.m_pMainWnd->GetMenu();
	menu->CheckMenuRadioItem(ID_VIEW_LIST, ID_VIEW_DETAILS, 
		ID_VIEW_LIST, MF_BYCOMMAND);
}

void CAutoTypeView::OnViewDetails() 
{
	wndStyle = LVS_REPORT;
	ModifyStyle(LVS_LIST, LVS_REPORT);	
	CMenu* menu = theApp.m_pMainWnd->GetMenu();
	menu->CheckMenuRadioItem(ID_VIEW_LIST, ID_VIEW_DETAILS, 
		ID_VIEW_DETAILS, MF_BYCOMMAND);
}

void CAutoTypeView::OnUpdateViewList(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(wndStyle == LVS_LIST ? TRUE:FALSE);
}

void CAutoTypeView::OnUpdateViewDetails(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(wndStyle == LVS_REPORT ? TRUE:FALSE);
}

/* -- OnColumnclick --
   Synopsis: 
	Called when the user has clicked on a column heading of
	the words list. The list is reordered according to the
	selected column.

   Parameters:
	pNMHDR			The iSubItem member indicates which
					column has been selected.
	pResult			Used to return a result code.
*/
void CAutoTypeView::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (curSort == pNMListView->iSubItem)
		descending ^= DESCENDING;
	else
		descending = 0;
	curSort = SortBy(pNMListView->iSubItem);

	Sort();
	
	*pResult = 0;
}

void CAutoTypeView::OnViewSort(UINT nID) 
{
	SortBy sort = SortBy(nID - ID_VIEW_SORT_ALPHA);

	if (curSort != sort || descending) {
		curSort = sort;
		descending = (sort == SB_LENGTH ? DESCENDING : 0);
		Sort();
	}
}

void CAutoTypeView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);

	// Automatically adjust column widths.
	CString s;
	s.LoadString(IDS_OCCURENCES);
	short occWidth = GetListCtrl().GetStringWidth(s) + 15;
	s.LoadString(IDS_AGE);
	short ageWidth = GetListCtrl().GetStringWidth(s) + 15;
	s.LoadString(IDS_WORD);
	short wordWidth = 
		max(GetListCtrl().GetStringWidth(s) + 15,
			cx - occWidth - ageWidth);
	
	GetListCtrl().SetColumnWidth(0, wordWidth);
	GetListCtrl().SetColumnWidth(1, occWidth);
	GetListCtrl().SetColumnWidth(2, ageWidth);

	GetParentFrame()->SendMessage(WM_SIZE, cx, cy);
}


/* -- OnEditEdit --
   Synopsis:
	Called when the user has selected the Edit - Edit menu entry.
	Starts editing of the first selected label.
*/
void CAutoTypeView::OnEditEdit() 
{
	GetListCtrl().EditLabel(GetListCtrl().GetNextItem(-1, LVNI_SELECTED));
}


void CAutoTypeView::OnUpdateEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetListCtrl().GetNextItem(-1, LVNI_SELECTED) != -1);
}


/* -- OnEndLabelEdit --
   Synopsis: 
	Called when the user is done editing a word. Checks if the
	modified word is unique, contains only valid characters and
	then stores it at the right place in the database.

   Parameters:
	pNMHDR			The item.pszText contains the word as
					entered by the user.
	pResult			Used to return a result code.
*/
void CAutoTypeView::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDI = (LV_DISPINFO*)pNMHDR;
	*pResult = FALSE;

	// If the user has canceled the editing, return immediately.
	if (!pDI->item.pszText) return;

	int wordIdx;
	VERIFY(listData.Lookup(pDI->item.lParam, wordIdx));

	*pResult = theDoc->ChangeWord(wordIdx, pDI->item.pszText);
}


void CAutoTypeView::OnDelete() 
{
	CIntArray selected;
	GetSelected(selected);

	for (int i = 0; i < selected.GetSize(); i++)
	{
		RemoveWord(selected[i]);
		theDoc->RemoveWord(selected[i]);

		// Update indices that have changed due to the last removal.
		for (int j = i+1; j < selected.GetSize(); j++)
			if (selected[j] > selected[i])
				selected[j]--;
	}
}

void CAutoTypeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_DELETE) OnDelete();
	else if (nChar == VK_APPS) ContextMenu();
	else if (nChar == 'A' && (GetKeyState(VK_CONTROL) & 0x80)) {
		CListCtrl& list = GetListCtrl();
		for (int i = 0; i < list.GetItemCount(); i++)
			list.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
	else CListView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CAutoTypeView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();

	if (!theApp.m_pMainWnd) return;

	CMenu* menu = theApp.m_pMainWnd->GetMenu();
	menu->CheckMenuRadioItem(ID_VIEW_LIST, ID_VIEW_DETAILS, 
		ID_VIEW_DETAILS, MF_BYCOMMAND);
}

void CAutoTypeView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CListView::OnRButtonDown(nFlags, point);

	POINT p;
	GetCursorPos(&p);
	ContextMenu(&p);
}
