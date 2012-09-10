#pragma once

#include <vector>
#include "ListCtrlSortClass.h"

// CSearchHistoryWnd
class SearchThread;

#define  MAX_SORT_COLUMN (6)

class CSearchHistoryWnd : public CFrameWnd
{
	DECLARE_DYNAMIC(CSearchHistoryWnd)

public:
	CSearchHistoryWnd();
	virtual ~CSearchHistoryWnd();
	CListCtrl& GetListCtrl() { return currentView_; }
	void SetKeyword(CString keyword, bool caseSensitive) { keyword_ = keyword; caseSensitive_ = caseSensitive; }

protected:
	DECLARE_MESSAGE_MAP()
	CTabCtrl	tab_;
	CMenu menu_;
	CListCtrl currentView_;	
	SearchThread* searchThread_;
	CString keyword_;
	bool caseSensitive_;

	CListCtrlSortClass::SortDataType columnType_[MAX_SORT_COLUMN];
	bool sortOrder_ [MAX_SORT_COLUMN];
	int curSortColumn_;
	HBITMAP hbmUp_;
	HBITMAP hbmDn_;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSearchItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSearchEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNewSearch();
	afx_msg void OnMnuSaveas();
	afx_msg void OnMnuResearch();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSearchStop();
	afx_msg void OnCustomdrawMyList( NMHDR* pNMHDR, LRESULT* pResult );
};



