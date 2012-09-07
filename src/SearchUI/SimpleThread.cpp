#include "StdAfx.h"
#include "SimpleThread.h"
#include <process.h>

#define  DCHECK ASSERT
#define  NOTREACHED(x) DCHECK(! x)
SimpleThread::SimpleThread(bool autoFree) : autoFree_(autoFree)
{
	thread_ = NULL;
	isSuspended_ = false;
}

SimpleThread::~SimpleThread(void)
{
	DCHECK(thread_ == NULL);
}

void SimpleThread::Start()
{
	DCHECK(IsValid() == FALSE);
	unsigned uThreadId = 0 ;
	thread_ = (HANDLE)_beginthreadex(NULL, 0, InternalThreadProc, this, 0, &uThreadId) ;
	DCHECK(IsValid());
}

unsigned CALLBACK SimpleThread::InternalThreadProc( void* pParam )
{
	SimpleThread* pthis =  reinterpret_cast<SimpleThread*> (pParam);	
	pthis->_EndThreadEx(pthis->Run());
	return 0;
}

void SimpleThread::Suspend()
{
	DCHECK(IsValid());
	if( IsValid())
	{
		::SuspendThread(thread_);
		isSuspended_ = true;
	}
}

void SimpleThread::Resume()
{
	DCHECK(IsValid());
	if( IsValid())
	{
		::ResumeThread(thread_);
		isSuspended_ = false;
	}
}

void SimpleThread::Terminate()
{
	DCHECK(IsValid());
	if( IsValid() )
	{
		::TerminateThread(thread_, 1);
		_CloseHandle();
	}
}

void SimpleThread::WaitTerminate( DWORD miliSecond )
{
	if( IsValid() )
	{
		if( WaitForSingleObject(thread_, miliSecond) == WAIT_TIMEOUT)
		{
			NOTREACHED(L"WaitTerminate() .. Terminiate()\n");
			Terminate();
		}
	}
}

bool SimpleThread::IsValid()
{
	return (thread_ != NULL);
}

bool SimpleThread::IsSuspended()
{
	return isSuspended_;
}

void SimpleThread::_CloseHandle()
{
	if ( IsValid() )
	{
		::CloseHandle(thread_);
		thread_ = NULL;
	}
}

void SimpleThread::_EndThreadEx(unsigned retVal)
{	
	_CloseHandle();
	if ( autoFree_ )
	{
		delete this;
	}
	_endthreadex(retVal);
}
