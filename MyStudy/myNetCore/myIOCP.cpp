#include "myIOCP.h"

bool myIOCP::Init()
{
	//iocp 생성
	m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	//스레드 할당
	for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
	{
		m_hWorkerThread[iThread] =
			::CreateThread(0, 0, WorkerThread, this, 0, &m_dwThreadID[iThread]);
	}
	return true;
}

bool myIOCP::Run()
{
	return true;
}

void myIOCP::SetBind(SOCKET sock, ULONG_PTR key)
{
	//iocp에 컴플리션 포트 생성
	m_hIOCP = ::CreateIoCompletionPort((HANDLE)sock, m_hIOCP, key, 0);
}

DWORD __stdcall myIOCP::WorkerThread(LPVOID param)
{
	myIOCP* iocp = (myIOCP*)param;
	BOOL bReturn;
	//작업진도
	DWORD dwTransfor;
	//컴플리션 키값
	ULONG_PTR keyValue;
	//오버랩 구조체
	OVERLAPPED* pOV;
	while (1)
	{
		//비동기 작업 결과를 iocp 큐에서 가져오기
		bReturn = ::GetQueuedCompletionStatus(iocp->m_hIOCP,
			&dwTransfor,
			&keyValue,
			&pOV, 1);
		//쓰레드 깨우기
		if (bReturn == TRUE)
		{
			myNetUser* pUser = (myNetUser*)keyValue;
			OVERLAPPED2* tOV = (OVERLAPPED2*)pOV;
			if (tOV->iType == OVERLAPPED2::MODE_RECV)
			{
				//// 로드완료
				if (pUser->DispatchRead(dwTransfor, tOV) == false)
				{
					//::SetEvent(g_hKillEvent);
				}
			}
			if (tOV->iType == OVERLAPPED2::MODE_SEND)
			{
				if (pUser->DispatchWrite(dwTransfor, tOV) == false)
				{
					//::SetEvent(g_hKillEvent);
				}
			}
		}
		else
		{
			DWORD dwError = WSAGetLastError();
			if (dwError == WAIT_TIMEOUT) { continue; }
		}
	}

	return 0;
}

myIOCP::myIOCP()
{
}

myIOCP::~myIOCP()
{
}
