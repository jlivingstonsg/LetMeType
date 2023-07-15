// AutoTdoc.h : interface of the CAutoTypeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __AUTOTDOC_H__
#define __AUTOTDOC_H__

#include <time.h>
#include "soundx.h"
#include "suggestw.h"
#include "SortedArray.h"

#ifdef _DEBUG
void AFXAPI DumpElements(CDumpContext& dc, const SoundEx* pElements, int nCount);
void AFXAPI DumpElements(CDumpContext& dc, const WORD* pElements, int nCount);
#endif

#include <afxtempl.h>

const short MAX_WND_CLASSES = 10;
const short MAX_SUCCESSORS = 25;
extern const char* DEFAULT_CHARS;
extern const long TIMER_SPAN;


// CNextWord holds information about the words that have
// followed a certain word so far.
class CNextWord : public CObject
{
public:
	CNextWord();
	CNextWord(const CNextWord& src);
	virtual void Serialize(CArchive& ar);

	CString spaceString;	// string by which the two words were separated
	WORD wordIdx;
	bool upperCase;		// whether the next word started with an upper letter
	WORD occurences;
	CMap<SoundEx, SoundEx&, WORD, WORD&> predecessors;
		// predecessors' soundex and count of occurences
	WORD lastUse;		// date of last use

	DECLARE_SERIAL(CNextWord)
};
void AFXAPI SerializeElements(CArchive& ar, CNextWord** pElements, int nCount);


// The words the user has typed are stored using CWordInfo.
class CWordInfo : public CObject
{
public:
	CWordInfo();
	CWordInfo(const CWordInfo& src);
	~CWordInfo();

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);

	int GetSize() const;
	DWORD TotalOccurences() const { return occurences[0]+occurences[1]; }

	DECLARE_SERIAL(CWordInfo)


	// Attributes

	CString word;
	WORD occurences[2];
		// distinguishes lower and upper starting letters
	CMap<SoundEx, SoundEx&, WORD, WORD&> predecessors;
		// predecessors' soundex and count of occurences
	CList<DWORD, DWORD&> wndClasses;
		// windows this word was typed in
	WORD lastUse;
		// date of last use
	CArray<CNextWord*, CNextWord*> successors;
		// possible successors
	bool persistent;
};
typedef CWordInfo *CWordInfoP;


// Misc. persistent settings of LMT documents.
// When changing these don't forget to update settings.cpp.
// Don't put any pointers or fancy data structures here - 
// this will be serialized as one binary string.
struct DocSettings
{
	BYTE minGiven,			// length a word has to have 
							// before suggestions are made
		 minAdded,			// minimum number of added characters
		 howCertain;		// value determining if a word is suggested
	WORD minTypeTime;		// time that has to elapse between two
							// arriving keyboard messages for activation
							// of suggestions
	WORD maxAge;			// max no. of days since word was last used
	WORD maxSize;			// max size of file (in KBytes)
	bool ringBell;			// ring the bell when suggestions dialog pops up
	bool autoSave;			// automatically save document every x minutes
	BYTE maxSuggested;		// max. number of made suggestions
	char wordChars[256-70];
	bool allowDigits, allowBrackets;
	bool multiWords;		// enables suggestion of word sequences
	bool addSpace;			// if true, a space is inserted after an
							// autotyped word
	POINT suggestCoord;		// either a fixed position or relative to
							// the current caret position
	bool fixedVocabulary;	// new words are not saved permanently
	bool caseSensitive;		// preserve word capitalization (first letter only,
							// the others are always case sensitive)
	bool numpad;			// only use numpad keys for suggestion selection
	bool ignorePasswords;	// ignore words entered into password fields
	bool allowNumbers;		// memorize "words" that start with a digit
	int suggestionTimeout;	// time in ms after which the suggestion window
							// will disappear; 0 if disabled
	SuggestionPosition suggestPos; // POS_CARET, POS_FIXED or POS_CURSOR
};

// Used to save clipboard contents in an array.
class CClipboardContent : public CObject
{
public:
	CClipboardContent();
	~CClipboardContent();

	UINT format;
	DWORD size;
	void *data;
};

class CAutoTypeView;
class CAutoTypeDoc : public CDocument
{
	friend class CWordInfo;

protected: // create from serialization only
	CAutoTypeDoc();
	DECLARE_DYNCREATE(CAutoTypeDoc)

// Attributes
public:
	CAutoTypeView* view;
	WORD m_nFileVersion;

// Operations
public:
	virtual void DeleteContents();
	const CWordInfo* GetInfo(int idx) const { return words[idx]; };

	void RemoveWord(int idx);
	bool ChangeWord(int oldIdx, CString word);
	void Paste(bool bPersistent, bool bFilter);

	void OnTimer();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoTypeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

// Implementation
public:
	virtual ~CAutoTypeDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o

	void ProcessMsg(MsgData msg);
	void SendSuggestion(int suggestionIdx, bool deleteChar);
	void Abort();
	void StopSuggesting();

	DocSettings GetSettings() const { return settings; }
	void ChangeSettings(DocSettings s);

	int GetWordCount() const { return words.GetSize(); }
	float* GetAvgStats() const;

	bool IsWordChar(char c) const;
	char ToUpper(char c) const;
	char ToLower(char c) const;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Information about current destination window
	HWND destWnd;
	HWND lastWnd;
	char m_destWndClass[256];
	DWORD m_destWndClassHash;		// hash value of window's class string
	bool isPasswordField;
	 
	// Used while serializing the document
	CArray<int, int&> tmpIdxMap;

	UINT timerId;			// AutoSave timer


protected:
	void Serialize(CArchive& ar, WORD& fileVersion, 
		CTypedPtrArray<CObArray, CWordInfo*>& words, DocSettings& settings, time_t& lastGC);
	void SaveChanged();

	enum SyncState { SYNC_NONE, SYNC_SUGGEST, SYNC_LEARN };
	void Restart(SyncState syncState = SYNC_LEARN);

	void NextChar(char c, bool makeSuggestion, POINT caretPos);
	void PrevChar(bool makeSuggestion, POINT caretPos);
	bool SaveWord(bool persistent = false, bool filter = false);
	bool ValidWord(bool filter) const;
	int GetOccurences(int idx) const;
	void ShiftWordPointers(int idx, short delta);

	void SendWord(CString w, bool deleteChar);
	bool IncrementalLookup();
	void SuggestWords(POINT caretPos);

	void CollectGarbage();

	int Find(const CString& word, int* insertIdx = NULL) const;

	void ReadFile(CFile* in, bool bPersistent = true, bool bFilter = false);

	// Information about word currently being typed
	CString curWord;
	CString prevSpaceString, curSpaceString;
	CList<int, int> last_ubound, last_lbound;
	bool firstUpper;

	// Whether it's safe to treat input as a word
	SyncState synchronized;

	// Whether we have yet displayed suggestions for the current word
	bool firstSuggestion;

	// Whether the incremental search can continue or we've reached a dead end
	bool canContinue;

	// Index of first character in current word that could not be matched
	WORD firstUnknownChar;

	// Soundex value of the two previous words.
	// prevSoundEx[0] holds the older one.
	SoundEx prevSoundEx[2];

	// Index of previous word
	int m_iPrevWord;

	// Time last msg was received
	time_t lastMsgTime;
	
	// Window displaying suggestions and info about it's status
	CSArray<CStringDWORDPair, CStringDWORDPair&> suggestions;
	bool suggesting;

////////////////////
// Persistent values

	// The words that have been typed by the user
	CTypedPtrArray<CObArray, CWordInfo*> words;

	// Misc. parameters
	DocSettings settings;

	// Time of last garbage collection.
	time_t lastGC;

// Generated message map functions
protected:
	//{{AFX_MSG(CAutoTypeDoc)
	afx_msg void OnViewRefresh();
	afx_msg void OnFileShortcut();
	afx_msg void OnUpdateFileShortcut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnFileRead();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnFileMerge();
	afx_msg void OnUpdateOptionsApplications(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void RestoreClipboard();
	void RemoveClipboardContents();
	void SaveClipboard();
	bool ignoreBackspace;
	CString sentWord;
	CArray<CClipboardContent *, CClipboardContent *> m_clipboardContents;
};

// Exceptions
enum WCException {OUT_OF_CHARACTERS};

/////////////////////////////////////////////////////////////////////////////

extern CAutoTypeDoc *theDoc;

#endif
