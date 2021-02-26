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

bool myTransform::SetMatrix(Matrix * pWorld, Matrix * pView, Matrix * pProj)
{
	if (pWorld != nullptr)
	{
		m_matWorld = *pWorld;
	}
	else
	{
		m_matWorld = m_matScale * m_matRot;
		m_matWorld._41 = m_vPos.x;
		m_matWorld._42 = m_vPos.y;
		m_matWorld._43 = m_vPos.z;
	}
	if (pView != nullptr)
	{
		m_matView = *pView;
	}
	if (pProj != nullptr)
	{
		m_matProj = *pProj;
	}
	return true;
}

void myTransform::UpdateVector()
{
	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;
	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;
	m_vRight.x = m_matView._11;
	m_vRight.y = m_matView._21;
	m_vRight.z = m_matView._31;

	m_vLook.Normalize();
	m_vUp.Normalize();
	m_vRight.Normalize();
}

void myTransform::SetPos(Vector3 p)
{
	m_vPos = p;
}

void myTransform::SetTarget(Vector3 p)
{
	m_vTarget = p;
}

void myTransform::FrontMovement(float fDir)
{
	Vector3 vOffset = m_pTransform->m_vLook * g_fSecondPerFrame * m_fSpeed * fDir;
	m_pTransform->m_vPos += vOffset;
}
void myTransform::RightMovement(float fDir)
{
	Vector3 vMove = m_pTransform->m_vRight * g_fSecondPerFrame * m_fSpeed * fDir;
	m_pTransform->m_vPos += vMove;
}
void myTransform::UpMovement(float fDir)
{
	Vector3 vMove = m_pTransform->m_vUp * g_fSecondPerFrame * m_fSpeed * fDir;
	m_pTransform->m_vPos += vMove;
}
void myTransform::FrontBase(float fDir)
{
	Vector3 vSide = { 0,0,1 };
	Vector3 vMove = vSide * g_fSecondPerFrame * m_fSpeed * fDir;
	m_pTransform->m_vPos += vMove;
	m_pTransform->m_vTarget += m_pTransform->m_vLook * m_fSpeed;
}
void myTransform::RightBase(float fDir)
{
	Vector3 vSide = { 1,0,0 };
	Vector3 vMove = vSide * g_fSecondPerFrame * m_fSpeed * fDir;
	m_pTransform->m_vPos += vMove;
	m_pTransform->m_vTarget += m_pTransform->m_vLook * m_fSpeed;
}
void myTransform::UpBase(float fDir)
{
	Vector3 vUp = { 0,1,0 };
	Vector3 vMove = vUp * g_fSecondPerFrame * m_fSpeed * fDir;
	m_pTransform->m_vPos += vMove;
	m_pTransform->m_vTarget += m_pTransform->m_vLook * m_fSpeed;
}

void myTransform::LookAt(Vector3 vTarget)
{
	m_vTarget = vTarget;
	Vector3 vUp = { 0,1,0 };
	m_pTransform->m_matView = Matrix::CreateLookAt(m_pTransform->m_vPos, vTarget, vUp);
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
	//UpdateVector();
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


myGameObject * myGameObject::Clone()
{
	//질문
	//for (m_ComIter = m_ComponentList.begin();
	//	m_ComIter != m_ComponentList.end();
	//	m_ComIter++)
	//{

	//}

	//for (m_ObjIter = m_Childs.begin();
	//	m_ObjIter != m_Childs.end();
	//	m_ObjIter++)
	//{

	//}
	//return new myGameObject(*this);
	return nullptr;
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
	//PreRender();

	//for (m_ComIter = m_ComponentList.begin();
	//	m_ComIter != m_ComponentList.end();
	//	m_ComIter++)
	//{
	//	(*m_ComIter).second->Render();
	//}

	//for (m_ObjIter = m_Childs.begin();
	//	m_ObjIter != m_Childs.end();
	//	m_ObjIter++)
	//{
	//	(*m_ObjIter).second->Render();
	//}

	//PostRender();

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
	m_Childs.clear();

	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
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
	//질문 
	list<myGameObject*>* result = new list<myGameObject*>;
	for (m_ObjIter = m_Childs.lower_bound(strName);
		m_ObjIter != m_Childs.upper_bound(strName); m_ObjIter++)
	{
		result->push_back(m_ObjIter->second);
	}
	return result;
}

