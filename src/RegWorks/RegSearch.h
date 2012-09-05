#pragma once

#include <atlstr.h>
#include <vector>
using namespace std;

namespace RegSearch
{
	#define MAX_REG_KEY_NAME		(512)
	#define MAX_REG_KEY_VALUE		(32767)

	bool FindKeyword(const CAtlString& source, const CAtlString& keyword, bool isCasesensitive);

	class RegItem
	{
	public:
		RegItem(LPCTSTR szText = L"") 
			: type_(0)
			, dwordData_(0)
			, qwordData_(0)
			, length_(0)
			, text_(szText)
		{
		}
		
		CAtlString key_;		// key
		DWORD	type_;			// type of value
		CAtlString	name_;		// name of value
		
		// value
		DWORD	dwordData_;		/// DWORD
		__int64	qwordData_;		/// QWord
		CAtlString	text_;		/// REG_SZ, REG_EXPAND_SZ, REG_MULTI_SZ, REG_BINARY

		FILETIME filetime_;
		int length_;
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
		static const int sizeData = MAX_REG_KEY_VALUE;
		static const int cchTempBuffer = MAX_REG_KEY_VALUE*3 + 10;
		RegistrySearch (HKEY root, CAtlString rootName) : root_(root), rootName_(rootName) 
		{
			data_ = new BYTE[sizeData];
			tempBuffer_ = new TCHAR [cchTempBuffer];
		}
		
		~RegistrySearch()
		{
			delete []data_;
			delete []tempBuffer_;
		}

		bool Search(CAtlString key, SearchOption& option, ISearchNotify* notify)
		{
			return _Search(root_, key, option, notify);
		}

	protected:
		HKEY root_;
		CAtlString rootName_;
		BYTE* data_;
		TCHAR* tempBuffer_;

		bool _Search(HKEY root, CAtlString key, SearchOption& option, ISearchNotify* notify);		
		void _SearchRegistryKeyValue(HKEY hKey, CAtlString key, RegItemList& resultList, const SearchOption& option);
	};
	
};