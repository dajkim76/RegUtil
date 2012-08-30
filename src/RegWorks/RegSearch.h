#pragma once

#include <atlstr.h>
#include <vector>
using namespace std;

namespace RegSearch
{
	#define MAX_REG_KEY_NAME		512
	#define MAX_REG_KEY_VALUE		32767

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
		CAtlString key_;
		DWORD	type_;
		CAtlString	name_;
		
		// result
		DWORD	dwordData_;		/// DWORD
		__int64	qwordData_;	/// QWord
		CAtlString	text_;	/// SZ, SZ_EX, MULTI_SZ
	};

	typedef std::vector< RegItem* > RegItemList;

	class SearchOption;

	void EnumRegistryKey(HKEY hKey, CAtlString sKeyName, vector< CAtlString >& ListEnumKey);
	void SearchRegistryKeyValue(HKEY hKey, CAtlString sKeyName, RegItemList& resultList, const SearchOption& option);
	void ClearRegItemList( RegItemList& itemList);

	//////////////////////////////////////////////////////////////////////////

	class SearchOption
	{
	public:
		SearchOption() 
				: caseSenstive_(false)
				, nameCheck_(true)
				, keyCheck_(true)
				, valueCheck_(true)
				, canceled_(false) {}
		CAtlString keyword_;
		bool caseSenstive_;
		bool nameCheck_;
		bool keyCheck_;
		bool valueCheck_;
		bool canceled_;
	};

	bool FindKeyword(const CAtlString& source, const CAtlString& keyword, bool isCasesensitive);

	class ISearchNotify
	{
	public:
		virtual void OnFound(CAtlString key, RegItem* item) = 0;
	};

	bool RegistrySearch(HKEY root, CAtlString key, const SearchOption& option, RegItemList& resultList, ISearchNotify* notify = NULL);

};