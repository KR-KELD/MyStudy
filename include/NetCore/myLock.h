#pragma once
#include "myServerObj.h"
class myLock
{
private:
	myServerObj*	m_pServerObj;
public:
	myLock(myServerObj* pObj);
	virtual ~myLock();
};

