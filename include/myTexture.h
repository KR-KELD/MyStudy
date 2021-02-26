#pragma once
#include "myGameObject.h"
#include "dxtk/WICTextureLoader.h"
#include "dxtk/DDSTextureLoader.h"
class myTexture : myComponent
{
public:
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV;
public:
	bool  Init() override;
	bool  Frame() override;
	bool  Render() override;
	bool  Release() override;
public:
	bool  Load(T_STR strTex);
public:
	myTexture();
	~myTexture();
};

