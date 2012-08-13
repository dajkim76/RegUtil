// DSCommon.cpp: implementation of the CDSCommon class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Common.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/** @ingroup DSUTIL
	dsGetPath  ����ȭ���� �ִ� ���� �н��� ��´�.

	szFile :  ���ϸ�, NULL�� �� ���� ����, szFile�� ù���ڿ� '\' ���ڰ� �ʿ����	
	return	: szFile�� NULL�̸�               ����ȭ���� ��ġ�� ������
	          szFile�� NULL�� �ƴϸ� �ƴϸ鿡 ����ȭ���� ��ġ�� ������ szFile�� �߰��� �н��� �����Ѵ�. 
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
	���α׷��� �ϳ��� �ν��Ͻ��� �������� �մϴ�. 
	@param szMutexName  �������ڿ�
	@return �ϳ��� �������̸� TRUE, �ΰ��̻� �������̸�  FALSE
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
	ȭ�� ���Ŵ� ����Դϴ�. 
	win2000������ Lockȭ���� ������
	xp������ �������ȯ������� �����մϴ�..
	win98,me������ ����� �� ���� ����Դϴ�..
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
	CreateDirectorySub ���� �������� �Ѳ����� ������ �����ϴ� �Լ���.	

	pszDir : �н� 
	bLastIsFile : ������ ��尡 �������� ���丮����, 
		���丮�̸� ���������� �����̸� ������ ���� ������������ �ʴ´�.
    
*/
BOOL	dsCreateDirectorySub(LPCTSTR pszDir, BOOL	bLastIsFile)
{
	CString strFullPath = pszDir;
	int length = strFullPath.GetLength();
	int pos = strFullPath.Find(_T(":\\"));
	if( -1 == pos) 
		return FALSE;	//��������� ã�� ���ϸ� error

	CString strPath;
	while(-1 != pos)
	{
		pos = strFullPath.Find(_T("\\"), pos+1);
		if(-1 == pos)  //���������� �˻��ؼ� \\ ���ڰ� �ɸ��� �ʾҴ�..
		{
			if(TRUE == bLastIsFile)
				break;
			strPath = strFullPath;	//������ ó���Ѵ�..
			goto CREATEDIRECTORYSUB_CREATE;
		}else// �߰��� ������ �����Ѵ�..
		{
			strPath = strFullPath.Left(pos);

CREATEDIRECTORYSUB_CREATE:
			//�������� �Ǵµ� �װ��� �����̸�..
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
		if( (length-1)==pos || -1==pos) //�������ε� ������ ���ڰ� \\ �̴�..
			break;
	}
	return TRUE;
}

/** @ingroup  DSUTIL
	DSTRACE �������� ����� ��忡 ������� �׻� Ʈ���̽� ����� ����մϴ�. �ִ� 1024������ ���ڿ��� Ʈ���̽� �մϴ�.

	pFormat  : Ʈ���̽��� ���ڿ�, ���� �Ķ���� ��밡��	
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
//	Ŭ�����忡�� Text�� ���ɴϴ�.......
//
//	�����ϸ� TRUE�� �����ϸ� FALSE�� �����մϴ�..
BOOL GetClipboardText(HWND hWnd, CString &strRet)
{
	ASSERT(hWnd);
	if( ::OpenClipboard( hWnd) == FALSE) return FALSE;
    if( FALSE == ::IsClipboardFormatAvailable(CF_TEXT) )
	{
		::CloseClipboard();	//�̰� �� ���ָ� ������ �������� ���Ѵ�...
        return FALSE;
	}
    HANDLE   hMem = ::GetClipboardData( CF_TEXT );
	if(hMem == NULL) 
	{
		::CloseClipboard();
		return FALSE;
	}
	char* __str = (LPSTR)GlobalLock((HGLOBAL)hMem);		//������ ANSI��..
    strRet = __str ;//(LPCTSTR)GlobalLock((HGLOBAL)hMem); //ANSI -> UNI or ANSI -> ANSI
    GlobalUnlock( (HGLOBAL)hMem );
    
    //::EmptyClipboard();		�̰� Ŭ���� �����..
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

	::memcpy(pText, (LPCTSTR)__str, nByte);		//ANSI�� �ٲ㼭 �����Ѵ�....

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
	if( dsFolderSelect(m_hWnd, "�����ض�", str) )
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
	dsIsAccess ������ �����ϴ��� Ȯ���մϴ�.

	szFilePath : ������ ��ü �н�
	return : �����̳� �����̸� TRUE, �������� ������ FALSE
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
