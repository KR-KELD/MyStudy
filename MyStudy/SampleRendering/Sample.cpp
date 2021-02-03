#include "Sample.h"
GAMERUN;
bool Sample::Init()
{
	//���ؽ� ����Ʈ �����
	m_VertexList.resize(4);
	m_VertexList[0] = { myVertex3(-0.5f, 0.5f, 0.5f), myVertex3(0,0,0) };
	m_VertexList[1] = { myVertex3( 0.5f, 0.5f, 0.5f), myVertex3(0,0,0) };
	m_VertexList[2] = { myVertex3(-0.5f,-0.5f, 0.5f), myVertex3(0,0,0) };
	m_VertexList[3] = { myVertex3( 0.5f,-0.5f, 0.5f), myVertex3(0,0,0) };

	//���� �ɼ� ����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//GPU�� �Ҵ��� ���ؽ��� ũ��X����
	bd.ByteWidth = sizeof(MY_VERTEX) * m_VertexList.size();
	//
	bd.Usage = D3D11_USAGE_DEFAULT;
	//���ؽ� ���� �÷��� ����
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	//
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	//�ý��� �޸��� �����ּ� �Ѱ��ֱ�
	sd.pSysMem = &m_VertexList.at(0);
	//���ؽ� ���� ����
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	//�ε��� ����Ʈ �����
	m_IndexList.resize(6);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 2;
	m_IndexList[4] = 1;
	m_IndexList[5] = 3;

	//���� �ɼ� �ʱ�ȭ
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//�ε��� ���� ������ �ѱ��
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	//�ε��� ���� �÷��� ����
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_IndexList.at(0);
	//�ε��� ���� ����
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	//���̴�,���� ���� ����
	ID3DBlob* pVSObj;
	ID3DBlob* pPSObj;
	ID3DBlob* pErrorMsgs;
	//���Ϸκ��� ���ؽ� ���̴� �Լ��� �����´�
	hr = D3DCompileFromFile(L"vs.txt", NULL, NULL, "VS", "vs_5_0", 0, 0, &pVSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	//������ �Լ��� ���ؽ� ���̴��� ����
	hr = m_pd3dDevice->CreateVertexShader(pVSObj->GetBufferPointer(), pVSObj->GetBufferSize(), NULL, &m_pVertexShader);
	//���Ϸκ��� �ȼ� ���̴� �Լ��� �����´�
	hr = D3DCompileFromFile(L"vs.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, &pPSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	//������ �Լ��� �ȼ� ���̴� ����
	hr = m_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL, &m_pPixelShader);

	//���̴� �Լ��� ������ ������ �ɼ� ����
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	//��ǲ ���̾ƿ� ����
	hr = m_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		pVSObj->GetBufferPointer(),
		pVSObj->GetBufferSize(),
		&m_pInputLayout
	);
	return true;
}

bool Sample::Render()
{
	//�ڸ� ����
	UINT iStride = sizeof(MY_VERTEX);
	//���� ��ġ
	UINT iOffset = 0;
	//IA ���ؽ� ���� ����
	m_pd3dContext->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &iStride, &iOffset);
	//IA �ε��� ���� ����
	m_pd3dContext->IASetIndexBuffer(m_pIndexBuffer,
		DXGI_FORMAT_R32_UINT, 0);
	//IA ����
	m_pd3dContext->IASetInputLayout(m_pInputLayout);
	//���ؽ� ���̴� ����
	m_pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	//�ȼ� ���̴� ����
	m_pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	//IA�� �׷��� Ÿ�� ����
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//m_pd3dContext->Draw(m_VertexList.size(), 0);
	//�׸���
	m_pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}

bool Sample::Release()
{
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	return true;
}

void Sample::CompilerCheck(ID3DBlob * pErrorMsgs)
{
	//���ӿ��� �޽��� ��������
	C_STR szMsg = (char*)pErrorMsgs->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}