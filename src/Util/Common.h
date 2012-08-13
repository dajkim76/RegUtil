// DSCommon.h: interface for the CDSCommon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSCOMMON_H__7E8E928D_BC23_4771_AA1C_236BC2F08416__INCLUDED_)
#define AFX_DSCOMMON_H__7E8E928D_BC23_4771_AA1C_236BC2F08416__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/** @ingroup DSUTIL
    ����ȭ���� �ִ� ������ �н��� ��´�. 
*/
CString _ModulePath(LPCTSTR szFileName);

/** @ingroup DSUTIL
	dsDeleteFile	szFilePath�� �����Ѵ�. readonly�Ӽ��� ���� ���ϵ� �����Ѵ�.
*/
inline BOOL	dsDeleteFile(LPCTSTR szFilePath)
{
	SetFileAttributes(szFilePath, FILE_ATTRIBUTE_NORMAL);
	return ::DeleteFile(szFilePath);
}


/** @ingroup DSUTIL
	dsIsDirectory ���丮�ΰ� ?

	strPath : ������ �н�
	return	: TRUE ���丮, ���ų� �����̸� FALSE
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
	���α׷��� �ϳ��� �ν��Ͻ��� �������� �մϴ�. 
	@param szMutexName  �������ڿ�
	@return �ϳ��� �������̸� TRUE, �ΰ��̻� �������̸�  FALSE
*/
BOOL    CheckMutextInstance(LPCTSTR szMutextName);

/**
	ȭ�� ���Ŵ� ����Դϴ�. 
	win2000������ Lockȭ���� ������
	xp������ �������ȯ������� �����մϴ�..
	win98,me������ ����� �� ���� ����Դϴ�..
*/
bool    dsLockWorkStation()  ;

/**
 *  ���� �������� �� ������ ���丮�� �����ϴ� �Լ�
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
