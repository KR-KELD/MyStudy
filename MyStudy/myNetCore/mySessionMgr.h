#pragma once
#include "myNetUser.h"
#include "myThread.h"
#include <map>

class mySessionMgr : public NetSingleTon<mySessionMgr>,
					 public myThread
{
private:
	friend class NetSingleTon<mySessionMgr>;
	HANDLE						m_hMutexPacketPool;
	std::list<myNetUser*>		m_UserList;
	std::vector<myPacket>	    m_PacketPool;
	std::vector<UPACKET>	    m_BroadcasttingPacketPool;
	std::map<int,void(mySessionMgr::*)(myPacket&)>	m_mapPacketFunc;
public:
	void			AddUser(myNetUser * pUser);
	void			AddPacket(myPacket& packet);
	virtual bool	Run() override;
	virtual bool	DelUser(myNetUser* pUser);
	virtual bool	SendData(myNetUser& user, UPACKET& msg);
	virtual bool	MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
public:
	void			PacketChatMsg(myPacket& packet);
	void			PacketLoginLeq(myPacket& packet);
	void			PacketUserPos(myPacket& packet);
protected:
	virtual bool	Broadcastting();
	virtual bool	CloseUser(myNetUser* user);
public:
	mySessionMgr();
	~mySessionMgr();
};
#define I_Session mySessionMgr::GetInstance()
