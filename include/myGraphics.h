#pragma once
#include "myGraphicsStruct.h"

#include "myBaseObject.h"

#pragma region staticFuncArea

//
//////////////////////////// 아래의 경고가 있을 경우 사용한다.
//// 이와 같은 경고는 이미 쉐이더 파이프라인에 할당(리소스 및 상태값들이)되어 사용 중일 경우에 발생한다.
////D3D11 WARNING : ID3D11DeviceContext::SOSetTargets : Resource being set to SO buffer slot 0 is still bound on input![STATE_SETTING WARNING #10: DEVICE_SOSETTARGETS_HAZARD]
////D3D11 WARNING : ID3D11DeviceContext::SOSetTargets : Forcing Vertex Buffer slot 0 to NULL.[STATE_SETTING WARNING #1: DEVICE_IASETVERTEXBUFFERS_HAZARD]
void		ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext);

HRESULT	CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
HRESULT D3DX11CompileFromFile(LPCWSTR pSrcFile, CONST D3D_SHADER_MACRO* pDefines, LPD3DINCLUDE pInclude, LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2,
	/*ID3DX11ThreadPump* pPump, */ID3DBlob** ppShader, ID3DBlob** ppErrorMsgs, HRESULT* pHResult);
ID3D11VertexShader* LoadVertexShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
ID3D11PixelShader*  LoadPixelShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, const char *pFuntionName = 0, bool bBinary = false, ID3DBlob** pRetBlob = nullptr);
ID3D11GeometryShader*  LoadGeometryShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
ID3D11HullShader* LoadHullShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
ID3D11DomainShader*  LoadDomainShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
ID3D11ComputeShader*  LoadComputeShaderFile(ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);


ID3D11InputLayout* CreateInputlayout(ID3D11Device*  pd3dDevice, DWORD dwSize, LPCVOID lpData, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements);
ID3D11Buffer* CreateVertexBuffer(ID3D11Device*  pd3dDevice, void *vertices, UINT iNumVertex, UINT iVertexSize, bool bDynamic = false);
ID3D11Buffer* CreateIndexBuffer(ID3D11Device*  pd3dDevice, void *indices, UINT iNumIndex, UINT iSize, bool bDynamic = false);
ID3D11Buffer* CreateConstantBuffer(ID3D11Device*  pd3dDevice, void *data, UINT iNumIndex, UINT iSize, bool bDynamic = false);

ID3D11ShaderResourceView*	CreateShaderResourceView(ID3D11Device* pDevice, const TCHAR* strFilePath);
ID3D11ShaderResourceView*	CreateShaderResourceView(ID3D11Device* pDevice, ID3D11DeviceContext*    pContext, const TCHAR* strFilePath);
ID3D11DepthStencilView* CreateDepthStencilView(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight);

bool AscendingTriangle(myTriangle& a, myTriangle& b);

bool CreateVnIFromTri(vector<PNCT_VERTEX>& vertexList, vector<DWORD>& indexList, vector<myTriangle>& triList);
#pragma endregion

class myGraphics : public myComponent
{
public:
	DEFINE_COMPONENT(myGraphics, myComponent, true);
public:
	//충돌체
	MY_BOX						m_BindingBox;
public:
	ID3DBlob*					m_pVSObj;
	UINT						m_iTopology;
	C_STR						m_szVertexShader;
	C_STR						m_szPixelShader;
public:
	//vector<myAnimTrack>			m_AnimTrackList;
	//int							m_iTick = 0;
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
	void	CompilerCheck(ID3DBlob* pErrorMsgs);
public:
	myGraphics();
	virtual ~myGraphics();
};

