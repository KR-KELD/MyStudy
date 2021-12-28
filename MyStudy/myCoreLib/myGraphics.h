#pragma once
#include "myGraphicsStruct.h"
#include "myBaseObject.h"

bool AscendingTriangle(myTriangle& a, myTriangle& b);
bool CreateVnIFromTri(vector<PNCT_VERTEX>& vertexList, vector<DWORD>& indexList, vector<myTriangle>& triList);

class myGraphics : public myComponent
{
public:
	DEFINE_COMPONENT(myGraphics, myComponent, true)
public:
	ID3DBlob*					m_pVSObj;
	UINT						m_iTopology;
	C_STR						m_szVertexShader;
	C_STR						m_szPixelShader;
public:
	Vector3						m_vMin;
	Vector3						m_vMax;
	Vector3						m_vCenter;
public:
	UINT						m_iNumVertex;
	UINT						m_iVertexSize;
	UINT						m_iNumIndex;
public:
	myDataCB					m_cbData;
	std::vector<PNCT_VERTEX>	m_VertexList;
	std::vector<DWORD>			m_IndexList;
	std::vector<myTriangle>		m_TriangleList;
	std::vector<wstring>		m_MaterialList;
	std::vector<mySubMesh>		m_SubMeshList;
	ComPtr<ID3D11Buffer>		m_pVertexBuffer;
	ComPtr<ID3D11Buffer>		m_pIndexBuffer;
	ComPtr<ID3D11Buffer>		m_pConstantBuffer;
	ComPtr<ID3D11InputLayout>	m_pInputLayout;
	ComPtr<ID3D11VertexShader>	m_pVertexShader;
	ComPtr<ID3D11PixelShader>	m_pPixelShader;
	ComPtr<ID3D11VertexShader>	m_pShadowVS;
	ComPtr<ID3D11PixelShader>	m_pShadowPS;
	myTexture*					m_pTexture;
	bool						m_isShadowRender = false;

	//std::vector<PNCT2_VERTEX>	m_VertexList;
	//std::vector<PNCTIW_VERTEX> m_VertexListIW;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual void    Update(ID3D11DeviceContext*	pd3dContext);
	virtual bool	PreRender(ID3D11DeviceContext*	pd3dContext);
	//기본 랜더링
	virtual bool	Render(ID3D11DeviceContext*	pd3dContext);
	virtual bool	PostRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool	Release();
	//파이프라인 세팅
	virtual bool	SettingPipeLine(ID3D11DeviceContext*	pd3dContext);
	//서브메시 랜더링
	virtual bool	MultiDraw(ID3D11DeviceContext*	pd3dContext);
	virtual bool	Draw(ID3D11DeviceContext*	pd3dContext);
	//버텍스 데이터 생성
	virtual bool    CreateVertexData(Vector3 vCenter, float fRange);
	virtual bool    CreateVertexData();
	//인덱스 데이터 생성
	virtual bool    CreateIndexData();
	//버텍스 버퍼 생성
	virtual bool    CreateVertexBuffer();
	//인덱스 버퍼 생성
	virtual bool    CreateIndexBuffer();
	//콘스턴트 버퍼 생성
	virtual bool    CreateConstantBuffer();
	//쉐이더 로드
	virtual bool	LoadShader(T_STR szVS, T_STR szPS);
	//레이아웃 설정 및 생성
	virtual bool	CreateInputLayout();
	//텍스쳐 로드
	virtual bool	LoadTexture(T_STR szTex);
	//모델 데이터 생성
	virtual bool	Create(T_STR szVS, T_STR szPS, T_STR	szTex);
	//쉐이더 컴파일
	void			CompilerCheck(ID3DBlob* pErrorMsgs);
public:
	myGraphics();
	virtual ~myGraphics();
};

