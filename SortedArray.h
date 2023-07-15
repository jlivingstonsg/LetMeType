// CSArray: Sorted arrays derived from CArray
// $Id: SortedArray.h,v 1.1.1.1 2002/05/12 14:40:56 cc Exp $

#ifndef _SortedArray_H_
#define _SortedArray_H_

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// global helpers (can be overridden)

// Function that determines if element1 is less, equal or greater than element2
template<class TYPE, class ARG_TYPE>
inline int AFXAPI SortElements(const TYPE element1, const ARG_TYPE element2)
{
	// this HAS to be overridden
	TRACE("SortElements has not been overridden!\n");
	ASSERT(0);

	return 0;	// never reached
}


enum MatchType { EXACT, PRECEDING, SUCCEEDING };
template<class TYPE, class ARG_TYPE>
class CSArray : public CArray<TYPE, ARG_TYPE>
{
public:
// Construction
	CSArray() { CArray<TYPE, ARG_TYPE>(); };

// Operations
	int Add(ARG_TYPE newElement);	// sorted insertion of an element

	// Lookup of an element with O(log n).
	int Find(ARG_TYPE element, MatchType = EXACT) const;	
};

/////////////////////////////////////////////////////////////////////////////
// CSArray<TYPE, ARG_TYPE> out-of-line functions

template<class TYPE, class ARG_TYPE>
int CSArray<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement)
{
	int lbound = 0, ubound = GetSize();

	// Optimization for cases where the new element is smaller than
	// the first or bigger than the last element.
	if (GetSize())
	{
		if (SortElements(GetAt(0), newElement) >= 0)
			ubound = 0;
		else if (SortElements(GetAt(GetSize()-1), newElement) <= 0)
			lbound = ubound = GetSize();
	}

	while (lbound != ubound)
	{
		int i = lbound + (ubound-lbound)/2;
		ASSERT(i >= 0 && i < GetSize());

		int result = SortElements(GetAt(i), newElement);
		if (result == 0)
		{
			lbound = i;
			break;
		}
		else if (result < 0)		// search right half
			lbound = i+1;
		else						// search left half
			ubound = i;
	}

	InsertAt(lbound, newElement);
	return lbound;
}


/* -- Find --
   Find: 
	Searches the sorted array to find a certain element. Boolean search is 
	used, so the complexity is O(log n). It's also possible to look for the 
	first element that is "<=" or ">=" a given element.
	Comparision between elements is done with the SortElements helper function.

   Parameters:
	element			The element to look for.
	matchType		Indicates if an exact match is wanted or if the 
					following or preceding element is sufficient in case
					there is no exact match.

   Return value:
	The element's index if one is found, else -1.
*/
template<class TYPE, class ARG_TYPE>
int CSArray<TYPE, ARG_TYPE>::Find(ARG_TYPE element, MatchType matchType) const
{
	int lbound = 0, ubound = GetSize();
	if (!ubound)
		return -1;

	while (lbound != ubound)
	{
		int i = lbound + (ubound-lbound)/2;
		ASSERT(i >= 0 && i < GetSize());

		int result = SortElements(GetAt(i), element);
		if (result == 0)			// Found exact match?
			return i;
		else if (result < 0)		// Search right half?
			lbound = i+1;
		else						// Search left half?
			ubound = i;
	}

	if (matchType == PRECEDING)
	{
		while (lbound >= 0 && 
			   (lbound == GetSize() || SortElements(GetAt(lbound), element) > 0))
			lbound--;
		return lbound;
	}
	else if (matchType == SUCCEEDING)
	{
		while (lbound < GetSize() && SortElements(GetAt(lbound), element) < 0)
			lbound++;
		return lbound;
	}

	return -1;	// Not found.
}

#endif // _SortedArray_H_
