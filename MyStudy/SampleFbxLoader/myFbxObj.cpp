#include "myFbxObj.h"

//���׸��� ���� �Ķ����
class ExportMaterialParameter
{
public:
	enum ExportMaterialParameterFlags
	{
		EMPF_NONE = 0,
		EMPF_BUMPMAP = 1,
		EMPF_DIFFUSEMAP = 2,
		EMPF_NORMALMAP = 4,
		EMPF_SPECULARMAP = 8,
		EMPF_ALPHACHANNEL = 16,
		EMPF_AOMAP = 32,
	};
	ExportMaterialParameter()
	{
		ZeroMemory(this, sizeof(ExportMaterialParameter));
	}
};

myFbxObj::myFbxObj(FbxManager* pFbxManager)
{
	m_pFbxManager = pFbxManager;
	m_pFbxImporter = nullptr;
	m_pFbxScene = nullptr;
}

myFbxObj::~myFbxObj()
{
	for (m_MeshIter = m_MeshList.begin();
		m_MeshIter != m_MeshList.end();
		m_MeshIter++)
	{
		(*m_MeshIter).second->Release();
		delete (*m_MeshIter).second;
	}
	m_MeshList.clear();
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
	//��Ʈ��带 �����ϰ� ��Ʈ�� �������� ��ȸ�ϸ鼭 �Ľ��Ѵ�
	FbxNode* pFbxRootNode = m_pFbxScene->GetRootNode();
	ParseNode(pFbxRootNode, Matrix::Identity);
	return false;
}

bool myFbxObj::Init(string strFileName)
{
	//�ʱ�ȭ
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

	//�ٲ����
	//��ǥ�� ��ȯ
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene);
	FbxAxisSystem SceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();
	//�ﰢ�� ��ȯ
	FbxGeometryConverter GeomConverter(m_pFbxManager);
	GeomConverter.Triangulate(m_pFbxScene, true);
	return true;
}

void myFbxObj::PreProcess(FbxNode * pFbxNode)
{
	//ī�޶� ����Ʈ�� �����ϰ� ��ȸ
	if (pFbxNode && (pFbxNode->GetCamera() || pFbxNode->GetLight())) return;
	Matrix mat = mat.Identity;
	m_dxMatIter = m_dxMatList.find(pFbxNode->GetName());
	if (m_dxMatIter == m_dxMatList.end())
	{
		m_dxMatList[pFbxNode->GetName()] = mat;
	}
	//�ڽ� ��带 ���鼭 �޽��� �� ������ ������´�
	int iChild = pFbxNode->GetChildCount();
	for (int i = 0; i < iChild; i++)
	{
		FbxNode* pChildNode = pFbxNode->GetChild(i);
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

void myFbxObj::ParseNode(FbxNode * pFbxNode, Matrix matParent)
{
	if (pFbxNode == nullptr) return;
	if (pFbxNode && (pFbxNode->GetCamera() || pFbxNode->GetLight()))
	{
		return;
	}
	//��带 ��ȸ�ϸ鼭 ������Ʈȭ ��Ű�� ����Ʈ�� �����Ѵ�
	myGameObject* pObj = myGameObject::CreateComponentObj(new myGraphics,
										to_mw(pFbxNode->GetName()).c_str());
	m_MeshList[pFbxNode] = pObj;
	//�θ��� ���带 ��ӹ޴´�
	Matrix matWorld = ParseTransform(pFbxNode, matParent);
	pObj->m_pTransform->m_matWorld = matWorld;

	if (pFbxNode->GetMesh() != nullptr)
	{
		ParseMesh(pFbxNode, pFbxNode->GetMesh(), pObj->GetComponent<myGraphics>());
	}
}

void myFbxObj::ParseMesh(FbxNode * pFbxNode, FbxMesh * pFbxMesh, myGraphics * pGraphics)
{
	//uv��ǥ
	vector<FbxLayerElementUV*> LayerUVVertices;
	//���׸��� ����Ʈ
	vector<FbxLayerElementMaterial*> LayerMaterials;

	//���̾��� ���� ��������(��Ƽ �ؽ��ĸ�)
	int iLayerCount = pFbxMesh->GetLayerCount();
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);
		//���ؽ� �÷�(����Ʈ ���ο� ��� ����)
		if (pLayer->GetVertexColors() != NULL)
		{

		}
		//uv��ǥ�� ����Ʈ�� ����
		if (pLayer->GetUVs() != NULL)
		{
			LayerUVVertices.push_back(pLayer->GetUVs());
		}
		//���׸����� ����Ʈ�� ����
		if (pFbxMesh->GetLayer(iLayer)->GetMaterials() != nullptr)
		{
			LayerMaterials.push_back(pFbxMesh->GetLayer(iLayer)->GetMaterials());
		}
	}
	//����� (����)���׸��� ���� ��������
	int iNumMtrl = pFbxNode->GetMaterialCount();
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pMtrl = pFbxNode->GetMaterial(iMtrl);
		if (pMtrl == nullptr)
		{
			continue;
		}
		//������ ���׸����� �̸��� ����Ʈ�� ����
		pGraphics->m_MaterialList.push_back(to_mw(ParseMaterial(pMtrl)));
	}
	//���׸����� 1�� �̻��̶�� ������׸����� �����ϹǷ�
	//����޽� ����Ʈ�� �������� ���ش�
	if (iNumMtrl > 1)
	{
		pGraphics->m_SubMeshList.resize(iNumMtrl);
	}

	//������Ʈ�� ��Ʈ����(���� �����̶� ����)
	FbxAMatrix geom;
	FbxVector4 trans = pFbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = pFbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pFbxNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);

	FbxAMatrix matNormal = geom;
	matNormal = matNormal.Inverse();
	matNormal = matNormal.Transpose();

	//FbxAMatrix globalMatrix = pFbxNode->EvaluateLocalTransform();
	//FbxAMatrix matrix = globalMatrix * geom;

	//�⺻ ���� ������ ���� ����
	int iPolyCount = pFbxMesh->GetPolygonCount();
	//������ ����
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	//�������� ����
	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();

	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		int iTriangleCount = iPolySize - 2;
		int iCornerIndices[3];
		myTriangle tri;

		// sub mtrl
		int iSubMtrl = 0;
		if (LayerMaterials.size() > 0)
		{
			switch (LayerMaterials[0]->GetMappingMode())
			{
			case FbxLayerElement::eByPolygon:
			{
				switch (LayerMaterials[0]->GetReferenceMode())
				{
				case FbxLayerElement::eIndex:
				{
					iSubMtrl = iPoly;
				}break;
				case FbxLayerElement::eIndexToDirect:
				{
					iSubMtrl = LayerMaterials[0]->GetIndexArray().GetAt(iPoly);
					pGraphics->m_SubMeshList[iSubMtrl].m_iPolyCount++;
				}break;
				}
			}
			default:
			{
				break;
			}
			}
		}

		for (int iTriangle = 0; iTriangle < iTriangleCount; iTriangle++)
		{
			INT iVertIndex[3] = { 0, (INT)iTriangle + 2, (INT)iTriangle + 1 };

			iCornerIndices[0] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[0]);
			iCornerIndices[1] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[1]);
			iCornerIndices[2] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[2]);
			// 0, 9, 8
			FbxVector4 vNormals[3];
			ZeroMemory(vNormals, 3 * sizeof(FbxVector4));
			INT iPolyIndex = (INT)iPoly;
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[0], vNormals[0]);
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[1], vNormals[1]);
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[2], vNormals[2]);

			int u[3];
			u[0] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[0]);
			u[1] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[1]);
			u[2] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[2]);

			for (int iIndex = 0; iIndex < 3; iIndex++)
			{
				PNCT_VERTEX v;
				//auto finalPos = matrix.MultT(pVertexPosiions[iCornerIndices[iIndex]]);
				auto finalPos = geom.MultT(pVertexPositions[iCornerIndices[iIndex]]);
				//finalPos = geom.MultT(finalPos);
				v.p.x = finalPos.mData[0]; // x
				v.p.y = finalPos.mData[2]; // z
				v.p.z = finalPos.mData[1]; // y


				v.c = Vector4(1, 1, 1, 1);
				v.n.x = 0;// vNormals[iCornerIndices[iIndex]].mData[0]; // x
				v.n.y = 0;//vNormals[iCornerIndices[iIndex]].mData[2]; // z
				v.n.z = 0;//vNormals[iCornerIndices[iIndex]].mData[1]; // y
				for (int iUVIndex = 0; iUVIndex < LayerUVVertices.size(); ++iUVIndex)
				{
					FbxLayerElementUV* pUVSet = LayerUVVertices[iUVIndex];
					FbxVector2 uv(0, 0);
					ReadTextureCoord(
						pFbxMesh,
						pUVSet,
						iCornerIndices[iIndex],
						u[iIndex],
						uv);
					v.t.x = uv.mData[0];
					v.t.y = 1.0f - uv.mData[1];
				}
				tri.vVertex[iIndex] = v;
			}
			if (iNumMtrl > 1)
			{
				pGraphics->m_SubMeshList[iSubMtrl].m_TriangleList.push_back(tri);
			}
			else
			{
				pGraphics->m_TriangleList.push_back(tri);
			}
		}
	}
}

Matrix myFbxObj::ParseTransform(FbxNode * pFbxNode, Matrix & matParentWorld)
{
	Matrix matWorld = Matrix::Identity;
	return matWorld;
}

//�׷��� ���� uv ��ǥ(u,v,w) �� dx�� uv ��ǥ(u,v) �� ��ȯ
void myFbxObj::ReadTextureCoord(FbxMesh * pFbxMesh, FbxLayerElementUV * pUVSet, int iVertexIndex, int iUVIndex, FbxVector2 & uv)
{
	FbxLayerElementUV *pFbxLayerElementUV = pUVSet;

	if (pFbxLayerElementUV == nullptr) return;

	switch (pFbxLayerElementUV->GetMappingMode())
	{
		//������ (��������)
		case FbxLayerElementUV::eByControlPoint:
		{
			switch (pFbxLayerElementUV->GetReferenceMode())
			{
				case FbxLayerElementUV::eDirect:
				{
					FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(iVertexIndex);
					uv.mData[0] = fbxUv.mData[0];
					uv.mData[1] = fbxUv.mData[1];
					break;
				}
				case FbxLayerElementUV::eIndexToDirect:
				{
					int id = pFbxLayerElementUV->GetIndexArray().GetAt(iVertexIndex);
					FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(id);
					uv.mData[0] = fbxUv.mData[0];
					uv.mData[1] = fbxUv.mData[1];
					break;
				}
			}
			break;
		}
		//������� ���ؽ�(������� ��������)
		case FbxLayerElementUV::eByPolygonVertex:
		{
			switch (pFbxLayerElementUV->GetReferenceMode())
			{
				// Always enters this part for the example model
				case FbxLayerElementUV::eDirect:
				case FbxLayerElementUV::eIndexToDirect:
				{
					uv.mData[0] = pFbxLayerElementUV->GetDirectArray().GetAt(iUVIndex).mData[0];
					uv.mData[1] = pFbxLayerElementUV->GetDirectArray().GetAt(iUVIndex).mData[1];
					break;
				}
			}
			break;
		}
	}
}

string myFbxObj::ParseMaterial(FbxSurfaceMaterial * pMtrl)
{
	string strName = pMtrl->GetName();
	//��ǻ�� �ؽ��� �ҷ�����
	auto Property = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (Property.IsValid())
	{
		//�ؽ����� ������ �ҷ��´�
		const FbxFileTexture* tex = Property.GetSrcObject<FbxFileTexture>(0);
		if (tex != nullptr)
		{
			//�̸��� �ҷ��ͼ� tga�����̸� dds�� �ٲٰ� �̸��� ����
			const CHAR* szFileName = tex->GetFileName();
			CHAR Drive[MAX_PATH];
			CHAR Dir[MAX_PATH];
			CHAR FName[MAX_PATH];
			CHAR Ext[MAX_PATH];
			_splitpath(szFileName, Drive, Dir, FName, Ext);
			std::string texName = FName;
			std::string ext = Ext;
			if (ext == ".tga")
			{
				ext.clear();
				ext = ".dds";
			}
			texName += ext;
			return texName;
		}
	}
	return std::string("");
}

void myFbxObj::ParseAnimation(FbxScene * pFbxScene)
{
}
