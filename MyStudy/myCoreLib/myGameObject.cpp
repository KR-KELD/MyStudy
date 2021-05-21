#include "myGameObject.h"
#include "myCollisionManager.h"
#include "myObjManager.h"
DECLARE_COMPONENT(myComponent);
DECLARE_COMPONENT(myTransform);
DECLARE_COMPONENT(myCollider);
DECLARE_COMPONENT(myGameObject);
myComponent * myComponent::Clone()
{
	return new myComponent(*this);
}
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
		//필요할때 풀기
		m_matScale = Matrix::CreateScale(m_vScale);
		//m_matRot = Matrix::CreateFromQuaternion(m_qRot);
		m_matRot = Matrix::CreateFromYawPitchRoll(m_qRot.y, m_qRot.x, m_qRot.z);
		m_matWorld = m_matScale * m_matRot;
		m_matWorld._41 = m_vPos.x;
		m_matWorld._42 = m_vPos.y;
		m_matWorld._43 = m_vPos.z;
	}
	if (m_pGameObject->m_pParent != nullptr)
		m_matWorld *= m_pGameObject->m_pParent->m_pTransform->m_matWorld;
	if (pView != nullptr)
	{
		m_matView = *pView;
	}
	if (pProj != nullptr)
	{
		m_matProj = *pProj;
	}
	//업데이트 벡터를 쓰면 카메라 기준으로 움직이고
	//쓰지않으면 오브젝트 기준으로 움직인다
	//썼을때 카메라의 각도에따라 이동단위가 변한다
	//UpdateVector();
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

bool myCollider::Init()
{
	if (m_pGameObject != nullptr)
	{
		m_pGameObject->m_pCollider = this;
		if (m_isSelectEnable)
		{
			g_CollisionMgr.AddSelectExecute(this, 
						std::bind(&myGameObject::OnMouseOverlap, m_pGameObject, 
						std::placeholders::_1,
						std::placeholders::_2));
		}
		if (m_isCollisionEnable)
		{
			g_CollisionMgr.AddCollisionExecute(this,
				std::bind(&myGameObject::NoneCollision, m_pGameObject,
					std::placeholders::_1,
					std::placeholders::_2));
			g_CollisionMgr.AddBeginExecute(this,
				std::bind(&myGameObject::OnCollisionEnter, m_pGameObject,
					std::placeholders::_1,
					std::placeholders::_2));
			g_CollisionMgr.AddStayExecute(this,
				std::bind(&myGameObject::OnCollisionStay, m_pGameObject,
					std::placeholders::_1,
					std::placeholders::_2));
			g_CollisionMgr.AddEndExecute(this,
				std::bind(&myGameObject::OnCollisionEnd, m_pGameObject,
					std::placeholders::_1,
					std::placeholders::_2));
		}

	}
	return true;
}

bool myCollider::Frame()
{
	return true;
}

bool myCollider::Render()
{
	return true;
}

bool myCollider::Release()
{
	return true;
}

myCollider::myCollider()
{
	m_isUnique = false;
	m_eType = COLLIDER_NONE;
}

myCollider::~myCollider()
{
}


myGameObject * myGameObject::Clone()
{
	myGameObject* pObj = new myGameObject(*this);

	for (pObj->m_ComIter = pObj->m_ComponentList.begin();
		pObj->m_ComIter != pObj->m_ComponentList.end();
		pObj->m_ComIter++)
	{
		if (!pObj->m_ComIter->second->m_isUnique)
		{
			pObj->m_ComIter->second = pObj->m_ComIter->second->Clone();
		}
		pObj->m_ComIter->second->Set(pObj);
	}
	myTransform* pTrans = pObj->GetComponent<myTransform>();
	pObj->m_pTransform = pTrans;
	myCollider* pCollider = pObj->GetComponent<myCollider>();
	if (pCollider != nullptr)
	{
		pObj->m_pCollider = pCollider;
	}

	for (pObj->m_ComIter = pObj->m_ComponentList.begin();
		pObj->m_ComIter != pObj->m_ComponentList.end();
		pObj->m_ComIter++)
	{
		pObj->m_pTransform = pTrans;
	}

	for (pObj->m_ObjIter = pObj->m_Childs.begin();
		pObj->m_ObjIter != pObj->m_Childs.end();
		pObj->m_ObjIter++)
	{
		pObj->m_ObjIter->second = pObj->m_ObjIter->second->Clone();
		pObj->m_ObjIter->second->m_pParent = pObj;
	}

	return pObj;
	//return nullptr;
}

bool myGameObject::Init()
{
	m_iObjectID = g_ObjMgr.SetObjectList(this);
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
		if ((*m_ComIter).second->m_isActive)
			(*m_ComIter).second->PreFrame();
	}
	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		if ((*m_ObjIter).second->m_isActive)
			(*m_ObjIter).second->PreFrame();
	}

	if (m_pParent)
	{

	}
	return true;
}

bool myGameObject::Frame()
{
	//PreFrame();
	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
		if ((*m_ComIter).second->m_isActive)
			(*m_ComIter).second->Frame();
	}
	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		if ((*m_ObjIter).second->m_isActive)
			(*m_ObjIter).second->Frame();
	}
	//PostFrame();
	return true;
}

bool myGameObject::PostFrame()
{
	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
		if ((*m_ComIter).second->m_isActive)
			(*m_ComIter).second->PostFrame();
	}
	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		if ((*m_ObjIter).second->m_isActive)
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
		if ((*m_ComIter).second->m_isActive && (*m_ComIter).second->m_isRender)
		{
			(*m_ComIter).second->PreRender();
		}
	}
	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		if ((*m_ObjIter).second->m_isActive && (*m_ObjIter).second->m_isRender)
		{
			(*m_ObjIter).second->PreRender();
		}
	}
	return true;
}

bool myGameObject::Render()
{
	//PreRender();
	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
		if ((*m_ComIter).second->m_isActive && (*m_ComIter).second->m_isRender)
		{
			(*m_ComIter).second->Render();
		}
	}
	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		if ((*m_ObjIter).second->m_isActive && (*m_ObjIter).second->m_isRender)
		{
			(*m_ObjIter).second->Render();
		}
	}
	//PostRender();
	return true;
}

bool myGameObject::PostRender()
{
	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
		if ((*m_ComIter).second->m_isActive && (*m_ComIter).second->m_isRender)
		{
			(*m_ComIter).second->PostRender();
		}
	}
	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		if ((*m_ObjIter).second->m_isActive && (*m_ObjIter).second->m_isRender)
		{
			(*m_ObjIter).second->PostRender();
		}
	}
	return true;
}

void myGameObject::Update()
{
	for (m_ComIter = m_ComponentList.begin();
		m_ComIter != m_ComponentList.end();
		m_ComIter++)
	{
		if ((*m_ComIter).second->m_isActive)
			(*m_ComIter).second->Update();
	}

	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		if ((*m_ObjIter).second->m_isActive)
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

void myGameObject::OnMouseOverlap(Vector3 vPos, DWORD dwState)
{
	if (m_pCollider != nullptr)
	{
		m_pCollider->m_dwSelectState = dwState;
	}
}

void myGameObject::NoneCollision(myCollider * pObj, DWORD dwState)
{
	if (m_pCollider != nullptr)
	{
		m_pCollider->m_dwCollisionState = dwState;
	}
}

void myGameObject::OnCollisionEnter(myCollider * pObj, DWORD dwState)
{
	if (m_pCollider != nullptr)
	{
		m_pCollider->m_dwCollisionState = dwState;
	}
}

void myGameObject::OnCollisionStay(myCollider * pObj, DWORD dwState)
{
	if (m_pCollider != nullptr)
	{
		m_pCollider->m_dwCollisionState = dwState;
	}
}

void myGameObject::OnCollisionEnd(myCollider * pObj, DWORD dwState)
{
	if (m_pCollider != nullptr)
	{
		m_pCollider->m_dwCollisionState = dwState;
	}
}

myGameObject* myGameObject::Add(wstring strName)
{
	myGameObject* obj;
	if (strName.empty())
	{
		obj = CreateGameObject();
		obj->SetParent(this);
		m_Childs.insert(make_pair(obj->m_strName, obj));
		return obj;
	}
	obj = CreateGameObject(strName.c_str());
	obj->SetParent(this);
	m_Childs.insert(make_pair(strName, obj));
	return obj;
}

myGameObject * myGameObject::Add(myGameObject * pGameObj)
{
	if (pGameObj == nullptr) return nullptr;
	
	pGameObj->SetParent(this);
	m_Childs.insert(make_pair(pGameObj->m_strName, pGameObj));
	return pGameObj;
}

myGameObject * myGameObject::GetGameObject(wstring strName)
{
	m_ObjIter = m_Childs.find(strName);
	myGameObject * pObj = nullptr;
	if (m_ObjIter != m_Childs.end())
	{
		return (*m_ObjIter).second;
	}

	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		pObj = GetGameObject(strName);
		if (pObj != nullptr)
		{
			return pObj;
		}
	}
	return nullptr;
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

void myGameObject::SetParent(myGameObject * pParent)
{
	m_pParent = pParent;
}


