#include "myIOCP.h"

bool myIOCP::Init()
{
	//iocp ����
	m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	//������ �Ҵ�
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
	//iocp�� ���ø��� ��Ʈ ����
	m_hIOCP = ::CreateIoCompletionPort((HANDLE)sock, m_hIOCP, key, 0);
}

DWORD __stdcall myIOCP::WorkerThread(LPVOID param)
{
	myIOCP* iocp = (myIOCP*)param;
	BOOL bReturn;
	//�۾�����
	DWORD dwTransfor;
	//���ø��� Ű��
	ULONG_PTR keyValue;
	//������ ����ü
	OVERLAPPED* pOV;
	while (1)
	{
		//�񵿱� �۾� ����� iocp ť���� ��������
		bReturn = ::GetQueuedCompletionStatus(iocp->m_hIOCP,
			&dwTransfor,
			&keyValue,
			&pOV, 1);
		//������ �����
		if (bReturn == TRUE)
		{
			myNetUser* pUser = (myNetUser*)keyValue;
			OVERLAPPED2* tOV = (OVERLAPPED2*)pOV;
			if (tOV->iType == OVERLAPPED2::MODE_RECV)
			{
				//// �ε�Ϸ�
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
