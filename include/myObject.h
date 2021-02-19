#pragma once
#include "myCollision.h"
#include "myGraphics.h"

typedef vector<RECT> RECT_ARRAY;
class myObject
{
public:
	myGraphics					m_Graphics;
	map<T_STR, myComponent>		m_Components;
public:
	virtual bool				Load(wstring strImageName);
	virtual void				SetTransition(DWORD dwEvent);
	virtual bool				Action();
public:
	virtual bool				Reset();
	virtual bool				Init();
	virtual bool				Frame();
	virtual bool				Render();
	virtual bool				Release();
public:
	myObject();
	virtual ~myObject();
};
