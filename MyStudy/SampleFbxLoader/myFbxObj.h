#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "myStd.h"
#include <fbxsdk.h>
#include "myModelObject.h"

// Y축 Z축 스왑
static Matrix DxConvertMatrix(Matrix m)
{
	Matrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;

	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
	return mat;
}
//Fbx 매트릭스를 Dx매트릭스로 변환
static Matrix ConvertMatrixA(const FbxMatrix& matrix)
{
	Matrix matResult;
	auto fData = reinterpret_cast<float*>(&matResult);
	auto pSrcData = reinterpret_cast<const DOUBLE*>(&matrix);
	for (DWORD i = 0; i < 16; ++i)
	{
		fData[i] = (float)pSrcData[i];
	}
	return matResult;
}

//struct AnimationScanNode
//{
//	INT iParentIndex;
//	FbxNode* pNode;
//	ExportAnimationTrack* pTrack;
//	DWORD dwFlags;
//	XMFLOAT4X4 matGlobal;
//};


class myFbxObj
{
public:
	shared_ptr<myModelObject>		m_pModelObject;

	FbxManager*						m_pFbxManager;
	FbxImporter*					m_pFbxImporter;
	FbxScene*						m_pFbxScene;
	//myAnimation						m_Animation;

	unordered_map<string, Matrix>					 m_dxMatList;
	unordered_map<string, Matrix>::iterator			 m_dxMatIter;
	unordered_map<FbxNode*, myGameObject*>			 m_NodeList;
	unordered_map<FbxNode*, myGameObject*>::iterator m_NodeIter;

	unordered_map<FbxNode*, int>				     m_pNodeMap;
public:
	bool		Load(string strFileName);
	bool		LoadFBX(string strFileName);
	bool		Init(string strFileName);
	void		PreProcess(FbxNode* pFbxNode);
	void		ParseNode(FbxNode* pFbxNode, Matrix matParent);
	void		ParseMesh(FbxNode* pFbxNode, FbxMesh* pFbxMesh, myModelGraphics* pGraphics);
	Matrix		ParseTransform(FbxNode* pFbxNode, Matrix& matParentWorld);
	void		ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet,
								int iVertexIndex, int iUVIndex, FbxVector2& uv);
	string		ParseMaterial(FbxSurfaceMaterial* pMtrl);
	FbxVector4  ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	FbxColor	ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, 
						FbxLayerElementVertexColor* pVertexColorSet, DWORD dwDCCIndex,
						DWORD dwVertexIndex);

	void		ParseAnimation(FbxScene* pFbxScene);
	void		ParseAnimStack(FbxScene* pFbxScene, FbxString* strAnimStackName);
	void		ParseNodeAnimation(FbxNode* pNode);


	bool ParseMeshSkinningMap(const FbxMesh* pFbxMesh, vector<myWeight>& skindata);
public:
	myFbxObj(FbxManager* pFbxManager);
	virtual ~myFbxObj();
};

