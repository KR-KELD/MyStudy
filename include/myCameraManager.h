#pragma once
#include "myDebugCamera.h"
#include "myModelViewCamera.h"
#include "myController.h"
class myCameraManager : public SingleTon<myCameraManager>
{
	friend class SingleTon<myCameraManager>;
public:
	//카메라들을 담는 컨테이너
	myGameObject			m_CameraContainer;
	//메인카메라 오브젝트
	myGameObject*			m_pMainCameraObj;
	//메인카메라 컴포넌트
	myCamera*				m_pMainCamera;
	//서브카메라 리스트
	list<myGameObject*>		m_SubCameraObjList;
public:
	//카메라 객체를 생성하고 컨테이너에 삽입
	myGameObject*	CreateCameraObj(wstring strObjName, myCamera* pCamera)
	{
		myGameObject* obj = m_CameraContainer.Add(strObjName);
		obj->InsertComponent(pCamera);
		pCamera->Init();
		pCamera->CreateFrustum();
		return obj;
	}
	//메인카메라 변경(객체 포인터)
	bool			SetMainCamera(myGameObject* pCameraObj);
	//메인카메라 변경(컨테이너 이름 검색)
	bool			SetMainCamera(wstring strCameraName);
	//서브카메라 등록
	bool			InsertSubCamera(myGameObject* pCameraObj)
	{
		m_SubCameraObjList.push_back(pCameraObj);
	}
	//서브카메라 제거
	bool			RemoveSubCamera(myGameObject* pCameraObj)
	{
		m_SubCameraObjList.remove(pCameraObj);
	}

public:
	bool	Init();
	bool	PreFrame();
	bool	Frame();
	bool	PostFrame();
	bool	PreRender();
	bool	Render();
	bool	PostRender();
	bool	Release();
public:
	myCameraManager();
	~myCameraManager();
};
#define g_CamMgr myCameraManager::GetInstance()
#define g_pMainCamTransform myCameraManager::GetInstance().m_pMainCamera->m_pTransform
#define g_CamList myCameraManager::GetInstance().m_CameraContainer


