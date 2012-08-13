// DSCommon.h: interface for the CDSCommon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSCOMMON_H__7E8E928D_BC23_4771_AA1C_236BC2F08416__INCLUDED_)
#define AFX_DSCOMMON_H__7E8E928D_BC23_4771_AA1C_236BC2F08416__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/** @ingroup DSUTIL
    실행화일이 있는 포더의 패스를 얻는다. 
*/
CString _ModulePath(LPCTSTR szFileName);

/** @ingroup DSUTIL
	dsDeleteFile	szFilePath를 삭제한다. readonly속성을 가진 파일도 삭제한다.
*/
inline BOOL	dsDeleteFile(LPCTSTR szFilePath)
{
	SetFileAttributes(szFilePath, FILE_ATTRIBUTE_NORMAL);
	return ::DeleteFile(szFilePath);
}


/** @ingroup DSUTIL
	dsIsDirectory 디렉토리인가 ?

	strPath : 파일의 패스
	return	: TRUE 디렉토리, 없거나 파일이면 FALSE
*/
inline BOOL	dsIsDirectory(LPCTSTR strPath)
{
	if ((GetFileAttributes(strPath) & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) 
		return TRUE;
	return FALSE;
}


class	CClientRect : public	CRect
{
public:
	CClientRect(HWND hWnd)
	{
		::GetClientRect(hWnd, this);
	}
	CClientRect(CWnd* pWnd)
	{
		pWnd->GetClientRect(this);
	}
};

class	CWindowRect : public	CRect
{
public:
	CWindowRect(HWND hWnd)
	{
		::GetWindowRect(hWnd, this);
	}
	CWindowRect(CWnd* pWnd)
	{
		pWnd->GetWindowRect(this);
	}
};


class	CWorkAreaRect : public CRect
{
public:
	CWorkAreaRect()
	{
		::SystemParametersInfo(SPI_GETWORKAREA,  0,  (LPVOID)this,  0);
	}
};

class	CScreenRect : public CRect
{
public:
	CScreenRect()
	{
		left=0;
		top =0;
		right = GetSystemMetrics(SM_CXSCREEN) -1;
		bottom = GetSystemMetrics(SM_CYSCREEN) -1;
	}
};

class CPointScreen : public CPoint
{
public:
	CPointScreen()
	{
		GetCursorPos(this);
	}
};

/**
	프로그램이 하나의 인스턴스만 가지도록 합니다. 
	@param szMutexName  고유문자열
	@return 하나만 실행중이면 TRUE, 두개이상 실행중이면  FALSE
*/
BOOL    CheckMutextInstance(LPCTSTR szMutextName);

/**
	화면 락거는 기능입니다. 
	win2000에서는 Lock화면이 나오고
	xp에서는 사용자전환기능으로 동작합니다..
	win98,me에서는 사용할 수 없는 기능입니다..
*/
bool    dsLockWorkStation()  ;

/**
 *  하위 폴더까지 한 꺼번에 디렉토리를 생성하는 함수
 */
BOOL dsCreateDirectorySub(LPCTSTR pszDir, BOOL	bLastIsFile) ;


void DSTRACE(LPCTSTR pFormat, ...);

int  dsMessageBox(UINT nType, LPCTSTR pFormat, ...);

BOOL SetClipboardText(HWND hWnd, CString str) ;

BOOL GetClipboardText(HWND hWnd, CString &strRet);

BOOL dsFolderSelect(HWND hWnd, CString strSubject, CString &path) ;

BOOL dsIsAccess(LPCTSTR szPath);

#define	MSGBOX	AfxMessageBox





#define WinVerMajor() LOBYTE(LOWORD(GetVersion()))
#define WinVerMinor() HIBYTE(LOWORD(GetVersion()))



#define IsWinVerNTs() (GetVersion() < 0x80000000)
#define IsWinVerNT351Plus() (IsWinVerNTs() && WinVerMajor() >= 3)
#define IsWinVerNT4Plus() (IsWinVerNTs() && WinVerMajor() > 3)
#define IsWinVer98Plus() (LOWORD(GetVersion()) != 4)
#define IsWinVerMEPlus() (WinVerMajor() >= 5 || WinVerMinor() > 10)
#define IsWinVer2000Plus() (WinVerMajor() >= 5)
#define IsWinVerXPPlus() (WinVerMajor() >= 5 && LOWORD(GetVersion()) != 5)
 
HWND dsFindChildWindow(HWND hwndStart, LPCTSTR szClass);

#define ISKEYDOWN(x)		((GetKeyState(x)&0x8000)==0x8000)

void dsWndPosState(BOOL bLoad, HWND hWnd, LPCTSTR szIniFile, LPCTSTR szSection);

#endif // !defined(AFX_DSCOMMON_H__7E8E928D_BC23_4771_AA1C_236BC2F08416__INCLUDED_)
