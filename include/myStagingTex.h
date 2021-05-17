#pragma once
#include "myGameObject.h"
class myStagingTex : public myComponent
{
public:
	ComPtr<ID3D11Texture2D>			 m_pStaging;
	ComPtr<ID3D11Texture2D>			 m_pTexture;
	ComPtr<ID3D11ShaderResourceView> m_pSRV;

public:
	bool		CopyCreate(ID3D11Texture2D* pTex = nullptr);
	bool		LoadTexture(const TCHAR* szFileName);
	bool		Create(int iTexSize = 1024);
};

