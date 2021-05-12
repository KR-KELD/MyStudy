#include "myShape.h"
DECLARE_COMPONENT(myShape);
DECLARE_COMPONENT(myShapeBox);
DECLARE_COMPONENT(myShapePlane);
DECLARE_COMPONENT(myShapeLine);
myShape::myShape()
{
	m_fRange = 1.0f;
}
myShape::~myShape()
{

}
bool myShape::SetInfo(Vector3 vCenter, float fRange)
{
	m_pTransform->m_vPos = vCenter;
	m_fRange = fRange;
	return true;
}
bool myShape::Init()
{
	return myGraphics::Init();
}
bool    myShape::CreateVertexData()
{
	CreateVertexData(m_pTransform->m_vPos, m_fRange);
	return true;
}

bool myShape::CreateVertexData(Vector3 vCenter, float fRange)
{
	return true;
}

bool    myShape::CreateIndexData()
{
	return true;
}

bool myShapeBox::CreateVertexData(Vector3 vCenter, float fRange)
{
	m_VertexList.resize(24);
	m_VertexList[0] = { Vector3(-m_fRange + vCenter.x, m_fRange + vCenter.y, -m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) };
	m_VertexList[1] = { Vector3(m_fRange + vCenter.x, m_fRange + vCenter.y, -m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) };
	m_VertexList[2] = { Vector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y, -m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) };
	m_VertexList[3] = { Vector3(m_fRange + vCenter.x,-m_fRange + vCenter.y, -m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) };

	m_VertexList[4] = { Vector3(m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) };
	m_VertexList[5] = { Vector3(-m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) };
	m_VertexList[6] = { Vector3(m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) };
	m_VertexList[7] = { Vector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) };

	m_VertexList[8] = { Vector3(-m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) };
	m_VertexList[9] = { Vector3(-m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) };
	m_VertexList[10] = { Vector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) };
	m_VertexList[11] = { Vector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) };

	m_VertexList[12] = { Vector3(m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) };
	m_VertexList[13] = { Vector3(m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) };
	m_VertexList[14] = { Vector3(m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) };
	m_VertexList[15] = { Vector3(m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) };

	m_VertexList[16] = { Vector3(-m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) };
	m_VertexList[17] = { Vector3(m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) };
	m_VertexList[18] = { Vector3(-m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) };
	m_VertexList[19] = { Vector3(m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) };

	m_VertexList[20] = { Vector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(0.0f, -1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) };
	m_VertexList[21] = { Vector3(m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(0.0f, -1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) };
	m_VertexList[22] = { Vector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, -1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) };
	m_VertexList[23] = { Vector3(m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, -1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) };

	m_iNumVertex = m_VertexList.size();
	m_iVertexSize = sizeof(PNCT_VERTEX);
	return true;
}
bool    myShapeBox::CreateIndexData()
{
	//인덱스 데이터 세팅
	m_IndexList.resize(36);
	for (int i = 0; i < 6; i++)
	{
		m_IndexList[0 + i * 6] = 0 + i * 4;
		m_IndexList[1 + i * 6] = 1 + i * 4;
		m_IndexList[2 + i * 6] = 2 + i * 4;
		m_IndexList[3 + i * 6] = 2 + i * 4;
		m_IndexList[4 + i * 6] = 1 + i * 4;
		m_IndexList[5 + i * 6] = 3 + i * 4;
	}
	m_iNumIndex = m_IndexList.size();
	return true;
}
myShapeBox::myShapeBox()
{

}
myShapeBox::~myShapeBox()
{

}

bool myShapePlane::CreateVertexData(Vector3 vCenter, float fRange)
{
	m_VertexList.resize(4);
	m_VertexList[0] =
	{
		Vector3(-fRange + vCenter.x, fRange + vCenter.y, vCenter.z),
		Vector3(0.0f,0.0f,-1.0f),
		Vector4(1,1,1,1),
		Vector2(0,0)
	};
	m_VertexList[1] =
	{
		Vector3(fRange + vCenter.x, fRange + vCenter.y, vCenter.z),
		Vector3(0.0f,0.0f,-1.0f),
		Vector4(1,1,1,1),
		Vector2(1,0)
	};
	m_VertexList[2] =
	{
		Vector3(-fRange + vCenter.x, -fRange + vCenter.y, vCenter.z),
		Vector3(0.0f,0.0f,-1.0f),
		Vector4(1,1,1,1),
		Vector2(0,1)
	};
	m_VertexList[3] =
	{
		Vector3(fRange + vCenter.x, -fRange + vCenter.y, vCenter.z),
		Vector3(0.0f,0.0f,-1.0f),
		Vector4(1,1,1,1),
		Vector2(1,1)
	};
	m_iNumVertex = m_VertexList.size();
	m_iVertexSize = sizeof(PNCT_VERTEX);
	return true;
}

bool myShapePlane::CreateIndexData()
{
	m_IndexList.resize(6);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 1;
	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 1;
	m_IndexList[iIndex++] = 3;
	m_iNumIndex = m_IndexList.size();
	return true;
}

myShapePlane::myShapePlane()
{
}

myShapePlane::~myShapePlane()
{
}

bool myShapeLine::Draw(ID3D11DeviceContext*	pd3dContext, Vector3 p, Vector3 e, Vector4 c)
{
	m_VertexList[0] = PNCT_VERTEX(p, Vector3(0.0f, 0.0f, -1.0f), c, Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(e, Vector3(0.0f, 0.0f, -1.0f), c, Vector2(1.0f, 0.0f));
	g_pImmediateContext->UpdateSubresource(
		m_pVertexBuffer.Get(), 0, NULL, &m_VertexList.at(0), 0, 0);
	return myShape::Render(pd3dContext);
}
bool myShapeLine::CreateVertexData()
{
	// Vertex Data
	m_VertexList.resize(2);
	m_VertexList[0] = PNCT_VERTEX(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(100.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_iNumVertex = m_VertexList.size();
	m_iVertexSize = sizeof(PNCT_VERTEX);
	return true;
}

bool myShapeLine::CreateIndexData()
{
	m_IndexList.resize(2);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 1;
	m_iNumIndex = m_IndexList.size();
	return true;
}

myShapeLine::myShapeLine()
{
	m_szPixelShader = "PSLine";
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}

myShapeLine::~myShapeLine()
{
}
