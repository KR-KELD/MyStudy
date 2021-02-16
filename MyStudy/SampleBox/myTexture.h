#pragma once
#include "myStd.h"
class myTexture
{
public:
	ID3D11ShaderResourceView* m_pTextureSRV;
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
public:
	bool  Load(wstring filename);
public:
	myTexture();
	~myTexture();
};

