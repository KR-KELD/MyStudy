#include "myFbxObj.h"

//메테리얼 종류 파라미터
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
	//루트노드를 설정하고 루트를 기준으로 순회하면서 파싱한다
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
	//삼각형 변환
	FbxGeometryConverter GeomConverter(m_pFbxManager);
	GeomConverter.Triangulate(m_pFbxScene, true);
	return true;
}

void myFbxObj::PreProcess(FbxNode * pFbxNode)
{
	//카메라나 라이트는 제외하고 순회
	if (pFbxNode && (pFbxNode->GetCamera() || pFbxNode->GetLight())) return;
	Matrix mat = mat.Identity;
	m_dxMatIter = m_dxMatList.find(pFbxNode->GetName());
	if (m_dxMatIter == m_dxMatList.end())
	{
		m_dxMatList[pFbxNode->GetName()] = mat;
	}
	//자식 노드를 돌면서 메쉬와 본 정보만 가지고온다
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
	//노드를 순회하면서 오브젝트화 시키고 리스트에 저장한다
	myGameObject* pObj = myGameObject::CreateComponentObj(new myGraphics,
										to_mw(pFbxNode->GetName()).c_str());
	m_MeshList[pFbxNode] = pObj;
	//부모의 월드를 상속받는다
	Matrix matWorld = ParseTransform(pFbxNode, matParent);
	pObj->m_pTransform->m_matWorld = matWorld;

	if (pFbxNode->GetMesh() != nullptr)
	{
		ParseMesh(pFbxNode, pFbxNode->GetMesh(), pObj->GetComponent<myGraphics>());
	}
}

void myFbxObj::ParseMesh(FbxNode * pFbxNode, FbxMesh * pFbxMesh, myGraphics * pGraphics)
{
	//uv좌표
	vector<FbxLayerElementUV*> LayerUVVertices;
	//메테리얼 리스트
	vector<FbxLayerElementMaterial*> LayerMaterials;

	//레이어의 갯수 가져오기(멀티 텍스쳐링)
	int iLayerCount = pFbxMesh->GetLayerCount();
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);
		//버텍스 컬러(라이트 맵핑에 사용 가능)
		if (pLayer->GetVertexColors() != NULL)
		{

		}
		//uv좌표를 리스트에 저장
		if (pLayer->GetUVs() != NULL)
		{
			LayerUVVertices.push_back(pLayer->GetUVs());
		}
		//메테리얼을 리스트에 저장
		if (pFbxMesh->GetLayer(iLayer)->GetMaterials() != nullptr)
		{
			LayerMaterials.push_back(pFbxMesh->GetLayer(iLayer)->GetMaterials());
		}
	}
	//노드의 (서브)메테리얼 갯수 가져오기
	int iNumMtrl = pFbxNode->GetMaterialCount();
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pMtrl = pFbxNode->GetMaterial(iMtrl);
		if (pMtrl == nullptr)
		{
			continue;
		}
		//지금은 메테리얼의 이름만 리스트에 저장
		pGraphics->m_MaterialList.push_back(to_mw(ParseMaterial(pMtrl)));
	}
	//메테리얼이 1개 이상이라면 서브메테리얼이 존재하므로
	//서브메시 리스트를 리사이즈 해준다
	if (iNumMtrl > 1)
	{
		pGraphics->m_SubMeshList.resize(iNumMtrl);
	}

	//지오메트리 매트릭스(질문 로컬이랑 같이)
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

	//기본 단위 도형의 정점 갯수
	int iPolyCount = pFbxMesh->GetPolygonCount();
	//정점의 갯수
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	//정점들의 정보
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

//그래픽 툴의 uv 좌표(u,v,w) 를 dx의 uv 좌표(u,v) 로 변환
void myFbxObj::ReadTextureCoord(FbxMesh * pFbxMesh, FbxLayerElementUV * pUVSet, int iVertexIndex, int iUVIndex, FbxVector2 & uv)
{
	FbxLayerElementUV *pFbxLayerElementUV = pUVSet;

	if (pFbxLayerElementUV == nullptr) return;

	switch (pFbxLayerElementUV->GetMappingMode())
	{
		//제어점 (공유정점)
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
		//폴리곤당 버텍스(폴리곤당 정점따로)
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
	//디퓨즈 텍스쳐 불러오기
	auto Property = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (Property.IsValid())
	{
		//텍스쳐의 정보를 불러온다
		const FbxFileTexture* tex = Property.GetSrcObject<FbxFileTexture>(0);
		if (tex != nullptr)
		{
			//이름을 불러와서 tga파일이면 dds로 바꾸고 이름을 리턴
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
