#pragma once
#include "myDebugCamera.h"
#include "myModelViewCamera.h"
#include "myController.h"
class myCameraManager : public SingleTon<myCameraManager>
{
	friend class SingleTon<myCameraManager>;
public:
	myGameObject	m_CameraContainer;
	myGameObject*	m_pMainCameraObj;
	myCamera*		m_pMainCamera;
public:
	myGameObject*	CreateCameraObj(ID3D11DeviceContext* pContext, wstring strObjName, myCamera* pCamera/*, myController* pController = nullptr*/)
	{
		myGameObject* obj = m_CameraContainer.Add(strObjName);
		//if (pController != nullptr)
		//{
		//	obj->InsertComponent(pController);
		//	pController->Init();
		//}
		obj->InsertComponent(pCamera);
		pCamera->Init();
		pCamera->CreateFrustum(pContext);
		return obj;
	}
	bool			SetMainCamera(myGameObject* pCameraObj);
	bool			SetMainCamera(wstring strCameraName);
public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();
public:
	myCameraManager();
	~myCameraManager();
};
#define g_CamMgr myCameraManager::GetInstance()
#define g_pMainCamTransform myCameraManager::GetInstance().m_pMainCamera->m_pTransform
#define g_CamList myCameraManager::GetInstance().m_CameraContainer


