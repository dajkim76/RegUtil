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

