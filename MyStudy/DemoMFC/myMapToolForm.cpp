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
	ON_BN_CLICKED(IDC_BUTTON3, &myMapToolForm::OnBnClickedTurret)
	ON_BN_CLICKED(IDC_BUTTON4, &myMapToolForm::OnBnClickedBarrel)
	ON_BN_CLICKED(IDC_BUTTON5, &myMapToolForm::OnBnClickedSave)
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
	UpdateData(FALSE);
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
		int iNumCell = pow(2, iSelTile);
		// 0-1  1-2 2-4  3-8
		if (iSelTile > 4) iSelTile = 4;
		pApp->m_Sample.m_QuadTree.m_iMaxdepth = iSelTile;
		int iNumTile = pow(2, iSelTile);
		int iTexSize = 128 * pow(2, iSelTex);
		desc.iNumCols = iNumCell * iNumTile + 1;//m_pMap->m_iNumCols;
		desc.iNumRows = iNumCell * iNumTile + 1;//m_pMap->m_iNumRows;
		desc.fCellDistance = m_iCellSize;
		desc.fScaleHeight = 10.0f;
		if (m_strTexName.IsEmpty()) m_strTexName = L"castle.jpg";
		desc.szTexFile = m_strTexName;
		desc.szVS = L"MapVS.txt";
		desc.szPS = L"MapPS.txt";
		pApp->m_Sample.m_pMap->CreateMap(desc);
		pApp->m_Sample.m_pMap->m_isRender = false;

		pApp->m_Sample.m_QuadTree.CreateQuadTree(pApp->m_Sample.m_pMap);
		pApp->m_Sample.m_isCreate = true;
		pApp->m_Sample.m_pMap->SetMapCBData(iNumCell, iNumTile, m_iCellSize);

		pApp->m_Sample.m_pMapTool = new myMapTool(pApp->m_Sample.m_pMap, &pApp->m_Sample.m_QuadTree);
		pApp->m_Sample.m_pMapTool->Init();
		pApp->m_Sample.m_pMapTool->SetTexture(iTexSize);

		pApp->m_Sample.m_pTopCamera->CreateOrthographic(desc.iNumCols * desc.fCellDistance, desc.iNumRows * desc.fCellDistance, 1.0f, 10000);
	}
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
	if (dlg.DoModal() == IDOK)
	{
		m_strTexName = dlg.GetPathName();
		UpdateData(FALSE);
	}

}


void myMapToolForm::OnBnClickedTurret()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->m_pTargetObject = g_FbxLoader.GetPtr("Turret_Deploy1.fbx")->m_pModelObject;
	}
}


void myMapToolForm::OnBnClickedBarrel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->m_pTargetObject = g_FbxLoader.GetPtr("SM_Barrel.fbx")->m_pModelObject;
	}
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

	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		CFile file;
		vector<CString> vDesc;
		wstring wstrTemp = L"";
		wstrTemp += L" NumTile ";
		wstrTemp = to_wstring(pApp->m_Sample.m_pMap->m_cbMapData.MapData[0]);
		wstrTemp += L"\n";
		CString cstrTemp = wstrTemp.c_str();
		vDesc.push_back(cstrTemp);

		wstrTemp += L" NumCell ";
		wstrTemp = to_wstring(pApp->m_Sample.m_pMap->m_cbMapData.MapData[1]);
		wstrTemp += L"\n";
		cstrTemp = wstrTemp.c_str();
		vDesc.push_back(cstrTemp);

		wstrTemp += L" CellSize ";
		wstrTemp = to_wstring(pApp->m_Sample.m_pMap->m_cbMapData.MapData[2]);
		wstrTemp += L"\n";
		cstrTemp = wstrTemp.c_str();
		vDesc.push_back(cstrTemp);

		file.Open(_T("../../data/test.txt"), CFile::modeCreate | CFile::modeWrite, NULL);
		for (int i = 0; i < vDesc.size(); i++)
		{
			file.Write(vDesc.at(i), vDesc.at(i).GetLength() * sizeof(TCHAR));
		}
		file.Close();
	}

}
