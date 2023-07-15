// $Id: AutoTdoc.cpp,v 1.5 2003/01/25 22:02:36 cc Exp $
//

#include "stdafx.h"
#include <mmsystem.h>
#include <afxcview.h>
#include "shortcut.h"
#include "AutoType.h"
#include "AutoTDoc.h"
#include "AutoTvw.h"
#include "soundx.h"
#include "crc16.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable: 4800)

const short MAX_LAG = 1000;		// max age of msg in milliseconds
const long TIMER_SPAN = 10*60*1000L;
const long SHORT_TIMER_SPAN = 30*1000;
const char* DEFAULT_CHARS = 
    "ˆ÷‰ƒ¸‹ﬂ¿¡¬√≈∆«»… ÀÃÕŒœ–—“”‘’ÿŸ⁄€›‡·‚„ÂÊÁËÈÍÎÏÌÓÔÒÚÛÙı¯˘˙˚˝ˇﬁ";

const short CUR_FILE_VERSION = 13;
  /* No  Date	  Author State	Description
      0	  1. 1.96  ccl	 extc	first format with fileVersion field
      1	 30.12.96  ccl	 extc	added CWordInfo::successors
      2	 18. 1.97  ccl	 extc	added DocSettings::multiWords
      3	 19. 1.97  ccl	 extc	added CNextWord::predecessors,
								derived CNextWord from CObject
      4	 21. 2.97  ccl	 extc	added DocSettings::addSpace
	  5   4. 5.97  ccl	 extc	changed encryption algorithm
	  6  13.11.97  ccl   extc	added suggestPos to DocSettings
	  7   8. 2.98  ccl   rtrd	added fixedVocabulary to DocSettings
	  8  20.11.98  ccl   rtrd	added caseSensitive to DocSettings
	  9  14. 3.99  ccl   rtrd	added numpad to DocSettings
	 10   1. 7.99  ccl   rtrd	added ignorePasswords to DocSettings
	 11   7. 8.99  ccl   rtrd	added allowNumbers to DocSettings
	 12   5. 9.99  ccl   rtrd	added suggestionTimeout to DocSettings
	 13   9. 9.99  ccl   actv	renamed suggestCoord and added new suggestPos

	 Meaning of state abbreviations:
	 extc: extinct; not supported any more
	 rtrd: retired; no files are created with this format, 
		   but it may still occur while reading an old file
	 actv: active; currently used to create files
  */

/////////////////////////////////////////////////////////////////////////////
// Collection helpers

#ifdef _DEBUG
void AFXAPI DumpElements(CDumpContext& dc, const SoundEx* pElements, int nCount)
{
	for (int i = 0; i < nCount; i++) dc << SoundEx2String(pElements[i]);
}

void AFXAPI DumpElements(CDumpContext& dc, const WORD* pElements, int nCount)
{
	for (int i = 0; i < nCount; i++) dc << pElements[i];
}
#endif

void AFXAPI SerializeElements(CArchive& ar, CNextWord** pElements, int nCount)
{
	for (int i = 0; i < nCount; i++) {
		if (ar.IsLoading()) pElements[i] = new CNextWord();
		pElements[i]->Serialize(ar);
	}
}

int AFXAPI SortElements(const BYTE& x, const BYTE& y)
{
	if (x > y) return 1;
	else if (x == y) return 0;
	else return -1;
}


/////////////////////////////////////////////////////////////////////////////
// CWordInfo

IMPLEMENT_SERIAL(CWordInfo, CObject, 0)

CWordInfo::CWordInfo()
{
	occurences[0] = occurences[1] = lastUse = 0;
	persistent = false;
}

// Copy constructor.
CWordInfo::CWordInfo(const CWordInfo& src)
{
	lastUse = src.lastUse;
	occurences[0] = src.occurences[0];
	occurences[1] = src.occurences[1];
	persistent = src.persistent;

	for (POSITION pos = src.predecessors.GetStartPosition(); pos;) {
		SoundEx soundEx;
		WORD freq;
		src.predecessors.GetNextAssoc(pos, soundEx, freq);
		predecessors[soundEx] = freq;
	}

	successors.SetSize(src.successors.GetSize());
	for (int i = 0; i < src.successors.GetSize(); i++)
		successors[i] = new CNextWord(*src.successors[i]);

	for (pos = src.wndClasses.GetHeadPosition(); pos;) {
		DWORD wndClass = src.wndClasses.GetNext(pos);
		wndClasses.AddTail(wndClass);
	}

	word = src.word;
}

CWordInfo::~CWordInfo()
{
	for (short i = 0; i < successors.GetSize(); i++) 
		delete successors[i];
}

#ifdef _DEBUG
void CWordInfo::Dump(CDumpContext& dc) const
{
	dc << word;
	for (short i = 0; i < successors.GetSize(); i++)
	{
		dc << "\t" << word << successors[i]->spaceString 
		   << theDoc->words[successors[i]->wordIdx]->word;
	}
}
#endif

/* -- Serialize --
   Synopsis: 
	Serializes a word. If storing, the word gets encrypted.

   Parameters:
	ar				CArchive to store to/load from.
*/
void CWordInfo::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);

	CAutoTypeDoc* doc = (CAutoTypeDoc*)ar.m_pDocument;
	if (ar.IsStoring())
		ar << word << occurences[0] << occurences[1] << lastUse;
	else
		ar >> word >> occurences[0] >> occurences[1] >> lastUse;
	predecessors.Serialize(ar);
	wndClasses.Serialize(ar);

	// If we are only storing persistent words, we have to remove
	// pointers to temporary words from the successors list.
	if (ar.IsLoading() || (doc && !doc->tmpIdxMap.GetSize()))
		successors.Serialize(ar);
	else 
	{
		CArray<CNextWord*, CNextWord*> persistent;
		for (short i = 0; i < successors.GetSize(); i++)
		{
			if (doc->tmpIdxMap[successors[i]->wordIdx] != -1)
				persistent.Add(successors[i]);
		}
		persistent.Serialize(ar);
	}

	persistent = true;
}

/* -- GetSize --
   Synopsis: 
	Compute size of serialized word.

   Return value:
	The number of bytes the word takes when serialized.
*/
int CWordInfo::GetSize() const
{
	int result = 
		word.GetLength()+sizeof(int) + sizeof(WORD)*2 + sizeof(time_t) +
		predecessors.GetCount()*(sizeof(SoundEx)+sizeof(WORD)) +
		wndClasses.GetCount()*sizeof(DWORD) +
		successors.GetSize()*(3*sizeof(WORD)+1) + 4;

	for (short i = 0; i < successors.GetSize(); i++)
		result += successors[i]->spaceString.GetLength() + 2 +
			successors[i]->predecessors.GetCount() * 
			(sizeof(SoundEx) + sizeof(WORD));

	return result;
}



/////////////////////////////////////////////////////////////////////////////
// CNextWord

IMPLEMENT_SERIAL(CNextWord, CObject, 0)

CNextWord::CNextWord()
{
	wordIdx = 0;
	lastUse = occurences = 0;
	upperCase = false;
}

CNextWord::CNextWord(const CNextWord& src)
{
	lastUse = src.lastUse;
	occurences = src.occurences;

	for (POSITION pos = src.predecessors.GetStartPosition(); pos;) {
		SoundEx soundEx;
		WORD freq;
		src.predecessors.GetNextAssoc(pos, soundEx, freq);
		predecessors[soundEx] = freq;
	}

	spaceString = src.spaceString;
	upperCase = src.upperCase;
	wordIdx = src.wordIdx;
}

void CNextWord::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		BYTE b;
		ar >> spaceString
		   >> wordIdx
		   >> b;
		upperCase = bool(b);
		ar >> occurences;
	}
	else
	{
		// If only persistent words are stored, we have to
		// convert the word indices written to disk to reflect
		// the removal of temporary words.
		CAutoTypeDoc* doc = (CAutoTypeDoc*)ar.m_pDocument;
		WORD i;
		if (doc->tmpIdxMap.GetSize() == 0) i = wordIdx;
		else {
			ASSERT(doc->tmpIdxMap[wordIdx] != -1);
			i = doc->tmpIdxMap[wordIdx];
		}

		ar << spaceString
		   << i
		   << BYTE(upperCase)
		   << occurences;
	}

	predecessors.Serialize(ar);
}


/////////////////////////////////////////////////////////////////////////////
// CClipboardContent

CClipboardContent::CClipboardContent()
{
	format = 0;
	size = 0;
	data = NULL;
}

CClipboardContent::~CClipboardContent()
{
	if (data) free(data);
}

/////////////////////////////////////////////////////////////////////////////
// CAutoTypeDoc

IMPLEMENT_DYNCREATE(CAutoTypeDoc, CDocument)

BEGIN_MESSAGE_MAP(CAutoTypeDoc, CDocument)
	//{{AFX_MSG_MAP(CAutoTypeDoc)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_FILE_SHORTCUT, OnFileShortcut)
	ON_UPDATE_COMMAND_UI(ID_FILE_SHORTCUT, OnUpdateFileShortcut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_FILE_READ, OnFileRead)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_FILE_MERGE, OnFileMerge)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_APPLICATIONS, OnUpdateOptionsApplications)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoTypeDoc construction/destruction

CAutoTypeDoc::CAutoTypeDoc()
{
	theDoc = this;
	suggesting = false;
	synchronized = SYNC_NONE;
	lastMsgTime = 0;
	isPasswordField = false;
	view = NULL;
	timerId = NULL;
	destWnd = 0;
	lastWnd = 0;
}

CAutoTypeDoc::~CAutoTypeDoc()
{
	RemoveClipboardContents();
}

void CAutoTypeDoc::DeleteContents()
{
	for (int i = 0; i < words.GetSize(); i++)
		delete words[i];
	words.RemoveAll();
}

BOOL CAutoTypeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument()) return FALSE;

	// Reset some attributes.
	prevSoundEx[0] = prevSoundEx[1] = SoundEx(0);
	m_iPrevWord = -1;

	// Default settings.
	memset(&settings, 0, sizeof(&settings));
	settings.minGiven = 2;
	settings.minAdded = 3;
	settings.howCertain = 15;
	settings.minTypeTime = 0;
	settings.maxAge = DB_MAXAGE_UNREG;
	settings.maxSize = DB_MAXSIZE_UNREG;
	settings.ringBell = false;
	settings.autoSave = true;
	settings.maxSuggested = 4;
	strcpy(settings.wordChars, "_");
	settings.allowDigits = true;
	settings.allowBrackets = false;
	settings.multiWords = true;
	settings.addSpace = false;
	settings.suggestCoord = CPoint(0, 14);
	settings.fixedVocabulary = false;
	settings.caseSensitive = (theApp.language == "de");
	settings.numpad = false;
	settings.ignorePasswords = true;
	settings.allowNumbers = true;
	settings.suggestionTimeout = 0;
	settings.suggestPos = POS_CARET;

	view->GetListCtrl().DeleteAllItems();

	time(&lastGC);

	if (timerId) {
		theApp.m_pMainWnd->KillTimer(timerId);
		timerId = NULL;
	}

	return TRUE;
}

BOOL CAutoTypeDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	// We set and check the fileVersion to catch empty files.
	// (Unfortunately CDocument::OnOpenDocument wants to be too clever.)
	m_nFileVersion = 0;
	if (!CDocument::OnOpenDocument(lpszPathName) || !m_nFileVersion) {
		CString msg;
		msg.Format(IDS_OPENDOC_ERROR, lpszPathName);
		AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// Play it safe and discard pending input.
	theApp.EmptyMsgBuffer();
	Abort();
	
	OnViewRefresh();
	
	// (Re)start autosave timer.
	// Because the document may be opened before the main window
	// exists, we have to check m_pMainWnd.
	if (theApp.m_pMainWnd) {
		if (timerId) theApp.m_pMainWnd->KillTimer(timerId);
		timerId = theApp.m_pMainWnd->SetTimer(1, TIMER_SPAN, NULL);
		ASSERT(timerId);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAutoTypeDoc serialization

void CAutoTypeDoc::Serialize(CArchive& ar)
{
	CDocument::Serialize(ar);

	if (ar.IsStoring() && time(NULL) - lastGC > DAY_SECONDS)
		CollectGarbage();

	Serialize(ar, m_nFileVersion, words, settings, lastGC);
}

void CAutoTypeDoc::Serialize(CArchive& ar,
							 WORD& fileVersion, 
							 CTypedPtrArray<CObArray, CWordInfo*>& words,
							 DocSettings& settings,
							 time_t& lastGC)
{
	// File version
	if (ar.IsStoring()) {
		fileVersion = CUR_FILE_VERSION;
		ar << fileVersion;
	}
	else 
	{
		ar >> fileVersion;
		if (fileVersion > CUR_FILE_VERSION || fileVersion < 6)
			AfxThrowArchiveException(CArchiveException::badSchema);
	}

	DWORD crc;
	if (ar.IsStoring())			// store CRC of uncoded word
	{
		ar << DWORD(0);

		if (settings.fixedVocabulary) {
			tmpIdxMap.SetSize(words.GetSize());
			for (int i = 0; i < tmpIdxMap.GetSize(); i++) tmpIdxMap[i] = i;

			CTypedPtrArray<CObArray, CWordInfo*> persistentWords;
			for (i = 0; i < words.GetSize(); i++) {
				if (words[i]->persistent) persistentWords.Add(words[i]);
				else {
					tmpIdxMap[i] = -1;
					for (int j = i+1; j < tmpIdxMap.GetSize(); j++)
						if (tmpIdxMap[j] != -1) tmpIdxMap[j]--;
				}
			}
			persistentWords.Serialize(ar);
		}
		else {
			tmpIdxMap.RemoveAll();
			words.Serialize(ar);
		}
	}
	else {
		ar >> crc;
		if (crc) {
		CString msg;
			msg.LoadString(IDS_DECYPHER_ERROR);
			AfxMessageBox(msg, MB_ICONEXCLAMATION);
			AfxThrowArchiveException(CArchiveException::none);
		}
		words.Serialize(ar);
	}

	if (ar.IsStoring()) {
		ar.Write(&settings, sizeof(settings));
		ar << lastGC;
	}
	else
	{
		short size = sizeof(settings);

		if (fileVersion < 7) {
			size -= 4;	// should be sizeof(bool), but
						// VC computes sizeof(settings) differently ...
			settings.fixedVocabulary = false;
		}

		if (fileVersion < 8) {
			size -= 4;
			settings.caseSensitive = (theApp.language == "de");
		}

		if (fileVersion < 9) {
			size -= 4;
			settings.numpad = false;
		}

		if (fileVersion < 10) {
			size -= 4;
			settings.ignorePasswords = true;
		}

		if (fileVersion < 11) {
			size -= 4;
			settings.allowNumbers = true;
		}

		if (fileVersion < 12) {
			size -= sizeof(settings.suggestionTimeout);
			settings.suggestionTimeout = 0;
		}

		if (fileVersion < 13) {
			size -= sizeof(settings.suggestPos);
			settings.suggestPos = POS_CARET;
		}

		ar.Read(&settings, size);
		ar >> lastGC;
		if (lastGC > time(NULL)) lastGC = time(NULL);

		if (fileVersion < 13) settings.suggestCoord = CPoint(0, 14);
	}
}

struct ILPair
{
	int i;
	long l;
};
typedef ILPair& ILPairR;

int AFXAPI SortElements(const ILPair p1, const ILPairR p2)
{
	if (p1.l > p2.l) return 1;
	else if (p1.l == p2.l) return 0;
	else return -1;
}

// Remove outdated words and trim file to fit into max size
void CAutoTypeDoc::CollectGarbage()
{
	BeginWaitCursor();

	DWORD size = 2+sizeof(settings)+sizeof(lastGC)+sizeof(DWORD);
	time_t now;
	time(&now);
	CSArray<ILPair, ILPair&> usageSorted;
	for (int i = 0; i < words.GetSize(); i++)
	{
		if (!settings.fixedVocabulary && now/DAY_SECONDS - words[i]->lastUse > settings.maxAge)
		{
			TRACE("Removing outdated word %s\n", words[i]->word);
			delete words[i];
			RemoveWord(i--);
		}
		else if (theApp.taboos.Find(CRC16(LPCTSTR(words[i]->word)))) {
			TRACE("Removing taboo word %s\n", words[i]->word);
			delete words[i];
			RemoveWord(i--);
		}
		else {
			size += words[i]->GetSize();
			ILPair x;
			x.i = i;
			x.l = words[i]->lastUse + (words[i]->occurences[0]+words[i]->occurences[1]-1)*3;
			usageSorted.Add(x);

			// Trim successors list if it gets too large. Words
			// not used for the longest time are removed first.
			if (words[i]->successors.GetSize() > MAX_SUCCESSORS)
			{
				CSArray<ILPair, ILPair&> sucSorted;
				for (short j = 0; j < words[i]->successors.GetSize(); j++) {
					ILPair x;
					x.i = j;
					x.l = words[i]->successors[j]->lastUse;
					sucSorted.Add(x);
				}
				short toRemove = words[i]->successors.GetSize() - MAX_SUCCESSORS;
				for (j = 0; j < toRemove ; j++) {
					words[i]->successors.RemoveAt(sucSorted[j].i);
					for (short k = j+1; k < toRemove; k++)
						if (sucSorted[k].i > sucSorted[j].i)
							sucSorted[k].i--;
				}
			}
		}
	}

	if (!settings.fixedVocabulary)
	{
		// Trim file to max size.
		for (i = 0; 
			 i < usageSorted.GetSize() && size > ULONG(settings.maxSize)*1024;
			 i++)
		{
			int j = usageSorted[i].i;
			size -= words[j]->GetSize();
			RemoveWord(j);
			for (int k = i+1; k < usageSorted.GetSize(); k++)
				if (usageSorted[k].i > j)
					usageSorted[k].i--;
		}

		OnViewRefresh();
	}

	lastGC = now;

	EndWaitCursor();
}

/* -- RemoveWord --
   Synopsis: 
	Removes a word from the words attribute and makes sure that
	links between words don't break.

   Parameters:
	idx				Identifies the word to be removed.
*/
void CAutoTypeDoc::RemoveWord(int idx)
{
	if (m_iPrevWord == idx) m_iPrevWord = -1;
	else if (m_iPrevWord > idx) m_iPrevWord--;

	words.RemoveAt(idx);
	for (int i = 0; i < words.GetSize(); i++)
		for (short j = 0; j < words[i]->successors.GetSize(); j++)
		{
			if (words[i]->successors[j]->wordIdx == idx)
				words[i]->successors.RemoveAt(j--);
			else if (words[i]->successors[j]->wordIdx > idx)
				words[i]->successors[j]->wordIdx--;
		}

	SetModifiedFlag();
}

// Add words from a vocabulary file to current vocabulary.
void CAutoTypeDoc::OnFileMerge() 
{
	CString filters;
	filters.LoadString(IDS_VOCABULARY_FILTERS);
	CFileDialog dlg(TRUE, "lmt", NULL, OFN_FILEMUSTEXIST, filters);
	if (dlg.DoModal() != IDOK) return;

	CString password;
	WORD fileVersion;
	CTypedPtrArray<CObArray, CWordInfo*> docWords;
	DocSettings settings;
	time_t lastGC;
	try {
		CFile in(dlg.GetPathName(), CFile::modeRead | CFile::shareDenyWrite);
		CArchive ar(&in, CArchive::load);
		Serialize(ar, fileVersion, docWords, settings, lastGC);
	} 
#ifdef _DEBUG
	catch (CException *e) {
		e->ReportError();
#else
	catch (...) {
#endif
		AfxMessageBox(IDS_READ_ERROR, MB_ICONEXCLAMATION); 
	}

	// This array maps indexes of the imported document to
	// indexes in our own word list.
	CArray<int, int&> docNewIdxs;
	docNewIdxs.SetSize(docWords.GetSize());

	// This array tells us where the newly added words are.
	CArray<int, int&> newWords;
	newWords.SetSize(docWords.GetSize());

	// First loop: Select words to be added and store their
	// new indexes.
	int thisIdx = 0, thisIdx2 = 0, docIdx = 0;
	while (docIdx < docWords.GetSize())
	{
		if (thisIdx < words.GetSize()) 
		{
			if (words[thisIdx]->word == docWords[docIdx]->word) { // Duplicate.
				docNewIdxs[docIdx] = thisIdx2;
				newWords[docIdx] = -1;

				thisIdx++;
				thisIdx2++;
				docIdx++;

				continue;
			}
			if (words[thisIdx]->word.Compare(docWords[docIdx]->word) < 0) { // Continue searching.
				thisIdx++;
				thisIdx2++;
				continue;
			}
		}

		// New word.
		ShiftWordPointers(thisIdx, 1);

		docNewIdxs[docIdx] = thisIdx2;
		newWords[docIdx] = thisIdx2;

		thisIdx2++;
		docIdx++;
	}

	// Second loop: Actually add words, and update their word pointers.
	for (int i = 0; i < newWords.GetSize(); i++)
	{
		if (newWords[i] != -1) 
		{
			CWordInfo *word = new CWordInfo(*docWords[i]);
			for (short j = 0; j < word->successors.GetSize(); j++)
				word->successors[j]->wordIdx = docNewIdxs[word->successors[j]->wordIdx];
			words.InsertAt(newWords[i], word);
		}
	}

	OnViewRefresh();
}


/////////////////////////////////////////////////////////////////////////////
// CAutoTypeDoc diagnostics

#ifdef _DEBUG
void CAutoTypeDoc::AssertValid() const
{
	CDocument::AssertValid();
	words.AssertValid();
}

void CAutoTypeDoc::Dump(CDumpContext& dc) const
{
	dc.SetDepth(1);
	CObject::Dump(dc);
	words.Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CAutoTypeDoc commands

// Start new search for a word
void CAutoTypeDoc::Restart(SyncState syncState)
{
	StopSuggesting();

	curWord.Empty();
	curSpaceString.Empty();
	last_lbound.RemoveAll();
	last_lbound.AddHead(0);
	last_ubound.RemoveAll();
	last_ubound.AddHead(words.GetSize());
	synchronized = syncState;
	firstSuggestion = true;

	if (words.GetSize() == 0)
		canContinue = false;
	else {
		firstUnknownChar = 0;
		canContinue = true;
	}
}

// Search for next character
void CAutoTypeDoc::NextChar(char c, bool makeSuggestion, POINT caretPos)
{
	if (curWord.IsEmpty()) {
		curSpaceString.Empty();

		firstUpper = IsCharUpper(c);
		if (firstUpper)	c = ToLower(c);
	}

	curWord += c;

	// Do we already know that the word is unknown?
	if (!canContinue) return;							

	canContinue = IncrementalLookup();
	if (!canContinue)
		firstUnknownChar = curWord.GetLength();
	else if (makeSuggestion &&
	         curWord.GetLength() >= settings.minGiven &&
			 (!isPasswordField || !settings.ignorePasswords))
		SuggestWords(caretPos);	// suggest possible continuations
}

// Undo last character (because the user pressed the backspace key)
void CAutoTypeDoc::PrevChar(bool makeSuggestion, POINT caretPos)
{
	if (curWord.IsEmpty())
		throw OUT_OF_CHARACTERS;

	curWord = curWord.Left(curWord.GetLength()-1);

	ASSERT(!last_lbound.IsEmpty() && !last_ubound.IsEmpty());
	if (canContinue) {
		last_lbound.RemoveHead();
		last_ubound.RemoveHead();
	}
	else if (curWord.GetLength()+1 == firstUnknownChar)
		canContinue = true;

	if (canContinue && makeSuggestion &&
	    curWord.GetLength() >= settings.minGiven)
		SuggestWords(caretPos);		// suggest possible continuations
}

/* -- Abort --
   Synopsis: 
	Aborts the processing of the current word. 
	This is called when the user has taken an action which makes
	it impossible to know how and when the word ends. It also
	prevents the learning of word sequences.
*/
void CAutoTypeDoc::Abort()
{
	curWord.Empty();
	curSpaceString.Empty();
	prevSpaceString.Empty();
	prevSoundEx[0] = prevSoundEx[1] = SoundEx(0);
	m_iPrevWord = -1;
	synchronized = SYNC_NONE;
}

/* -- SaveWord --
   Synopsis: 
	Saves information about a word when the user is starting the next one.

   Parameters:
	persistent		Specifies if the word is to be saved or discarded when
					the document is closed.
	filter			If true, only certain character combinations
					are recognized as valid words and stored.
*/
bool CAutoTypeDoc::SaveWord(bool persistent, bool filter)
{
	if (synchronized != SYNC_LEARN || curSpaceString.IsEmpty() || curWord.IsEmpty())
		return false;

	SoundEx oldPrevSoundEx = prevSoundEx[0];
	prevSoundEx[0] = prevSoundEx[1];
	prevSoundEx[1] = String2SoundEx(curWord);
	int oldPrevWord = m_iPrevWord;
	CString oldSpaceString = prevSpaceString;

	m_iPrevWord = -1;	// default

	prevSpaceString = curSpaceString;
	curSpaceString.Empty();

	if (curWord.GetLength() < settings.minGiven || 
		curWord.GetLength() > MAX_WORD_LEN || 

		!isalpha(curWord[0]) && 
		(!settings.allowDigits || !settings.allowNumbers || !isdigit(curWord[0])) && 
		!strchr(DEFAULT_CHARS, curWord[0]) && 
		curWord[0] != '_' ||

		theApp.taboos.Find(CRC16(LPCTSTR(curWord))) ||
		!ValidWord(filter))
	{
		TRACE("Ignoring invalid word %s\n", curWord);
	 	Abort();
		Restart();
		return false;
	}

	time_t t;
	time(&t);

	// Look for place where new word is to be inserted into
	// the sorted word list.
	for (int curIdx = last_lbound.GetHead(); 
		 curIdx < words.GetSize() && strcmp(words[curIdx]->word, curWord) < 0; 
		 curIdx++);

	// Either update existing item or create a new word.
	CWordInfo *curInfo;
	bool newWord;
	if (curIdx < words.GetSize() && strcmp(words[curIdx]->word, curWord) == 0) {
		TRACE("Updating word %s\n", curWord);
		curInfo = words[curIdx];
		if (persistent) curInfo->persistent = persistent;
		newWord = false;
	}
	else if (isPasswordField && settings.ignorePasswords || !theApp.m_bLearn) {
		TRACE("Ignoring new word %s\n", curWord);
		Abort();
		Restart();
		return false;
	}
	else 
	{
		TRACE("Storing new word %s\n", curWord);
		curInfo = new CWordInfo;
		curInfo->word = curWord;
		curInfo->occurences[0] = curInfo->occurences[1] = 0;
		curInfo->persistent = persistent;

		words.InsertAt(curIdx, curInfo);
		if (curIdx <= oldPrevWord) oldPrevWord++;
		newWord = true;

		ASSERT((curIdx == 0 || 
				   strcmp(words[curIdx-1]->word, words[curIdx]->word) < 0) &&
			   (curIdx == words.GetUpperBound() || 
				   strcmp(words[curIdx]->word, words[curIdx+1]->word) < 0));

		ShiftWordPointers(curIdx, 1);
	}
	curInfo->occurences[firstUpper ? 1 : 0]++;
	curInfo->lastUse = t/DAY_SECONDS;
	m_iPrevWord = curIdx;

	// Add word to list view.
	if (newWord) view->AddWord(curIdx, curInfo);

	if (!settings.fixedVocabulary || curInfo->persistent) SetModifiedFlag();

	// Update successors list of previous word.
	if (settings.multiWords && oldPrevWord != -1 && 
		!oldSpaceString.IsEmpty())
	{
		CWordInfo *prevInfo = words[oldPrevWord];
		short i;
		if (!newWord)
			for (i = 0; i < prevInfo->successors.GetSize(); i++)
				if (prevInfo->successors[i]->wordIdx == curIdx &&
					prevInfo->successors[i]->spaceString == oldSpaceString &&
					prevInfo->successors[i]->upperCase == firstUpper)
					break;

		// Either insert a new item into successors list or
		// update an existing one.
		CNextWord *nextWord;
		if (!newWord && i < prevInfo->successors.GetSize()) {
			nextWord = prevInfo->successors[i];
			nextWord->occurences++;
		}
		else {
			nextWord = new CNextWord;
			nextWord->wordIdx = curIdx;
			nextWord->spaceString = oldSpaceString;
			nextWord->upperCase = firstUpper;
			nextWord->occurences = 1;
			prevInfo->successors.Add(nextWord);
		}
		nextWord->lastUse = t/DAY_SECONDS;

		// Update list of predecessors.
		if (oldPrevSoundEx) {
			WORD occurences = 0;
			nextWord->predecessors.Lookup(oldPrevSoundEx, occurences);
			occurences++;
			nextWord->predecessors.SetAt(oldPrevSoundEx, occurences);
		}
	}

	// Don't store additional information if it's a short word.
	if (curWord.GetLength() < settings.minGiven + settings.minAdded) {
		Restart();
		return true;
	}

	// Update list of predecessors.
	if (prevSoundEx[0]) {
		WORD occurences = 0;
		curInfo->predecessors.Lookup(prevSoundEx[0], occurences);
		occurences++;
		curInfo->predecessors.SetAt(prevSoundEx[0], occurences);
	}

	// Update list of window classes. The list is ordered by time of last use,
	// i.e. the one not used for the longest time is at the list's tail.
	if (!m_destWndClassHash) {
		Restart();
		return true;
	}

	POSITION pos = curInfo->wndClasses.Find(m_destWndClassHash);
	if (pos)
	{
		if (pos != curInfo->wndClasses.GetHeadPosition()) {
			curInfo->wndClasses.RemoveAt(pos);
			curInfo->wndClasses.AddHead(m_destWndClassHash);
		}
	}
	else
	{
		if (curInfo->wndClasses.GetCount() >= MAX_WND_CLASSES) {
			TRACE("Purging wndClasses list of \"%s\"\n", curInfo->word);
			curInfo->wndClasses.RemoveTail();
		}
		curInfo->wndClasses.AddHead(m_destWndClassHash);
	}

	Restart();
	return true;
}


// Shift word pointers.
void CAutoTypeDoc::ShiftWordPointers(int idx, short delta)
{
	ASSERT(delta == 1 || delta == -1);

	for (int i = 0; i < words.GetSize(); i++)
		for (short j = 0; j < words[i]->successors.GetSize(); j++)
		{
			if (delta == -1 && words[i]->successors[j]->wordIdx == idx)
				words[i]->successors.RemoveAt(j--);
			else if (words[i]->successors[j]->wordIdx >= idx)
				words[i]->successors[j]->wordIdx += delta;
		}
}

/* -- RepeatedChars --
   Synopsis: 
	Checks if a word contains an unusual count of character repitions.
	This can be caused by the user playing a game that's controlled
	with the keyboard.
	"Unusual" means more than 3 identic characters in a row or
	two sequences of identic characters which are at least 3
	characters long each.

   Return value:
	true if the string probably is no word, else false.
*/
bool CAutoTypeDoc::ValidWord(bool filter) const
{
	short repitions = 0, consonants = 0, curCount = 0;
	for (int i = 0; i < curWord.GetLength(); i++)
	{
		// Four instances of the same character in a row -> not valid
		// Two separate characters repeated three times -> not valid
		if (i && curWord[i] == curWord[i-1]) {
			if (curCount) curCount++;
			else curCount = 2;
			if (curCount == 4) return false;
		}
		else if (curCount)
		{
			if (curCount == 3) {
				if (repitions) return false;
				else repitions = 1;
			}
			else if (curCount > 3) return false;
			curCount = 0;
		}

		// More than six consonants in a row -> not valid
		if (!strchr("aeuioyˆ‰¸", ToLower(curWord[i])) && !isdigit(curWord[i])) {
			consonants++;
			if (consonants > 6) return false;
		}
		else consonants = 0;
	}

	return !(repitions && curCount == 3 || curCount > 3);
}


/* -- StopSuggesting --
   Synopsis: 
	Called when the user has taken an action that indicates the start of a new 
	word, e.g. changing the active window or selecting a suggested word. 
	Hides the suggestion window (if visible) and makes sure a pending
	store is executed.
*/
void CAutoTypeDoc::StopSuggesting()
{
	if (!suggesting) return;

	theApp.m_dlgSuggestions->ShowWindow(SW_HIDE);
	suggesting = false;

	// Save window position.
	CRect r;
	theApp.m_dlgSuggestions->GetWindowRect(&r);
	CPoint initial = theApp.m_dlgSuggestions->m_ptInitial;
	if (r.left != initial.x || r.top != initial.y ||
		theApp.m_dlgSuggestions->m_nPosition != settings.suggestPos)
	{
		if (settings.suggestPos == POS_CARET && theApp.m_dlgSuggestions->m_nPosition == POS_CARET)
		{
			settings.suggestCoord.x += r.left - initial.x;
			if (settings.suggestCoord.y >= 0 &&
				settings.suggestCoord.y + (r.top - initial.y) < 0)
			{
				if (r.bottom < initial.y - settings.suggestCoord.y)
					settings.suggestCoord.y += r.bottom - initial.y;

			}
			else if (settings.suggestCoord.y < 0 &&
				settings.suggestCoord.y + (r.top - initial.y) >= 0)
			{
				if (r.top > initial.y + r.bottom-r.top + (-settings.suggestCoord.y))
					settings.suggestCoord.y += r.top - (initial.y + r.bottom-r.top);
			}
			else settings.suggestCoord.y += r.top - initial.y;
		}
		else if (theApp.m_dlgSuggestions->m_nPosition == POS_CARET)
			settings.suggestCoord = CPoint(0, 14);
		else if (theApp.m_dlgSuggestions->m_nPosition == POS_CURSOR)
			settings.suggestCoord = CPoint(0, 0);
		else // POS_FIXED
			settings.suggestCoord = CPoint(r.left, r.top);
		settings.suggestPos = theApp.m_dlgSuggestions->m_nPosition;
		SetModifiedFlag();
	}
}

// Send word to destWnd and delete character user typed to select 
// the suggestion
void CAutoTypeDoc::SendWord(CString w, bool deleteChar)
{
	const BYTE SK_BACK = MapVirtualKey(VK_BACK, 0);
	const BYTE SK_SHIFT = MapVirtualKey(VK_SHIFT, 0);
	const BYTE SK_CONTROL = MapVirtualKey(VK_CONTROL, 0);
	const BYTE SK_MENU = MapVirtualKey(VK_MENU, 0);
	const BYTE SK_INSERT = MapVirtualKey(VK_INSERT, 0);

	if (deleteChar) {
		keybd_event(VK_BACK, SK_BACK, 0, 0);
		keybd_event(VK_BACK, SK_BACK, KEYEVENTF_KEYUP, 0);

		// Tell ProcessMsg to ignore this backspace.
		ignoreBackspace = true;
	}

	CWnd *oldForegroundWindow = NULL;
	if (strcmp(m_destWndClass, "RXClientClass") == 0) {
		// If we are sending a suggestion to the Reflection X Window server,
		// we have to switch the foreground window. Otherwise, RX would
		// not copy our data from the Windows clipboard into its own
		// clipboard.

		oldForegroundWindow = theApp.m_dlgSuggestions->GetForegroundWindow();
		theApp.m_dlgSuggestions->SetForegroundWindow();
	}

	theApp.ignoreClipboard = true;
	theApp.m_pMainWnd->OpenClipboard();

	SaveClipboard();

	EmptyClipboard();

	void *hglbCopy = GlobalAlloc(GMEM_DDESHARE, w.GetLength()+1); 
    if (hglbCopy) { 
	    // Lock the handle and copy the text to the buffer. 
 
		char *lptstrCopy = (char*)GlobalLock(hglbCopy); 
		memcpy(lptstrCopy, w, w.GetLength()); 
		lptstrCopy[w.GetLength()] = 0;
		GlobalUnlock(hglbCopy); 
	
		SetClipboardData(CF_TEXT, hglbCopy);
	}

	CloseClipboard();

	Sleep(50);

	if (oldForegroundWindow) oldForegroundWindow->SetForegroundWindow();

	theApp.ignoreClipboard = false;

	keybd_event(VK_SHIFT, SK_SHIFT, 0, 0);
	keybd_event(VK_INSERT, SK_INSERT, KEYEVENTF_EXTENDEDKEY, 0);
	keybd_event(VK_INSERT, SK_INSERT, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	keybd_event(VK_SHIFT, SK_SHIFT, KEYEVENTF_KEYUP, 0);
}

// Process intercepted WM_CHAR, WM_KEYDOWN or WM_LBUTTONDOWN message
void CAutoTypeDoc::ProcessMsg(MsgData msg)
{
	static DWORD lastTime = 0;			// last time a message arrived
	static MsgData lastMsg;				// last message received
	static bool lastMsgValid = false;	// whether lastMsg's content is valid
	char newWndClass[256], destWndFileName[512];

	bool makeSuggestion = true;			// can be disabled by 'fast typing'
	bool isSelectionChar, isWordChar;

	ASSERT(synchronized != SYNC_NONE || curWord.IsEmpty());

	// If the window has disappeared in the meantime, stop.
	if (!IsWindow(msg.hWnd)) return;

	// Hack for Microsoft's Intelligent Form which duplicates
	// WM_CHAR messages and sends them to its own window.
	GetClassName(msg.hWnd, newWndClass, sizeof(newWndClass));
	if (strcmp(newWndClass, "IntelliFormClass") == 0) return;

	time(&lastMsgTime);
	destWnd = msg.hWnd;

	if (theApp.m_bPositiveApplications || theApp.m_bNegativeApplications) 
	{
		if (theApp.GetWindowModuleFileName(destWnd, destWndFileName, sizeof(destWndFileName)))
		{
			// We got the application filename, now check it.
			bool applicationOK;
			if (theApp.m_bPositiveApplications)
			{
				// For the positive list, set default to "deny",
				// and search for a positive match.
				applicationOK = false;
				for (int i = 0; i < theApp.m_positiveApplications.GetSize(); i++)
					if (theApp.m_positiveApplications[i].CompareNoCase(destWndFileName) == 0)
					{
						applicationOK = true;
						break;
					}
			}
			else
			{
				// For the negative list, set default to "pass",
				// and search for a negative match.
				applicationOK = true;
				for (int i = 0; i < theApp.m_negativeApplications.GetSize(); i++)
					if (theApp.m_negativeApplications[i].CompareNoCase(destWndFileName) == 0)
					{
						applicationOK = false;
						break;
					}
			}

			if (!applicationOK) return;
		}
	}

	switch (msg.id)
	{
		case WM_CHAR:
			// We already have caught VK_BACK as a WM_KEYDOWN message.
			if (msg.wParam == VK_BACK) break;

			// Don't make suggestions if the user is typing too fast
			// or if we've got a severe lag in msg processing.
			if (msg.time - lastTime < settings.minTypeTime ||
				msg.time+MAX_LAG < timeGetTime())
				makeSuggestion = false;
			lastTime = msg.time;

			if (destWnd != lastWnd)
			{
				TRACE("Destination window for WM_CHAR changed from %x to %x\n", lastWnd, destWnd);
				Abort();
				Restart();
				lastWnd = destWnd;
				m_destWndClassHash = HashKey(newWndClass);

				LONG wndStyle = GetWindowLong(destWnd, GWL_STYLE);
				isPasswordField = (strcmp(newWndClass, "Edit") == 0) && (wndStyle & ES_PASSWORD);
			}

			isSelectionChar = 
				(suggesting && msg.wParam >= BYTE('1') && 
				 msg.wParam < BYTE('1'+min(settings.maxSuggested, 
										   suggestions.GetSize())));
			if (settings.numpad && 
				(LOBYTE(HIWORD(msg.lParam)) < 0x47 || LOBYTE(HIWORD(msg.lParam)) > 0x51) ||
				!settings.numpad &&
				(LOBYTE(HIWORD(msg.lParam)) >= 0x47 && LOBYTE(HIWORD(msg.lParam)) <= 0x51))
				isSelectionChar = false;

			isWordChar = IsWordChar(msg.wParam);

			if (isSelectionChar)	// Has the user selected a suggestion?
				SendSuggestion(msg.wParam-'1', true);
			else if (isWordChar)	// Has the user expanded a word?
			{
				theApp.IncrTypedSelf();

				SaveWord();
				StopSuggesting();
				if (synchronized != SYNC_NONE)
					NextChar(char(msg.wParam), makeSuggestion, msg.caretPos);
			}
			else				// Not a word character?
			{
				StopSuggesting();

				if (msg.wParam >= ' ')		// Word break?
				{
					theApp.IncrTypedSelf();

					if (synchronized == SYNC_LEARN)
					{
						if (!curWord.IsEmpty()) curSpaceString += char(msg.wParam);
					}
					else Restart();
				}
			}
			break;

		case WM_KEYDOWN:
			switch (msg.wParam)
			{
				// Backspace
				case VK_BACK:
					if (ignoreBackspace)
						ignoreBackspace = false;
					else if (!curSpaceString.IsEmpty())
						curSpaceString = curSpaceString.Left(curSpaceString.GetLength() - 1);
					else
					{
						StopSuggesting();

						if (!curWord.IsEmpty()) PrevChar(true, msg.caretPos);
						if (curWord.IsEmpty()) Abort();
					}
					break;

				// Word break
				case VK_TAB:
				case VK_RETURN:
					if (synchronized != SYNC_LEARN) Restart();
					else
					{
						StopSuggesting();
						if (!curWord.IsEmpty())	curSpaceString += " ";
						SaveWord();
					}
					break;

				case VK_UP:
				case VK_DOWN:
				case VK_RIGHT:
					Restart(SYNC_SUGGEST);
				case VK_SHIFT:
				case VK_CONTROL:
					StopSuggesting();
					break;

				case VK_INSERT:
				{
					COleDataObject cbOle;

					// Get clipboard content as text,
					// and process it as if the user had
					// typed it.
					if (cbOle.AttachClipboard()) {
						if (cbOle.IsDataAvailable(CF_TEXT)) {
							HGLOBAL hGlobal = cbOle.GetGlobalData(CF_TEXT);
							if (hGlobal) {
								char *s = (char*)GlobalLock(hGlobal);

								for (int i = 0; s[i]; i++)
								{
									if (IsWordChar(s[i]))
									{
										SaveWord();
										if (synchronized != SYNC_NONE)
											// Only make a suggestion after
											// the last character.
											NextChar(s[i], !s[i+1], msg.caretPos);
									}
									else
									{
										StopSuggesting();

										if (s[i] >= ' ')		// Word break?
										{
											if (synchronized == SYNC_LEARN)
											{
												if (!curWord.IsEmpty()) 
													curSpaceString += s[i];
											}
											else Restart();
										}
									}
								}

								GlobalUnlock(s);
								GlobalFree(hGlobal);
							}
						}

						if (m_clipboardContents.GetSize())
						{
							Sleep(100);

							CWnd *oldForegroundWindow = NULL;
							if (strcmp(m_destWndClass, "RXClientClass") == 0) {
								// If we are sending a suggestion to the Reflection X Window server,
								// we have to switch the foreground window. Otherwise, RX would
								// not copy our data from the Windows clipboard into its own
								// clipboard.

								oldForegroundWindow = theApp.m_dlgSuggestions->GetForegroundWindow();
								theApp.m_dlgSuggestions->SetForegroundWindow();
							}

							theApp.ignoreClipboard = true;
							theApp.m_pMainWnd->OpenClipboard();
							RestoreClipboard();
							CloseClipboard();
							Sleep(50);

							if (oldForegroundWindow) oldForegroundWindow->SetForegroundWindow();

							theApp.ignoreClipboard = false;
						}
					}
					break;
				}

				default:
					if (msg.wParam == VK_ESCAPE ||
						msg.wParam >= VK_PRIOR && msg.wParam <= VK_DOWN ||
						msg.wParam >= VK_F1 && msg.wParam <= VK_F24)
					{
						StopSuggesting();
						Abort();
					}
			}
			break;

		case WM_SETFOCUS:
		{
			// Ignore SETFOCUS if we have already received a
			// WM_CHAR for this window. Also ignore focus changes
			// to or from our own suggestion window, and events where
			// the former and current windows are child resp. parent,
			// or where they are equal.
			if (lastMsgValid && lastMsg.id == WM_CHAR && lastMsg.hWnd == destWnd ||
				destWnd == theApp.m_dlgSuggestions->m_hWnd ||
				IsChild(theApp.m_dlgSuggestions->m_hWnd, destWnd) ||
				lastWnd == theApp.m_dlgSuggestions->m_hWnd ||
				IsChild(theApp.m_dlgSuggestions->m_hWnd, lastWnd) ||
				IsChild(destWnd, lastWnd) || IsChild(lastWnd, destWnd) ||
				destWnd == lastWnd)
			{
				lastWnd = destWnd;
				break;
			}

			// Hack for Excel97.
			GetClassName(lastMsg.hWnd, m_destWndClass, sizeof(m_destWndClass));
			if (strncmp(newWndClass, "EXCEL", 5) == 0 && strncmp(m_destWndClass, "EXCEL", 5) == 0)
			{
				lastWnd = destWnd;
				break;
			}

			SaveWord();
			Abort();
			Restart();

			break;
		}

		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			// Ignore this if the click goes to our suggestion window.
			if (destWnd == theApp.m_dlgSuggestions->m_hWnd || 
				IsChild(theApp.m_dlgSuggestions->m_hWnd, destWnd))
				break;

			SaveWord();
			StopSuggesting();
			Restart(SYNC_SUGGEST);
			break;

		default:
			TRACE("ProcessMsg: Unknown message ID %d\n", msg.id);
	}

	strcpy(m_destWndClass, newWndClass);

	lastMsgValid = true;
	lastMsg = msg;
}

/* -- IsWordChar --
   Synopsis: 
	Tests if a character can be part of a word.

   Parameters:
	c				The character to be tested.

   Return value:
	true if it is a word character, else false.
*/
bool CAutoTypeDoc::IsWordChar(char c) const
{
	return c && (c > 0 && isalpha(c) || 
		settings.allowDigits && c > 0 && isdigit(c) ||
		settings.allowBrackets && strchr("(){}[]<>", c) ||
		strchr(settings.wordChars, c) ||
		strchr(DEFAULT_CHARS, c));
}

bool CAutoTypeDoc::ChangeWord(int oldIdx, CString word)
{
	if (word == words[oldIdx]->word) return false;

	// Check if any invalid characters are used.
	CString msg;
	for (int i = 0; i < word.GetLength(); i++)
	{
		if (!IsWordChar(word[i])) {
			msg.Format(IDS_ILLCHAR, word[i]);
			AfxMessageBox(msg);
			return false;
		}
	}

	// Check if the word is too long.
	if (word.GetLength() > MAX_WORD_LEN) {
		msg.Format(IDS_TOOLONG, MAX_WORD_LEN);
		AfxMessageBox(msg);
		return false;
	}

	// Check if the first letter is lowercase
	// (and make it lowercase if not).
	if (IsCharUpper(word[0])) word.SetAt(0, ToLower(word[0]));

	// Check if the word is unique.
	int insertIdx;
	if (Find(word, &insertIdx) != -1) {
		AfxMessageBox(IDS_DUPE);
		return false;
	}
	if (insertIdx > oldIdx) insertIdx--;

	// Update word indices in the database to reflect the removal
	// and reinsertion of the changed word.
	for (i = 0; i < words.GetSize(); i++)
		for (short j = 0; j < words[i]->successors.GetSize(); j++)
		{
			if (words[i]->successors[j]->wordIdx == oldIdx)
				words[i]->successors[j]->wordIdx = insertIdx;
			else 
			{
				if (words[i]->successors[j]->wordIdx > oldIdx)
					words[i]->successors[j]->wordIdx--;
				if (words[i]->successors[j]->wordIdx >= insertIdx)
					words[i]->successors[j]->wordIdx++;
			}
		}

	// Move word.
	CWordInfo* info = words[oldIdx];

	time_t t;
	time(&t);
	info->lastUse = t/DAY_SECONDS;
	info->word = word;

	words.RemoveAt(oldIdx);
	words.InsertAt(insertIdx, info);

	view->RemoveWord(oldIdx);
	view->AddWord(insertIdx, info);

	SetModifiedFlag();

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// Private methods

// Look for next character in our sorted word array using boolean search
bool CAutoTypeDoc::IncrementalLookup()
{
	int wordsSize = words.GetSize();

	int ubound = last_ubound.GetHead();
	int lbound = last_lbound.GetHead();
	ASSERT(ubound > 0 && ubound <= wordsSize);
	ASSERT(lbound >= 0 && lbound < wordsSize);

	while (lbound != ubound)
	{
		int i = lbound + (ubound-lbound)/2;

		ASSERT(i >= 0 && i < wordsSize);

		// Check database integrity.
		ASSERT(lbound == 0 || 
			   strncmp(words[lbound]->word, curWord, curWord.GetLength()) <= 0 ||
			   strncmp(words[lbound-1]->word, curWord, curWord.GetLength()) <= 0);
		ASSERT(ubound == wordsSize ||
			   strncmp(words[ubound]->word, curWord, curWord.GetLength()) > 0);

		int result = strncmp(words[i]->word, curWord, curWord.GetLength());
		if (result == 0)
		{
			while (lbound < wordsSize &&
			       strncmp(words[lbound]->word, curWord, curWord.GetLength()) < 0)
				lbound++;
			while (ubound > 0 &&
				   strncmp(words[ubound-1]->word, curWord, curWord.GetLength()) > 0)
				ubound--;

			last_ubound.AddHead(ubound);
			last_lbound.AddHead(lbound);

			return true;
		}
		else if (result < 0)		// search right half
			lbound = i+1;
		else						// search left half
			ubound = i;
	}

	return false;
}

/* -- Find --
   Synopsis: 
	Tests if a given word already exists in the database.
	The algorithm is the same used by IncrementalLookup.

   Parameters:
	word			The string to look for.

   Return value:
	If a word is found, its index is returned. 
	Otherwise the result is -1.
*/
int CAutoTypeDoc::Find(const CString& word, int* insertIdx) const
{
	int wordsSize = words.GetSize();

	int lbound = 0;
	int ubound = words.GetSize();

	while (lbound != ubound)
	{
		int i = lbound + (ubound-lbound)/2;

		ASSERT(i >= 0 && i < wordsSize);

		int result = strcmp(words[i]->word, word);
		if (result == 0)
			return i;
		else if (result < 0)		// search right half
			lbound = i+1;
		else						// search left half
			ubound = i;
	}

	if (insertIdx)
		*insertIdx = lbound;

	return -1;
}

/* -- GetOccurences --
   Synopsis: 
	Determines a word's frequency by using the attributes firstUpper
	and settings.caseSensitive.

   Parameters:
	idx				Identifies the word.
*/
int CAutoTypeDoc::GetOccurences(int idx) const
{
	int result = 0;
	if (firstUpper || !settings.caseSensitive)
		result += words[idx]->occurences[1];
	if (!firstUpper || !settings.caseSensitive)
		result += words[idx]->occurences[0];
	return result;
}

/* -- SuggestWords --
   Synopsis: 
	Select words which should be suggested to the user and display dialog.
	The possible words have already been determined by IncrementalLookup.
*/
void CAutoTypeDoc::SuggestWords(POINT caretPos)
{
	if (!theApp.m_bSuggest) return;

	static short lastHeight = 0;
	static short lastWidth = 0;
	int lbound = last_lbound.GetHead();
	int ubound = last_ubound.GetHead();
	int lastIdx = -1;
	int curLength = curWord.GetLength();

	// These variables are used to determine if
	// the official caret position is updated or
	// if we should use the mouse cursor's 
	// position instead.
	static short lastLength = -1;
	static CPoint lastCaretPos(-1, -1);
	static HWND lastDestWnd = NULL;
	static bool useMousePos = false;

	// Determine if we should use the caret's or the cursor's
	// position.
	if (lastDestWnd != destWnd)	{
		useMousePos = false;
		lastDestWnd = destWnd;
	}
	else if (lastLength != curLength)
		useMousePos = (settings.suggestPos != POS_FIXED && lastCaretPos == caretPos);
	lastCaretPos = caretPos;
	lastLength = curLength;

	suggestions.RemoveAll();

	// Determine how many occurences of the currently possible words
	// there have been. The longer a word has been dormant, the less
	// its frequency is valued.
	int totalOccurences = 0;
	time_t now = time(NULL) / DAY_SECONDS;
	for (int i = lbound; i < ubound; i++) {
		int wordAge = max(now - words[i]->lastUse, 20);
		totalOccurences += max(1, GetOccurences(i) - wordAge/2);
	}
	if (!totalOccurences) return;

	CStringDWORDPair word;
	bool group = true;
	DWORD groupOccurences = 0;
	for (i = lbound; i < ubound; i++)
	{
		int wordOccurences = GetOccurences(i);
		int wordAge = now - words[i]->lastUse;

		if (wordOccurences)
		{
			// Check if word contains a digit at the current position.
			// If so, we must not make ANY suggestions.
			if (curLength < words[i]->word.GetLength())
			{
				char c = words[i]->word[curLength];
				if (c >= '1' && c < BYTE('1'+settings.maxSuggested)) {
					suggestions.RemoveAll();
					return;
				}
			}

			// Check if word is long enough to be suggested.
			if (words[i]->word.GetLength() - curLength >= settings.minAdded)
			{
				// Compute probability of this word, and only suggest it
				// if it's above the lower limit.
				word.w = max(1, wordOccurences - wordAge/2) * 100 / totalOccurences;
				if (word.w >= settings.howCertain) 
				{
					word.s = firstUpper ? 
						char(CharUpper(LPTSTR(BYTE(words[i]->word[0])))) + words[i]->word.Mid(1) :
						words[i]->word;

					// Check if previous word is a known predecessor.
					WORD prevOccurences;
					if (prevSoundEx[1] && words[i]->predecessors.Lookup(prevSoundEx[1], prevOccurences))
						word.w += DWORD(prevOccurences) * 100 / totalOccurences;

					// Check if current window class is known to word.
					if (m_destWndClassHash && words[i]->wndClasses.Find(m_destWndClassHash))
						word.w *= 2;

					suggestions.Add(word);
				}
			}

			// Add word pairs.
			if (settings.multiWords)
				for (short j = 0; j < words[i]->successors.GetSize(); j++)
				{
					CWordInfo *successor = words[words[i]->successors[j]->wordIdx];
					if (words[i]->word.GetLength() - curLength + 
						words[i]->successors[j]->spaceString.GetLength() +
						successor->word.GetLength() >= settings.minAdded)
					{
						word.w = DWORD(words[i]->successors[j]->occurences) * 100 / totalOccurences;
						if (word.w >= settings.howCertain)
						{
							word.s = 
								(firstUpper ? 
								 ToUpper(words[i]->word[0]) + words[i]->word.Mid(1) :
								 words[i]->word) +
								words[i]->successors[j]->spaceString +
								(words[i]->successors[j]->upperCase ?
								 ToUpper(successor->word[0]) + successor->word.Mid(1) :
								 successor->word);

							// Check if previous word is a known predecessor.
							WORD prevOccurences;
							if (prevSoundEx[1] && words[i]->successors[j]->predecessors.Lookup(prevSoundEx[1], prevOccurences))
								word.w += DWORD(prevOccurences) * 100 / totalOccurences;

							suggestions.Add(word);
						}
					}
				}

			// Group words which only differ in their last character.
			if (lastIdx >= 0 && group &&
				words[i]->word.Left(words[i]->word.GetLength()-1)
					== words[lastIdx]->word.Left(words[lastIdx]->word.GetLength()-1))
			{
				if (!groupOccurences)
					groupOccurences = GetOccurences(lastIdx);
				groupOccurences += wordOccurences;
			}
			else
			{
				if (!group &&
					words[i]->word.Left(words[i]->word.GetLength()-1)
						!= words[lastIdx]->word.Left(words[lastIdx]->word.GetLength()-1))
					group = true;
				if (groupOccurences && 
					words[lastIdx]->word.GetLength()-1 - curLength >= settings.minAdded)
				{
					word.s = words[lastIdx]->word.Left(words[lastIdx]->word.GetLength()-1);
					word.w = groupOccurences * 100 / totalOccurences;
					suggestions.Add(word);
				}
				groupOccurences = 0;
			}
			if (lastIdx >= 0 && !groupOccurences &&
				words[i]->word.Left(words[i]->word.GetLength()-1) == words[lastIdx]->word)
				group = false;
			lastIdx = i;
		}
	}
	if (groupOccurences &&
		words[lastIdx]->word.GetLength()-1 - curLength >= settings.minAdded)
	{
		word.s = words[lastIdx]->word.Left(words[lastIdx]->word.GetLength()-1);
		word.w = groupOccurences * 100 / totalOccurences;
		suggestions.Add(word);
	}

	if (suggestions.GetSize()) 
	{
		if (settings.ringBell && firstSuggestion)
			Beep(250, 500);		// these parameters are ignored by Win95
		firstSuggestion = false;

		// Use either the caret, the cursor or a fixed position for
		// the suggestion window.
		POINT wndPos;
		if (settings.suggestPos == POS_CURSOR || useMousePos) {
			if (!theApp.m_dlgSuggestions->m_bMouseSelection)
				GetCursorPos(&wndPos);
			else
				wndPos = theApp.m_dlgSuggestions->m_ptInitial;
		}
		else if (settings.suggestPos == POS_CARET) {
			wndPos = caretPos;
			wndPos.x += settings.suggestCoord.x;
			wndPos.y += settings.suggestCoord.y;
			ClientToScreen(destWnd, &wndPos);
		}
		else // POS_FIXED 
			wndPos = settings.suggestCoord;

		theApp.m_dlgSuggestions->foreColor = theApp.foreColor;
		theApp.m_dlgSuggestions->backColor = theApp.backColor;

		short height;
		short width = theApp.m_dlgSuggestions->SetSuggestions(settings.maxSuggested, suggestions, height);
		short count = min(settings.maxSuggested, suggestions.GetSize());
		RECT screen, winD;
		GetWindowRect(GetDesktopWindow(), &screen);
		winD.top = winD.left = winD.right = 0;
		winD.bottom = 16 + count*height;
		theApp.m_dlgSuggestions->MapDialogRect(&winD);
		winD.bottom += count * 2;
		winD.right = width+10;
		if (wndPos.x+winD.right > screen.right)
			wndPos.x = screen.right - winD.right;
		if (settings.suggestPos == POS_CARET && settings.suggestCoord.y < 0) 
		{
			wndPos.y -= winD.bottom;
			if (wndPos.y < screen.top) 
				wndPos.y += winD.bottom + 2*(-settings.suggestCoord.y);
		}
		else if (wndPos.y+winD.bottom > screen.bottom)
			wndPos.y -= 2*settings.suggestCoord.y + winD.bottom;
		theApp.m_dlgSuggestions->m_ptInitial = wndPos;
		theApp.m_dlgSuggestions->SetWindowPos(NULL, wndPos.x, wndPos.y, winD.right, winD.bottom,
			(count*height != lastHeight || width != lastWidth ? 0 : SWP_NOSIZE) | 
			SWP_NOACTIVATE | SWP_NOZORDER);

		// Adapt position and height of static text to currently
		// chosen font.
		winD.left = winD.top = 2;
		winD.bottom = height;
		theApp.m_dlgSuggestions->MapDialogRect(&winD);
		for (i = 0; i < count; i++) 
		{
			theApp.m_dlgSuggestions->m_Static[i].SetWindowPos(NULL, 
				winD.left, winD.top + i*(winD.bottom+2), width, winD.bottom,
				SWP_NOACTIVATE | SWP_NOZORDER);
		}
		theApp.m_dlgSuggestions->ShowWindow(SW_SHOWNA, settings.suggestPos, 
			settings.suggestionTimeout);
		lastHeight = count*height;
		lastWidth = width;
		suggesting = true;
	}
}

/* -- ChangeSettings --
   Synopsis: 
	Changes the settings for this document.
	Before doing this, the settings are checked for
	validity.

   Parameters:
	s				The new settings.
*/
void CAutoTypeDoc::ChangeSettings(DocSettings s)
{
	ASSERT(s.minGiven >= 1);
	ASSERT(s.minAdded >= 1);
	ASSERT(s.howCertain <= 100);
	ASSERT(s.maxSuggested < 10);

	if (memcmp(&settings, &s, sizeof(s))) {
		if (settings.suggestPos != s.suggestPos)
			switch (s.suggestPos)
			{
				case POS_CARET:
					s.suggestCoord = CPoint(0, 14);
					break;
				case POS_CURSOR:
					s.suggestCoord = CPoint(0, 0);
			}
		settings = s;
		SetModifiedFlag();
	}
}

/* -- ReadFile --
   Synopsis: 
	Reads a real file, or the clipboard contents mapped to a file,
	and saves the contained words as if the user had typed them.

   Parameters:
	in				The input stream.
	bFilter			Only certain character combinations should be
					recognized as valid words and stored.
	bPersistent		New words are marked as persistent. Should be
					set for real and unset for virtual files (clipboard).
*/
void CAutoTypeDoc::ReadFile(CFile* in, bool bPersistent, bool bFilter)
{
	BeginWaitCursor();
	view->SetRedraw(FALSE);

	// Start saving words.
	Abort();
	Restart();

	try
	{
		char buf[4096];
		UINT n;
		bool discardMultiWord = false;
		do 
		{
			n = in->Read(buf, 4096);
			for (UINT i = 0; i < n && buf[i]; i++)
			{
				if (IsWordChar(buf[i]))
				{
					// Try to keep spaceString sensible.
					if (curSpaceString.GetLength() > 3) curSpaceString = " ";

					SaveWord(bPersistent, bFilter);
					if (discardMultiWord) {
						m_iPrevWord = -1;
						discardMultiWord = false;
					}

					NextChar(buf[i], false, CPoint(0,0));
				}
				else if (buf[i] == '\n') {
					curSpaceString += ' ';
					discardMultiWord = true;
				}
				else if (buf[i] >= ' ' && buf[i] <= '~')
					curSpaceString += buf[i];
				else
					curSpaceString += ' ';
			}
		} while (n == 4096);
	} catch (CFileException*) {}

	// Save last word.
	if (curSpaceString.IsEmpty()) curSpaceString = " ";
	SaveWord(bPersistent);
	Abort();

	view->SetRedraw(TRUE);
	EndWaitCursor();
}

// Automatically save changed document.
void CAutoTypeDoc::SaveChanged()
{
	CString pathName = GetPathName();
	if (settings.autoSave && IsModified() && !pathName.IsEmpty())
		OnSaveDocument(pathName);
}

// Return statistically information about this document
float* CAutoTypeDoc::GetAvgStats() const
{
	static float result[2];
	if (words.GetSize())
	{
		DWORD totalOccurences = 0, totalAge = 0;
		time_t t;
		time(&t);
		t /= DAY_SECONDS;
		for (int i = 0; i < words.GetSize(); i++)
		{
			totalOccurences += words[i]->occurences[0] + words[i]->occurences[1];
			totalAge += t - words[i]->lastUse;
		}
		result[0] = float(totalOccurences) / words.GetSize();
		result[1] = float(totalAge) / words.GetSize();
	}
	else
		memset(result, 0, sizeof(float)*2);
	return result;
}

/* -- ToUpper --
   Synopsis: 
	Converts a character to upper case. Takes care of
	German umlauts.

   Parameters:
	c				The character to be converted.

   Return value:
	The converted character.
*/
char CAutoTypeDoc::ToUpper(char c) const
{
	return char(CharUpper(LPTSTR(BYTE(c))));
}

/* -- ToLower --
   Synopsis: 
	Converts a character to lower case. Takes care of
	German umlauts.

   Parameters:
	c				The character to be converted.

   Return value:
	The converted character.
*/
char CAutoTypeDoc::ToLower(char c) const
{
	return char(CharLower(LPTSTR(BYTE(c))));
}

/* -- SaveModified --
   Synopsis: 
	Automatically saves the document if the according
	option is set. Else the user is asked if the document
	should be saved.

   Return value:
	TRUE if it is safe to continue and close the document; 
	FALSE if the document should not be closed.

*/
BOOL CAutoTypeDoc::SaveModified() 
{
	SaveChanged();	
	return CDocument::SaveModified();
}

void CAutoTypeDoc::Paste(bool bPersistent, bool bFilter)
{
	COleDataObject text;
	if (text.AttachClipboard()) {
		if (text.IsDataAvailable(CF_TEXT)) {
			CFile *in = text.GetFileData(CF_TEXT);
			if (in) {
				if (!bFilter || in->GetLength() < 4096) 
					ReadFile(in, bPersistent, bFilter);
				delete in; 
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// Message handlers

BOOL CAutoTypeDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if (IsModified() || GetPathName() != lpszPathName)
	{
		if (GetPathName() == lpszPathName)		// backup old file
		{
			CString backup = lpszPathName;
			if (backup.Right(4).CompareNoCase(".lmt") == 0)
				backup = backup.Left(backup.GetLength()-3) + "bak";
			DeleteFile(backup);
			MoveFile(lpszPathName, backup);
		}

		if (!timerId) timerId = theApp.m_pMainWnd->SetTimer(1, TIMER_SPAN, NULL);
		ASSERT(timerId);

		return CDocument::OnSaveDocument(lpszPathName);
	}
	else return TRUE;
}

// Do some work every now and then.
void CAutoTypeDoc::OnTimer()
{
	if (!IsModified() || !settings.autoSave || GetPathName().IsEmpty())
		return;

	time_t t;
	time(&t);
	static shortTimer = false;

	if (!curWord.IsEmpty() || t - lastMsgTime < 15)
	{
		if (shortTimer) return;

		theApp.m_pMainWnd->KillTimer(timerId);
		timerId = theApp.m_pMainWnd->SetTimer(1, SHORT_TIMER_SPAN, NULL);
		ASSERT(timerId);
		shortTimer = true;
	}
	else
	{
		SaveChanged();

		if (shortTimer) {
			theApp.m_pMainWnd->KillTimer(timerId);
			timerId = theApp.m_pMainWnd->SetTimer(1, TIMER_SPAN, NULL);
			ASSERT(timerId);
			shortTimer = false;
		}
	}
}

/* -- OnCloseDocument --
   Synopsis: 
	Called when the document is about to be closed.
	Before doing so, the current placement of LMT's
	main window is saved.
*/
void CAutoTypeDoc::OnCloseDocument() 
{
	if (timerId)
	{
		theApp.m_pMainWnd->KillTimer(timerId);
		timerId = NULL;
	}

	theApp.SaveWndPlace();
	CDocument::OnCloseDocument();
}

void CAutoTypeDoc::OnViewRefresh() 
{
	BeginWaitCursor();
	view->SetRedraw(FALSE);
	view->RemoveAll();
	view->GetListCtrl().SetItemCount(words.GetSize());
	for (int i = 0; i < words.GetSize(); i++)
		view->AddWord(i, words[i]);
	view->SetRedraw(TRUE);
	EndWaitCursor();
}

void CAutoTypeDoc::OnFileShortcut() 
{
	// Let user select the directory where the shortcut is to be saved.
	CString docPath = GetPathName();
	docPath = docPath.Mid(docPath.ReverseFind('\\')+1);
	docPath = docPath.Left(docPath.GetLength() - 3) + "lnk";

	CString filters;
	filters.LoadString(IDS_SHORTCUT_FILTERS);

	// Look for StartUp directory.
	LPITEMIDLIST idl;
	SHGetSpecialFolderLocation(NULL, CSIDL_STARTUP, &idl);
	char startUp[MAX_PATH];
	SHGetPathFromIDList(idl, startUp);
	SetCurrentDirectory(startUp);

	CFileDialog dlg(FALSE, "lnk", docPath, 
		OFN_HIDEREADONLY | OFN_NODEREFERENCELINKS | OFN_NONETWORKBUTTON | 
		OFN_NOREADONLYRETURN, filters);
	if (dlg.DoModal() != IDOK)
		return;

	// Retrieve LMT's path (needed for the shortcut's icon).
	CString lmtPath = theApp.m_pszExeName;
	lmtPath += ".EXE";

	// Create shortcut.
	CShortcut shortcut;

	shortcut.Create(dlg.GetPathName(), GetPathName(), "", "", "", 
		lmtPath, 0, 0, SW_SHOWMINNOACTIVE);
}

void CAutoTypeDoc::OnUpdateFileShortcut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetPathName().IsEmpty());
}

void CAutoTypeDoc::OnFileRead() 
{
	CString filters;
	filters.LoadString(IDS_TEXT_FILTERS);
	CFileDialog dlg(TRUE, "txt", NULL, OFN_FILEMUSTEXIST, filters);
	if (dlg.DoModal() != IDOK) return;

	try {
		CStdioFile in(dlg.GetPathName(), CFile::modeRead | CFile::shareDenyWrite);
		ReadFile(&in);
	} catch (CFileException*) {}
}

void CAutoTypeDoc::OnEditPaste() 
{
	Paste(true, false);
}

void CAutoTypeDoc::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!theApp.m_bMonitorClipboard && IsClipboardFormatAvailable(CF_TEXT));
}

void CAutoTypeDoc::SendSuggestion(int suggestionIdx, bool deleteChar)
{
	StopSuggesting();

	// We have to give the application a chance
	// to process the message that triggered the
	// suggestion sending.
	Sleep(20);

	sentWord = suggestions[suggestionIdx].s.Mid(curWord.GetLength());

	if (settings.addSpace) sentWord += ' ';
	SendWord(sentWord, deleteChar);

	theApp.IncrTypedSelf();
	theApp.IncrTypedAuto(sentWord.GetLength());
}

void CAutoTypeDoc::OnUpdateOptionsApplications(CCmdUI* pCmdUI) 
{
	if (theApp.m_osVersion.dwPlatformId == VER_PLATFORM_WIN32_NT && 
		theApp.m_osVersion.dwMajorVersion >= 4)
		pCmdUI->Enable(BOOL(theApp.pGetModuleFileNameEx));
}

// Save clipboard contents, so we can restore them
// when we no longer need the clipboard.
void CAutoTypeDoc::SaveClipboard()
{
	RemoveClipboardContents();

	UINT format = 0;
	while (format = EnumClipboardFormats(format))
	{
		HGLOBAL hData = GetClipboardData(format);
		if (!hData) continue;

		CClipboardContent *c = new CClipboardContent();
		DWORD size = GlobalSize(hData);
		LPVOID lpData = GlobalLock(hData);
		if (size && lpData)
		{
			c->format = format;
			c->size = size;
			c->data = malloc(size);
			memcpy(c->data, lpData, size);
		}
		GlobalUnlock(hData);

		m_clipboardContents.Add(c);
	}
}

void CAutoTypeDoc::RestoreClipboard()
{
	EmptyClipboard();
	for (int i = 0; i < m_clipboardContents.GetSize(); i++)
	{
        HGLOBAL hData = NULL;
		if (m_clipboardContents[i]->data)
		{
			hData = GlobalAlloc(GMEM_DDESHARE, m_clipboardContents[i]->size);
			LPVOID lpData = GlobalLock(hData);
			memcpy(lpData, m_clipboardContents[i]->data, m_clipboardContents[i]->size);
			GlobalUnlock(hData);
		}
        SetClipboardData(m_clipboardContents[i]->format, hData);
	}
	RemoveClipboardContents();
}

void CAutoTypeDoc::RemoveClipboardContents()
{
	for (int i = 0; i < m_clipboardContents.GetSize(); i++)
		delete m_clipboardContents[i];
	m_clipboardContents.RemoveAll();
}
