#pragma once

class SimpleThread
{
public:
	SimpleThread(bool autoFree = false);
	virtual ~SimpleThread(void);

	void Start();
	void Suspend();
	void Resume();
	void Terminate();	
	bool IsValid();
	bool IsSuspended();
	void WaitTerminate(DWORD miliSecond = 1000);

protected:
	virtual UINT Run() = 0;
	void _CloseHandle();
	void _EndThreadEx(unsigned retVal);
	HANDLE thread_;
	bool isSuspended_;
	bool autoFree_;

private:
	static unsigned CALLBACK InternalThreadProc(void* pParam);
};
