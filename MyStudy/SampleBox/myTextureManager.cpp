#include "myTextureManager.h"

myTexture * myTextureManager::CreateLoad(LONG iWidth, LONG iHeight, const TCHAR * filename, int iBitCount)
{
	return nullptr;
}

myTexture * myTextureManager::Load(const TCHAR * filename)
{
	return nullptr;
}

myTexture * myTextureManager::GetPtr(wstring filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool myTextureManager::Init()
{
	return true;
}

bool myTextureManager::Frame()
{
	return true;
}

bool myTextureManager::Render()
{
	return true;
}

bool myTextureManager::Release()
{
	for (m_iter = m_List.begin();
		m_iter != m_List.end();
		m_iter++)
	{
		delete (*m_iter).second;
	}
	m_List.clear();
	return true;
}

myTextureManager::myTextureManager()
{
	m_szDefaultPath = L"../../data/bitmap/";
}

myTextureManager::~myTextureManager()
{
	Release();
}
