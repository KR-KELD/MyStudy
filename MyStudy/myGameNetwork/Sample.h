#pragma once
#include "myNetwork.h"
#include "myCore.h"
class Sample : public myCore
{
public:
	myNetwork	m_Net;
public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
};

