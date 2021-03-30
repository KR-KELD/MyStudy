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
	m_pModelObject = make_shared<myModelObject>();
}

myFbxObj::~myFbxObj()
{
	//for (m_NodeIter = m_NodeMap.begin();
	//	m_NodeIter != m_NodeMap.end();
	//	m_NodeIter++)
	//{
	//	(*m_NodeIter).second->Release();
	//	delete (*m_NodeIter).second;
	//}
	m_NodeMap.clear();
	//매니저에 넣을거면 매니저에서 딜리트
}

bool myFbxObj::ModelInit()
{
	for (auto data : m_pModelObject->m_myNodeList)
	{
		myModelGraphics* pGraphics = data->GetComponent<myModelGraphics>();
		//서브메쉬마다 버텍스리스트,버퍼,텍스쳐를 각각 세팅해준다
		for (int iSub = 0; iSub < pGraphics->m_SubMeshList.size(); iSub++)
		{
			mySubMesh* pSub = &pGraphics->m_SubMeshList[iSub];
			if (pSub->m_TriangleList.size() <= 0) continue;
			pSub->m_VertexList.resize(pSub->m_TriangleList.size() * 3);
			for (int iFace = 0; iFace < pSub->m_TriangleList.size(); iFace++)
			{
				int iIndex = iFace * 3;
				pSub->m_VertexList[iIndex + 0] =
					pSub->m_TriangleList[iFace].vVertex[0];
				pSub->m_VertexList[iIndex + 1] =
					pSub->m_TriangleList[iFace].vVertex[1];
				pSub->m_VertexList[iIndex + 2] =
					pSub->m_TriangleList[iFace].vVertex[2];
			}

			// vb
			ID3D11Buffer* vb =
				CreateVertexBuffer(g_pd3dDevice,
					&pSub->m_VertexList.at(0),
					pSub->m_VertexList.size(),
					sizeof(PNCT_VERTEX));
			pSub->m_pVertexBuffer.Attach(vb);

			// vbiw
			pSub->m_VertexListIW.resize(pSub->m_TriangleList.size() * 3);
			for (int iFace = 0; iFace < pSub->m_TriangleList.size(); iFace++)
			{
				int iIndex = iFace * 3;
				pSub->m_VertexListIW[iIndex + 0] =
					pSub->m_TriangleList[iFace].vVertexIW[0];
				pSub->m_VertexListIW[iIndex + 1] =
					pSub->m_TriangleList[iFace].vVertexIW[1];
				pSub->m_VertexListIW[iIndex + 2] =
					pSub->m_TriangleList[iFace].vVertexIW[2];
			}

			ID3D11Buffer* vbiw =
				CreateVertexBuffer(g_pd3dDevice,
					&pSub->m_VertexListIW.at(0),
					pSub->m_VertexListIW.size(),
					sizeof(IW_VERTEX));
			pSub->m_pVertexBufferIW.Attach(vbiw);

			wstring loadTex = pGraphics->m_MaterialList[iSub].c_str();
			pSub->m_pTexture = g_TextureMgr.Load(loadTex.c_str());
		}
		if (!pGraphics->Create(L"../../data/shader/vs.txt", L"../../data/shader/ps.txt", L""))
		{
			return false;
		}
	}
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
	if (Init(strFileName) == false)
	{
		return false;
	}
	//루트노드를 설정하고 루트를 기준으로 순회하면서 파싱한다
	FbxNode* pFbxRootNode = m_pFbxScene->GetRootNode();
	PreProcess(pFbxRootNode);
	ParseNode(pFbxRootNode, Matrix::Identity);
	ParseAnimation(m_pFbxScene);
	ModelInit();
	return true;
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
	//FbxGeometryConverter GeomConverter(m_pFbxManager);
	//GeomConverter.Triangulate(m_pFbxScene, true);
	return true;
}

void myFbxObj::PreProcess(FbxNode * pFbxNode)
{
	//카메라나 라이트는 제외하고 순회
	if (pFbxNode && (pFbxNode->GetCamera() || pFbxNode->GetLight())) return;
	Matrix mat = mat.Identity;
	m_dxMatIter = m_dxMatMap.find(pFbxNode->GetName());
	if (m_dxMatIter == m_dxMatMap.end())
	{
		m_dxMatMap[pFbxNode->GetName()] = mat;
		m_pNodeIndexMap[pFbxNode] = m_pModelObject->m_nodeMatList.size();
		m_pModelObject->m_nodeMatList.push_back(mat);
	}
	//자식 노드를 돌면서 메쉬와 본 정보만 가지고온다
	int iChild = pFbxNode->GetChildCount();
	for (int i = 0; i < iChild; i++)
	{
		FbxNode* pChildNode = pFbxNode->GetChild(i);
		//if (pChildNode->GetNodeAttribute() != NULL)
		//{
		//	FbxNodeAttribute::EType AttributeType = pChildNode->GetNodeAttribute()->GetAttributeType();
		//	if (AttributeType != FbxNodeAttribute::eMesh &&
		//		AttributeType != FbxNodeAttribute::eSkeleton&&
		//		AttributeType != FbxNodeAttribute::eNull)
		//	{
		//		continue;
		//	}
		//}
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
	myGameObject* pObj = myGameObject::CreateComponentObj(new myModelGraphics,
										to_mw(pFbxNode->GetName()).c_str());
	m_NodeMap[pFbxNode] = pObj;
	m_pModelObject->m_myNodeList.push_back(pObj);
	//부모의 월드를 상속받는다
	Matrix matWorld = ParseTransform(pFbxNode, matParent);
	pObj->m_pTransform->m_matWorld = matWorld;

	if (pFbxNode->GetMesh() != nullptr)
	{
		ParseMesh(pFbxNode, pFbxNode->GetMesh(), pObj->GetComponent<myModelGraphics>());
	}
	int dwChild = pFbxNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pFbxNode->GetChild(dwObj);
		ParseNode(pChildNode, matWorld);
	}
}

void myFbxObj::ParseMesh(FbxNode * pFbxNode, FbxMesh * pFbxMesh, myModelGraphics * pGraphics)
{
	//uv좌표
	vector<FbxLayerElementUV*> LayerUVVertices;
	//메테리얼 리스트
	vector<FbxLayerElementMaterial*> LayerMaterials;
	//정점 컬러 리스트
	vector<FbxLayerElementVertexColor*> LayerVertexColors;

	//레이어의 갯수 가져오기(멀티 텍스쳐링)
	int iLayerCount = pFbxMesh->GetLayerCount();
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);
		//버텍스 컬러(라이트 맵핑에 사용 가능)
		if (pLayer->GetVertexColors() != NULL)
		{
			LayerVertexColors.push_back(pLayer->GetVertexColors());
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
	else
	{
		pGraphics->m_SubMeshList.resize(1);
	}

	//지오메트리 매트릭스(해당 메쉬의 좌표를 로컬에서 본좌표(뼈대의 월드상 위치))
	//로 변환해주는 매트릭스
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

	
	pGraphics->m_pTransform->m_matWorld = 
		DxConvertMatrix(
		ConvertMatrixA(
			pFbxNode->EvaluateGlobalTransform(1.0f)));

	//기본 단위 도형의 정점 갯수
	int iPolyCount = pFbxMesh->GetPolygonCount();
	//정점의 갯수
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	//정점들의 정보
	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();


	bool bSkinnedMesh = ParseMeshSkinningMap(pFbxMesh, pGraphics->m_WeightList);
	pGraphics->m_bSkinnedMesh = bSkinnedMesh;

	//폴리곤의 누적 인덱스
	int iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		int iTriangleCount = iPolySize - 2;
		int iCornerIndices[3];
		myTriangle tri;

		//서브 메테리얼의 종류
		int iSubMtrl = 0;
		if (LayerMaterials.size() > 0)
		{
			switch (LayerMaterials[0]->GetMappingMode())
			{
				//폴리곤 단위 맵핑이면
				case FbxLayerElement::eByPolygon:
				{
					//래퍼런스 타입에따라
					switch (LayerMaterials[0]->GetReferenceMode())
					{
						//폴리곤의 인덱스를 따라간다
						case FbxLayerElement::eIndex:
						{
							iSubMtrl = iPoly;
						}break;
						//가지고있는 인덱스를 가져와서 쓴다
						case FbxLayerElement::eIndexToDirect:
						{
							iSubMtrl = LayerMaterials[0]->GetIndexArray().GetAt(iPoly);
						}break;
					}
				}
				default:
				{
					break;
				}
			}
		}
		//삼각형 정보 컨버트
		for (int iTriangle = 0; iTriangle < iTriangleCount; iTriangle++)
		{
			//인덱스 순서를 반시계(그래픽 툴)에서 시계(DX)로 바꿔준다
			INT iVertIndex[3] = { 0, (INT)iTriangle + 2, (INT)iTriangle + 1 };
			//삼각형 정점의 정보
			iCornerIndices[0] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[0]);
			iCornerIndices[1] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[1]);
			iCornerIndices[2] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[2]);
			//삼각형 노말의 정보
			FbxVector4 vNormals[3];
			ZeroMemory(vNormals, 3 * sizeof(FbxVector4));
			INT iPolyIndex = (INT)iPoly;
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[0], vNormals[0]);
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[1], vNormals[1]);
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[2], vNormals[2]);
			//삼각형 uv좌표의 정보
			int u[3];
			u[0] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[0]);
			u[1] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[1]);
			u[2] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[2]);

			for (int iIndex = 0; iIndex < 3; iIndex++)
			{
				PNCT_VERTEX v;
				//삼각형 정점에 지오매트릭 매트릭스를 곱해줘서 해당 위치로 보내준다
				auto finalPos = geom.MultT(pVertexPositions[iCornerIndices[iIndex]]);
				//Dx 좌표로 바꿔서 넣어준다
				v.p.x = finalPos.mData[0]; // x
				v.p.y = finalPos.mData[2]; // z
				v.p.z = finalPos.mData[1]; // y

				FbxColor color = FbxColor(1, 1, 1, 1);
				//컬러 정보를 변환해서 pnct버텍스에 넣어준다
				if (LayerVertexColors.size())
				{
					color = ReadColor(pFbxMesh,
						LayerVertexColors.size(),
						LayerVertexColors[0],
						iCornerIndices[iIndex],
						iBasePolyIndex + iVertIndex[iIndex]);
				}
				v.c.x = (float)color.mRed;
				v.c.y = (float)color.mGreen;
				v.c.z = (float)color.mBlue;
				v.c.w = 1;
				//노말 정보를 변환해서 pnct버텍스에 넣어준다
				FbxVector4 normal = ReadNormal(pFbxMesh,
					iCornerIndices[iIndex],
					iBasePolyIndex + iVertIndex[iIndex]);
				finalPos = matNormal.MultT(normal);
				v.n.x = finalPos.mData[0]; // x
				v.n.y = finalPos.mData[2]; // z
				v.n.z = finalPos.mData[1]; // y
				//uv좌표 정보를 변환해서 pnct버텍스에 넣어준다
				if (LayerUVVertices.size())
				{
					for (int iUVIndex = 0; iUVIndex < 1/*LayerUVVertices.size()*/; ++iUVIndex)
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
				}
				//영향을 미치는 정점과 가중치 등록
				IW_VERTEX iw;
				if (pGraphics->m_bSkinnedMesh)
				{
					myWeight* pW = &pGraphics->m_WeightList[iCornerIndices[iIndex]];
					for (int i = 0; i < 4; i++)
					{
						iw.i1[i] = pW->nodeIndex[i];
						iw.w1[i] = pW->nodeWeight[i];
					}
				}
				else
				{
					auto data = m_pNodeIndexMap.find(pFbxNode);
					iw.i1[0] = data->second; // 자기 자신
					iw.w1[0] = 1.0f;
				}
				tri.vVertex[iIndex] = v;
				tri.vVertexIW[iIndex] = iw;
			}
			//서브메테리얼이 존재하면
			//서브메쉬의 트라이앵글리스트에 삼각형을 등록하고
			//존재하지 않는다면 바로 기존 오브젝트의 
			//트라이앵글리스트에 삼각형을 등록해준다
			if (iNumMtrl > 1)
			{
				pGraphics->m_SubMeshList[iSubMtrl].m_TriangleList.push_back(tri);
			}
			else
			{
				pGraphics->m_SubMeshList[0].m_TriangleList.push_back(tri);
			}
		}
		iBasePolyIndex += iPolySize;
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
		//제어점 (공유정점) 단위로
		case FbxLayerElementUV::eByControlPoint:
		{
			switch (pFbxLayerElementUV->GetReferenceMode())
			{
				//직접 대입
				case FbxLayerElementUV::eDirect:
				{
					FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(iVertexIndex);
					uv.mData[0] = fbxUv.mData[0];
					uv.mData[1] = fbxUv.mData[1];
					break;
				}
				//인덱스좌표로 대입
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
		//폴리곤당 정점 단위로
		case FbxLayerElementUV::eByPolygonVertex:
		{
			switch (pFbxLayerElementUV->GetReferenceMode())
			{
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
			_splitpath_s(szFileName, Drive, Dir, FName, Ext);
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

FbxVector4 myFbxObj::ReadNormal(const FbxMesh* mesh,
	int controlPointIndex,
	int vertexCounter)
{
	if (mesh->GetElementNormalCount() < 1) {}

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	// 노말 획득 
	FbxVector4 result;
	// 노말 벡터를 저장할 벡터 
	switch (vertexNormal->GetMappingMode()) 	// 매핑 모드 
	{
		// 제어점 마다 1개의 매핑 좌표가 있다.
	case FbxGeometryElement::eByControlPoint:
	{
		// control point mapping 
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		} break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
			// 인덱스를 얻어온다. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// 정점 마다 1개의 매핑 좌표가 있다.
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
			// 인덱스를 얻어온다. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}
	}break;
	}
	return result;
}
FbxColor myFbxObj::ReadColor(const FbxMesh* mesh,
	DWORD dwVertexColorCount,
	FbxLayerElementVertexColor* pVertexColorSet,
	DWORD dwDCCIndex, DWORD dwVertexIndex)
{
	FbxColor Value(1, 1, 1, 1);
	if (dwVertexColorCount > 0 && pVertexColorSet != NULL)
	{
		// Crack apart the FBX dereferencing system for Color coordinates		
		switch (pVertexColorSet->GetMappingMode())
		{
		case FbxLayerElement::eByControlPoint:
			switch (pVertexColorSet->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				Value = pVertexColorSet->GetDirectArray().GetAt(dwDCCIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwDCCIndex);
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
		case FbxLayerElement::eByPolygonVertex:
			switch (pVertexColorSet->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				int iColorIndex = dwVertexIndex;
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwVertexIndex);
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
			break;
		}
	}
	return Value;
}
