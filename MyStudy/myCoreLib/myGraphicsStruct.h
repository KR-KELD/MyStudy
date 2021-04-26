#pragma once
#include "myGameObject.h"
#include "myTextureManager.h"

struct P_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	P_VERTEX() {}
	P_VERTEX(
		Vector3 p,
		Vector3 n,
		Vector4 c,
		Vector2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};

struct PNCT_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	bool operator == (const PNCT_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(
		Vector3 vp,
		Vector3 vn,
		Vector4 vc,
		Vector2 vt)
	{
		p = vp, n = vn, c = vc, t = vt;
	}
};

struct PNCT2_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	Vector3 tangent;
	bool operator == (const PNCT2_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t && tangent == Vertex.tangent)
		{
			return true;
		}
		return  false;
	}
	PNCT2_VERTEX() {}
	PNCT2_VERTEX(
		Vector3 vp,
		Vector3 vn,
		Vector4 vc,
		Vector2 vt,
		Vector3 vTangent)
	{
		p = vp, n = vn, c = vc, t = vt, tangent = vTangent;
	}
};

struct IW_VERTEX
{
	float i1[4];
	float w1[4];
	IW_VERTEX()
	{
		i1[0] = i1[1] = i1[2] = i1[3] = 0.0f;
		w1[0] = w1[1] = w1[2] = w1[3] = 0.0f;
	}
};

struct PNCTIW_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	Vector4 i;
	Vector4	w;
	bool operator == (const PNCTIW_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t
			&& i == Vertex.i && w == Vertex.w)
		{
			return true;
		}
		return  false;
	}
	PNCTIW_VERTEX() {}
	PNCTIW_VERTEX(
		Vector3 vp,
		Vector3 vn,
		Vector4 vc,
		Vector2 vt,
		Vector4 vi,
		Vector4 vw)
	{
		p = vp, n = vn, c = vc, t = vt, i = vi, w = vw;
	}
};

struct myTriangle
{
	//pnct2로 변환
	PNCT_VERTEX vVertex[3];
	IW_VERTEX vVertexIW[3];
	Vector3		vNormal;
	int			iSubIndex;
	myTriangle(int iIndex) : iSubIndex(iIndex) {}
	myTriangle() : iSubIndex(-1) {}
};

struct myTriangle2
{
	PNCT2_VERTEX vVertex[3];
	IW_VERTEX	 vVertexIW[3];
	Vector3		 vNormal;
	int			 iSubIndex;
	myTriangle2(int iIndex) : iSubIndex(iIndex) {}
	myTriangle2() : iSubIndex(-1) {}
};

//테스트
struct myInstance
{
	Matrix matWorld;
};

struct mySubMesh
{
	vector<myTriangle>		m_TriangleList;
	//pnct2
	vector<PNCT_VERTEX>		m_VertexList;
	vector<IW_VERTEX>		m_VertexListIW;
	vector<DWORD>			m_IndexList;
	ComPtr<ID3D11Buffer>	m_pVertexBuffer;
	ComPtr<ID3D11Buffer>	m_pVertexBufferIW;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
	myTexture*				m_pTexture;
	int						m_iFaceCount;
	UINT					m_iNumVertex;
	UINT					m_iNumIndex;
	void	SetUniqueBuffer(myTriangle& tri)
	{
	//비어있는 인덱스 버퍼와 버텍스 버퍼를 만들어놓고
	//트라이앵글 리스트를 돌리면서
	//버텍스 리스트에 들어있으면 인덱스리스트에만 푸시
	//버텍스 리스트에 들어있지않으면
	//버텍스,인덱스리스트 모두 푸시
		for (int iVertex = 0; iVertex < 3; iVertex++)
		{
			bool bAdd = true;
			int iPosReturn = -1;
			for (int iIndex = 0; iIndex < m_VertexList.size(); iIndex++)
			{
				if (m_VertexList[iIndex] == tri.vVertex[iVertex])
				{
					m_IndexList.push_back(iIndex);
					bAdd = false;
					break;
				}
			}
			if (bAdd)
			{
				m_VertexList.push_back(tri.vVertex[iVertex]);
				m_VertexListIW.push_back(tri.vVertexIW[iVertex]);
				m_IndexList.push_back(m_VertexList.size() - 1);
			}
		}
		m_iFaceCount++;
	}
	mySubMesh()
	{
		m_iFaceCount = 0;
		m_iNumVertex = 0;
		m_iNumIndex = 0;
	}
};

struct mySubMesh2 : public mySubMesh
{
	vector<myTriangle2>		m_TriangleList2;
	vector<PNCT2_VERTEX>	m_VertexList2;
	void	SetUniqueBuffer(myTriangle2& tri)
	{
		for (int iVertex = 0; iVertex < 3; iVertex++)
		{
			bool bAdd = true;
			int iPosReturn = -1;
			for (int iIndex = 0; iIndex < m_VertexList2.size(); iIndex++)
			{
				if (m_VertexList2[iIndex] == tri.vVertex[iVertex])
				{
					m_IndexList.push_back(iIndex);
					m_iNumIndex++;
					bAdd = false;
					break;
				}
			}
			if (bAdd)
			{
				m_VertexList2.push_back(tri.vVertex[iVertex]);
				m_VertexListIW.push_back(tri.vVertexIW[iVertex]);
				m_IndexList.push_back(m_VertexList2.size() - 1);
				m_iNumIndex++;
				m_iNumVertex++;
			}
		}
		m_iFaceCount++;
	}
	mySubMesh2()
	{
		mySubMesh::mySubMesh();
	}
};

struct myDataCB
{
	Matrix  matWorld;
	Matrix  matView;
	Matrix  matProject;
	float vColor[4];
	float vTime[4];
};
