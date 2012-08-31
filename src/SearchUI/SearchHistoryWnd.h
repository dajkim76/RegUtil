#pragma once

#include <vector>

// CSearchHistoryWnd

class CSearchHistoryWnd : public CFrameWnd
{
	DECLARE_DYNAMIC(CSearchHistoryWnd)

public:
	CSearchHistoryWnd();
	virtual ~CSearchHistoryWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CTabCtrl	tab_;
	CMenu menu_;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSearchItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSearchEnd(WPARAM wParam, LPARAM lParam);

	std::vector<CListCtrl*> viewList_;
	CListCtrl* currentView_;	
	afx_msg void OnNewSearch();
};



