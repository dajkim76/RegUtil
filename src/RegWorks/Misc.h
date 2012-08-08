#pragma once

class Misc
{
public:
	Misc(void);
	~Misc(void);
};

DWORD ExecuteWait(LPCTSTR szPath, int nShow, LPCTSTR szParameter);
void Execute(LPCTSTR szPath, int nShow, LPCTSTR szParameter, SHELLEXECUTEINFO *shExIf = NULL);

BOOL KillProcessID(UINT nPID);
BOOL KillProcess( LPCTSTR szProcName );