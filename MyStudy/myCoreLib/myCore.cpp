#include "myCore.h"

HRESULT myCore::DeleteDXResource()
{
	//�������� ���ҽ� ����
	g_Draw.DeleteDependentResource();
	return S_OK;
}

HRESULT myCore::CreateDXResource(UINT w, UINT h)
{
	//����� �ʱ�ȭ
	ComPtr<IDXGISurface1> pBackBuffer = nullptr;
	//����� ��������
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)pBackBuffer.GetAddressOf());
	//���ҽ����� ����ۿ� ���������ؼ� �ܴ�
	g_Draw.ResizeDevice(w, h, pBackBuffer.Get());
	//����� ����
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
	//dx2d ��ο� ����
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
	//HeapEnableTerminateOnCorruption �ɼ��� �����ϸ� �ջ�� ���� �̿��ϴ� ���� �ǿ뿡 ���� ���ø����̼��� ������ ���� �� �����Ƿ� �����ϰ� ����.
	//HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	HRESULT hr = S_OK;
	//CoInitializeEx �� COM ���̺귯���� ����ϴ� �� �����忡 ���� �� �� �̻� ȣ���ؾ�  �Ѵ�.
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
	//�̴ϼȶ����� ����
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