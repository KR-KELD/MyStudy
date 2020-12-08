#include "myLock.h"

myLock::myLock(myServerObj * pObj) : m_pServerObj(pObj)
{
	if (m_pServerObj == nullptr) return;
	EnterCriticalSection(&m_pServerObj->m_cs);
}

myLock::~myLock()
{
	if (m_pServerObj == nullptr) return;
	LeaveCriticalSection(&m_pServerObj->m_cs);
}
