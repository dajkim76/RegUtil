#include "StdAfx.h"
#include "RegSearch.h"
#include <strsafe.h>

#define  DTRACE(...)

namespace RegSearch
{
	#define MAX_REG_KEY_NAME		(512)
	#define MAX_REG_KEY_VALUE		(32767)


	bool FindKeyword(const CAtlString& source, const CAtlString& keyword, bool isCasesensitive)
	{
		ATLASSERT(keyword.GetLength());
		if( isCasesensitive )
		{
			return source.Find(keyword) >= 0;
		}

		CAtlString tempSource = source;
		tempSource.MakeLower();
		CAtlString tempKeyword = keyword;
		tempKeyword.MakeLower();
		return tempSource.Find(tempKeyword) >= 0;
	}

	void EnumRegistryKey(HKEY hKey, CAtlString key, vector< CAtlString >& keyList)
	{
		LONG retcode = ERROR_SUCCESS;
		HKEY hOpenKey = NULL;
		TCHAR str[MAX_PATH];
		memset( str, '\0', sizeof(str));
		keyList.clear();

		if( key.GetLength() > 0)
		{
			retcode = RegOpenKey( hKey, key, &hOpenKey);
		}
		else
		{
			hOpenKey = hKey;
		}

		if( retcode != (DWORD)ERROR_SUCCESS )
		{
			return;
		}

		for (int i = 0, retCode = ERROR_SUCCESS; retCode == ERROR_SUCCESS; i++) 
		{
			retCode = RegEnumKey(hOpenKey, i, str, MAX_PATH	); 

			if (retCode == (DWORD)ERROR_SUCCESS) 
			{
				CAtlString sNewKeyName;
				sNewKeyName = str;

				if(sNewKeyName.GetLength() > 0)
				{
					keyList.push_back( sNewKeyName );
				}
			}
		}

		if(hKey)
		{
			RegCloseKey( hOpenKey );
		}
	}

	void SearchRegistryKeyValue(HKEY hKey, CAtlString key, RegItemList& resultList, const SearchOption& option)
	{
		if( ! option.nameCheck_ && ! option.valueCheck_ )
		{
			return;
		}

		LONG retcode = 0;
		HKEY hOpenKey = NULL;
		DWORD dwType = REG_SZ;
		TCHAR name[MAX_REG_KEY_NAME];
		memset( name, _T('\0'), sizeof(name));

		if(key.GetLength() > 0)
		{
			retcode = RegOpenKeyEx(hKey, key, 0, KEY_READ, &hOpenKey);
		}
		else
		{
			hOpenKey = hKey;
		}

		if( retcode != (DWORD)ERROR_SUCCESS )
		{
			return;
		}

		static BYTE data[MAX_REG_KEY_VALUE];
		static TCHAR tempBuffer[MAX_REG_KEY_VALUE*3 + 10] = { NULL };
		memset( data, _T('\0'), sizeof(data));

		resultList.clear();

		for (int i = 0; true; i++) 
		{
			DWORD nameSize = MAX_REG_KEY_NAME;
			DWORD valueSize = MAX_REG_KEY_VALUE;

			retcode = RegEnumValue(hOpenKey, i, name, &nameSize, NULL, &dwType, data, &valueSize );

			if ( retcode == ERROR_NO_MORE_ITEMS )
			{
				break;
			}

			if ( retcode == ERROR_MORE_DATA )
			{
				/*
				Registry Element	Size Limit
				Key name	255 characters
				Value name	16,383 characters
				Windows 2000:  260 ANSI characters or 16,383 Unicode characters.
				Value	Available memory (latest format)
					1 MB (standard format)
				Tree	A registry tree can be 512 levels deep. You can create up to 32 levels at a time through a single registry API call.
				*/
				//DTRACE(L"\n******* MoreData[%s][%s] size=%u\n\n", key, name, valueSize);
				continue;
			}

			if ( valueSize == 0 || valueSize >= MAX_REG_KEY_VALUE )
			{
				continue;
			}

			if( dwType == REG_NONE )
			{
				continue;
			}

			if ( retcode != ERROR_SUCCESS )
			{
				continue;
			}

			const bool isNamecheckSuccess = ( option.nameCheck_ && FindKeyword(name, option.keyword_, option.caseSenstive_));
			
			LPTSTR buffer = NULL;

			if( dwType == REG_SZ )
			{
				if ( isNamecheckSuccess || option.valueCheck_ )
				{
					LPTSTR pszValue = reinterpret_cast<LPTSTR> (data);
					const int cch = valueSize / sizeof(TCHAR); // with NULL
					if ((valueSize % sizeof(TCHAR) != 0) || (pszValue[cch - 1] != 0))
					{
						pszValue[cch] = _T('\0');
						buffer = (TCHAR*)data;
					}
					else
					{
						buffer = (TCHAR*)data;
					}
				}
			}
			else if( dwType == REG_EXPAND_SZ )
			{
				if ( isNamecheckSuccess || option.valueCheck_ )				
				{
					LPTSTR pszValue = reinterpret_cast<LPTSTR> (data);
					const int cch = valueSize / sizeof(TCHAR); // with NULL
					if ( (valueSize % sizeof(TCHAR) != 0) || (pszValue[valueSize / sizeof(TCHAR) - 1] != 0))
					{
						pszValue[cch] = _T('\0');
						buffer = (TCHAR*)data;
					}
					else
					{
						buffer = (TCHAR*)data;
					}
				}
			}
			else if( dwType == REG_MULTI_SZ )
			{
				if ( isNamecheckSuccess || option.valueCheck_ )				
				{
					LPTSTR pszValue = reinterpret_cast<LPTSTR> (data);
					ATLASSERT( (valueSize % sizeof(TCHAR) == 0) );
					const int cch = (valueSize / sizeof(TCHAR));

					if ( cch >= 1 )
					{
						for(int i = 0; i < cch; i++)
						{
							if( pszValue[i] == _T('\0') )
							{
								pszValue[i] = _T('\r');
							}
						}

						// last \r -> NULL
						pszValue[cch - 1] = _T('\0');

						buffer = (TCHAR*)data;
					}
				}
			}
			else if(	(dwType == REG_BINARY ) || 
						(dwType == REG_RESOURCE_LIST) || 
						(dwType == REG_FULL_RESOURCE_DESCRIPTOR) || 
						(dwType == REG_RESOURCE_REQUIREMENTS_LIST) )
			{
				
				if( valueSize > 0 && isNamecheckSuccess)
				{
					TCHAR temp[4];
					memset(temp, '\0', sizeof(temp));

					tempBuffer[0] = NULL;
					for(int j=0; j < (int)valueSize; j++)
					{
						StringCchPrintf(temp, _countof(temp), _T("%02X "), (long int)data[j]);
						StringCchCat( tempBuffer, _countof(tempBuffer),  temp );
					}

					buffer = tempBuffer;
				}
			}
			else if( dwType == REG_DWORD )
			{
				if ( isNamecheckSuccess )
				{
					DWORD dwData = 0;
					memcpy( &dwData, data, sizeof(DWORD));
					StringCchPrintf(tempBuffer, _countof(tempBuffer), _T("0x%x(%u)"), dwData, dwData );
					buffer = tempBuffer;
				}
			}
			else if( dwType == REG_QWORD ) // 64bit word
			{
				if ( isNamecheckSuccess )
				{
					__int64 qData = 0;
					memcpy( &qData, data, sizeof(__int64));
					StringCchPrintf(tempBuffer, _countof(tempBuffer), _T("0x%I64x(%I64d)"), qData, qData);
					buffer = tempBuffer;
				}
			}
			else
			{	/*
				Unknown type : 64 ???
				if( dwType != REG_NONE )
				{
					DTRACE(L"\n###### UnSupported reg type: %d\n\n", dwType);
				}
				_ASSERT_EXPR(dwType == REG_NONE, str);
				*/
				continue;
			}

			const bool isValuecheckSuccess = (	!isNamecheckSuccess && 
												buffer && 
												option.valueCheck_ && 
												FindKeyword(buffer, option.keyword_, option.caseSenstive_ ) );
			
			if ( isNamecheckSuccess || isValuecheckSuccess )
			{
				RegItem* p = new RegItem(buffer);
				p->type_ = dwType;
				p->name_ = name;
				resultList.push_back( p );
			}
		} // for

		if(hKey)
		{
			RegCloseKey( hOpenKey );
		}		
	}

	void ClearRegItemList( RegItemList& itemList)
	{
		for( unsigned i = 0; i < itemList.size(); i ++)
		{
			delete itemList[i];
		}
		itemList.clear();
	}

	bool RegistrySearch::_Search(HKEY root, CAtlString key, SearchOption& option, ISearchNotify* notify)
	{
		if( option.canceled_ )
		{
			return false;
		}

		if( ! option.keyword_.GetLength())
		{
			return true;
		}

		const CAtlString& keyword = option.keyword_;
		vector<CAtlString> keyList;
		EnumRegistryKey(root, key, keyList);

		for( unsigned i = 0; i < keyList.size(); i++ )
		{
			vector< RegItem* > itemList;
			CAtlString& keyItem = keyList[i];
			CAtlString subKey = key;
			if( subKey.GetLength()) 
			{
				subKey += _T("\\") + keyItem;
			}
			else
			{
				subKey = keyItem;
			}

			if ( option.keyCheck_ && FindKeyword( keyItem, keyword, option.caseSenstive_) )
			{			
				DTRACE(L"%d: key:[%s] \n", __count++, subKey);
				if ( notify )
				{
					if ( ! notify->OnFound(subKey, NULL) )
					{
						return false;
					}
				}
			}

			SearchRegistryKeyValue(root, subKey, itemList, option);
			for( unsigned j = 0; j < itemList.size(); j++ )
			{
				RegItem* item = itemList[j];
				item->key_ = subKey;
				if ( item->name_.IsEmpty() )
				{
					item->name_ = _T("(Default)");
				}

				DTRACE(L"%d: key:[%s] -> [%s]=[%s] \n", __count++, subKey, item->name_, item->text_ );
				if ( notify )
				{
					itemList[j] = NULL;
					if ( ! notify->OnFound(subKey, item ) )
					{
						return false;
					}
				}
			}

			//clear
			ClearRegItemList(itemList);

			// goto child key
			if ( ! _Search(root, subKey, option, notify))
			{
				return false;
			}
		}

		return true;
	}	
}