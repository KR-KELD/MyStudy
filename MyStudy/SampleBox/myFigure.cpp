#include "myFigure.h"

bool myBaseFigure::Init(ID3D11Device* pDevice)
{
	HRESULT hr = NULL;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));

	//���� �ɼ� ����
	//D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//GPU�� �Ҵ��� ���ؽ��� ũ��X����
	bd.ByteWidth = sizeof(MY_VERTEX) * m_VertexList.size();
	//
	bd.Usage = D3D11_USAGE_DEFAULT;
	//���ؽ� ���� �÷��� ����
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	//D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	//�ý��� �޸��� �����ּ� �Ѱ��ֱ�
	sd.pSysMem = &m_VertexList.at(0);
	//���ؽ� ���� ����
	hr = pDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	//�ε��� ���� - �����Ұ���
	//�ε��� ����Ʈ �����

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
	//�ڸ� ����
	UINT iStride = sizeof(MY_VERTEX);
	//���� ��ġ
	UINT iOffset = 0;
	//IA ���ؽ� ���� ����
	pDeviceContext->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &iStride, &iOffset);
	//IA �ε��� ���� ����
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer,
		DXGI_FORMAT_R32_UINT, 0);
	//�׸���
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

bool myBox::Init(ID3D11Device * pDevice)
{
	for (int i = 0; i < 6; i++)
	{
		m_sSquare[i].Init(pDevice);
	}
	return true;
}

bool myBox::Render(ID3D11DeviceContext * pDeviceContext)
{
	for (int i = 0; i < 6; i++)
	{
		m_sSquare[i].Render(pDeviceContext);
	}
	return true;
}

bool myBox::Release()
{
	for (int i = 0; i < 6; i++)
	{
		m_sSquare[i].Release();
	}
	return true;
}
