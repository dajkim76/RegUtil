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