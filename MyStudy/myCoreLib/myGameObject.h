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
		// ������Ʈ �ĺ��� ID ���
		//--------------------------------------------------------
		size_t componentId = get_component<component_t>::type::getComponentId();

		//--------------------------------------------------------
		// ������Ʈ�� �����̳ʿ� ����
		//--------------------------------------------------------
		this->components[componentId] = component;
	}

	template <class component_t>
	component_t* getComponent(void)
	{
		//--------------------------------------------------------
		// ������Ʈ �ĺ��� ID ���
		//--------------------------------------------------------
		size_t componentId = get_component<component_t>::type::getComponentId();

		//--------------------------------------------------------
		// ������Ʈ�� �����̳ʿ� �������� �ʴٸ� nullptr ����
		//--------------------------------------------------------
		if (this->components.find(componentId) == this->components.end())
			return nullptr;

		//--------------------------------------------------------
		// ������Ʈ�� �����Ѵٸ� ������Ʈ ����
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

