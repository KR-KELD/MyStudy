#pragma once
#include "myGraphicsStruct.h"
#include "myBaseObject.h"

#pragma region staticFuncArea

//////////////////////////// 아래의 경고가 있을 경우 사용한다.
//// 이와 같은 경고는 이미 쉐이더 파이프라인에 할당(리소스 및 상태값들이)되어 사용 중일 경우에 발생한다.
////D3D11 WARNING : ID3D11DeviceContext::SOSetTargets : Resource being set to SO buffer slot 0 is still bound on input![STATE_SETTING WARNING #10: DEVICE_SOSETTARGETS_HAZARD]
////D3D11 WARNING : ID3D11DeviceContext::SOSetTargets : Forcing Vertex Buffer slot 0 to NULL.[STATE_SETTING WARNING #1: DEVICE_IASETVERTEXBUFFERS_HAZARD]
bool AscendingTriangle(myTriangle& a, myTriangle& b);

bool CreateVnIFromTri(vector<PNCT_VERTEX>& vertexList, vector<DWORD>& indexList, vector<myTriangle>& triList);
#pragma endregion

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
	//std::vector<PNCT2_VERTEX>	m_VertexList;
	std::vector<PNCT_VERTEX>	m_VertexList;
	//std::vector<PNCTIW_VERTEX>	m_VertexListIW;
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
	myTexture*					m_pTexture;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual void    Update(ID3D11DeviceContext*	pd3dContext);
	virtual bool	PreRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool	Render(ID3D11DeviceContext*	pd3dContext);
	virtual bool	PostRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool	Release();
	virtual bool	SettingPipeLine(ID3D11DeviceContext*	pd3dContext);
	//임시
	virtual bool	MultiDraw(ID3D11DeviceContext*	pd3dContext);
	virtual bool	Draw(ID3D11DeviceContext*	pd3dContext);
	virtual bool    CreateVertexData(Vector3 vCenter, float fRange);
	virtual bool    CreateVertexData();

	virtual bool    CreateIndexData();
	virtual bool    CreateVertexBuffer();
	virtual bool    CreateIndexBuffer();
	virtual bool    CreateConstantBuffer();
	virtual bool	LoadShader(T_STR szVS, T_STR szPS);
	virtual bool	CreateInputLayout();
	virtual bool	LoadTexture(T_STR szTex);
	virtual bool	Create(T_STR szVS, T_STR szPS, T_STR	szTex);
	void			CompilerCheck(ID3DBlob* pErrorMsgs);
public:
	myGraphics();
	virtual ~myGraphics();
};

