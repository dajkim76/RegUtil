#pragma once

#include <vector>

// CSearchHistoryWnd

class CSearchHistoryWnd : public CFrameWnd
{
	DECLARE_DYNAMIC(CSearchHistoryWnd)

public:
	CSearchHistoryWnd();
	virtual ~CSearchHistoryWnd();
	CListCtrl& GetListCtrl() { return currentView_; }

protected:
	DECLARE_MESSAGE_MAP()
	CTabCtrl	tab_;
	CMenu menu_;
	CListCtrl currentView_;	

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSearchItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSearchEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNewSearch();
	afx_msg void OnMnuSaveas();
	afx_msg void OnMnuResearch();
};



