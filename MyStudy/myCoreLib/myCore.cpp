#include "myCore.h"
//myCamera*		g_pMainCamera = nullptr;

HRESULT myCore::DeleteDXResource()
{
	//종속적인 리소스 제거
	g_Draw.DeleteDependentResource();
	return E_NOTIMPL;
}

HRESULT myCore::CreateDXResource(UINT w, UINT h)
{
	//백버퍼 초기화
	IDXGISurface1* pBackBuffer = nullptr;
	//백버퍼 가져오기
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&pBackBuffer);
	//리소스들을 백버퍼에 리사이즈해서 단다
	g_Draw.ResizeDevice(w, h, pBackBuffer);
	//백버퍼 제거
	if (pBackBuffer) pBackBuffer->Release();
	return S_OK;
}

bool myCore::ChangeMainCamera(myGameObject * pCameraObj)
{
	if (pCameraObj != nullptr)
	{
		myCamera* camera = pCameraObj->GetComponent<myCamera>();
		if (camera != nullptr)
		{
			m_pMainCameraObj = pCameraObj;
			m_pMainCamera = camera;
			return true;
		}
	}
	return false;
}

bool myCore::GameInit()
{
	PreInit();
	m_isGameRun = true;
	if (myDevice::Init() == false)
	{
		return false;
	}
	SetMode(m_bFullScreen);

	g_Timer.Init();
	g_Input.Init();
	//g_Draw.Init();
	//m_Graphics.Init();
	g_SoundMgr.Init();
	g_ObjMgr.Init();
	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),
		(LPVOID*)&pBackBuffer);
	//dx2d 드로우 세팅
	g_Draw.Set(m_hWnd,
		g_rtClient.right,
		g_rtClient.bottom, pBackBuffer);
	if (pBackBuffer) pBackBuffer->Release();

	m_pBasisLine = new myShapeLine;
	g_GameObject.InsertComponent(m_pBasisLine);
	m_pBasisLine->Init();
	if (!m_pBasisLine->Create(m_pd3dContext, L"vs.txt", L"ps.txt",
	L"../../data/bitmap/flametank.bmp"))
	{
		return false;
	}

	myDebugCamera* pDebugCamera = new myDebugCamera;
	m_pDebugCameraObj = myGameObject::CreateGameObject(L"DebugCamera");
	m_pDebugCameraObj->InsertComponent(pDebugCamera);
	pDebugCamera->Init();
	pDebugCamera->CreateFrustum(m_pd3dContext);
	//g_ObjMgr.CreateObjComponent(L"MainCamera", m_pDebugCamera);

	pDebugCamera->CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	pDebugCamera->CreateProjMatrix(1, 1000, PI2D, fAspect);

	ChangeMainCamera(m_pDebugCameraObj);

	Init();
	PostInit();
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	return true;
}

bool myCore::GameFrame()
{
	PreFrame();
	g_Timer.Frame();
	g_Input.Frame();
	//m_Graphics.Frame();
	g_ObjMgr.Frame();
	g_SoundMgr.Frame();
	Frame();
	g_Draw.Frame();
	CameraFrame();
	PostFrame();
	return true;
}

void myCore::CameraFrame()
{
	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_pMainCamera->FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_pMainCamera->FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_pMainCamera->RightMovement(-1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_pMainCamera->RightMovement(1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_pMainCamera->UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_pMainCamera->UpMovement(-1.0f);
	}
	m_pMainCameraObj->Frame();
}

bool myCore::PreRender()
{
	myDevice::PreRender();
	//m_Graphics.PreRender();
	return true;
}

bool myCore::GameRender()
{
	if (PreRender() == false) return false;
	if (Render() == false) return false;
	if (PostRender() == false) return false;
	return true;
}

bool myCore::PostRender()
{
	m_pBasisLine->SetMatrix(NULL, &m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_pBasisLine->Draw(Vector3(0, 0, 0), Vector3(50, 0, 0), Vector4(1, 0, 0, 1));
	m_pBasisLine->Draw(Vector3(0, 0, 0), Vector3(0, 50, 0), Vector4(0, 1, 0, 1));
	m_pBasisLine->Draw(Vector3(0, 0, 0), Vector3(0, 0, 50), Vector4(0, 0, 1, 1));
	g_Timer.Render();
	g_Input.Render();
	g_ObjMgr.Render();
	g_SoundMgr.Render();
	g_Draw.Render();
	g_Draw.Draw(0, 0, g_Timer.m_szBuffer);
	myDevice::PostRender();
	//m_Graphics.PostRender();
	return true;
}

bool myCore::GameRun()
{
	if (GameFrame() == false) return false;
	if (GameRender() == false) return false;
	return true;
}

bool myCore::Run()
{
	//HeapEnableTerminateOnCorruption 옵션을 설정하면 손상된 힙을 이용하는 보안 악용에 대한 애플리케이션의 노출을 줄일 수 있으므로 강력하게 권장.
	//HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	HRESULT hr = S_OK;
	//CoInitializeEx 는 COM 라이브러리를 사용하는 각 스레드에 대해 한 번 이상 호출해야  한다.
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	GameInit();
	while (m_isGameRun)
	{
		if (MsgProcess() == true)
		{
			GameRun();
		}
		else
		{
			m_isGameRun = false;
		}
	}
	GameRelease();
	//이니셜라이즈 해제
	CoUninitialize();
	return true;
}

myCore::myCore()
{
	m_pMainCamera = nullptr;
}

myCore::~myCore()
{
}

bool myCore::GameRelease()
{
	Release();
	m_pDebugCameraObj->Release();
	delete m_pDebugCameraObj;
	g_Draw.Release();
	g_Timer.Release();
	g_Input.Release();
	g_ObjMgr.Release();
	g_SoundMgr.Release();
	myDevice::Release();
	//m_Graphics.Release();
	return true;
}