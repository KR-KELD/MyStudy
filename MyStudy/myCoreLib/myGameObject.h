#pragma once
#include "myTransform.h"
class myGameObject
{
public:
	myGameObject*			m_Parent;
	vector<myGameObject*>	m_Childs;
public:
	static myGameObject* CreateGameObject() { return new myGameObject; }
public:

	template <class component_t>
	void insertComponent(component_t* component)
	{
		//--------------------------------------------------------
		// 컴포넌트 식별자 ID 취득
		//--------------------------------------------------------
		size_t componentId = get_component<component_t>::type::getComponentId();

		//--------------------------------------------------------
		// 컴포넌트를 컨테이너에 삽입
		//--------------------------------------------------------
		this->components[componentId] = component;
	}

	template <class component_t>
	component_t* getComponent(void)
	{
		//--------------------------------------------------------
		// 컴포넌트 식별자 ID 취득
		//--------------------------------------------------------
		size_t componentId = get_component<component_t>::type::getComponentId();

		//--------------------------------------------------------
		// 컴포넌트가 컨테이너에 존재하지 않다면 nullptr 리턴
		//--------------------------------------------------------
		if (this->components.find(componentId) == this->components.end())
			return nullptr;

		//--------------------------------------------------------
		// 컴포넌트가 존재한다면 컴포넌트 리턴
		//--------------------------------------------------------
		return reinterpret_cast<component_t*>(this->components[componentId]);
	}

private:
	unordered_map<size_t, myComponent*> components;
	unordered_map<size_t, myComponent*>::iterator m_iter;
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
public:
	myGameObject();
	~myGameObject();
};

