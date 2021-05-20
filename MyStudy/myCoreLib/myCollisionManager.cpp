#include "myCollisionManager.h"

void myCollisionManager::AddCollisionExecute(myCollider * owner, CollisionFunction func)
{
	owner->m_iCollisionID = m_iExecuteCollisionID++;
	//충돌 리스트에 등록
	m_CollisionObjectList.insert(std::make_pair(owner->m_iCollisionID, owner));
	//기본상태 등록
	m_fnNoneExecute.insert(std::make_pair(owner->m_iCollisionID, func));
	//다른 오브젝트들에게 자신을 등록
	//for (auto obj : m_CollisionObjectList)
	//{
	//	myGameObject* pObj = obj.second;
	//	pObj->m_OtherObjectState.insert(
	//		std::make_pair(owner->m_iCollisionID, OVERLAP_NONE));
	//}

}

void myCollisionManager::AddBeginExecute(myCollider * owner, CollisionFunction func)
{
	m_fnBeginExecute.insert(
		std::make_pair(owner->m_iCollisionID, func));
}

void myCollisionManager::AddStayExecute(myCollider * owner, CollisionFunction func)
{
	m_fnStayExecute.insert(
		std::make_pair(owner->m_iCollisionID, func));
}

void myCollisionManager::AddEndExecute(myCollider * owner, CollisionFunction func)
{
	m_fnEndExecute.insert(
		std::make_pair(owner->m_iCollisionID, func));
}

void myCollisionManager::DeleteCollision(int iID)
{
	//다른 오브젝트들에서 자신의 상태값 삭제
	//for (auto obj : m_CollisionObjectList)
	//{
	//	myGameObject* pObj = obj.second;
	//	auto obj = pObj->m_OtherObjectState.find(iID);
	//	if (obj != pObj->m_OtherObjectState.end())
	//	{
	//		pObj->m_OtherObjectState.erase(obj);
	//	}
	//}

	auto obj = m_CollisionObjectList.find(iID);
	if (obj != m_CollisionObjectList.end())
	{
		m_CollisionObjectList.erase(obj);
	}
	auto obj1 = m_fnNoneExecute.find(iID);
	if (obj1 != m_fnNoneExecute.end())
	{
		m_fnNoneExecute.erase(obj1);
	}
	auto obj2 = m_fnBeginExecute.find(iID);
	if (obj2 != m_fnBeginExecute.end())
	{
		m_fnBeginExecute.erase(obj2);
	}
	auto obj3 = m_fnStayExecute.find(iID);
	if (obj3 != m_fnStayExecute.end())
	{
		m_fnStayExecute.erase(obj3);
	}
	auto obj4 = m_fnEndExecute.find(iID);
	if (obj4 != m_fnEndExecute.end())
	{
		m_fnEndExecute.erase(obj4);
	}
}

bool myCollisionManager::Init()
{
	return true;
}

bool myCollisionManager::Frame()
{
	map<int, myCollider*>::iterator selectIter = m_SelectObjectList.begin();
	for (; selectIter != m_SelectObjectList.end(); selectIter++)
	{
		myCollider* pCollider = selectIter->second;
		if (pCollider->m_isActive == false || pCollider->m_isSelectEnable == false)
			continue;
		m_Mouse.ScreenToRay();
		
		if (pCollider->m_eType == COLLIDER_BOX)
		{
			myBoxCollider* pBox = (myBoxCollider*)selectIter->second;
			if (myCollision::IntersectRayToBox(m_Mouse.m_myRay, pBox->m_Box))
			{
				auto fnFunc = m_fnSelectExecute.find(pBox->m_iSelectID);
				if (fnFunc != m_fnSelectExecute.end())
				{
					SelectFunction call = fnFunc->second;
					call(m_Mouse.m_vIntersectionPos, myOverlapState::OVERLAP_HOVER);
				}
				if (g_Input.GetKey(VK_LBUTTON) == KEY_HOLD)
				{
					SelectFunction call = fnFunc->second;
					call(m_Mouse.m_vIntersectionPos, myOverlapState::OVERLAP_ACTIVE);
				}
				if (g_Input.GetKey(VK_LBUTTON) == KEY_UP)
				{
					SelectFunction call = fnFunc->second;
					call(m_Mouse.m_vIntersectionPos, myOverlapState::OVERLAP_SELECTED);
				}
			}
			else
			{
				if (pBox->m_dwSelectState != myOverlapState::OVERLAP_DEFAULT)
				{
					auto fnFunction = m_fnSelectExecute.find(pBox->m_iSelectID);
					SelectFunction call = fnFunction->second;
					call(m_Mouse.m_myRay.vOrigin, myOverlapState::OVERLAP_DEFAULT);
				}
			}
		}
	}
	return true;
}

bool myCollisionManager::Render()
{
	return true;
}

bool myCollisionManager::Release()
{
	return true;
}

myCollisionManager::myCollisionManager()
{
}

myCollisionManager::~myCollisionManager()
{
}

void myCollisionManager::AddSelectExecute(myCollider * owner, SelectFunction func)
{
	owner->m_iSelectID = m_iExecuteSelectID++;
	m_SelectObjectList.insert(
		std::make_pair(owner->m_iSelectID, owner));
	m_fnSelectExecute.insert(
		std::make_pair(owner->m_iSelectID, func));
}

void myCollisionManager::DeleteSelect(int iID)
{
	auto obj = m_SelectObjectList.find(iID);
	if (obj != m_SelectObjectList.end())
	{
		m_SelectObjectList.erase(obj);
	}

	auto obj1 = m_fnSelectExecute.find(iID);
	if (obj1 != m_fnSelectExecute.end())
	{
		m_fnSelectExecute.erase(obj1);
	}
}
