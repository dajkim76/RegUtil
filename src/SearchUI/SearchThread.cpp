#include "stdafx.h"
#include "SearchThread.h"
#include "RegWorks\RegWorks.h"

SearchThread::SearchThread(SearchThread*& threadPtr, HWND notifyWnd, SearchOption option) 
		: SimpleThread(true)
		, notifyWnd_(notifyWnd)
		, option_(option)
		, threadPtr_(threadPtr)
{
}

SearchThread::~SearchThread(void)
{
	if ( ! option_.canceled_ )
	{
		threadPtr_ = NULL;
	}
}

UINT SearchThread::Run()
{
	startTick_ = GetTickCount();
	
	bool goon = true;
	
	if( option_.customkey_.GetLength())
	{
		CAtlString path = option_.customkey_;
	
		HKEY root = KeyRoot::toRootkey(path);
		ATLASSERT(root);

		int pos = path.Find(L"\\");
		if(pos > 0)
		{
			path = path.Mid(pos + 1);
		}

		RegistrySearch RS(root, L"");
		goon = RS.Search(path, option_, this);
	}
	else
	{
		if( goon && option_.searchHKLM_ )
		{
			RegistrySearch RS(HKEY_LOCAL_MACHINE, L"");
			goon = RS.Search(L"", option_, this);
		}
		if( goon && option_.searchHKCU_ )
		{
			RegistrySearch RS(HKEY_CURRENT_USER, L"");
			goon = RS.Search(L"", option_, this);
		}
		if( goon && option_.searchHKCR_)
		{
			RegistrySearch RS(HKEY_CLASSES_ROOT, L"");
			goon = RS.Search(L"", option_, this);
		}
		if( goon && option_.searchHKUSERS_ )
		{
			RegistrySearch RS(HKEY_USERS, L"");
			goon = RS.Search(L"", option_, this);
		}
		if( goon && option_.searchHKCONFIG_ )
		{
			RegistrySearch RS(HKEY_CURRENT_CONFIG, L"");
			goon = RS.Search(L"", option_, this);
		}
	}

	if( !option_.canceled_)
	{
		int second = (GetTickCount() - startTick_) / 1000;
		PostMessage(notifyWnd_, UM_SEARCH_END, WPARAM(second), NULL);
	}
	return 0;
}

bool SearchThread::OnFound( CAtlString key, RegItem* item )
{
	if( option_.canceled_ )
	{
		return false;
	}

	if ( !IsWindow(notifyWnd_))
	{
		delete item;
		return false;
	}
	PostMessage(notifyWnd_, UM_SEARCH_ITEM, (WPARAM) new CAtlString(key), (LPARAM)item);
	return true;
}

void SearchThread::Cancel()
{
	// NULL 포인터 참조시 안전하게
	// 쓰레드에서 이미지 소멸되었다면
	if( this == NULL )
	{
		return ;
	}
	option_.canceled_ = true;
	threadPtr_ = NULL;
}
