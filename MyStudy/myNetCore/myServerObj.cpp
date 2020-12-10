#include "myServerObj.h"

myServerObj::myServerObj()
{
	InitializeCriticalSection(&m_cs);
}

myServerObj::~myServerObj()
{
	DeleteCriticalSection(&m_cs);
}
