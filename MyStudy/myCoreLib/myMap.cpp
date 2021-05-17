#include "myMap.h"
DECLARE_COMPONENT(myMap);
bool    myMap::CreateVertexData()
{
	// 정점 가로 및 세수 = 2N승+1
	m_VertexList.resize(m_iNumVertices);
	float fHalfCols = (m_iNumCols - 1) / 2.0f;
	float fHalfRows = (m_iNumRows - 1) / 2.0f;
	float fOffsetU = 1.0f / (m_iNumCols - 1);
	float fOffsetV = 1.0f / (m_iNumRows - 1);
	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int iIndex = iRow * m_iNumCols + iCol;
			m_VertexList[iIndex].p.x = (iCol - fHalfCols)*m_fCellDistance;
			m_VertexList[iIndex].p.y = GetFaceHeight(iIndex);
			m_VertexList[iIndex].p.z = (iRow - fHalfRows)*m_fCellDistance*-1.f;
			m_VertexList[iIndex].t.x = iCol * fOffsetU;
			m_VertexList[iIndex].t.y = iRow * fOffsetV;

			//일단 0으로 초기화
			m_VertexList[iIndex].n = { 0,1,0 };
			m_VertexList[iIndex].c = { 1,1,1,1 };
		}
	}
	m_iNumVertex = m_VertexList.size();
	m_iVertexSize = sizeof(PNCT_VERTEX);
	return true;
}
bool    myMap::CreateIndexData()
{
	m_IndexList.resize(m_iNumFaces * 3);
	int iIndex = 0;
	for (int iRow = 0; iRow < m_iNumCellRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCellCols; iCol++)
		{
			int iNextRow = iRow + 1;
			int iNextCol = iCol + 1;
			m_IndexList[iIndex + 0] = iRow * m_iNumCols + iCol;
			m_IndexList[iIndex + 1] = iRow * m_iNumCols + iNextCol;
			m_IndexList[iIndex + 2] = iNextRow * m_iNumCols + iCol;
			m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex + 5] = iNextRow * m_iNumCols + iNextCol;
			iIndex += 6;
		}
	}
	m_iNumIndex = m_IndexList.size();
	m_iNumFaces = m_iNumIndex / 3;
	return true;
}

void myMap::Update(ID3D11DeviceContext * pd3dContext)
{
	myGraphics::Update(pd3dContext);

	D3D11_MAPPED_SUBRESOURCE mr;
	HRESULT hr = pd3dContext->Map(m_pMapCB.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mr);
	if (SUCCEEDED(hr))
	{
		myMapCB* pData = (myMapCB*)mr.pData;

		pData->MapData[0] = m_cbMapData.MapData[0];
		pData->MapData[1] = m_cbMapData.MapData[1];
		pData->MapData[2] = m_cbMapData.MapData[2];
		pData->MapData[3] = 1.0f;
		pd3dContext->Unmap(m_pMapCB.Get(), 0);
	}

}

bool myMap::PreRender(ID3D11DeviceContext * pd3dContext)
{
	pd3dContext->VSSetConstantBuffers(1, 1, m_pMapCB.GetAddressOf());
	return true;
}

bool myMap::Frame()
{
	return true;
}

bool myMap::Draw(ID3D11DeviceContext*	pd3dContext)
{
	pd3dContext->DrawIndexed(m_iNumFaces * 3, 0, 0);
	return true;
}

myMap::myMap()
{

}

myMap::~myMap()
{
}

void myMap::SetMapCBData(int iNumCell, int iNumTile, int iCellSize, int iSpaceDivision)
{
	m_cbMapData.MapData[0] = iNumTile;
	m_cbMapData.MapData[1] = iNumCell;
	m_cbMapData.MapData[2] = iCellSize;
	m_cbMapData.MapData[3] = iSpaceDivision;
}

void myMap::CreateTexMatrix(int iNumCell, int iNumTile, int iCellSize)
{
	matTexCoord = Matrix::Identity;
	matTexCoord._11 = 1.0f / iCellSize;
	matTexCoord._22 = 0.0f;
	matTexCoord._33 = 0.0f;
	matTexCoord._32 = 1.0f / iCellSize;
	matTexCoord._41 = iNumCell * iNumTile / 2.0f;
	matTexCoord._42 = iNumCell * iNumTile / 2.0f;
	matTexCoord *= 1.0f / iNumCell * iNumTile;
}

bool myMap::CreateMap(myMapDesc  desc)
{
	m_MapDesc = desc;
	m_iNumRows = desc.iNumRows;
	m_iNumCols = desc.iNumCols;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumCellRows = m_iNumRows - 1;
	m_iNumVertices = m_iNumCols * m_iNumRows;
	m_iNumFaces = m_iNumCellCols * m_iNumCellRows * 2;
	m_fCellDistance = desc.fCellDistance;

	Create(
		desc.szVS,
		desc.szPS,
		desc.szTexFile);

	GetVertexNormal();

	g_pImmediateContext->UpdateSubresource(
		m_pVertexBuffer.Get(), 0, NULL, &m_VertexList.at(0), 0, 0);
	return true;
}

bool myMap::Create(T_STR szVS, T_STR szPS, T_STR szTex)
{
	myGraphics::Create(szVS, szPS, szTex);

	D3D11_BUFFER_DESC vbdesc;
	ZeroMemory(&vbdesc, sizeof(D3D11_BUFFER_DESC));
	vbdesc.ByteWidth = sizeof(myMapCB);
	vbdesc.Usage = D3D11_USAGE_DYNAMIC;
	vbdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_cbMapData;
	HRESULT hr = g_pd3dDevice->CreateBuffer(&vbdesc, NULL, m_pMapCB.GetAddressOf());
	return true;
}

//bool myMap::CalNormal()
//{
//	int iIndex = 0;
//	for (int iFace = 0; iFace < m_IndexList.size() / 3; iFace++)
//	{
//		Vector3 n;
//		Vector3 e0 = m_VertexList[m_IndexList[iIndex + 1]].p - m_VertexList[m_IndexList[iIndex + 0]].p;
//		Vector3 e1 = m_VertexList[m_IndexList[iIndex + 2]].p - m_VertexList[m_IndexList[iIndex + 0]].p;
//		n = e0.Cross(e1);
//		n.Normalize();
//		m_VertexList[m_IndexList[iIndex + 0]].n += n;
//		m_VertexList[m_IndexList[iIndex + 1]].n += n;
//		m_VertexList[m_IndexList[iIndex + 2]].n += n;
//		iIndex += 3;
//	}
//
//	for (int i = 0; i < m_VertexList.size(); i++)
//	{
//		m_VertexList[i].n.Normalize();
//	}
//	return true;
//}

float myMap::Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - (fStart*fTangent) + (fEnd*fTangent);
}

float myMap::GetHeight(float fPosX, float fPosZ)
{
	// fPosX/fPosZ의 위치에 해당하는 높이맵셀을 찾는다.
// m_iNumCols와m_iNumRows은 가로/세로의 실제 크기값임.
	float fCellX = (float)(m_iNumCellCols*m_fCellDistance / 2.0f + fPosX);
	float fCellZ = (float)(m_iNumCellRows*m_fCellDistance / 2.0f - fPosZ);

	// 셀의 크기로 나누어 0~1 단위의 값으로 바꾸어 높이맵 배열에 접근한다.
	fCellX /= (float)m_fCellDistance;
	fCellZ /= (float)m_fCellDistance;

	// fCellX, fCellZ 값보다 작거나 같은 최대 정수( 소수부분을 잘라낸다.)
	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);

	// 높이맵 범위를 벗어나면 강제로 초기화 한다.
	if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
	if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
	if ((float)(m_iNumCols - 2) < fVertexCol)	fVertexCol = (float)(m_iNumCols - 2);
	if ((float)(m_iNumRows - 2) < fVertexRow)	fVertexRow = (float)(m_iNumRows - 2);

	// 계산된 셀의 플랜을 구성하는 4개 정점의 높이값을 찾는다. 
	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D
	float A = GetHeightMap((int)fVertexRow, (int)fVertexCol);
	float B = GetHeightMap((int)fVertexRow, (int)fVertexCol + 1);
	float C = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol);
	float D = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol + 1);

	// A정점의 위치에서 떨어진 값(변위값)을 계산한다. 0 ~ 1.0f
	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;
	// 보간작업를 위한 기준 페잇스를 찾는다. 
	float fHeight = 0.0f;
	// 윗페이스를 기준으로 보간한다.
	// fDeltaZ + fDeltaX < 1.0f
	if (fDeltaZ < (1.0f - fDeltaX))  //ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C	
						  // 두 정점의 높이값의 차이를 비교하여 델타X의 값에 따라 보간값을 찾는다.		
		fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	// 아래페이스를 기준으로 보간한다.
	else // DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B
						  // 두 정점의 높이값의 차이를 비교하여 델타Z의 값에 따라 보간값을 찾는다.		
		fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
	}
	return fHeight;
}

float myMap::GetFaceHeight(UINT index)
{
	return 0.0f;
}

float myMap::GetHeightMap(int row, int col)
{
	return m_VertexList[row * m_iNumRows + col].p.y;
}

//처음 1번 호출로 노말값 생성
void myMap::GetVertexNormal()
{
	InitFaceNormals();
	GenNormalLookupTable();
	CalcPerVertexNormalsFastLookup();
}
Vector3 myMap::ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2)
{
	Vector3 normal;
	Vector3 v0 = m_VertexList[i1].p - m_VertexList[i0].p;
	Vector3 v1 = m_VertexList[i2].p - m_VertexList[i0].p;
	normal = v0.Cross(v1);
	normal.Normalize();
	return normal;
}
void myMap::CalcFaceNormals()
{
	int index = 0;
	for (int i = 0; i < m_iNumFaces * 3; i += 3)
	{
		m_FaceNormals[index++] = ComputeFaceNormal(
			m_IndexList[i],
			m_IndexList[i + 1],
			m_IndexList[i + 2]);
	}
}
void myMap::InitFaceNormals()
{
	m_FaceNormals.resize(m_iNumFaces);
	for (int i = 0; i < m_iNumFaces; i++)
	{
		m_FaceNormals[i] = Vector3::Zero;
	}
}
void myMap::GenNormalLookupTable()
{
	m_LookupTabel.resize(m_iNumVertices);
	for (int iFace = 0; iFace < m_iNumFaces; iFace++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				int id = m_IndexList[iFace * 3 + j];
				if (m_LookupTabel[id].index[k] == -1)
				{
					m_LookupTabel[id].index[k] = iFace;
					break;
				}
			}
		}
	}
}
//생성된 테이블을 기반으로 노말값 갱신
void myMap::CalcPerVertexNormalsFastLookup()
{
	CalcFaceNormals();
	for (int i = 0; i < m_iNumVertices; i++)
	{
		Vector3 avgNormal = { 0,0,0 };
		for (int f = 0; f < 6; f++)
		{
			int index = m_LookupTabel[i].index[f];
			if (index != -1)
			{
				avgNormal += m_FaceNormals[index];
			}
			else
			{
				break;
			}
		}
		m_VertexList[i].n = avgNormal;
		m_VertexList[i].n.Normalize();
	}
}