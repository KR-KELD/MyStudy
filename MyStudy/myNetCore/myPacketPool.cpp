#include "myPacketPool.h"
void myPacketPool::AddPacket(myPacket & packet)
{
	EnterCriticalSection(&m_cs);
		m_list.push_back(packet);
	LeaveCriticalSection(&m_cs);
}

void myPacketPool::Clear()
{
	EnterCriticalSection(&m_cs);
		m_list.clear();
	LeaveCriticalSection(&m_cs);
}

//void myPacketPool::Lock()
//{
//	EnterCriticalSection(&m_cs);
//}
//
//void myPacketPool::Unlock()
//{
//	LeaveCriticalSection(&m_cs);
//}

myPacketPool::myPacketPool()
{
}

myPacketPool::~myPacketPool()
{
}
