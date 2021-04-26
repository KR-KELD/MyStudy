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
	//애님씬 입력값오류
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
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },

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
	for (int iSub = 0; iSub < m_SubMeshList2.size(); iSub++)
	{
		mySubMesh2* pMesh = &m_SubMeshList2[iSub];
		if (pMesh->m_iFaceCount <= 0) continue;

		ID3D11Buffer* vb[2] = { pMesh->m_pVertexBuffer.Get(), pMesh->m_pVertexBufferIW.Get() };
		//pnct2
		UINT iStride[2] = { m_iVertexSize ,sizeof(IW_VERTEX) };
		UINT iOffset[2] = { 0,0 };
		pd3dContext->IASetVertexBuffers(0, 2, vb, iStride, iOffset);
		pd3dContext->IASetIndexBuffer(pMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		if (pMesh->m_pTexture != nullptr)
		{
			pd3dContext->PSSetShaderResources(0, 1,
				pMesh->m_pTexture->m_pTextureSRV.GetAddressOf());
		}
		if (pMesh->m_pIndexBuffer.Get() == nullptr)
		{
			pd3dContext->Draw(pMesh->m_iNumVertex, 0);
		}
		else
		{
			pd3dContext->DrawIndexed(pMesh->m_iNumIndex, 0, 0);
		}
	}
	return true;
}

myModelGraphics::myModelGraphics()
{
	m_iNextTrackIndex = 0;
	m_iTrackIndex = 0;
	m_iVertexSize = sizeof(PNCT2_VERTEX);
}

myModelGraphics::~myModelGraphics()
{
}


//시계방향으로 돌리면서 각 정점의 탄젠트를 채우기
//간소화하면 노말과 uv만써서 바이노말만 구해도 됨
//012면 012 120 201 순
void myModelGraphics::CreateTangentData(Vector3 *v1, Vector3 *v2, Vector3 *v3,
	Vector2 uv1, Vector2 uv2, Vector2 uv3,
	Vector3 *vTangent, Vector3 *vBiNormal, Vector3 *vNormal)
{
	//삼각형의 엣지 두개
	Vector3 vEdge1 = *v2 - *v1;
	Vector3 vEdge2 = *v3 - *v1;
	//vEdge1.Normalize();
	//vEdge2.Normalize();
	//그 엣지의 uv비율좌표

	Vector2 fUV1 = uv2 - uv1;
	Vector2 fUV2 = uv3 - uv1;
	//fUV1.Normalize();
	//fUV2.Normalize();
	//판별식
	float fDenominator = fUV1.x * fUV2.y - fUV2.x * fUV1.y;
	//분모가 0이면 안되기때문에
	if (fDenominator < 0.0001f && fDenominator > -0.0001f)
	{
		*vTangent = Vector3(1.0f, 0.0f, 0.0f);
		*vBiNormal = Vector3(0.0f, 1.0f, 0.0f);
		*vNormal = Vector3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		//역행렬식
		float fScale1 = 1.0f / fDenominator;

		*vTangent = (vEdge1 * fUV2.y - vEdge2 * fUV1.y) * fScale1;
		*vBiNormal = (vEdge2 * fUV1.x - vEdge1 * fUV2.x) * fScale1;
		*vNormal = vTangent->Cross(*vBiNormal);
	}
}