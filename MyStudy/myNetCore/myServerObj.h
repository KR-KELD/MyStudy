#pragma once
#include "myNetStd.h"
class myServerObj
{
public:
	CRITICAL_SECTION m_cs;
public:
	myServerObj();
	virtual ~myServerObj();
};

