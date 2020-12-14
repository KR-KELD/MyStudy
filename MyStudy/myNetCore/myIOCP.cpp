#include "myIOCP.h"
#include "mySessionMgr.h"

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

bool myIOCP::Release()
{
	CloseHandle(m_hIOCP);
	for (int iThread = 0;
		iThread < MAX_WORKER_THREAD;
		iThread++)
	{
		CloseHandle(m_hWorkerThread[iThread]);
	}
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
	OVERLAPPED2* pOV;
	while (1)
	{
		//�񵿱� �۾� ����� iocp ť���� ��������
		bReturn = ::GetQueuedCompletionStatus(iocp->m_hIOCP,
			&dwTransfor,
			&keyValue,
			(LPOVERLAPPED*)&pOV, 1);
		myNetUser* pUser = (myNetUser*)keyValue;
		if (pOV != nullptr && pOV->iType == OVERLAPPED2::MODE_EXIT)
		{
			//�ٲٱ�
			//I_Session.DelUser(pUser);
			continue;
		}
		//������ �����
		if (bReturn == TRUE)
		{
			// ��������
			if (dwTransfor == 0)
			{
				pOV->iType = OVERLAPPED2::MODE_EXIT;
				PostQueuedCompletionStatus(iocp->m_hIOCP, 0,
					keyValue, (LPOVERLAPPED)pOV);
			}
			if (pOV->iType == OVERLAPPED2::MODE_RECV)
			{
				//// �ε�Ϸ�
				if (pUser->DispatchRead(dwTransfor, pOV) == false)
				{
					pOV->iType = OVERLAPPED2::MODE_EXIT;
					PostQueuedCompletionStatus(iocp->m_hIOCP, 0,
						keyValue, (LPOVERLAPPED)pOV);
				}
			}
			if (pOV->iType == OVERLAPPED2::MODE_SEND)
			{
				if (pUser->DispatchWrite(dwTransfor, pOV) == false)
				{
					pOV->iType = OVERLAPPED2::MODE_EXIT;
					PostQueuedCompletionStatus(iocp->m_hIOCP, 0,
						keyValue, (LPOVERLAPPED)pOV);
				}
			}
		}
		else
		{
			DWORD dwError = WSAGetLastError();
			if (dwError == WAIT_TIMEOUT) { continue; }
			closesocket(pUser->m_Sock);
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
