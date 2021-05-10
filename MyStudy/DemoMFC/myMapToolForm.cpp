// myMapToolForm.cpp: 구현 파일
//

#include "pch.h"
#include "DemoMFC.h"
#include "myMapToolForm.h"


// myMapToolForm

IMPLEMENT_DYNCREATE(myMapToolForm, CFormView)

myMapToolForm::myMapToolForm()
	: CFormView(IDD_myMapToolForm)
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
}

BEGIN_MESSAGE_MAP(myMapToolForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &myMapToolForm::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBOCELL, &myMapToolForm::OnCbnSelchangeComboCell)
	ON_CBN_SELCHANGE(IDC_COMBOTILE, &myMapToolForm::OnCbnSelchangeComboTile)
	ON_WM_CREATE()
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
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (!pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_QuadTree.CreateQuadTree(pApp->m_Sample.m_Map);
		pApp->m_Sample.m_isCreate = true;
	}
}


void myMapToolForm::OnCbnSelchangeComboCell()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void myMapToolForm::OnCbnSelchangeComboTile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

int myMapToolForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//m_TileCount.AddString(L"1");
	//m_TileCount.AddString(L"2");
	//m_TileCount.AddString(L"4");
	//m_TileCount.AddString(L"16");
	//m_TileCount.AddString(L"32");
	//m_TileCount.SetCurSel(0);
	//m_CellCount.AddString(L"1");
	//m_CellCount.AddString(L"2");
	//m_CellCount.AddString(L"4");
	//m_CellCount.AddString(L"16");
	//m_CellCount.AddString(L"32");
	//m_CellCount.SetCurSel(0);
	//UpdateData(FALSE);

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}
