#pragma once
#include "myNetwork.h"
#include "myCore.h"
#include "myDraw.h"

class Sample : public myCore
{
	myNetwork	m_Net;
public:
	void  SendTest(const char* pData);
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
};

