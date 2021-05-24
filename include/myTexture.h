#pragma once
#include "myGameObject.h"
class myTexture : myComponent
{
public:
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV;
public:
	bool  Init() override;
	bool  Release() override;
public:
	bool  Load(T_STR strTex);
public:
	myTexture();
	~myTexture();
};

