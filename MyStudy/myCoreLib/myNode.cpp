#include "myNode.h"


myNode::myNode()
{
	m_iDepth = 0;
	m_isLeaf = FALSE;
	m_myBox.vCenter = Vector3(0.0f, 0.0f, 0.0f);
	m_myBox.vMax = Vector3(1.0f, 1.0f, 1.0f);
	m_myBox.vMin = Vector3(-1.0f, -1.0f, -1.0f);
	m_myBox.vAxis[0] = Vector3(1.0f, 0.0f, 0.0f);
	m_myBox.vAxis[1] = Vector3(0.0f, 1.0f, 0.0f);
	m_myBox.vAxis[2] = Vector3(0.0f, 0.0f, 1.0f);
	m_myBox.fExtent[0] = 1;
	m_myBox.fExtent[1] = 1;
	m_myBox.fExtent[2] = 1;
	m_dwFaceNum = 0;
	m_mySphere.fRadius = 1.0f;
	m_mySphere.vCenter = Vector3::Zero;
}
myNode::~myNode()
{
	if (!m_CornerList.empty()) m_CornerList.clear();
	if (!m_CornerIndexList.empty()) m_CornerIndexList.clear();
	if (!m_IndexList.empty()) m_IndexList.clear();
	if (!m_NodeIndexList.empty()) m_NodeIndexList.clear();
	if (!m_NeighborList.empty()) m_NeighborList.clear();
	for (int i = 0; i < m_ChildList.size(); i++)
	{
		SAFE_DEL(m_ChildList[i]);
	}
}
