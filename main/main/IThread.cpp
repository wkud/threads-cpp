#include "IThread.h"
#include <windows.system.h>

IThread::IThread() : status(IThread::Status::STOPPED), thr(nullptr)
{
	
}


IThread::~IThread()
{
	Stop();
}

void IThread::Start()
{
	if (!thr)
	{
		status = IThread::Status::RUNNING;
		thr = new std::thread(&IThread::ThreadRoutine, this);
	}
}

void IThread::Stop()
{
	if (thr)
	{
		status = IThread::Status::STOPPED;
		TerminateThread(thr->native_handle(), 0); //instant unsafe killing thread
		delete thr;
		thr = nullptr;
	}
}

void IThread::Join()
{
	if (thr)
	{
		status = IThread::Status::STOPPED;
		thr->join(); //safe, not instant pausing thread
		delete thr;
		thr = nullptr;
	}
}

bool IThread::isRunning()
{
	return status == IThread::Status::RUNNING;
}
