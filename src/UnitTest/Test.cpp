#include "StdAfx.h"
#include "Test.h"
#include "..\RegWorks\Misc.h"
#include "..\RegWorks\RegWorks.h"

Test::Test(void)
{
}

Test::~Test(void)
{
}

void Test1()
{
	// 있으면 종료
	if( KillProcess(L"RegEdit.exe"))
	{
		Sleep(500);
	}
	TEST( RegWorks::FindRegEdit() == FALSE );

	// 실행
	RegWorks::LaunchRegEditor(true);
	TEST( RegWorks::FindRegEdit() );

	//GetKey
	TEST( KeyRoot::GetKey(KeyRoot::HKLM) == HKEY_LOCAL_MACHINE );
	TEST( KeyRoot::GetKey(KeyRoot::HKCU) == HKEY_CURRENT_USER);
	TEST( KeyRoot::GetKey(KeyRoot::HKCR) == HKEY_CLASSES_ROOT);
	TEST( KeyRoot::GetKey(KeyRoot::HKUSERS) == HKEY_USERS);
	TEST( KeyRoot::GetKey(KeyRoot::HKCC) == HKEY_CURRENT_CONFIG);

	//toType
	TEST( KeyRoot::toType( L"HKEY_LOCAL_MACHINE" ) == KeyRoot::HKLM);
	TEST( KeyRoot::toType( L"HKEY_CURRENT_USER" ) == KeyRoot::HKCU);
	TEST( KeyRoot::toType( L"HKEY_CLASSES_ROOT" ) == KeyRoot::HKCR);
	TEST( KeyRoot::toType( L"HKEY_USERS" ) == KeyRoot::HKUSERS);
	TEST( KeyRoot::toType( L"HKEY_CURRENT_CONFIG") == KeyRoot::HKCC);

	TEST( KeyRoot::toType( L"hklm" ) == KeyRoot::HKLM);
	TEST( KeyRoot::toType( L"hkcu" ) == KeyRoot::HKCU);
	TEST( KeyRoot::toType( L"hkcr" ) == KeyRoot::HKCR);
	TEST( KeyRoot::toType( L"hkusers" ) == KeyRoot::HKUSERS);
	TEST( KeyRoot::toType( L"hkcc") == KeyRoot::HKCC);
}

void Test2()
{
	RegWorks::Lookup(KeyRoot::HKCU, L"Software\\NHN Corporation\\naveragent", L"KeepToolbar");
}

#if 0
class RegistrySearch
{
	BOOL Search( CStringW csString, CStringW csStartKey, CStringArray& csaResults, 
							 int nRegSearchType /* = 3 */,
							 BOOL bCaseSensitive /* = TRUE*/)
	{
		if (csString.GetLength() < 1) {
			return FALSE;
		}

		//
		SetPathVars(csStartKey);

		CString csFullKey = CheckRegFullPath(csStartKey);
		CString csTmpKey = csFullKey;

		ANSI_STRING asKey;
		RtlZeroMemory(&asKey,sizeof(asKey));
		RtlInitAnsiString(&asKey,csFullKey);

		UNICODE_STRING usKeyName;
		RtlZeroMemory(&usKeyName,sizeof(usKeyName));

		RtlAnsiStringToUnicodeString(&usKeyName,&asKey,TRUE);

		if (m_bHidden) {
			usKeyName.Length += 2;
			usKeyName.MaximumLength += 2;
		}

		//
		if (!bCaseSensitive) {
			csTmpKey.MakeLower();
			csString.MakeLower();
		}

		//
		CString csFound = _T("");
		int nFound = -1;

		if (nRegSearchType == 1 || nRegSearchType == 3) {
			// We are searching in Keys, so check the Key
			nFound = csTmpKey.Find(csString,0);
		}

		OBJECT_ATTRIBUTES ObjectAttributes;
		InitializeObjectAttributes(&ObjectAttributes,&usKeyName,OBJ_CASE_INSENSITIVE,m_hMachineReg,NULL);

		HANDLE hKey = NULL;
		m_NtStatus = NtOpenKey(&hKey, KEY_ALL_ACCESS, &ObjectAttributes);
		if(!NT_SUCCESS(m_NtStatus)) {
			// Can't go any further :-(
			return FALSE;
		}
		else {
			//
			if (nFound >= 0) {
				// Found it, so add it to the array.
				csaResults.Add(csStartKey);
				// If we here, it means that we are just 
				// searching in Keys, so we need to "goto" 
				// the Exit point...
				goto End_It;
			}

			ULONG resultLength;
			CHAR szValueInfo[8192];
			UINT i=0;

			CString csValue, csTmp;

			if (nRegSearchType > 1) {
				//
				// Scan for values
				while((m_NtStatus=NtEnumerateValueKey(hKey,i,KeyValueFullInformation,szValueInfo,sizeof(szValueInfo),&resultLength))==STATUS_SUCCESS)
				{
					csFound.Empty();

					PKEY_VALUE_FULL_INFORMATION tInfo= (PKEY_VALUE_FULL_INFORMATION)szValueInfo;
					csTmp = tInfo->Name;
					csValue = csTmp.Left(tInfo->NameLength / 2);
					csValue.MakeLower();
					if (csValue == _T("")) {
						csValue = _T("(Default)");
					}

					if (csValue.Find(csString,0) >= 0) {
						csFound.Format(_T("%s[%s]"), csStartKey, csValue);
						csaResults.Add(csFound);
					}

					if (tInfo->Type == REG_SZ || 
						tInfo->Type == REG_EXPAND_SZ || 
						tInfo->Type == REG_MULTI_SZ)
					{
						CString csValueName;
						csValueName = csTmp.Mid((int)tInfo->NameLength / 2, (int)tInfo->DataLength / 2);
						csValueName.MakeLower();
						if (csValueName.Find(csString,0) >= 0) {
							//
							csFound.Format(_T("%s[%s]:[%s]"), csStartKey, csValue, csValueName);
							csaResults.Add(csFound);
						}
					}

					// next
					i++;

				} // while
			} // if()

			CHAR szSubkeyInfo[1024];

			CString csSubkey, csNewSubkey;

			i=0;
			// Scan for subkeys
			while((m_NtStatus=NtEnumerateKey(hKey,i,KeyBasicInformation,szSubkeyInfo,sizeof(szSubkeyInfo),&resultLength))==STATUS_SUCCESS)
			{
				PKEY_BASIC_INFORMATION tInfo= (PKEY_BASIC_INFORMATION)szSubkeyInfo;
				csSubkey = tInfo->Name;

				//
				csNewSubkey.Format(_T("%s\\%s"), csStartKey, csSubkey.Left(tInfo->NameLength / 2));

				// Search for more recursively ...
				Search(csString, csNewSubkey, csaResults, nRegSearchType);
				
				// next
				i++;

			} // while
		}

	End_It:

		if (hKey) {
			NtClose(hKey);
		}

		return TRUE;
	}
};

#endif
void Test3()
{

}