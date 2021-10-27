// myMapControlForm.cpp: 구현 파일
//

#include "pch.h"
#include "DemoMFC.h"
#include "myMapControlForm.h"


// myMapControlForm

IMPLEMENT_DYNCREATE(myMapControlForm, CFormView)

myMapControlForm::myMapControlForm()
	: CFormView(IDD_myMapControlForm)
	, m_fOutRadius(0)
	, m_fSpeed(0)
	, m_fInRadius(0)
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
	DDX_Text(pDX, IDC_EDIT3, m_fOutRadius);
	DDX_Text(pDX, IDC_EDIT4, m_fSpeed);
	DDX_Control(pDX, IDC_PIC, m_PictureControl);
	DDX_Text(pDX, IDC_EDIT6, m_fInRadius);
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
	ON_BN_CLICKED(IDC_BUTTON3, &myMapControlForm::OnBnClickedBarrel)
	ON_BN_CLICKED(IDC_BUTTON4, &myMapControlForm::OnBnClickedTurret)
	ON_BN_CLICKED(IDC_BUTTON13, &myMapControlForm::OnBnClickedMove)
	ON_BN_CLICKED(IDC_BUTTON14, &myMapControlForm::OnBnClickedTurn)
	ON_BN_CLICKED(IDC_BUTTON15, &myMapControlForm::OnBnClickedScale)
	ON_BN_CLICKED(IDC_BUTTON16, &myMapControlForm::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON17, &myMapControlForm::OnBnClickedSplatErase)
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
	UpdateData(TRUE);
	if (m_fInRadius > m_fOutRadius) m_fInRadius = m_fOutRadius;
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(100);
		pApp->m_Sample.m_pMapTool->m_fSpeed = m_fSpeed;
		pApp->m_Sample.m_pMapTool->m_vBrushRad.x = m_fOutRadius;
		pApp->m_Sample.m_pMapTool->m_vBrushRad.y = m_fInRadius;
	}
	UpdateData(FALSE);
}


void myMapControlForm::OnBnClickedTerrainDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_fInRadius > m_fOutRadius) m_fInRadius = m_fOutRadius;
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(101);
		pApp->m_Sample.m_pMapTool->m_fSpeed = m_fSpeed;
		pApp->m_Sample.m_pMapTool->m_vBrushRad.x = m_fOutRadius;
		pApp->m_Sample.m_pMapTool->m_vBrushRad.y = m_fInRadius;
	}
	UpdateData(FALSE);
}


void myMapControlForm::OnBnClickedTerrainFlat()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_fInRadius > m_fOutRadius) m_fInRadius = m_fOutRadius;
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(102);
		pApp->m_Sample.m_pMapTool->m_fSpeed = m_fSpeed;
		pApp->m_Sample.m_pMapTool->m_vBrushRad.x = m_fOutRadius;
		pApp->m_Sample.m_pMapTool->m_vBrushRad.y = m_fInRadius;
	}
	UpdateData(FALSE);
}


void myMapControlForm::OnBnClickedUpdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_fInRadius > m_fOutRadius) m_fInRadius = m_fOutRadius;
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->m_fSpeed = m_fSpeed;
		pApp->m_Sample.m_pMapTool->m_vBrushRad.x = m_fOutRadius;
		pApp->m_Sample.m_pMapTool->m_vBrushRad.y = m_fInRadius;
	}
	UpdateData(FALSE);
}


void myMapControlForm::OnBnClickedSplat1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_fInRadius > m_fOutRadius) m_fInRadius = m_fOutRadius;
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->m_vBrushRad.x = m_fOutRadius;
		pApp->m_Sample.m_pMapTool->m_vBrushRad.y = m_fInRadius;
		pApp->m_Sample.m_pMapTool->SetMode(200);
		pApp->m_Sample.m_pMapTool->m_eMakingMode = TOOL_SPLAT;
		pApp->m_Sample.m_pMapTool->m_eSplatType = SPLAT_TEX_01;
		CRect rt;
		m_PictureControl.GetWindowRect(rt);
		CDC* dc;
		dc = m_PictureControl.GetDC();
		CImage image;
		CString strPath = DataFolderPath;
		strPath += "object/tile1.jpg";
		image.Load(strPath);
		image.StretchBlt(dc->m_hDC, 0, 0, rt.Width(), rt.Height(), SRCCOPY);
		ReleaseDC(dc);
	}
	UpdateData(FALSE);
}


void myMapControlForm::OnBnClickedSplat2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_fInRadius > m_fOutRadius) m_fInRadius = m_fOutRadius;
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->m_vBrushRad.x = m_fOutRadius;
		pApp->m_Sample.m_pMapTool->m_vBrushRad.y = m_fInRadius;
		pApp->m_Sample.m_pMapTool->SetMode(201);
		pApp->m_Sample.m_pMapTool->m_eMakingMode = TOOL_SPLAT;
		pApp->m_Sample.m_pMapTool->m_eSplatType = SPLAT_TEX_02;
		CRect rt;
		m_PictureControl.GetWindowRect(rt);
		CDC* dc;
		dc = m_PictureControl.GetDC();
		CImage image;
		CString strPath = DataFolderPath;
		strPath += "object/tile2.jpg";
		image.Load(strPath);
		image.StretchBlt(dc->m_hDC, 0, 0, rt.Width(), rt.Height(), SRCCOPY);
		ReleaseDC(dc);
	}
	UpdateData(FALSE);
}


void myMapControlForm::OnBnClickedSplat3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_fInRadius > m_fOutRadius) m_fInRadius = m_fOutRadius;
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->m_vBrushRad.x = m_fOutRadius;
		pApp->m_Sample.m_pMapTool->m_vBrushRad.y = m_fInRadius;
		pApp->m_Sample.m_pMapTool->SetMode(202);
		pApp->m_Sample.m_pMapTool->m_eMakingMode = TOOL_SPLAT;
		pApp->m_Sample.m_pMapTool->m_eSplatType = SPLAT_TEX_03;
		CRect rt;
		m_PictureControl.GetWindowRect(rt);
		CDC* dc;
		dc = m_PictureControl.GetDC();
		CImage image;
		CString strPath = DataFolderPath;
		strPath += "object/tile3.jpg";
		image.Load(strPath);
		image.StretchBlt(dc->m_hDC, 0, 0, rt.Width(), rt.Height(), SRCCOPY);
		ReleaseDC(dc);
	}
	UpdateData(FALSE);
}


void myMapControlForm::OnBnClickedSplat4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_fInRadius > m_fOutRadius) m_fInRadius = m_fOutRadius;
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->m_vBrushRad.x = m_fOutRadius;
		pApp->m_Sample.m_pMapTool->m_vBrushRad.y = m_fInRadius;
		pApp->m_Sample.m_pMapTool->SetMode(203);
		pApp->m_Sample.m_pMapTool->m_eMakingMode = TOOL_SPLAT;
		pApp->m_Sample.m_pMapTool->m_eSplatType = SPLAT_TEX_04;
		CRect rt;
		m_PictureControl.GetWindowRect(rt);
		CDC* dc;
		dc = m_PictureControl.GetDC();
		CImage image;
		CString strPath = DataFolderPath;
		strPath += "object/flagstone.bmp";
		image.Load(strPath);
		image.StretchBlt(dc->m_hDC, 0, 0, rt.Width(), rt.Height(), SRCCOPY);
		ReleaseDC(dc);
	}
	UpdateData(FALSE);
}

void myMapControlForm::OnBnClickedSplatErase()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_fInRadius > m_fOutRadius) m_fInRadius = m_fOutRadius;
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->m_vBrushRad.x = m_fOutRadius;
		pApp->m_Sample.m_pMapTool->m_vBrushRad.y = m_fInRadius;
		pApp->m_Sample.m_pMapTool->SetMode(299);
		pApp->m_Sample.m_pMapTool->m_eMakingMode = TOOL_SPLAT;
		pApp->m_Sample.m_pMapTool->m_eSplatType = SPLAT_TEX_NONE;
		CRect rt;
		m_PictureControl.GetWindowRect(rt);
		CDC* dc;
		dc = m_PictureControl.GetDC();
		CImage image;
		CString strPath = DataFolderPath;
		strPath += "object/white.png";
		image.Load(strPath);
		image.StretchBlt(dc->m_hDC, 0, 0, rt.Width(), rt.Height(), SRCCOPY);
		ReleaseDC(dc);
	}
	UpdateData(FALSE);
}


void myMapControlForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_fOutRadius = 20.0f;
	m_fInRadius = 10.0f;
	m_fSpeed = 0.1f;
	UpdateData(FALSE);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


//void myMapToolForm::OnBnClickedTurret()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
//	if (pApp->m_Sample.m_isCreate)
//	{
//		pApp->m_Sample.m_pMapTool->m_pTargetObject = g_FbxLoader.GetPtr("Turret_Deploy1.fbx")->m_pModelObject;
//		pApp->m_Sample.m_pMapTool->SetMode(303);
//	}
//}
//
//
//void myMapToolForm::OnBnClickedBarrel()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
//	if (pApp->m_Sample.m_isCreate)
//	{
//		pApp->m_Sample.m_pMapTool->m_pTargetObject = g_FbxLoader.GetPtr("SM_Barrel.fbx")->m_pModelObject;
//		pApp->m_Sample.m_pMapTool->SetMode(303);
//	}
//}

void myMapControlForm::OnBnClickedBarrel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->m_pTargetObject = g_FbxLoader.GetPtr("SM_Barrel.fbx")->m_pModelObject;
		pApp->m_Sample.m_pMapTool->SetMode(303);
	}
}


void myMapControlForm::OnBnClickedTurret()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->m_pTargetObject = g_FbxLoader.GetPtr("Turret_Deploy1.fbx")->m_pModelObject;
		pApp->m_Sample.m_pMapTool->SetMode(303);
	}
}


void myMapControlForm::OnBnClickedMove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(300);
	}
}


void myMapControlForm::OnBnClickedTurn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(302);
	}
}


void myMapControlForm::OnBnClickedScale()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(301);
	}
}


void myMapControlForm::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDemoMFCApp* pApp = (CDemoMFCApp*)AfxGetApp();
	if (pApp->m_Sample.m_isCreate)
	{
		pApp->m_Sample.m_pMapTool->SetMode(304);
	}
}



