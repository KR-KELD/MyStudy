#include "myFbxObj.h"

myFbxObj::myFbxObj(FbxManager* pFbxManager)
{
	m_pFbxManager = pFbxManager;
	m_pFbxImporter = nullptr;
	m_pFbxScene = nullptr;
}

myFbxObj::~myFbxObj()
{
}

bool myFbxObj::Load(string strFileName)
{
	if (LoadFBX(strFileName))
	{
		return true;
	}
	return false;
}

bool myFbxObj::LoadFBX(string strFileName)
{
	if (Init(strFileName))
	{
		return true;
	}
	FbxNode* pFbxRootNode = m_pFbxScene->GetRootNode();
	ParseNode(pFbxRootNode, Matrix::Identity);
	return false;
}

bool myFbxObj::Init(string strFileName)
{
	//초기화
	if (m_pFbxManager == nullptr) return false;
	if (m_pFbxImporter == nullptr)
	{
		m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
		if (m_pFbxImporter == nullptr) return false;
	}
	if (m_pFbxScene == nullptr)
	{
		m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
		if (m_pFbxScene == nullptr) return false;
	}

	bool bRet = m_pFbxImporter->Initialize(strFileName.c_str(), -1, 
							m_pFbxManager->GetIOSettings());
	if (bRet == false) return false;

	bRet = m_pFbxImporter->Import(m_pFbxScene);

	//바꿔야함
	//좌표계 변환
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene);
	FbxAxisSystem SceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();
	FbxGeometryConverter GeomConverter(m_pFbxManager);
	GeomConverter.Triangulate(m_pFbxScene, true);
	return true;
}

void myFbxObj::PreProcess(FbxNode * pNode)
{
	if (pNode && (pNode->GetCamera() || pNode->GetLight())) return;
	Matrix mat = mat.Identity;
	m_dxMatIter = m_dxMatList.find(pNode->GetName());
	if (m_dxMatIter == m_dxMatList.end())
	{
		m_dxMatList[pNode->GetName()] = mat;
	}
	int iChild = pNode->GetChildCount();
	for (int i = 0; i < iChild; i++)
	{
		FbxNode* pChildNode = pNode->GetChild(i);
		if (pChildNode->GetNodeAttribute() != NULL)
		{
			FbxNodeAttribute::EType AttributeType = pChildNode->GetNodeAttribute()->GetAttributeType();
			if (AttributeType != FbxNodeAttribute::eMesh &&
				AttributeType != FbxNodeAttribute::eSkeleton&&
				AttributeType != FbxNodeAttribute::eNull)
			{
				continue;
			}
		}
		PreProcess(pChildNode);
	}
}

void myFbxObj::ParseNode(FbxNode * pNode, Matrix matParent)
{
	if (pNode == nullptr) return;
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}

}

void myFbxObj::ParseMesh(FbxNode * pNode, FbxMesh * pMesh, myGraphics * pGraphics)
{
}

Matrix myFbxObj::ParseTransform(FbxNode * pNode, Matrix & matParentWorld)
{
	Matrix matWorld = Matrix::Identity;
	return matWorld;
}

void myFbxObj::ReadTextureCoord(FbxMesh * pFbxMesh, FbxLayerElementUV * pUVSet, int iVertexIndex, int iUVIndex, FbxVector2 & uv)
{
}

string myFbxObj::ParseMaterial(FbxSurfaceMaterial * pMtrl)
{
	return string();
}

void myFbxObj::ParseAnimation(FbxScene * pFbxScene)
{
}
