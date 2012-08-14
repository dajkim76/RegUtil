// stdafx.cpp : source file that includes just the standard includes
// UnitTest.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

CString _ModulePath( LPCTSTR szFileName ) 
{	
	static CString s_sDir;
	if(s_sDir.IsEmpty())
	{
		HMODULE 	hModule;
		TCHAR	sFilepath[MAX_PATH];
		TCHAR*	p;
		hModule = ::GetModuleHandle (NULL);
		if(NULL== ::GetModuleFileName (hModule, sFilepath, MAX_PATH))   	return CString (_T(""));
		if ((p = _tcsrchr (sFilepath, _T('\\'))) == NULL)
		if ((p = _tcsrchr (sFilepath, _T('/'))) == NULL)   return CString ();

		*p = 0;    
		s_sDir = sFilepath;
	}
	if(NULL == szFileName)
		return s_sDir ;
	
	return s_sDir + L"\\"  + szFileName;
} 