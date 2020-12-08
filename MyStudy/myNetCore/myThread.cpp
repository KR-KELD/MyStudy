#include "myThread.h"

void myThread::CreateThread()
{
	if (m_bStarted == true) return;
	m_hThread = _beginthreadex(NULL, 0, HandleRunner, (LPVOID)this, 0, &m_iThreadID);
	m_bStarted = true;
}

unsigned int __stdcall myThread::HandleRunner(LPVOID arg)
{
	myThread* pThread = (myThread*)arg;
	pThread->Run();
	return 0;
}

myThread::myThread() : m_bStarted(false)
{
}

myThread::~myThread()
{
	CloseHandle((HANDLE)m_hThread);
}
