#include "stdafx.h"
#include "Misc.h"

Misc::Misc(void)
{
}

Misc::~Misc(void)
{
}



DWORD ExecuteWait(LPCTSTR szFileName, int nShow, LPCTSTR szDir, LPCTSTR szParameter)
{
	SHELLEXECUTEINFO shExIf;
	::ZeroMemory(&shExIf, sizeof(SHELLEXECUTEINFO));

	shExIf.cbSize = sizeof( SHELLEXECUTEINFO );
	shExIf.lpVerb = _T("open");
	shExIf.lpFile = szFileName;
	shExIf.nShow = nShow;
	shExIf.lpDirectory = szDir;
	shExIf.lpParameters = szParameter;
	shExIf.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShellExecuteEx( &shExIf );

	DWORD dwExitCode ;
	while(1)
	{
		::GetExitCodeProcess( shExIf.hProcess, &dwExitCode );
		if( dwExitCode != STILL_ACTIVE )
		{
			break;
		}

		Sleep(10);
	}

	return dwExitCode;
}


DWORD ExecuteWait(LPCTSTR szPath, int nShow, LPCTSTR szParameter)
{
	CString path(szPath);
	CString fileName;
	CString dirName;

	int posDir = path.ReverseFind(_T('\\'));

	fileName = path.Mid(posDir + 1);
	dirName = path.Mid(0, posDir);

	return ExecuteWait(fileName, nShow, dirName, szParameter);
}


void Execute(LPCTSTR szFileName, int nShow, LPCTSTR szDir, LPCTSTR szParameter, SHELLEXECUTEINFO *shExIf)
{
	SHELLEXECUTEINFO localInfo;
	if ( shExIf == NULL )
	{
		shExIf = &localInfo;
	}

	::ZeroMemory(shExIf, sizeof(SHELLEXECUTEINFO));

	shExIf->cbSize = sizeof( SHELLEXECUTEINFO );
	shExIf->lpVerb = _T("open");
	shExIf->lpFile = szFileName;
	shExIf->nShow = nShow;
	shExIf->lpDirectory = szDir;
	shExIf->lpParameters = szParameter;
	shExIf->fMask = SEE_MASK_NOCLOSEPROCESS;
	ShellExecuteEx( shExIf );
}


void Execute(LPCTSTR szPath, int nShow, LPCTSTR szParameter, SHELLEXECUTEINFO *shExIf)
{
	CString path(szPath);
	CString fileName;
	CString dirName;

	int posDir = path.ReverseFind(_T('\\'));

	fileName = path.Mid(posDir + 1);
	dirName = path.Mid(0, posDir);

	Execute(fileName, nShow, dirName, szParameter, shExIf);
}

//////////////////////////////////////////////////////////////////////////

#include <tlhelp32.h>
//MBCS로 동작하도록 undef시킨다.
//#undef PROCESSENTRY32
//#undef Process32First
//#undef Process32Next
BOOL KillProcessID(UINT nPID)
{
	HANDLE hProcess = OpenProcess( PROCESS_TERMINATE, FALSE, nPID );

	if ( NULL != hProcess )
	{
		BOOL b = ::TerminateProcess(hProcess, 1);
		CloseHandle(hProcess);
		return b ;
	}
	return FALSE ;
}

BOOL KillProcess( LPCTSTR szProcName )
{
	CString sExeName = szProcName;
	int rpos = sExeName.ReverseFind(_T('\\'));
	if ( rpos > 0 )
	{
		sExeName = sExeName.Mid( rpos + 1);
	}

	PROCESSENTRY32W proc;
	HANDLE snapshot;
	BOOL bOK = FALSE ;

	proc.dwSize = sizeof(PROCESSENTRY32W);
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(  Process32First(snapshot, &proc))
	{
		do
		{
			CString sPath = proc.szExeFile;
			rpos = sPath.ReverseFind(_T('\\'));
			if ( rpos > 0 )
			{
				sPath = sPath.Mid( rpos + 1);
			}

			if ( sPath.CompareNoCase( sExeName ) == 0 )
			{
				if ( KillProcessID( proc.th32ProcessID ) )
				{
					bOK = TRUE;
				}
			}

		}
		while (TRUE == Process32Next(snapshot, &proc));
	}

	CloseHandle (snapshot );
	return bOK;
}