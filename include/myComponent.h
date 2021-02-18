#pragma once
#include "myStd.h"
//template <class T>
class myComponent
{
public:
	T_STR			m_strName;
public:
	//virtual T		GetComponent(Type type)
	//{
	//	return this;
	//}
public:
	virtual bool	Init();
	virtual bool	PreFrame();
	virtual bool	Frame();
	virtual bool	PostFrame();
	virtual bool	PreRender();
	virtual bool	Render();
	virtual bool	PostRender();
	virtual void	Update();
	virtual void	Reset();
	virtual bool	Action();
	virtual bool	Release();
};

