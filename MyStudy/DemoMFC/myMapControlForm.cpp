// myMapControlForm.cpp: 구현 파일
//

#include "pch.h"
#include "DemoMFC.h"
#include "myMapControlForm.h"


// myMapControlForm

IMPLEMENT_DYNCREATE(myMapControlForm, CFormView)

myMapControlForm::myMapControlForm()
	: CFormView(IDD_myMapControlForm)
	, m_fRadius(0)
	, m_fSpeed(0)
{

}

myMapControlForm::~myMapControlForm()
{
}

myMapControlForm * myMapControlForm::CreateOnce(CWnd * pParent)
{
	myMapControlForm* pForm = new myMapControlForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 100),
		pParent, 0, NULL);

	return pForm;
}

void myMapControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_fRadius);
	DDX_Text(pDX, IDC_EDIT4, m_fSpeed);
}

BEGIN_MESSAGE_MAP(myMapControlForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON5, &myMapControlForm::OnBnClickedTerrainUp)
	ON_BN_CLICKED(IDC_BUTTON6, &myMapControlForm::OnBnClickedTerrainDown)
	ON_BN_CLICKED(IDC_BUTTON7, &myMapControlForm::OnBnClickedTerrainFlat)
	ON_BN_CLICKED(IDC_BUTTON8, &myMapControlForm::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_BUTTON9, &myMapControlForm::OnBnClickedSplat1)
	ON_BN_CLICKED(IDC_BUTTON10, &myMapControlForm::OnBnClickedSplat2)
	ON_BN_CLICKED(IDC_BUTTON11, &myMapControlForm::OnBnClickedSplat3)
	ON_BN_CLICKED(IDC_BUTTON12, &myMapControlForm::OnBnClickedSplat4)
END_MESSAGE_MAP()


// myMapControlForm 진단

#ifdef _DEBUG
void myMapControlForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void myMapControlForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// myMapControlForm 메시지 처리기


void myMapControlForm::OnBnClickedTerrainUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(100);
		pApp->m_Sample.m_pMapTool->m_fSpeed = m_fSpeed;
		pApp->m_Sample.m_pMapTool->m_fOutRad = m_fRadius;
	}
}


void myMapControlForm::OnBnClickedTerrainDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(101);
		pApp->m_Sample.m_pMapTool->m_fSpeed = m_fSpeed;
		pApp->m_Sample.m_pMapTool->m_fOutRad = m_fRadius;
	}
}


void myMapControlForm::OnBnClickedTerrainFlat()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);

	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(102);
		pApp->m_Sample.m_pMapTool->m_fSpeed = m_fSpeed;
		pApp->m_Sample.m_pMapTool->m_fOutRad = m_fRadius;
	}
}


void myMapControlForm::OnBnClickedUpdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->m_fSpeed = m_fSpeed;
		pApp->m_Sample.m_pMapTool->m_fOutRad = m_fRadius;
	}
}


void myMapControlForm::OnBnClickedSplat1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(200);
		pApp->m_Sample.m_pMapTool->m_eMakingMode = TOOL_SPLAT;
		pApp->m_Sample.m_pMapTool->m_eSplatType = SPLAT_TEX_01;
	}
}


void myMapControlForm::OnBnClickedSplat2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(201);
		pApp->m_Sample.m_pMapTool->m_eMakingMode = TOOL_SPLAT;
		pApp->m_Sample.m_pMapTool->m_eSplatType = SPLAT_TEX_02;
	}
}


void myMapControlForm::OnBnClickedSplat3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(202);
		pApp->m_Sample.m_pMapTool->m_eMakingMode = TOOL_SPLAT;
		pApp->m_Sample.m_pMapTool->m_eSplatType = SPLAT_TEX_03;
	}
}


void myMapControlForm::OnBnClickedSplat4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(203);
		pApp->m_Sample.m_pMapTool->m_eMakingMode = TOOL_SPLAT;
		pApp->m_Sample.m_pMapTool->m_eSplatType = SPLAT_TEX_04;
	}
}


void myMapControlForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_fRadius = 20.0f;
	m_fSpeed = 0.1f;
	UpdateData(FALSE);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
