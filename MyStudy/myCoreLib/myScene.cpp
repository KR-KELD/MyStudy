#include "myScene.h"
#include "myObjManager.h"

bool myScene::Load(myParsingScript* Parser, const TCHAR* szFileName)
{
	Parser->ObjInfoParsing(Parser->LoadFile(szFileName), &m_vecObjInfo);
	return true;
}

void myScene::DistributeInfo(int iPoolCount)
{
}

myPoint myScene::GetPlayerPos()
{
	return myPoint();
}

bool myScene::Reset()
{
	return true;
}

bool myScene::Init()
{
	return true;
}

bool myScene::Frame()
{
	//for (myObjectInfo* att : m_vecObjInfo)
	//{
	//	m_pObject = g_ObjMgr.GetPtr(att->strObjName);
	//	m_pObject->LinkInfo(att);
	//	m_pObject->Frame();
	//}
	return true;
}

bool myScene::Render()
{
	//for (myObjectInfo* att : m_vecObjInfo)
	//{
	//	m_pObject = g_ObjMgr.GetPtr(att->strObjName);
	//	m_pObject->LinkInfo(att);
	//	m_pObject->Render();
	//}
	return true;
}

bool myScene::Release()
{
	for (myObjectInfo* info : m_vecObjInfo)
	{
		delete info;
	}
	m_vecObjInfo.clear();
	return true;
}

myScene::myScene()
{
	m_iSceneID = 0;
	m_bAbleChange = false;
}

myScene::~myScene()
{
}
