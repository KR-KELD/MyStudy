#pragma once
#include "myNetStd.h"
#include "myNetUser.h"
#define MAX_WORKER_THREAD 3
class myServer;

class myIOCP/* : public NetSingleTon<myIOCP>*/
{
//private:
//	friend class NetSingleTon<myIOCP>;
public:
	HANDLE			m_hIOCP;
	HANDLE			m_hWorkerThread[MAX_WORKER_THREAD];
	DWORD			m_dwThreadID[MAX_WORKER_THREAD];
	myServer*		m_pServer;
public:
	virtual bool		Init();
	virtual bool		Run();
	virtual bool		Release();
	virtual void		SetBind(SOCKET sock, ULONG_PTR key);
	virtual void		SendLogoutUser(myNetUser* pUser);
	virtual void		GetServer(myServer* pServer);
	static DWORD WINAPI WorkerThread(LPVOID param);
public:
	myIOCP();
	virtual ~myIOCP();
};
//#define I_Iocp myIOCP::GetInstance()
