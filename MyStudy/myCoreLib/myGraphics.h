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
	//�⺻ ������
	virtual bool	Render(ID3D11DeviceContext*	pd3dContext);
	virtual bool	PostRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool	Release();
	//���������� ����
	virtual bool	SettingPipeLine(ID3D11DeviceContext*	pd3dContext);
	//����޽� ������
	virtual bool	MultiDraw(ID3D11DeviceContext*	pd3dContext);
	virtual bool	Draw(ID3D11DeviceContext*	pd3dContext);
	//���ؽ� ������ ����
	virtual bool    CreateVertexData(Vector3 vCenter, float fRange);
	virtual bool    CreateVertexData();
	//�ε��� ������ ����
	virtual bool    CreateIndexData();
	//���ؽ� ���� ����
	virtual bool    CreateVertexBuffer();
	//�ε��� ���� ����
	virtual bool    CreateIndexBuffer();
	//�ܽ���Ʈ ���� ����
	virtual bool    CreateConstantBuffer();
	//���̴� �ε�
	virtual bool	LoadShader(T_STR szVS, T_STR szPS);
	//���̾ƿ� ���� �� ����
	virtual bool	CreateInputLayout();
	//�ؽ��� �ε�
	virtual bool	LoadTexture(T_STR szTex);
	//�� ������ ����
	virtual bool	Create(T_STR szVS, T_STR szPS, T_STR	szTex);
	//���̴� ������
	void			CompilerCheck(ID3DBlob* pErrorMsgs);
public:
	myGraphics();
	virtual ~myGraphics();
};

