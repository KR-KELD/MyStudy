#include "myObjectPoolManager.h"


bool myObjectPoolManager::AddEffectPool(wstring strName, vector<myObjectInfo*> vecObjPool)
{
	m_mapEffectPool.insert(make_pair(strName, vecObjPool));
	return true;
}

bool myObjectPoolManager::AddProjectilePool(wstring strName, vector<myObjectInfo*> vecObjPool)
{
	m_mapProjectilePool.insert(make_pair(strName, vecObjPool));
	return true;
}

myObjectInfo * myObjectPoolManager::GetEffect(wstring strName)
{
	m_iterPool = m_mapEffectPool.find(strName);
	if (m_iterPool != m_mapEffectPool.end())
	{
		for (myObjectInfo* pInfo : m_iterPool->second)
		{
			if (pInfo->bDead == true)
			{
				pInfo->bDead = false;
				pInfo->fTimer = 0.0f;
				pInfo->fTmpTimer = 0.0f;
				pInfo->iRectIndex = 0;
				return pInfo;
			}
		}
	}
	return nullptr;
}

myObjectInfo * myObjectPoolManager::GetProjectile(wstring strName)
{
	m_iterPool = m_mapProjectilePool.find(strName);
	if (m_iterPool != m_mapProjectilePool.end())
	{
		for (myObjectInfo* pInfo : m_iterPool->second)
		{
			if (pInfo->bDead == true)
			{
				pInfo->bDead = false;
				pInfo->fTimer = 0.0f;
				pInfo->fTmpTimer = 0.0f;
				pInfo->iRectIndex = 0;
				return pInfo;
			}
		}
	}
	return nullptr;
}

bool myObjectPoolManager::StartEffect(wstring strName, myPoint ptPos, vector<myObjectInfo*>* playPool)
{
	myObjectInfo* infoTemp = GetEffect(strName);
	if (infoTemp == nullptr)
		return false;
	infoTemp->ptPos = ptPos;
	playPool->push_back(infoTemp);
	return true;
}

bool myObjectPoolManager::StartProjectile(wstring strName, myPoint ptPos, myPoint ptDest, vector<myObjectInfo*>* playPool)
{
	myObjectInfo* infoTemp = GetProjectile(strName);
	if (infoTemp == nullptr)
		return false;
	infoTemp->ptPos = ptPos;
	infoTemp->ptDest = ptDest;
	infoTemp->ptDir = myPoint(ptDest - ptPos).Normalize();
	playPool->push_back(infoTemp);
	return true;
}

bool myObjectPoolManager::Init()
{
	return true;
}

bool myObjectPoolManager::Frame()
{
	return true;
}

bool myObjectPoolManager::Render()
{
	return true;
}

bool myObjectPoolManager::Release()
{
	return true;
}

myObjectPoolManager::myObjectPoolManager()
{
}

myObjectPoolManager::~myObjectPoolManager()
{
	for (m_iterPool = m_mapEffectPool.begin();
		m_iterPool != m_mapEffectPool.end();
		m_iterPool++)
	{
		for (myObjectInfo* pInfo : m_iterPool->second)
		{
			SAFE_DELETE(pInfo);
		}
	}
	for (m_iterPool = m_mapProjectilePool.begin();
		m_iterPool != m_mapProjectilePool.end();
		m_iterPool++)
	{
		for (myObjectInfo* pInfo : m_iterPool->second)
		{
			SAFE_DELETE(pInfo);
		}
	}
}
