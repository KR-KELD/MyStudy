#pragma once
#include "myGameObject.h"
class myStagingTex : public myComponent
{
public:
	ComPtr<ID3D11Texture2D>			 m_pStaging;
	ComPtr<ID3D11Texture2D>			 m_pTexture;
	ComPtr<ID3D11ShaderResourceView> m_pSRV;
public:
	D3D11_TEXTURE2D_DESC			 m_texDesc;
public:
	void		SetDesc();
	bool		Create(D3D11_TEXTURE2D_DESC* pDesc = nullptr);
};

