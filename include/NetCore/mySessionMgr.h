#pragma once
#include "myNetUser.h"
#include "myThread.h"

class mySessionMgr : public NetSingleTon<mySessionMgr>,
					 public myThread
{
private:
	friend class NetSingleTon<mySessionMgr>;
	HANDLE						m_hMutexPacketPool;
public:
	std::list<myNetUser*>		m_UserList;
	std::vector<myPacket>	    m_PacketPool;
public:
	void	AddPacket(myPacket& packet);
	virtual  bool Run() override;
public:
	mySessionMgr();
	~mySessionMgr();
};
#define I_Session mySessionMgr::GetInstance()
