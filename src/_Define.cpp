#include "stdafx.h"
#include <strsafe.h>

CString __S( LPCWSTR pszFormat, ...)
{
	WCHAR pszDest[ 1024 ] = { NULL };
	const int cchDest = _countof(pszDest);
	HRESULT hr;

#if _MSC_VER >= 1600
	hr = StringValidateDestW(pszDest, cchDest, STRSAFE_MAX_CCH);
#else
	hr = StringValidateDestW(pszDest, cchDest, NULL, STRSAFE_MAX_CCH);
#endif
	ATLASSERT(SUCCEEDED(hr));
	if (SUCCEEDED(hr))
	{
		va_list argList;
		va_start(argList, pszFormat);
		hr = StringVPrintfWorkerW(pszDest,
			cchDest,
			NULL,
			pszFormat,
			argList);
		va_end(argList);
	}

	return pszDest;
}

// profile
LPCWSTR kSection = L"main";
#define  INIPATH PROFILE_INI

CString _GetString( CString key, CString defaultValue )
{
	TCHAR buffer[4096] = { NULL };
	GetPrivateProfileString(kSection, key, defaultValue, buffer, _countof(buffer), INIPATH);
	return buffer;
}

void _WriteString( CString key, CString value )
{
	WritePrivateProfileString(kSection, key, value, INIPATH);
}

int _GetInt( CString key, int defaultValue )
{
	TCHAR buffer[4096] = { NULL };
	GetPrivateProfileString(kSection, key, Int2Str(defaultValue), buffer, _countof(buffer), INIPATH);
	return _ttoi(buffer);
}

void _WriteInt( CString key, int value )
{
	WritePrivateProfileString(kSection, key, Int2Str(value), INIPATH);
}
