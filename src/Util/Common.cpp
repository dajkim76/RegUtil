// DSCommon.cpp: implementation of the CDSCommon class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Common.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/** @ingroup DSUTIL
	dsGetPath  실행화일이 있는 폴더 패스를 얻는다.

	szFile :  파일명, NULL이 들어갈 수도 있음, szFile의 첫문자에 '\' 문자가 필요없음	
	return	: szFile이 NULL이면               실행화일이 설치된 폴더를
	          szFile이 NULL이 아니면 아니면에 실행화일이 설치된 폴더에 szFile이 추가된 패스를 리턴한다. 
*/
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




/**
	프로그램이 하나의 인스턴스만 가지도록 합니다. 
	@param szMutexName  고유문자열
	@return 하나만 실행중이면 TRUE, 두개이상 실행중이면  FALSE
*/
BOOL CheckMutextInstance(LPCTSTR szMutextName)
{
	HANDLE hMutexOneInstance = ::CreateMutex(NULL, TRUE, szMutextName ); 
    if(::GetLastError() == ERROR_ALREADY_EXISTS) 
		return FALSE;
    if(hMutexOneInstance) 
		::ReleaseMutex(hMutexOneInstance); 
	return TRUE;
}


/**
	화면 락거는 기능입니다. 
	win2000에서는 Lock화면이 나오고
	xp에서는 사용자전환기능으로 동작합니다..
	win98,me에서는 사용할 수 없는 기능입니다..
*/
bool dsLockWorkStation() 
{
	HMODULE hMod = LoadLibrary(L"User32.dll");
	if(NULL == hMod)
		return false;
	typedef BOOL (WINAPI *func_LockWorkStation)(void);
	func_LockWorkStation  pLockWorkStation = 
			(func_LockWorkStation)GetProcAddress(hMod, "LockWorkStation");
	if(pLockWorkStation)
	{
		pLockWorkStation();	
		return true;
	}
	return false;
}




/** @ingroup DSUTIL
	CreateDirectorySub 서브 폴더까지 한꺼번에 폴더를 생성하는 함수임.	

	pszDir : 패스 
	bLastIsFile : 마지막 노드가 파일인지 디렉토리인지, 
		디렉토리이면 생성하지만 파일이면 마지막 노드는 폴더생성하지 않는다.
    
*/
BOOL	dsCreateDirectorySub(LPCTSTR pszDir, BOOL	bLastIsFile)
{
	CString strFullPath = pszDir;
	int length = strFullPath.GetLength();
	int pos = strFullPath.Find(_T(":\\"));
	if( -1 == pos) 
		return FALSE;	//루뜨폴더를 찾지 못하면 error

	CString strPath;
	while(-1 != pos)
	{
		pos = strFullPath.Find(_T("\\"), pos+1);
		if(-1 == pos)  //마지막까지 검사해서 \\ 문자가 걸리지 않았다..
		{
			if(TRUE == bLastIsFile)
				break;
			strPath = strFullPath;	//폴더로 처리한다..
			goto CREATEDIRECTORYSUB_CREATE;
		}else// 중간에 폴더를 생성한다..
		{
			strPath = strFullPath.Left(pos);

CREATEDIRECTORYSUB_CREATE:
			//엑세스가 되는데 그것이 파일이면..
			if(dsIsAccess(strPath))
			{
				if( ! dsIsDirectory(strPath))
					return FALSE;
			}else
			{
				if( ! CreateDirectory(strPath, NULL)) 
					return FALSE;
			}
		}
		if( (length-1)==pos || -1==pos) //마지막인데 마지막 문자가 \\ 이다..
			break;
	}
	return TRUE;
}

/** @ingroup  DSUTIL
	DSTRACE 릴리스나 디버그 모드에 상관없이 항상 트레이스 기능을 사용합니다. 최대 1024길이의 문자열을 트레이스 합니다.

	pFormat  : 트레이스할 문자열, 동적 파라미터 사용가능	
*/

void DSTRACE(LPCTSTR pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);
	TCHAR buffer [1024*sizeof(TCHAR)];
	_vstprintf(buffer, pFormat, args);    	
	va_end(args);
	OutputDebugString(buffer);	
}



//
//	클리보드에서 Text를 얻어옵니다.......
//
//	성공하면 TRUE를 실패하면 FALSE를 리턴합니다..
BOOL GetClipboardText(HWND hWnd, CString &strRet)
{
	ASSERT(hWnd);
	if( ::OpenClipboard( hWnd) == FALSE) return FALSE;
    if( FALSE == ::IsClipboardFormatAvailable(CF_TEXT) )
	{
		::CloseClipboard();	//이걸 안 해주면 다음에 복사하지 못한다...
        return FALSE;
	}
    HANDLE   hMem = ::GetClipboardData( CF_TEXT );
	if(hMem == NULL) 
	{
		::CloseClipboard();
		return FALSE;
	}
	char* __str = (LPSTR)GlobalLock((HGLOBAL)hMem);		//무조건 ANSI다..
    strRet = __str ;//(LPCTSTR)GlobalLock((HGLOBAL)hMem); //ANSI -> UNI or ANSI -> ANSI
    GlobalUnlock( (HGLOBAL)hMem );
    
    //::EmptyClipboard();		이건 클립을 지운다..
    ::CloseClipboard();
	return TRUE;
}

BOOL SetClipboardText(HWND hWnd, CString str)
{
	if (str.IsEmpty())
		return FALSE;

#ifdef	_UNICODE
	USES_CONVERSION;
	char* __str = W2A(str);
	int nByte = strlen(__str);
#else
	int nByte = str.GetLength() ;
#define	__str str
#endif


	HGLOBAL hText = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, nByte + 1);
	if (hText == NULL)
		return FALSE;

	BYTE* pText = (BYTE*)::GlobalLock(hText);
	if (pText == NULL)
		return FALSE;

	::memcpy(pText, (LPCTSTR)__str, nByte);		//ANSI로 바꿔서 복사한다....

	::GlobalUnlock(hText);

	::OpenClipboard(hWnd);
	::EmptyClipboard();
	::SetClipboardData(CF_TEXT, hText);
	::CloseClipboard();
	return TRUE;
}


#include <shlobj.h>

typedef struct
{
	LPCTSTR	pszInitialDir;
	LPCTSTR	pszDlgTitle;
} BROWSEINIT, *LPBROWSEINIT;


extern "C" int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM /*lParam*/, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
	{
		// Set initial directory
		if ( ((LPBROWSEINIT)lpData)->pszInitialDir != NULL )
			SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)((LPBROWSEINIT)lpData)->pszInitialDir);

		// Set dialog's window title
		if ( ((LPBROWSEINIT)lpData)->pszDlgTitle != NULL )
			SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)((LPBROWSEINIT)lpData)->pszDlgTitle);
	}

	return 0;
}

/************************************************************************
	CString str= "D:\\setup";
	if( dsFolderSelect(m_hWnd, "선택해라", str) )
		AfxMessageBox( str );
************************************************************************/
BOOL dsFolderSelect(HWND hWnd, CString strSubject, CString &path)
{
	//LPTSTR pszPath= NULL; 
	TCHAR pszPath[MAX_PATH] = {0};
	if(path.IsEmpty() == FALSE)
		_tcscpy(pszPath, (LPCTSTR)path);
	LPCTSTR pszTitle = strSubject;
	LPCTSTR pszDlgTitle = strSubject;
	BOOL bResult = FALSE;
//	CoInitialize(0);
	LPMALLOC pShlMalloc;
	if (SHGetMalloc(&pShlMalloc) == NOERROR)
	{
		BROWSEINFO BrsInfo = {0};
		BrsInfo.hwndOwner = hWnd;
		BrsInfo.lpszTitle = (pszTitle != NULL) ? pszTitle : pszDlgTitle;
		BrsInfo.ulFlags = BIF_VALIDATE | BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_SHAREABLE | BIF_DONTGOBELOWDOMAIN;

		BROWSEINIT BrsInit = {0};
		if (pszPath != NULL || pszTitle != NULL || pszDlgTitle != NULL){
			// Need the 'BrowseCallbackProc' to set those strings
			BrsInfo.lpfn = BrowseCallbackProc;
			BrsInfo.lParam = (LPARAM)&BrsInit;
			BrsInit.pszDlgTitle = (pszDlgTitle != NULL) ? pszDlgTitle : NULL/*pszTitle*/;
			BrsInit.pszInitialDir = pszPath;
		}

		LPITEMIDLIST pidlBrowse;
		if ((pidlBrowse = SHBrowseForFolder(&BrsInfo)) != NULL){
			if (SHGetPathFromIDList(pidlBrowse, pszPath))
			{
				path = pszPath;
				bResult = TRUE;
			}
			pShlMalloc->Free(pidlBrowse);
		}
		pShlMalloc->Release();
	}
//	CoUninitialize();
	return bResult;
}



int dsMessageBox(UINT nType, LPCTSTR pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);
	TCHAR buffer [1024*sizeof(TCHAR)];
	_vstprintf(buffer, pFormat, args);    
	va_end(args);	

	return AfxMessageBox(buffer, nType);
}

/** @ingroup DSUTIL
	dsIsAccess 파일이 존재하는지 확인합니다.

	szFilePath : 파일의 전체 패스
	return : 파일이나 폴더이면 TRUE, 존재하지 않으면 FALSE
*/
#ifndef		INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)	//From PlatFormSDK winbase.h 
#endif
BOOL dsIsAccess(LPCTSTR szFilePath)
{
	DWORD  dwRet = ::GetFileAttributes(szFilePath);
	if(INVALID_FILE_ATTRIBUTES == dwRet)
		return FALSE;
	return TRUE;
}


HWND dsFindChildWindow(HWND hwndStart, LPCTSTR szClass)
{
	HWND hwndFound=NULL;
	if (((hwndFound = 
		FindWindowEx(hwndStart, NULL, szClass, NULL)) != NULL))
		return hwndFound;

	HWND hwndEnum=NULL;
	while ((hwndEnum = FindWindowEx(hwndStart, hwndEnum, NULL, NULL)) != NULL){
		if ((hwndFound = dsFindChildWindow(hwndEnum, szClass)) != NULL)
			return hwndFound;
	}

	return NULL;
}

void dsWndPosState(BOOL bLoad, HWND hWnd, LPCTSTR szIniFile, LPCTSTR szSection)
{
	if(bLoad)	
	{
		TCHAR sz[1024];
		int bZoomed = GetPrivateProfileInt(szSection, L"__bZoomed", 0, szIniFile);
		if(bZoomed)
		{
			ShowWindow(hWnd, SW_MAXIMIZE);
		}else
		{
			GetPrivateProfileString(szSection, L"__rcPosition", L"", sz, 1024, szIniFile);
			if( _tcslen(sz) != 0)
			{
				CString s[4];
				for(int i=0;i<4;i++)
					if(FALSE == AfxExtractSubString(s[i], sz, i, _T(','))) return;
				MoveWindow(hWnd,  _ttoi(s[0]),  _ttoi(s[1]),  _ttoi(s[2]),  _ttoi(s[3]), FALSE);						
			}
		}
	}else
	{
		if( ! IsWindowVisible(hWnd))
			return;
		if(IsIconic(hWnd))
			ShowWindow(hWnd, SW_RESTORE);

		RECT rc;
		GetWindowRect(hWnd, &rc);
		CString s;
		s.Format(L"%d,%d,%d,%d", rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
		WritePrivateProfileString(szSection, L"__rcPosition", s, szIniFile);
		s.Format(L"%d", ::IsZoomed(hWnd));
		WritePrivateProfileString(szSection, L"__bZoomed"		, s, szIniFile);
	}
}
