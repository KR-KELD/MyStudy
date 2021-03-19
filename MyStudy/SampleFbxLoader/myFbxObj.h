#pragma once
#include "myStd.h"
#include <fbxsdk.h>
#include "myGraphics.h"


class myFbxObj
{
public:
	FbxManager*						m_pFbxManager;
	FbxImporter*					m_pFbxImporter;
	FbxScene*						m_pFbxScene;
	unordered_map<string, Matrix>	m_dxMatList;
	unordered_map<string, Matrix>::iterator	m_dxMatIter;
	unordered_map<FbxNode*, myGameObject*> m_MeshList;
public:
	bool	Load(string strFileName);
	bool	LoadFBX(string strFileName);
	bool	Init(string strFileName);
	void	PreProcess(FbxNode* pNode);
	void	ParseNode(FbxNode* pNode, Matrix matParent);
	void	ParseMesh(FbxNode* pNode, FbxMesh* pMesh, myGraphics* pGraphics);
	Matrix  ParseTransform(FbxNode* pNode, Matrix& matParentWorld);
	void	ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet,
		int iVertexIndex, int iUVIndex, FbxVector2& uv);
	string	ParseMaterial(FbxSurfaceMaterial* pMtrl);
	void	ParseAnimation(FbxScene* pFbxScene);
public:
	myFbxObj(FbxManager* pFbxManager);
	virtual ~myFbxObj();
};

