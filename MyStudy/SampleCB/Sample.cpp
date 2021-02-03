#include "Sample.h"
GAMERUN;
bool Sample::Init()
{
	//D3D11_RASTERIZER_DESC rdesc;
	//ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	//rdesc.FillMode = D3D11_FILL_WIREFRAME;
	//rdesc.CullMode = D3D11_CULL_BACK;
	//HRESULT hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRS);
	//if (FAILED(hr))
	//{
	//	return false;
	//}

	//버텍스 리스트 만들기
	m_VertexList.resize(4);
	m_VertexList[0] = { myVertex3(-0.5f, 0.5f, 0.5f), myVertex3(0,0,0) };
	m_VertexList[1] = { myVertex3( 0.5f, 0.5f, 0.5f), myVertex3(0,0,0) };
	m_VertexList[2] = { myVertex3(-0.5f,-0.5f, 0.5f), myVertex3(0,0,0) };
	m_VertexList[3] = { myVertex3( 0.5f,-0.5f, 0.5f), myVertex3(0,0,0) };


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//GPU에 할당할 버텍스의 크기X갯수
	bd.ByteWidth = sizeof(myDataCB);
	//접근 방법
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//버텍스 버퍼 플래그 설정
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	//
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	//시스템 메모리의 시작주소 넘겨주기
	sd.pSysMem = &m_cbData;
	//버텍스 버퍼 생성
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pConstantBuffer);


	//버퍼 옵션 설정
	//D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//GPU에 할당할 버텍스의 크기X갯수
	bd.ByteWidth = sizeof(MY_VERTEX) * m_VertexList.size();
	//
	bd.Usage = D3D11_USAGE_DEFAULT;
	//버텍스 버퍼 플래그 설정
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	//
	//D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	//시스템 메모리의 시작주소 넘겨주기
	sd.pSysMem = &m_VertexList.at(0);
	//버텍스 버퍼 생성
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	//인덱스 리스트 만들기
	m_IndexList.resize(6);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 2;
	m_IndexList[4] = 1;
	m_IndexList[5] = 3;

	//버퍼 옵션 초기화
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//인덱스 버퍼 사이즈 넘기기
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	//인덱스 버퍼 플래그 설정
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_IndexList.at(0);
	//인덱스 버퍼 생성
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	//섀이더,에러 블롭 생성
	ID3DBlob* pVSObj;
	ID3DBlob* pPSObj;
	ID3DBlob* pErrorMsgs;
	//파일로부터 버텍스 섀이더 함수를 가져온다
	hr = D3DCompileFromFile(L"vs.txt", NULL, NULL, "VS", "vs_5_0", 0, 0, &pVSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	//가져온 함수로 버텍스 섀이더를 생성
	hr = m_pd3dDevice->CreateVertexShader(pVSObj->GetBufferPointer(), pVSObj->GetBufferSize(), NULL, &m_pVertexShader);
	//파일로부터 픽셀 섀이더 함수를 가져온다
	hr = D3DCompileFromFile(L"ps.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, &pPSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	//가져온 함수로 픽셀 섀이더 생성
	hr = m_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL, &m_pPixelShader);

	//섀이더 함수에 전달할 변수의 옵션 설정
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	//인풋 레이아웃 생성
	hr = m_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		pVSObj->GetBufferPointer(),
		pVSObj->GetBufferSize(),
		&m_pInputLayout
	);
	return true;
}

bool Sample::Frame()
{
	D3D11_MAPPED_SUBRESOURCE mr;

	HRESULT hr = m_pd3dContext->
		Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	if (SUCCEEDED(hr))
	{
		myDataCB* pData = (myDataCB*)mr.pData;
		pData->vColor[0] = cosf(g_fGameTimer);
		pData->vColor[1] = sinf(g_fGameTimer);
		pData->vColor[2] = 1.0f - cosf(g_fGameTimer);
		pData->vColor[3] = 1;
		pData->vTime[0] = cosf(g_fGameTimer) * 0.5f + 0.5f;
		pData->vTime[1] = g_fGameTimer;
		m_pd3dContext->Unmap(m_pConstantBuffer,0);
	}
	return true;
}

bool Sample::Render()
{
	//자를 단위
	UINT iStride = sizeof(MY_VERTEX);
	//시작 위치
	UINT iOffset = 0;
	//IA 버텍스 버퍼 설정
	m_pd3dContext->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &iStride, &iOffset);
	//IA 인덱스 버퍼 설정
	m_pd3dContext->IASetIndexBuffer(m_pIndexBuffer,
		DXGI_FORMAT_R32_UINT, 0);
	//IA 세팅
	m_pd3dContext->IASetInputLayout(m_pInputLayout);
	//콘스턴트 버퍼 세팅
	m_pd3dContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pd3dContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//버텍스 섀이더 세팅
	m_pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	//픽셀 섀이더 세팅
	m_pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	//IA에 그려줄 타입 설정
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//m_pd3dContext->Draw(m_VertexList.size(), 0);
	//m_pd3dContext->RSSetState(m_pRS);
	//그리기
	m_pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}

bool Sample::Release()
{
	m_pConstantBuffer->Release();
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	return true;
}

void Sample::CompilerCheck(ID3DBlob * pErrorMsgs)
{
	//블롭에서 메시지 가져오기
	C_STR szMsg = (char*)pErrorMsgs->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}
