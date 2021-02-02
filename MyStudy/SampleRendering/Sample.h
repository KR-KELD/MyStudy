#pragma once
#include "myCore.h"
#include "myVertex.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

struct MY_VERTEX
{
	myVertex3 p;
	myVertex3 n;
	MY_VERTEX() {}
	MY_VERTEX(myVertex3 p, myVertex3 n = { 0,0,0 })
	{
		this->p = p;
		this->n = n;
	}
};

class Sample : public myCore
{
	std::vector<MY_VERTEX>	m_VertexList;
	std::vector<DWORD>		m_IndexList;
	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11Buffer*			m_pIndexBuffer;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
public:
	bool Init() override;
	bool Render() override;
	bool Release() override;
public:
	void CompilerCheck(ID3DBlob* pErrorMsgs);
};

