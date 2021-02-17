#pragma once
#include "myTexture.h"
class myTextureManager : public SingleTon<myTextureManager>
{
public:
	friend  class SingleTon<myTextureManager>;
private:
	std::map<wstring, myTexture*>				m_List;
	std::map<wstring, myTexture*>::iterator		m_iter;
	std::wstring								m_szDefaultPath;
public:
	myTexture*		Load(ID3D11Device* pd3dDevice, const TCHAR* filename);
	myTexture*		GetPtr(wstring filename);
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
private:
	myTextureManager();
public:
	~myTextureManager();
};
#define g_TextureMgr myTextureManager::GetInstance()
