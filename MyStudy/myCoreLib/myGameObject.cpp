#include "myGameObject.h"

DECLARE_COMPONENT(myComponent);
DECLARE_COMPONENT(myTransform);
DECLARE_COMPONENT(myGameObject);
bool myComponent::Init()
{
	return true;
}

bool myComponent::PreFrame()
{
	return true;
}

bool myComponent::Frame()
{
	return true;
}

bool myComponent::PostFrame()
{
	return true;
}

bool myComponent::PreRender()
{
	return true;
}

bool myComponent::Render()
{
	return true;
}

bool myComponent::PostRender()
{
	return true;
}

void myComponent::Update()
{
}

void myComponent::Reset()
{

}

bool myComponent::Action()
{
	return true;
}

bool myComponent::Release()
{
	return true;
}

void myTransform::ComputeMatWorld()
{

}

bool myTransform::Init()
{
	return true;
}

bool myTransform::PreFrame()
{
	return true;
}

bool myTransform::Frame()
{
	return true;
}

bool myTransform::PostFrame()
{
	return true;
}

bool myTransform::PreRender()
{
	return true;
}

bool myTransform::Render()
{
	return true;
}

bool myTransform::PostRender()
{
	return true;
}

void myTransform::Update()
{

}

void myTransform::Reset()
{

}

bool myTransform::Action()
{
	return true;
}

bool myTransform::Release()
{
	return true;
}


bool myGameObject::Init()
{
	//for (m_ComIter = m_ComponentList.begin();
	//	m_ComIter != m_ComponentList.end();
	//	m_ComIter++)
	//{
	//	(*m_ComIter).second->Init();
	//}

	//for (m_ObjIter = m_Childs.begin();
	//	m_ObjIter != m_Childs.end();
	//	m_ObjIter++)
	//{
	//	(*m_ObjIter).second->Init();
	//}
	return true;
}

bool myGameObject::PreFrame()
{
	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
		(*m_ComIter).second->PreFrame();
	}

	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		(*m_ObjIter).second->PreFrame();
	}
	return true;
}

bool myGameObject::Frame()
{
	PreFrame();
	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
		(*m_ComIter).second->Frame();
	}

	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		(*m_ObjIter).second->Frame();
	}
	PostFrame();
	return true;
}

bool myGameObject::PostFrame()
{
	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
		(*m_ComIter).second->PostFrame();
	}

	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		(*m_ObjIter).second->PostFrame();
	}
	return true;
}

bool myGameObject::PreRender()
{
	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
		(*m_ComIter).second->PreRender();
	}

	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		(*m_ObjIter).second->PreRender();
	}
	return true;
}

bool myGameObject::Render()
{
	PreRender();

	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
		(*m_ComIter).second->Render();
	}

	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		(*m_ObjIter).second->Render();
	}

	PostRender();

	return true;
}

bool myGameObject::PostRender()
{
	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
		(*m_ComIter).second->PostRender();
	}

	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		(*m_ObjIter).second->PostRender();
	}

	return true;
}

void myGameObject::Update()
{
	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
		(*m_ComIter).second->Update();
	}

	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		(*m_ObjIter).second->Update();
	}
}

void myGameObject::Reset()
{
}

bool myGameObject::Action()
{
	return true;
}

bool myGameObject::Release()
{
	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		(*m_ObjIter).second->Release();
		delete (*m_ObjIter).second;
	}
	m_ComponentList.clear();

	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
		if ((*m_ComIter).second == this)
		{
			continue;
		}
		(*m_ComIter).second->Release();
		delete (*m_ComIter).second;
	}
	m_ComponentList.clear();
	return true;
}

myGameObject* myGameObject::Add(wstring strName)
{
	myGameObject* obj;
	if (strName.empty())
	{
		obj = CreateGameObject(L"myGameObject");
		m_Childs.insert(make_pair(L"myGameObject", obj));
		return obj;
	}
	obj = CreateGameObject(strName.c_str());
	m_Childs.insert(make_pair(strName, obj));
	return obj;
}

myGameObject * myGameObject::GetGameObject(wstring strName)
{
	m_ObjIter = m_Childs.find(strName);
	if (m_ObjIter == m_Childs.end())
	{
		return nullptr;
	}
	return (*m_ObjIter).second;
}

list<myGameObject*>* myGameObject::GetGameObjects(wstring strName)
{
	//Áú¹® 
	list<myGameObject*>* result = new list<myGameObject*>;
	for (m_ObjIter = m_Childs.lower_bound(strName);
		m_ObjIter != m_Childs.upper_bound(strName); m_ObjIter++)
	{
		result->push_back(m_ObjIter->second);
	}
	return result;
}

