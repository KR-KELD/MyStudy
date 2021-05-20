#pragma once
#include "myMouse.h"
#include "myCollider.h"

using CollisionFunction = std::function<void(myCollider*, DWORD)>;
using SelectFunction = std::function<void(Vector3, DWORD)>;

class myCollisionManager : public SingleTon<myCollisionManager>
{
	friend class SingleTon<myCollisionManager>;
private:
	int			m_iExecuteSelectID = 0;
	int			m_iExecuteCollisionID = 0;
public:
	myMouse		m_Mouse;
public:
	//���� ������ ������Ʈ ����Ʈ
	std::map<int, myCollider*>   m_SelectObjectList;
	//���� ������ ������Ʈ���� �ݹ��Լ� ����Ʈ
	std::map<int, SelectFunction> m_fnSelectExecute;
	//�Ŵ����� ����ϴ� �Լ�
	void AddSelectExecute(myCollider* owner, SelectFunction func);
	void DeleteSelect(int iID);
public:
	std::map<int, myCollider*>   m_CollisionObjectList;
	std::map<int, CollisionFunction> m_fnNoneExecute;
	std::map<int, CollisionFunction> m_fnBeginExecute;
	std::map<int, CollisionFunction> m_fnStayExecute;
	std::map<int, CollisionFunction> m_fnEndExecute;
	void AddCollisionExecute(myCollider* owner, CollisionFunction func);
	void AddBeginExecute(myCollider* owner, CollisionFunction func);
	void AddStayExecute(myCollider* owner, CollisionFunction func);
	void AddEndExecute(myCollider* owner, CollisionFunction func);
	void DeleteCollision(int iID);
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
private:
	myCollisionManager();
public:
	~myCollisionManager();
};
#define g_CollisionMgr myCollisionManager::GetInstance()
