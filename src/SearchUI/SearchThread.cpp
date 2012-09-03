#include "stdafx.h"
#include "SearchThread.h"

SearchThread::SearchThread(HWND notifyWnd, SearchOption option) 
		: notifyWnd_(notifyWnd)
		, option_(option)
{
}

SearchThread::~SearchThread(void)
{
}

UINT SearchThread::Run()
{
	startTick_ = GetTickCount();
	
	bool goon = true;
	
	if( goon && option_.searchHKLM_ )
	{
		RegistrySearch RS(HKEY_LOCAL_MACHINE, L"HKLM");
		goon = RS.Search(L"", option_, this);
	}
	if( goon && option_.searchHKCU_ )
	{
		RegistrySearch RS(HKEY_CURRENT_USER, L"HKCU");
		goon = RS.Search(L"", option_, this);
	}
	if( goon && option_.searchHKCR_)
	{
		RegistrySearch RS(HKEY_CLASSES_ROOT, L"HKCR");
		goon = RS.Search(L"", option_, this);
	}
	if( goon && option_.searchHKUSERS_ )
	{
		RegistrySearch RS(HKEY_USERS, L"HKUSERS");
		goon = RS.Search(L"", option_, this);
	}
	if( goon && option_.searchHKCONFIG_ )
	{
		RegistrySearch RS(HKEY_CURRENT_CONFIG, L"HKCC");
		goon = RS.Search(L"", option_, this);
	}

	int second = (GetTickCount() - startTick_) / 1000;
	PostMessage(notifyWnd_, UM_SEARCH_END, WPARAM(second), NULL);
	return 0;
}

bool SearchThread::OnFound( CAtlString key, RegItem* item )
{
	if ( !IsWindow(notifyWnd_))
	{
		delete item;
		return false;
	}
	PostMessage(notifyWnd_, UM_SEARCH_ITEM, (WPARAM) new CAtlString(key), (LPARAM)item);
	return true;
}
