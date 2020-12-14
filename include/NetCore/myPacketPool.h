#pragma once
#include "myNetUser.h"

class myPacketPool : public myServerObj
{
public:
	std::vector<myPacket>	    m_list;
public:
	void	AddPacket(myPacket& packet);
	void	Clear();
	void	Lock();
	void	Unlock();
public:
	myPacketPool();
	virtual ~myPacketPool();
};

