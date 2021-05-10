// myMapDlg.cpp: 구현 파일
//

#include "pch.h"
#include "DemoMFC.h"
#include "myMapDlg.h"
#include "afxdialogex.h"


// myMapDlg 대화 상자

IMPLEMENT_DYNAMIC(myMapDlg, CDialogEx)

myMapDlg::myMapDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_myMapDlg, pParent)
{

}

myMapDlg::~myMapDlg()
{
}

void myMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_TileCount);
	DDX_Control(pDX, IDC_COMBO2, m_CellCount);
	DDX_Control(pDX, IDC_EDIT1, m_CellSize);
}


BEGIN_MESSAGE_MAP(myMapDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &myMapDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// myMapDlg 메시지 처리기


BOOL myMapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_TileCount.InsertString(0,L"1");
	m_TileCount.InsertString(1, L"2");
	m_TileCount.InsertString(2, L"4");
	m_TileCount.InsertString(3, L"8");
	m_TileCount.InsertString(4, L"16");
	m_TileCount.SetCurSel(0);
	m_CellCount.InsertString(0, L"1");
	m_CellCount.InsertString(1, L"2");
	m_CellCount.InsertString(2, L"4");
	m_CellCount.InsertString(3, L"8");
	m_CellCount.InsertString(4, L"16");
	m_CellCount.SetCurSel(0);
	UpdateData(FALSE);
	m_CellSize.SetWindowTextW(L"1");
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void myMapDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strCellCount, strTileCount, strCellSize;
	int iSelTile = m_TileCount.GetCurSel();
	//m_TileCount.GetLBText(iSelTile, strTileCount);
	int iSelCell = m_CellCount.GetCurSel();
	m_CellCount.GetLBText(iSelCell, strCellCount);
	if (strCellSize.IsEmpty())
	{
		strCellSize = L"1";
	}
	m_CellSize.GetWindowTextW(strCellSize);
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (!pApp->m_Sample.m_isCreate)
	{
		myMapDesc desc;
		int iNumCell = _ttoi(strCellCount);
		int iCellSize = _ttoi(strCellSize);
		// 0-1  1-2 2-4  3-8
		pApp->m_Sample.m_QuadTree.m_iMaxdepth = iSelTile;
		int iNumTile = pow(2, iSelTile);
		desc.iNumCols = iNumCell * iNumTile + 1;//m_Map->m_iNumCols;
		desc.iNumRows = iNumCell * iNumTile + 1;//m_Map->m_iNumRows;
		desc.fCellDistance = iCellSize;
		desc.fScaleHeight = 10.0f;
		desc.szTexFile = L"../../data/castle.jpg";
		desc.szVS = L"../../data/shader/BasisVS.txt";
		desc.szPS = L"../../data/shader/BasisPS.txt";
		pApp->m_Sample.m_Map->CreateMap(desc);
		pApp->m_Sample.m_Map->m_isRender = false;


		pApp->m_Sample.m_QuadTree.CreateQuadTree(pApp->m_Sample.m_Map);
		pApp->m_Sample.m_isCreate = true;
	}

	CDialogEx::OnOK();
}
