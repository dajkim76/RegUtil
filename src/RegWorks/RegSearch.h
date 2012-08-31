#pragma once

#include <atlstr.h>
#include <vector>
using namespace std;

namespace RegSearch
{
	class RegItem
	{
	public:
		RegItem(LPCTSTR szText = L"") 
			: type_(0)
			, dwordData_(0)
			, qwordData_(0)
		{
			text_ = szText;
		}
		
		CAtlString key_;		// key
		DWORD	type_;			// type of value
		CAtlString	name_;		// name of value
		
		// value
		DWORD	dwordData_;		/// DWORD
		__int64	qwordData_;		/// QWord
		CAtlString	text_;		/// REG_SZ, REG_EXPAND_SZ, REG_MULTI_SZ, REG_BINARY
	};

	typedef std::vector< RegItem* > RegItemList;
	void ClearRegItemList( RegItemList& itemList);

	class SearchOption
	{
	public:
		SearchOption() 
				: caseSenstive_(false)
				, nameCheck_(true)
				, keyCheck_(true)
				, valueCheck_(true)
				, canceled_(false) 
		{
			searchHKLM_ = true;
			searchHKCU_ = true;
			searchHKCR_ = false;
			searchHKUSERS_ = false;
			searchHKCONFIG_ = false;
		}
		CAtlString keyword_;
		bool caseSenstive_;
		bool keyCheck_;
		bool nameCheck_;		
		bool valueCheck_;
		bool canceled_;

		bool searchHKLM_;
		bool searchHKCU_;
		bool searchHKCR_;
		bool searchHKUSERS_;
		bool searchHKCONFIG_;		
	};	

	/// 검색시 발견되면 Post로 노티할 인터페이스
	class ISearchNotify
	{
	public:
		// false 이면 끝
		virtual bool OnFound(CAtlString key, RegItem* item) = 0;
	};
	
	class RegistrySearch
	{
	public:
		RegistrySearch (HKEY root) : root_(root) {}
		
		bool Search(CAtlString key, SearchOption& option, ISearchNotify* notify)
		{
			return _Search(root_, key, option, notify);
		}

	protected:
		HKEY root_;

		bool _Search(HKEY root, CAtlString key, SearchOption& option, ISearchNotify* notify);		
	};
	
};