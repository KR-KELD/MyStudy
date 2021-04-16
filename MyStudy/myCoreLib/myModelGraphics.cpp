#include "myModelGraphics.h"
DECLARE_COMPONENT(myModelGraphics);

void myModelGraphics::GetAnimSRT(int iAnimIndex, float fTick, Vector3& vScale, Quaternion& qRot, Vector3& vTrans)
{
	m_iTrackIndex = GetTrackIndex(iAnimIndex, fTick);
	if (m_iTrackIndex >= 0)
	{
		int iStart = m_AnimTrackList[iAnimIndex][m_iTrackIndex - 1].iTick;
		int iEnd = m_AnimTrackList[iAnimIndex][m_iTrackIndex].iTick;
		int iStepTick = iEnd - iStart;
		float t = (fTick - iStart) / iStepTick;
		Vector3 vStart, vEnd;
		vStart = m_AnimTrackList[iAnimIndex][m_iTrackIndex - 1].vTrans;
		vEnd = m_AnimTrackList[iAnimIndex][m_iTrackIndex].vTrans;
		vTrans = Vector3::Lerp(vStart, vEnd, t);
		vStart = m_AnimTrackList[iAnimIndex][m_iTrackIndex - 1].vScale;
		vEnd = m_AnimTrackList[iAnimIndex][m_iTrackIndex].vScale;
		vScale = Vector3::Lerp(vStart, vEnd, t);
		Quaternion q1, q2;
		q1 = m_AnimTrackList[iAnimIndex][m_iTrackIndex - 1].qRot;
		q2 = m_AnimTrackList[iAnimIndex][m_iTrackIndex].qRot;
		qRot = Quaternion::Slerp(q1, q2, t);
	}
	if (m_iTrackIndex == -1)
	{
		vTrans = m_AnimTrackList[iAnimIndex][0].vTrans;
		vScale = m_AnimTrackList[iAnimIndex][0].vScale;
		qRot = m_AnimTrackList[iAnimIndex][0].qRot;
	}
}

int myModelGraphics::GetTrackIndex(int iAnimIndex, float fTick)
{
	//¾Ö´Ô¾À ÀÔ·Â°ª¿À·ù
	if (iAnimIndex < 0 || iAnimIndex >= m_AnimTrackList.size()) return -2;
	for (int i = 1; i < m_AnimTrackList[iAnimIndex].size(); i++)
	{
		if (fTick <= m_AnimTrackList[iAnimIndex][i].iTick) return i;

	}
	return -1;
}

bool myModelGraphics::CreateInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	hr = g_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		m_pVSObj->GetBufferPointer(),
		m_pVSObj->GetBufferSize(),
		&m_pInputLayout
	);
	if (FAILED(hr)) return false;
	return true;
}

bool myModelGraphics::MultiDraw(ID3D11DeviceContext*	pd3dContext)
{
	pd3dContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pd3dContext->IASetInputLayout(m_pInputLayout.Get());
	pd3dContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pd3dContext->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pd3dContext->VSSetShader(m_pVertexShader.Get(), NULL, 0);
	pd3dContext->PSSetShader(m_pPixelShader.Get(), NULL, 0);
	pd3dContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);

	for (int iSub = 0; iSub < m_SubMeshList.size(); iSub++)
	{
		mySubMesh* pMesh = &m_SubMeshList[iSub];
		if (pMesh->m_TriangleList.size() <= 0) continue;

		ID3D11Buffer* vb[2] = { pMesh->m_pVertexBuffer.Get(), pMesh->m_pVertexBufferIW.Get() };
		UINT iStride[2] = { sizeof(PNCT_VERTEX) ,sizeof(IW_VERTEX) };
		UINT iOffset[2] = { 0,0 };
		pd3dContext->IASetVertexBuffers(0, 2, vb, iStride, iOffset);

		if (m_SubMeshList[iSub].m_pTexture != nullptr)
		{
			pd3dContext->PSSetShaderResources(0, 1,
				m_SubMeshList[iSub].m_pTexture->m_pTextureSRV.GetAddressOf());
		}
		if (m_pIndexBuffer.Get() == nullptr)
		{
			pd3dContext->Draw(m_SubMeshList[iSub].m_VertexList.size(), 0);
		}
		else
		{
			pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);
		}
	}
	return true;
}

myModelGraphics::myModelGraphics()
{
	m_iNextTrackIndex = 0;
	m_iTrackIndex = 0;
}

myModelGraphics::~myModelGraphics()
{
}
