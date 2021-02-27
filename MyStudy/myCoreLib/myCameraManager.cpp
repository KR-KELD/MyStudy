#include "myCameraManager.h"

bool myCameraManager::SetMainCamera(myGameObject * pCameraObj)
{
	if (pCameraObj != nullptr)
	{
		myCamera* camera = pCameraObj->GetComponent<myCamera>();
		if (camera != nullptr)
		{
			m_pMainCameraObj = pCameraObj;
			m_pMainCamera = camera;
			return true;
		}
	}
	return false;
}

bool myCameraManager::SetMainCamera(wstring strCameraName)
{
	myGameObject* obj = m_CameraContainer.GetGameObject(strCameraName);
	if (SetMainCamera(obj))
	{
		return true;
	}
	return false;
}

bool myCameraManager::Init()
{
	return true;
}

bool myCameraManager::PreFrame()
{
	m_pMainCameraObj->PreFrame();
	for (myGameObject* obj: m_SubCameraObjList)
	{
		obj->PreFrame();
	}
	return true;
}

bool myCameraManager::Frame()
{
	m_pMainCameraObj->Frame();
	for (myGameObject* obj : m_SubCameraObjList)
	{
		obj->Frame();
	}
	return true;
}

bool myCameraManager::PostFrame()
{
	m_pMainCameraObj->PostFrame();
	for (myGameObject* obj : m_SubCameraObjList)
	{
		obj->PostFrame();
	}
	return true;
}

bool myCameraManager::PreRender()
{
	return true;
}

bool myCameraManager::Render()
{
	return true;
}

bool myCameraManager::PostRender()
{
	return true;
}

bool myCameraManager::Release()
{
	m_CameraContainer.Release();
	m_SubCameraObjList.clear();
	return true;
}

myCameraManager::myCameraManager()
{

}

myCameraManager::~myCameraManager()
{

}
