// myMapToolForm.cpp: 구현 파일
//

#include "pch.h"
#include "DemoMFC.h"
#include "myMapToolForm.h"


// myMapToolForm

IMPLEMENT_DYNCREATE(myMapToolForm, CFormView)

myMapToolForm::myMapToolForm()
	: CFormView(IDD_myMapToolForm)
	, m_iCellSize(0)
	, m_strTexName(_T(""))
{

}

myMapToolForm::~myMapToolForm()
{
}

myMapToolForm * myMapToolForm::CreateOnce(CWnd * pParent)
{
	myMapToolForm* pForm = new myMapToolForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 100),
		pParent, 0, NULL);

	return pForm;
}

void myMapToolForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOTILE, m_TileCount);
	DDX_Control(pDX, IDC_COMBOCELL, m_CellCount);
	DDX_Text(pDX, IDC_EDIT1, m_iCellSize);

	DDX_Text(pDX, IDC_EDIT2, m_strTexName);

	DDX_Control(pDX, IDC_COMBOTEXSIZE, m_TexSize);
}

BEGIN_MESSAGE_MAP(myMapToolForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &myMapToolForm::OnBnClickedButton1)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON2, &myMapToolForm::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &myMapToolForm::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON6, &myMapToolForm::OnBnClickedLoad)
END_MESSAGE_MAP()


// myMapToolForm 진단

#ifdef _DEBUG
void myMapToolForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void myMapToolForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// myMapToolForm 메시지 처리기


void myMapToolForm::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strCellCount, strTileCount;
	int iSelTile = m_TileCount.GetCurSel();
	//m_TileCount.GetLBText(iSelTile, strTileCount);
	int iSelCell = m_CellCount.GetCurSel();
	int iSelTex = m_TexSize.GetCurSel();
	m_CellCount.GetLBText(iSelCell, strCellCount);
	if (m_iCellSize <= 0)
	{
		m_iCellSize = 1;
	}
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (!pApp->m_Sample.m_isCreate)
	{
		myMapDesc desc;
		//int iNumCell = _ttoi(strCellCount);
		int iNumTile = pow(2, iSelTile);
		int iNumCell = pow(2, iSelCell);
		int iTexSize = 128 * pow(2, iSelTex);
		int iSpaceDivision = iSelTile > 4 ? 4 : iSelTile;
		// 0-1  1-2 2-4  3-8
		pApp->m_Sample.m_QuadTree.m_iMaxdepth = iSpaceDivision;
		desc.iNumCols = iNumCell * iNumTile + 1;//m_pMap->m_iNumCols;
		desc.iNumRows = iNumCell * iNumTile + 1;//m_pMap->m_iNumRows;
		desc.fCellDistance = m_iCellSize;
		desc.fScaleHeight = 10.0f;
		if (m_strTexName.IsEmpty()) m_strTexName = L"main.png";
		desc.szTexFile = m_strTexName;
		desc.szVS = L"MapVS.hlsl";
		desc.szPS = L"MapPS.hlsl";
		pApp->m_Sample.m_pMap->CreateMap(desc);
		pApp->m_Sample.m_pMap->m_isRender = false;

		pApp->m_Sample.m_QuadTree.CreateQuadTree(pApp->m_Sample.m_pMap);
		pApp->m_Sample.m_isCreate = true;
		pApp->m_Sample.m_pMap->SetMapCBData(iNumCell, iNumTile, m_iCellSize, iSpaceDivision);

		pApp->m_Sample.m_pMapTool = new myMapTool(
			pApp->m_Sample.m_pMap,
			&pApp->m_Sample.m_QuadTree);
		pApp->m_Sample.m_pMapTool->Init();
		pApp->m_Sample.m_pMapTool->CreateTex(iTexSize);
		//pApp->m_Sample.m_pMapTool->m_NormalTex.Create(iTexSize);
		//pApp->m_Sample.m_pMapTool->m_HeightTex.Create(iTexSize);

		pApp->m_Sample.m_pTopCamera->CreateOrthographic(
			desc.iNumCols * desc.fCellDistance, 
			desc.iNumRows * desc.fCellDistance, 1.0f, 10000);
	}
	UpdateData(FALSE);
}

int myMapToolForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void myMapToolForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_TileCount.InsertString(0, L"1");
	m_TileCount.InsertString(1, L"2");
	m_TileCount.InsertString(2, L"4");
	m_TileCount.InsertString(3, L"8");
	m_TileCount.InsertString(4, L"16");
	m_TileCount.InsertString(5, L"32");
	m_TileCount.SetCurSel(3);
	m_CellCount.InsertString(0, L"1");
	m_CellCount.InsertString(1, L"2");
	m_CellCount.InsertString(2, L"4");
	m_CellCount.InsertString(3, L"8");
	m_CellCount.InsertString(4, L"16");
	m_TileCount.InsertString(5, L"32");
	m_CellCount.SetCurSel(3);

	m_TexSize.InsertString(0, L"128");
	m_TexSize.InsertString(1, L"256");
	m_TexSize.InsertString(2, L"512");
	m_TexSize.InsertString(3, L"1024");
	m_TexSize.InsertString(4, L"2048");
	m_TexSize.InsertString(5, L"4096");
	m_TexSize.SetCurSel(3);
	m_iCellSize = 5;

	UpdateData(FALSE);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void myMapToolForm::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("이미지 파일(*.BMP, *.JPEG, *.JPG) | *.bmp;*.jpeg;*.jpg | 모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);
	UpdateData(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		m_strTexName = dlg.GetPathName();
		
	}
	UpdateData(FALSE);
}

void myMapToolForm::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//static TCHAR BASED_CODE szFilter[] =
	//	_T("이미지 파일(*.BMP, *.JPEG, *.JPG) | *.bmp;*.jpeg;*.jpg | 모든파일(*.*)|*.*||");
	//CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);
	//if (dlg.DoModal() == IDOK)
	//{
	//	m_strTexName = dlg.GetPathName();
	//	UpdateData(FALSE);
	//}

	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("파일이 저장될 폴더를 선택하세요");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);				// 파일경로 읽어오기

	wstring wstrFilePath;
	wstrFilePath = szBuffer;
	wstrFilePath += L"\\";

	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		CFile file;
		vector<CString> vecDesc;
		wstring wstrTemp = L"";
		wstring wstrFileName = L"SaveMap";

		//wstrTemp = L"NumTile ";
		int iNumTemp = FLOAT_TO_INT(pApp->m_Sample.m_pMap->m_cbMapData.MapData[0]);
		wstrTemp += to_wstring(iNumTemp);
		wstrTemp += L"\n";
		CString cstrTemp = wstrTemp.c_str();
		vecDesc.push_back(cstrTemp);

		wstrTemp.clear();
		//wstrTemp = L"NumCell ";
		iNumTemp = FLOAT_TO_INT(pApp->m_Sample.m_pMap->m_cbMapData.MapData[1]);
		wstrTemp += to_wstring(iNumTemp);
		wstrTemp += L"\n";
		cstrTemp = wstrTemp.c_str();
		vecDesc.push_back(cstrTemp);

		wstrTemp.clear();
		//wstrTemp = L"CellSize ";
		iNumTemp = FLOAT_TO_INT(pApp->m_Sample.m_pMap->m_cbMapData.MapData[2]);
		wstrTemp += to_wstring(iNumTemp);
		wstrTemp += L"\n";
		cstrTemp = wstrTemp.c_str();
		vecDesc.push_back(cstrTemp);

		wstrTemp.clear();
		//wstrTemp = L"SpaceDivision ";
		iNumTemp = FLOAT_TO_INT(pApp->m_Sample.m_pMap->m_cbMapData.MapData[3]);
		wstrTemp += to_wstring(iNumTemp);
		wstrTemp += L"\n";
		cstrTemp = wstrTemp.c_str();
		vecDesc.push_back(cstrTemp);

		wstrTemp.clear();
		//wstrTemp = L"MapTexName ";
		wstrTemp += pApp->m_Sample.m_pMap->m_MapDesc.szTexFile;
		wstrTemp += L"\n";
		cstrTemp = wstrTemp.c_str();
		vecDesc.push_back(cstrTemp);

		wstrTemp.clear();
		//wstrTemp = L"TexSize ";
		iNumTemp = pApp->m_Sample.m_pMapTool->m_iTexSizeX;
		wstrTemp += to_wstring(iNumTemp);
		wstrTemp += L"\n";
		cstrTemp = wstrTemp.c_str();
		vecDesc.push_back(cstrTemp);

		wstrTemp.clear();
		//wstrTemp = L"HeightMapPath ";
		wstrTemp += wstrFilePath;
		wstrTemp += wstrFileName;
		wstrTemp += L"_HeightMap.dds";
		DirectX::SaveDDSTextureToFile(g_pImmediateContext,
			pApp->m_Sample.m_pMapTool->m_HeightTex.m_pTexture.Get(),
			wstrTemp.c_str());
		wstrTemp += L"\n";
		cstrTemp = wstrTemp.c_str();
		vecDesc.push_back(cstrTemp);

		wstrTemp.clear();
		//wstrTemp = L"AlphaMaskMapPath ";
		wstrTemp += wstrFilePath;
		wstrTemp += wstrFileName;
		wstrTemp += L"_AlphaMask.dds";
		DirectX::SaveDDSTextureToFile(g_pImmediateContext,
			pApp->m_Sample.m_pMapTool->m_SplatCS.m_pTexture.Get(),
			wstrTemp.c_str());
		//DirectX::SaveDDSTextureToFile(g_pImmediateContext,
		//	pApp->m_Sample.m_pMapTool->m_NormalTex.m_pTexture.Get(),
		//	wstrTemp.c_str());
		wstrTemp += L"\n";
		cstrTemp = wstrTemp.c_str();
		vecDesc.push_back(cstrTemp);

		if (pApp->m_Sample.m_pMapTool->InstanceList.size() > 0)
		{
			wstrTemp.clear();
			wstrTemp += L"InstanceStart\n";
			cstrTemp = wstrTemp.c_str();
			vecDesc.push_back(cstrTemp);

			for (int i = 0; i < pApp->m_Sample.m_pMapTool->InstanceList.size(); i++)
			{
				wstrTemp.clear();
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->iID);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->iNodeIndex);
				wstrTemp += L"/";
				wstrTemp += to_wstring((int)pApp->m_Sample.m_pMapTool->InstanceList[i]->isActive);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->vPos.x);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->vPos.y);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->vPos.z);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->vScale.x);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->vScale.y);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->vScale.z);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->qRot.x);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->qRot.y);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->qRot.z);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->qRot.w);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->SphereCollider.vCenter.x);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->SphereCollider.vCenter.y);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->SphereCollider.vCenter.z);
				wstrTemp += L"/";
				wstrTemp += to_wstring(pApp->m_Sample.m_pMapTool->InstanceList[i]->SphereCollider.fRadius);
				wstrTemp += L"\n";
				cstrTemp = wstrTemp.c_str();
				vecDesc.push_back(cstrTemp);
			}

			wstrTemp.clear();
			wstrTemp += L"InstanceEnd\n";
			cstrTemp = wstrTemp.c_str();
			vecDesc.push_back(cstrTemp);
		}


		//wstrTemp = DataFolderPath;
		//wstrTemp += L"save/";

		wstrTemp = wstrFilePath;
		wstrTemp += wstrFileName;
		wstrTemp += L".txt";
		file.Open(wstrTemp.c_str(), CFile::modeCreate | CFile::modeWrite, NULL);
		for (int i = 0; i < vecDesc.size(); i++)
		{
			file.Write(vecDesc.at(i), vecDesc.at(i).GetLength() * sizeof(TCHAR));
		}
		file.Close();
	}

}


void myMapToolForm::OnBnClickedLoad()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strSaveFile = L"";
	static TCHAR BASED_CODE szFilter[] =
		_T("텍스트 파일(*.txt)|*.txt|모든파일(*.*)|*.*|");
	CFileDialog dlg(TRUE, _T("(*.txt)"),NULL , 
		OFN_FILEMUSTEXIST |
		OFN_PATHMUSTEXIST | 
		OFN_HIDEREADONLY
		, szFilter);
	if (dlg.DoModal() == IDOK)
	{
		strSaveFile = dlg.GetPathName();


		CFile file;
		std::vector<CString> vecDesc;
		file.Open(strSaveFile, CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		CString cstrTemp;
		while (ar.ReadString(cstrTemp))
		{
			vecDesc.push_back(cstrTemp);
		}
		ar.Close();
		file.Close();


		CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
		if (!pApp->m_Sample.m_isCreate)
		{
			myMapDesc desc;

			//int iSpaceDivision = iSelTile > 4 ? 4 : iSelTile;

			//int iNumCell = _ttoi(strCellCount);
			int iNumTile = _ttoi(vecDesc[0]);
			int iNumCell = _ttoi(vecDesc[1]);
			int iCellSize = _ttoi(vecDesc[2]);
			int iSpaceDivision = _ttoi(vecDesc[3]);
			int iTexSize = _ttoi(vecDesc[5]);
			// 0-1  1-2 2-4  3-8
			pApp->m_Sample.m_QuadTree.m_iMaxdepth = iSpaceDivision;
			desc.iNumCols = iNumCell * iNumTile + 1;//m_pMap->m_iNumCols;
			desc.iNumRows = iNumCell * iNumTile + 1;//m_pMap->m_iNumRows;
			desc.fCellDistance = iCellSize;
			desc.fScaleHeight = 1.0f;
			if (m_strTexName.IsEmpty()) m_strTexName = L"main.png";
			desc.szTexFile = vecDesc[4];
			desc.szVS = L"MapVS.txt";
			desc.szPS = L"MapPS.txt";

			pApp->m_Sample.m_pMapTool = new myMapTool(pApp->m_Sample.m_pMap,
				&pApp->m_Sample.m_QuadTree);

			pApp->m_Sample.m_pMapTool->Init();
			TCHAR* szTexPath = (TCHAR*)(LPCTSTR)vecDesc[6];
			pApp->m_Sample.m_pMapTool->m_HeightTex.LoadTexture(szTexPath);
			szTexPath = (TCHAR*)(LPCTSTR)vecDesc[7];
			//pApp->m_Sample.m_pMapTool->m_NormalTex.LoadTexture(szTexPath);
			pApp->m_Sample.m_pMapTool->m_SplatCS.CreateFromTexFile(szTexPath);
			pApp->m_Sample.m_pMapTool->CreateTex(iTexSize);

			if (vecDesc.size() >= 8 && vecDesc[8] == "InstanceStart")
			{
				int iIndex = 8;
				while (true)
				{
					iIndex++;
					if (vecDesc[iIndex] == "InstanceEnd") break;
					vector<CString> InstanceData;
					CString tempString;
					int numP = 0;
					while (!vecDesc[iIndex].IsEmpty())
					{
						tempString = vecDesc[iIndex].Tokenize(_T("/"), numP);
						if (tempString.IsEmpty()) break;
						InstanceData.push_back(tempString);
					}
					SampleIns* ins = new SampleIns;
					ins->iID = _ttoi(InstanceData[0]);
					ins->iNodeIndex = _ttoi(InstanceData[1]);
					ins->isActive = _ttoi(InstanceData[2]);
					ins->vPos.x = _ttof(InstanceData[3]);
					ins->vPos.y = _ttof(InstanceData[4]);
					ins->vPos.z = _ttof(InstanceData[5]);
					ins->vScale.x = _ttof(InstanceData[6]);
					ins->vScale.y = _ttof(InstanceData[7]);
					ins->vScale.z = _ttof(InstanceData[8]);
					ins->qRot.x = _ttof(InstanceData[9]);
					ins->qRot.y = _ttof(InstanceData[10]);
					ins->qRot.z = _ttof(InstanceData[11]);
					ins->qRot.w = _ttof(InstanceData[12]);
					ins->SphereCollider.vCenter.x = _ttof(InstanceData[13]);
					ins->SphereCollider.vCenter.y = _ttof(InstanceData[14]);
					ins->SphereCollider.vCenter.z = _ttof(InstanceData[15]);
					ins->SphereCollider.fRadius = _ttof(InstanceData[16]);
					pApp->m_Sample.m_pMapTool->InstanceList.push_back(ins);
				}
			}

			pApp->m_Sample.m_pMap->SetMapCBData(iNumCell, iNumTile, m_iCellSize, iSpaceDivision);
			//높이맵
			pApp->m_Sample.m_pMap->CreateHeightMap(
				pApp->m_Sample.m_pMapTool->m_HeightTex.m_pStaging.Get(), desc,
				pApp->m_Sample.m_pMap->m_cbMapData);

			pApp->m_Sample.m_pMap->m_isRender = false;
			pApp->m_Sample.m_QuadTree.CreateQuadTree(pApp->m_Sample.m_pMap);
			pApp->m_Sample.m_isCreate = true;

			for (int i = 0; i < pApp->m_Sample.m_pMapTool->InstanceList.size(); i++)
			{
				pApp->m_Sample.m_QuadTree.m_LeafNodeList[
					pApp->m_Sample.m_pMapTool->InstanceList[i]->iNodeIndex]->
					m_ObjectList.push_back(pApp->m_Sample.m_pMapTool->InstanceList[i]);
			}

			pApp->m_Sample.m_pTopCamera->CreateOrthographic(
				desc.iNumCols * desc.fCellDistance,
				desc.iNumRows * desc.fCellDistance, 1.0f, 10000);
		}
	}
	UpdateData(FALSE);
}
