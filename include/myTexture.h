#pragma once
#include "myGameObject.h"
#include "dxtk/WICTextureLoader.h"
class myTexture : myComponent
{
public:
	ID3D11ShaderResourceView* m_pTextureSRV;
public:
	bool  Init() override;
	bool  Frame() override;
	bool  Render() override;
	bool  Release() override;
public:
	bool  Load(ID3D11Device* pd3dDevice, T_STR strTex);
public:
	myTexture();
	~myTexture();
};

