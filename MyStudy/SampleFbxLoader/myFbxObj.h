#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "myStd.h"
#include <fbxsdk.h>
#include "myGraphics.h"
#include "myAnimation.h"

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

class SkinData
{
public:
	static int				iNumMaxWeight;
	std::vector<FbxNode*>	InfluenceNodes;
	std::unordered_map<T_STR, Matrix> m_matBindPoseMap;

	size_t					dwVertexCount;
	DWORD					dwVertexStride;
	std::unique_ptr<int[]>	pBoneIndices;
	std::unique_ptr<float[]> pBoneWeights;
public:
	void	Alloc(size_t dwCount, DWORD dwStride);
	int*	GetIndices(size_t dwIndex);
	float*	GetWeights(size_t dwIndex);
	DWORD	GetBoneCount() const;
	void	InsertWeight(size_t dwIndex, DWORD dwBoneIndex, float fBoneWeight);
public:
	SkinData() : dwVertexCount(0), dwVertexStride(0) {}
	~SkinData() {}
};

class myFbxObj
{
public:
	FbxManager*						m_pFbxManager;
	FbxImporter*					m_pFbxImporter;
	FbxScene*						m_pFbxScene;
	unordered_map<string, Matrix>	m_dxMatList;
	unordered_map<string, Matrix>::iterator	m_dxMatIter;
	unordered_map<FbxNode*, myGameObject*> m_MeshList;
	unordered_map<FbxNode*, myGameObject*>::iterator m_MeshIter;

	myScene				m_Scene;
public:
	bool		Load(string strFileName);
	bool		LoadFBX(string strFileName);
	bool		Init(string strFileName);
	void		PreProcess(FbxNode* pFbxNode);
	void		ParseNode(FbxNode* pFbxNode, Matrix matParent);
	void		ParseMesh(FbxNode* pFbxNode, FbxMesh* pFbxMesh, myGraphics* pGraphics);
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

	bool ParseMeshSkinning(const FbxMesh* pFbxMesh, SkinData* skindata);
public:
	myFbxObj(FbxManager* pFbxManager);
	virtual ~myFbxObj();
};

