#include "myFigure.h"

bool myBaseFigure::Init(ID3D11Device* pDevice)
{
	HRESULT hr = NULL;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));

	//버퍼 옵션 설정
	//D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//GPU에 할당할 버텍스의 크기X갯수
	bd.ByteWidth = sizeof(MY_VERTEX) * m_VertexList.size();
	//
	bd.Usage = D3D11_USAGE_DEFAULT;
	//버텍스 버퍼 플래그 설정
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	//D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	//시스템 메모리의 시작주소 넘겨주기
	sd.pSysMem = &m_VertexList.at(0);
	//버텍스 버퍼 생성
	hr = pDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	//인덱스 버퍼 - 공유불가능
	//인덱스 리스트 만들기

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
	hr = pDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool myBaseFigure::Frame()
{
	return true;
}

bool myBaseFigure::Render()
{
	return true;
}

bool myBaseFigure::Render(ID3D11DeviceContext * pDeviceContext)
{
	//자를 단위
	UINT iStride = sizeof(MY_VERTEX);
	//시작 위치
	UINT iOffset = 0;
	//IA 버텍스 버퍼 설정
	pDeviceContext->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &iStride, &iOffset);
	//IA 인덱스 버퍼 설정
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer,
		DXGI_FORMAT_R32_UINT, 0);
	//그리기
	pDeviceContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}

bool myBaseFigure::Release()
{
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	return true;
}

bool mySquare::Release()
{
	return true;
}

bool mySquare::SetSquareRot(myVector3 & vCenter, SquareRot type)
{
	switch (type)
	{
		case mySquare::FRONT:
		{
			m_VertexList[0].p = {-m_fRange + vCenter.x, m_fRange + vCenter.y, -m_fRange + vCenter.z };
			m_VertexList[1].p = { m_fRange + vCenter.x, m_fRange + vCenter.y, -m_fRange + vCenter.z };
			m_VertexList[2].p = {-m_fRange + vCenter.x,-m_fRange + vCenter.y, -m_fRange + vCenter.z };
			m_VertexList[3].p = { m_fRange + vCenter.x,-m_fRange + vCenter.y, -m_fRange + vCenter.z };
		}
		break;
		case mySquare::BACK:
		{
			m_VertexList[0].p = { m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z };
			m_VertexList[1].p = {-m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z };
			m_VertexList[2].p = { m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z };
			m_VertexList[3].p = {-m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z };
		}
		break;
		case mySquare::LEFT:
		{
			m_VertexList[0].p = {-m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z };
			m_VertexList[1].p = {-m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z };
			m_VertexList[2].p = {-m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z };
			m_VertexList[3].p = {-m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z };
		}
		break;
		case mySquare::RIGHT:
		{
			m_VertexList[0].p = { m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z };
			m_VertexList[1].p = { m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z };
			m_VertexList[2].p = { m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z };
			m_VertexList[3].p = { m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z };
		}
		break;
		case mySquare::UP:
		{
			m_VertexList[0].p = {-m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z };
			m_VertexList[1].p = { m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z };
			m_VertexList[2].p = {-m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z };
			m_VertexList[3].p = { m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z };
		}
		break;
		case mySquare::DOWN:
		{
			m_VertexList[0].p = {-m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z };
			m_VertexList[1].p = { m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z };
			m_VertexList[2].p = {-m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z };
			m_VertexList[3].p = { m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z };
		}
		break;
		default:
		break;
	}
	return true;
}

bool myBox::SetBox(myVector3 & vCenter)
{

	m_VertexList[0]   = { myVector3(-m_fRange + vCenter.x, m_fRange + vCenter.y, -m_fRange + vCenter.z ),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(0,0)};
	m_VertexList[1]   = { myVector3( m_fRange + vCenter.x, m_fRange + vCenter.y, -m_fRange + vCenter.z ),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(1,0)};
	m_VertexList[2]   = { myVector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y, -m_fRange + vCenter.z ),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(0,1)};
	m_VertexList[3]   = { myVector3( m_fRange + vCenter.x,-m_fRange + vCenter.y, -m_fRange + vCenter.z ),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(1,1)};

	m_VertexList[4]   = { myVector3( m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(0,0) };
	m_VertexList[5] = { myVector3(-m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(1,0) };
	m_VertexList[6] = { myVector3(m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(0,1) };
	m_VertexList[7] = { myVector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(1,1) };

	m_VertexList[8]   = { myVector3(-m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(0,0) };
	m_VertexList[9] = { myVector3(-m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(1,0) };
	m_VertexList[10] = { myVector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(0,1) };
	m_VertexList[11] = { myVector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(1,1) };

	m_VertexList[12]  = { myVector3(m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(0,0) };
	m_VertexList[13] = { myVector3(m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(1,0) };
	m_VertexList[14] = { myVector3(m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(0,1) };
	m_VertexList[15] = { myVector3(m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(1,1) };

	m_VertexList[16]  = { myVector3(-m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(0,0) };
	m_VertexList[17] = { myVector3(m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(1,0) };
	m_VertexList[18] = { myVector3(-m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(0,1) };
	m_VertexList[19] = { myVector3(m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(1,1) };

	m_VertexList[20]  = { myVector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(0,0) };
	m_VertexList[21] = { myVector3(m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(1,0) };
	m_VertexList[22] = { myVector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						myVector3(0,0,0), myVector4(1,1,1,1), myVector2(0,1) };
	m_VertexList[23] = { myVector3(m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
				  myVector3(0, 0, 0), myVector4(1, 1, 1, 1), myVector2(1,1) };

	return true;
}
