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
				, canceled_(false) {}
		CAtlString keyword_;
		bool caseSenstive_;
		bool keyCheck_;
		bool nameCheck_;		
		bool valueCheck_;
		bool canceled_;
	};	

	/// �˻��� �߰ߵǸ� Post�� ��Ƽ�� �������̽�
	class ISearchNotify
	{
	public:
		virtual void OnFound(CAtlString key, RegItem* item) = 0;
	};
	
	bool RegistrySearch(HKEY root, CAtlString key, const SearchOption& option, RegItemList& resultList, ISearchNotify* notify = NULL);
};