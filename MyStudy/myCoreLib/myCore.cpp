#include "myCore.h"

HRESULT myCore::DeleteDXResource()
{
	//종속적인 리소스 제거
	g_Draw.DeleteDependentResource();
	return S_OK;
}

HRESULT myCore::CreateDXResource(UINT w, UINT h)
{
	//백버퍼 초기화
	ComPtr<IDXGISurface1> pBackBuffer = nullptr;
	//백버퍼 가져오기
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)pBackBuffer.GetAddressOf());
	//리소스들을 백버퍼에 리사이즈해서 단다
	g_Draw.ResizeDevice(w, h, pBackBuffer.Get());
	//백버퍼 제거
	return S_OK;
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
	g_SoundMgr.Init();
	//g_CamMgr.Init();
	//g_ObjMgr.Init();
	ComPtr<IDXGISurface1> pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),
		(LPVOID*)pBackBuffer.GetAddressOf());
	//dx2d 드로우 세팅
	g_Draw.Set(m_hWnd,
		g_rtClient.right,
		g_rtClient.bottom, pBackBuffer.Get());

	//m_pBasisLine = new myShapeLine;
	//g_GameObject.InsertComponent(m_pBasisLine);
	//m_pBasisLine->Init();
	//if (!m_pBasisLine->Create(L"vs.txt", L"ps.txt",
	//L"../../data/bitmap/flametank.bmp"))
	//{
	//	return false;
	//}

	myDebugCamera* pDebugCamera = new myDebugCamera;
	myGameObject* obj = g_CamMgr.CreateCameraObj(L"DebugCamera", pDebugCamera);
	g_CamList.GetGameObject(L"DebugCamera")->InsertComponent(new myController);
	pDebugCamera->CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	pDebugCamera->CreateProjMatrix(1, 1000, PI2D, fAspect);

	g_CamMgr.SetMainCamera(obj);

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
	g_SoundMgr.Frame();
	Frame();
	g_ObjMgr.Frame();
	g_CamMgr.Frame();
	PostFrame();
	return true;
}

bool myCore::PreRender()
{
	myDevice::PreRender();
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
	//m_pBasisLine->m_pTransform->SetMatrix(NULL, 
	//	&g_pMainCamTransform->m_matView,
	//	&g_pMainCamTransform->m_matProj);
	//m_pBasisLine->Draw(Vector3(0, 0, 0), Vector3(50, 0, 0), Vector4(1, 0, 0, 1));
	//m_pBasisLine->Draw(Vector3(0, 0, 0), Vector3(0, 50, 0), Vector4(0, 1, 0, 1));
	//m_pBasisLine->Draw(Vector3(0, 0, 0), Vector3(0, 0, 50), Vector4(0, 0, 1, 1));
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

}

myCore::~myCore()
{
}

bool myCore::GameRelease()
{
	Release();
	g_CamMgr.Release();
	g_Draw.Release();
	g_Timer.Release();
	g_Input.Release();
	g_ObjMgr.Release();
	g_SoundMgr.Release();
	myDevice::Release();
	//m_Graphics.Release();
	return true;
}