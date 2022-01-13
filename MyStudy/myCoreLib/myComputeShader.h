#pragma once
#include "myGameObject.h"
#include "myStaticFunc.h"

class myComputeShader : public myComponent
{
public:
	ComPtr<ID3D11ComputeShader>			m_pCS;
	// 구조화버퍼
	ComPtr<ID3D11Buffer>				m_pStBuff;
	ComPtr<ID3D11ShaderResourceView>	m_pStBuffSRV;

	ComPtr<ID3D11Texture2D>				m_pTexture;
	ComPtr<ID3D11ShaderResourceView>	m_pSRV;
	ComPtr<ID3D11UnorderedAccessView>	m_pUAV;
	ComPtr<ID3D11Texture2D>				m_pTextureCopy;
	ComPtr<ID3D11ShaderResourceView>	m_pSRVCopy;
public:

	HRESULT CreateComputeShader(ID3D11Device* pDevice,
		LPCWSTR srcfile,
		LPCSTR funcName);
	void RunComputeShader(ID3D11DeviceContext* pContext, 
		ID3D11ShaderResourceView** ppSRV,
		int iNumSRV, UINT uX, UINT uY, UINT uZ);
	HRESULT CreateStructuredBuffer(ID3D11Device* pDevice, UINT uElementSize,
		UINT uCount, void* pInitData);
	HRESULT CreateBufferSRV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer);
	HRESULT CreateTextureUAV(ID3D11Device* pDevice, void* pInitData, int iWidth, int iHeight);

	bool CreateFromTexFile(const TCHAR* szFileName);
};

