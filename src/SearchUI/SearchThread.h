#pragma once
#include "SearchUI\SimpleThread.h"
#include "RegWorks\RegSearch.h"
using namespace RegSearch;

#define  UM_SEARCH_ITEM (WM_APP)
#define  UM_SEARCH_END (WM_APP + 1)

class SearchThread : public SimpleThread
						, public ISearchNotify
{
public:
	SearchThread(SearchThread*& threadPtr, HWND notifyWnd, SearchOption option);
	~SearchThread(void);
	void Cancel();
	virtual bool OnFound(CAtlString key, RegItem* item);

protected:
	virtual UINT Run();
	HWND notifyWnd_;
	SearchOption option_;
	UINT startTick_;
	SearchThread*& threadPtr_;
};
