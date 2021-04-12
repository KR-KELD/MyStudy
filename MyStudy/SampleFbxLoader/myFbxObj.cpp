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
	m_pModelObject = make_shared<myModelObject>();
}

myFbxObj::~myFbxObj()
{
	//for (m_pNodeIter = m_pNodeMap.begin();
	//	m_pNodeIter != m_pNodeMap.end();
	//	m_pNodeIter++)
	//{
	//	(*m_pNodeIter).second->Release();
	//	delete (*m_pNodeIter).second;
	//}
	m_pNodeMap.clear();
	//�Ŵ����� �����Ÿ� �Ŵ������� ����Ʈ
}

bool myFbxObj::ModelInit()
{
	for (auto data : m_pModelObject->m_myNodeList)
	{
		myModelGraphics* pGraphics = data->GetComponent<myModelGraphics>();
		//����޽����� ���ؽ�����Ʈ,����,�ؽ��ĸ� ���� �������ش�
		for (int iSub = 0; iSub < pGraphics->m_SubMeshList.size(); iSub++)
		{
			mySubMesh* pSub = &pGraphics->m_SubMeshList[iSub];
			if (pSub->m_iFaceCount <= 0) continue;
			// vb
			ID3D11Buffer* vb =
				CreateVertexBuffer(g_pd3dDevice,
					&pSub->m_VertexList.at(0),
					pSub->m_VertexList.size(),
					sizeof(PNCT_VERTEX));
			pSub->m_pVertexBuffer.Attach(vb);
			// vbiw
			ID3D11Buffer* vbiw =
				CreateVertexBuffer(g_pd3dDevice,
					&pSub->m_VertexListIW.at(0),
					pSub->m_VertexListIW.size(),
					sizeof(IW_VERTEX));
			pSub->m_pVertexBufferIW.Attach(vbiw);
			// ib
			ID3D11Buffer* ib =
				CreateIndexBuffer(g_pd3dDevice,
					&pSub->m_IndexList.at(0),
					pSub->m_IndexList.size(),
					sizeof(DWORD));
			pSub->m_pIndexBuffer.Attach(ib);

			wstring loadTex = pGraphics->m_MaterialList[iSub].c_str();
			pSub->m_pTexture = g_TextureMgr.Load(loadTex.c_str());
			if (pSub->m_pTexture == nullptr)
				pSub->m_pTexture = g_TextureMgr.m_pWhiteTexture;
		}
	}
	if (!m_pModelObject->m_pGraphics->
		Create(L"../../data/shader/vs.txt", L"../../data/shader/ps.txt", L""))
	{
		return false;
	}
}

bool myFbxObj::CuttingAnimScene(wstring strSceneName, int iFirstFrame, int iLastFrame)
{
	myAnimation* pAnim = m_pModelObject->GetComponent<myAnimation>();
	if (pAnim == nullptr) return false;
	pAnim->AddAnim(strSceneName, m_AnimScene.CuttingAnimScene(iFirstFrame, iLastFrame));

	return true;
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
	//��Ʈ��带 �����ϰ� ��Ʈ�� �������� ��ȸ�ϸ鼭 �Ľ��Ѵ�
	FbxNode* pFbxRootNode = m_pFbxScene->GetRootNode();
	PreProcess(pFbxRootNode);

	for (int iNode = 0; iNode < m_pFbxNodeList.size(); iNode++)
	{
		FbxNode* pNode = m_pFbxNodeList[iNode];
		myGameObject* pObj = myGameObject::CreateComponentObj(new myModelGraphics,
			to_mw(pNode->GetName()).c_str());
		pObj->SetParent(m_pNodeMap[pNode->GetParent()]);
		//��忡 �����ϴ� ���ӿ�����Ʈ ���� �� �ʿ� �߰�
		m_pNodeMap[pNode] = pObj;
		//���Ϳ��� �߰�
		m_pModelObject->m_myNodeList.push_back(pObj);

		if (pNode->GetMesh() != nullptr)
		{
			ParseMesh(pNode, pNode->GetMesh(), pObj->GetComponent<myModelGraphics>());
		}
	}

#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR==2014) && (FBXSDK_VERSION_MINOR>1) ) )
	auto anim = m_pFbxScene->GetAnimationEvaluator();
#else
	auto anim = m_pFbxScene->GetEvaluator();
#endif
	ParseAnimation(m_pFbxScene);

	float fCurrentTime = 0.0f;
	//�ӵ����� �׳� �Լ�ȣ�⶧��

	while (fCurrentTime <= m_AnimScene.fLastTime)
	{
		FbxTime t;
		t.SetSecondDouble(fCurrentTime);
		for (int iNode = 0; iNode < m_pFbxNodeList.size(); iNode++)
		{
			FbxNode* pNode = m_pFbxNodeList[iNode];
			////�ð��� �ش��ϴ� �ִϸ��̼� ��Ʈ���� ���� ��������
			//FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, t);
			////Ʈ������ ����
			//myAnimTrack track;
			//track.iTick = fCurrentTime * 30 * 160;
			//track.matWorld = DxConvertMatrix(ConvertMatrixA(mat));
			////�ش��忡 ���� ����

			//pGraphics->m_AnimTrackList.push_back(track);

			Matrix pChildGlobal = DxConvertMatrix(
				ConvertMatrixA(anim->GetNodeGlobalTransform(pNode, t)));
			FbxNode* pParentNode = pNode->GetParent();

			Matrix matInvParentGlobal = Matrix::Identity;
			if (pParentNode)
			{
				matInvParentGlobal = DxConvertMatrix(
					ConvertMatrixA(anim->GetNodeGlobalTransform(pParentNode, t)));
				matInvParentGlobal = matInvParentGlobal.Invert();
			}
			Matrix m = pChildGlobal * matInvParentGlobal;

			// ����
			Vector3 vScale;
			Quaternion qRot;
			Vector3 vTrans;

			myAnimTrack track;
			track.iTick = fCurrentTime * 30 * 160;
			track.matWorld = pChildGlobal;
			myModelGraphics* pGraphics = m_pNodeMap.find(pNode)->
				second->GetComponent<myModelGraphics>();
			if (pGraphics->m_AnimTrackList.empty())
				pGraphics->m_AnimTrackList.push_back(AnimTrackList());
			if (m.Decompose(vScale, qRot, vTrans))
			{
				track.vScale = vScale;
				track.qRot = qRot;
				track.vTrans = vTrans;
			}
			else
			{
				track.vScale = Vector3(1.0f, 1.0f, 1.0f);
				track.qRot = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
				track.vTrans = Vector3(0.0f, 0.0f, 0.0f);
			}
			pGraphics->m_AnimTrackList.back().push_back(track);
		}
		fCurrentTime += m_AnimScene.fDeltaTime * 1.0f;
	}

	//�ð��� SRT ��ȭ���� üũ�ؼ� ��ȭ�� ������ �ִϸ��̼��� ���°ɷ� �����ϰ�
	//Ʈ���� �����Ѵ�
	bool isAnim = false;
	Vector3 vCheckScale;
	Quaternion qCheckRot;
	Vector3 vCheckTrans;
	for (int iNode = 0; iNode < m_pModelObject->m_myNodeList.size(); iNode++)
	{
		myModelGraphics* pGraphics = m_pModelObject->
			m_myNodeList[iNode]->GetComponent<myModelGraphics>();
		bool isFirst = true;
		for (int iIndex = 0; iIndex < pGraphics->m_AnimTrackList.back().size(); iIndex++)
		{
			myAnimTrack* pTrack = &pGraphics->m_AnimTrackList.back()[iIndex];
			if (isFirst)
			{
				vCheckScale = pTrack->vScale;
				qCheckRot = pTrack->qRot;
				vCheckTrans = pTrack->vTrans;
				isFirst = false;
			}
			else
			{
				if (vCheckTrans != pTrack->vTrans ||
					qCheckRot != pTrack->qRot ||
					vCheckScale != pTrack->vScale)
				{
					isAnim = true;
					break;
				}
			}
		}
		if (isAnim)
			break;
	}

	if (!isAnim)
	{
		for (int iNode = 0; iNode < m_pModelObject->m_myNodeList.size(); iNode++)
		{
			myModelGraphics* pGraphics = m_pModelObject->
				m_myNodeList[iNode]->GetComponent<myModelGraphics>();
			pGraphics->m_AnimTrackList.back().clear();
		}
	}
	ModelInit();
	return true;
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
	//FbxGeometryConverter GeomConverter(m_pFbxManager);
	//GeomConverter.Triangulate(m_pFbxScene, true);
	return true;
}

void myFbxObj::PreProcess(FbxNode * pFbxNode)
{
	//ī�޶� ����Ʈ�� �����ϰ� ��ȸ
	if (pFbxNode && (pFbxNode->GetCamera() || pFbxNode->GetLight())) return;
	Matrix mat = mat.Identity;
	//��� �ε��� �߰�
	m_pNodeIndexMap.insert(make_pair(pFbxNode, m_pModelObject->m_nodeMatList.size()));
	//�ε����� �ش��ϴ� ��Ʈ���� �߰�
	m_pModelObject->m_nodeMatList.push_back(mat);
	//��帮��Ʈ �߰�
	m_pFbxNodeList.push_back(pFbxNode);

	//�ڽ� ��带 ���鼭 �޽��� �� ������ ������´�
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

	//��带 ��ȸ�ϸ鼭 ������Ʈȭ ��Ű�� ����Ʈ�� �����Ѵ�
	myGameObject* pObj = myGameObject::CreateComponentObj(new myModelGraphics,
										to_mw(pFbxNode->GetName()).c_str());

	//��忡 �����ϴ� ���ӿ�����Ʈ ���� �� �ʿ� �߰�
	m_pNodeMap[pFbxNode] = pObj;
	//���Ϳ��� �߰�
	m_pModelObject->m_myNodeList.push_back(pObj);
	//�θ��� ���带 ��ӹ޴´�
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
	//uv��ǥ
	vector<FbxLayerElementUV*> LayerUVVertices;
	//���׸��� ����Ʈ
	vector<FbxLayerElementMaterial*> LayerMaterials;
	//���� �÷� ����Ʈ
	vector<FbxLayerElementVertexColor*> LayerVertexColors;

	//���̾��� ���� ��������(��Ƽ �ؽ��ĸ�)
	int iLayerCount = pFbxMesh->GetLayerCount();

	if (iLayerCount == 0 || pFbxMesh->GetLayer(0)->GetNormals() == nullptr)
	{
		pFbxMesh->InitNormals();
#if (FBXSDK_VERSION_MAJOR >= 2015)
		pFbxMesh->GenerateNormals();
#else
		pFbxMesh->ComputeVertexNormals();
#endif
	}

	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);
		//���ؽ� �÷�(����Ʈ ���ο� ��� ����)
		if (pLayer->GetVertexColors() != NULL)
		{
			LayerVertexColors.push_back(pLayer->GetVertexColors());
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


	//�⺻ ���� ������ ����
	int iPolyCount = pFbxMesh->GetPolygonCount();
	//������ ����
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	//Ʈ���̾ޱ��� ����
	int iMaxTriangleCount = 0;
	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		iMaxTriangleCount += iPolySize - 2;
	}

	//����� (����)���׸��� ���� ��������
	int iNumMtrl = pFbxNode->GetMaterialCount();
	//���׸����� 1�� �̻��̶�� ������׸����� �����ϹǷ�
	//����޽� ����Ʈ�� �������� ���ش�
	if (iNumMtrl > 1)
	{
		pGraphics->m_SubMeshList.resize(iNumMtrl);
	}
	else
	{
		pGraphics->m_SubMeshList.resize(1);
		//Ʈ���̾ޱ��� ����x3��ŭ ������ ������ �����صд�
		pGraphics->m_SubMeshList[0].m_VertexList.reserve(iMaxTriangleCount * 3);
	}


	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pMtrl = pFbxNode->GetMaterial(iMtrl);
		if (pMtrl == nullptr)
		{
			continue;
		}
		//������ ���׸����� �̸��� ����Ʈ�� ����
		pGraphics->m_MaterialList.push_back(to_mw(ParseMaterial(pMtrl)));
		pGraphics->m_SubMeshList[iMtrl].m_VertexList.reserve(iMaxTriangleCount * 3);
	}

	//������Ʈ�� ��Ʈ����(�ش� �޽��� ��ǥ�� ���ÿ��� ����ǥ(������ ����� ��ġ))
	//�� ��ȯ���ִ� ��Ʈ����
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

	//�������� ����
	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();

	bool bSkinnedMesh = ParseMeshSkinningMap(pFbxMesh, pGraphics->m_WeightList, pGraphics);
	pGraphics->m_bSkinnedMesh = bSkinnedMesh;

	//�������� ���� �ε���
	int iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		int iTriangleCount = iPolySize - 2;
		int iCornerIndices[3];
		myTriangle tri;

		//���� ���׸����� ����
		int iSubMtrl = 0;
		if (LayerMaterials.size() > 0)
		{
			switch (LayerMaterials[0]->GetMappingMode())
			{
				//������ ���� �����̸�
				case FbxLayerElement::eByPolygon:
				{
					//���۷��� Ÿ�Կ�����
					switch (LayerMaterials[0]->GetReferenceMode())
					{
						//�������� �ε����� ���󰣴�
						case FbxLayerElement::eIndex:
						{
							iSubMtrl = iPoly;
						}break;
						//�������ִ� �ε����� �����ͼ� ����
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
		//�ﰢ�� ���� ����Ʈ
		for (int iTriangle = 0; iTriangle < iTriangleCount; iTriangle++)
		{
			//�ε��� ������ �ݽð�(�׷��� ��)���� �ð�(DX)�� �ٲ��ش�
			INT iVertIndex[3] = { 0, (INT)iTriangle + 2, (INT)iTriangle + 1 };
			//�ﰢ�� ������ ����
			iCornerIndices[0] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[0]);
			iCornerIndices[1] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[1]);
			iCornerIndices[2] = pFbxMesh->GetPolygonVertex(iPoly, iVertIndex[2]);
			//�ﰢ�� �븻�� ����
			FbxVector4 vNormals[3];
			ZeroMemory(vNormals, 3 * sizeof(FbxVector4));
			INT iPolyIndex = (INT)iPoly;
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[0], vNormals[0]);
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[1], vNormals[1]);
			pFbxMesh->GetPolygonVertexNormal(iPolyIndex, iVertIndex[2], vNormals[2]);
			//�ﰢ�� uv��ǥ�� ����
			int u[3];
			u[0] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[0]);
			u[1] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[1]);
			u[2] = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[2]);

			for (int iIndex = 0; iIndex < 3; iIndex++)
			{
				PNCT_VERTEX v;
				//�ﰢ�� ������ ������Ʈ�� ��Ʈ������ �����༭ �ش� ��ġ�� �����ش�
				auto finalPos = geom.MultT(pVertexPositions[iCornerIndices[iIndex]]);
				//Dx ��ǥ�� �ٲ㼭 �־��ش�
				v.p.x = finalPos.mData[0]; // x
				v.p.y = finalPos.mData[2]; // z
				v.p.z = finalPos.mData[1]; // y

				FbxColor color = FbxColor(1, 1, 1, 1);
				//�÷� ������ ��ȯ�ؼ� pnct���ؽ��� �־��ش�
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
				//�븻 ������ ��ȯ�ؼ� pnct���ؽ��� �־��ش�
				FbxVector4 normal = ReadNormal(pFbxMesh,
					iCornerIndices[iIndex],
					iBasePolyIndex + iVertIndex[iIndex]);
				finalPos = matNormal.MultT(normal);
				v.n.x = finalPos.mData[0]; // x
				v.n.y = finalPos.mData[2]; // z
				v.n.z = finalPos.mData[1]; // y
				//uv��ǥ ������ ��ȯ�ؼ� pnct���ؽ��� �־��ش�
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
				//������ ��ġ�� ������ ����ġ ���
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
					iw.i1[0] = data->second; // �ڱ� �ڽ�
					iw.w1[0] = 1.0f;
				}
				tri.vVertex[iIndex] = v;
				tri.vVertexIW[iIndex] = iw;
			}
			//������׸����� �����ϸ�
			//����޽��� Ʈ���̾ޱ۸���Ʈ�� �ﰢ���� ����ϰ�
			//�������� �ʴ´ٸ� �ٷ� ���� ������Ʈ�� 
			//Ʈ���̾ޱ۸���Ʈ�� �ﰢ���� ������ش�
			if (iNumMtrl > 1)
			{
				//Ʈ���̾ޱ� ����Ʈ�� �ʿ��Ҷ� �ּ� Ǯ��
				//pGraphics->m_SubMeshList[iSubMtrl].m_TriangleList.push_back(tri);
				pGraphics->m_SubMeshList[iSubMtrl].SetUniqueBuffer(tri);
			}
			else
			{
				//pGraphics->m_SubMeshList[0].m_TriangleList.push_back(tri);
				pGraphics->m_SubMeshList[0].SetUniqueBuffer(tri);
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

//�׷��� ���� uv ��ǥ(u,v,w) �� dx�� uv ��ǥ(u,v) �� ��ȯ
void myFbxObj::ReadTextureCoord(FbxMesh * pFbxMesh, FbxLayerElementUV * pUVSet, int iVertexIndex, int iUVIndex, FbxVector2 & uv)
{
	FbxLayerElementUV *pFbxLayerElementUV = pUVSet;

	if (pFbxLayerElementUV == nullptr) return;

	switch (pFbxLayerElementUV->GetMappingMode())
	{
		//������ (��������) ������
		case FbxLayerElementUV::eByControlPoint:
		{
			switch (pFbxLayerElementUV->GetReferenceMode())
			{
				//���� ����
				case FbxLayerElementUV::eDirect:
				{
					FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(iVertexIndex);
					uv.mData[0] = fbxUv.mData[0];
					uv.mData[1] = fbxUv.mData[1];
					break;
				}
				//�ε�����ǥ�� ����
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
		//������� ���� ������
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
	// �븻 ȹ�� 
	FbxVector4 result;
	// �븻 ���͸� ������ ���� 
	switch (vertexNormal->GetMappingMode()) 	// ���� ��� 
	{
		// ������ ���� 1���� ���� ��ǥ�� �ִ�.
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
			// �ε����� ���´�. 
			result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// ���� ���� 1���� ���� ��ǥ�� �ִ�.
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
			// �ε����� ���´�. 
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
