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
	g_TextureMgr.Init();
	//g_CamMgr.Init();
	//g_ObjMgr.Init();
	ComPtr<IDXGISurface1> pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),
		(LPVOID*)pBackBuffer.GetAddressOf());
	//dx2d ��ο� ����
	g_Draw.Set(m_hWnd,
		g_rtClient.right,
		g_rtClient.bottom, pBackBuffer.Get());

	m_pSkyBoxObj = myGameObject::CreateComponentObj(new mySkyBox);
	m_pSkyBox = m_pSkyBoxObj->GetComponent<mySkyBox>(); 
	m_pSkyBox->Create(L"SkyBoxVS.txt", L"SkyBoxPS.txt", L"");

	m_pBasisLine = new myShapeLine;
	g_RunGameObject.InsertComponent(m_pBasisLine);
	m_pBasisLine->Init();
	if (!m_pBasisLine->Create(L"BasisVS.txt", L"BasisPS.txt", L""))
	{
		return false;
	}

	myDebugCamera* pDebugCamera = new myDebugCamera;
	myGameObject* obj = g_CamMgr.CreateCameraObj(L"DebugCamera", pDebugCamera);
	g_CamList.GetGameObject(L"DebugCamera")->InsertComponent(new myController);
	pDebugCamera->CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	pDebugCamera->CreateProjMatrix(1, 1000, PI2D, fAspect);

	g_CamMgr.SetMainCamera(obj);
	g_FbxLoader.Init();
	g_CollisionMgr.Init();

	Init();
	PostInit();
	return true;
}

bool myCore::GameFrame()
{
	PreFrame();
	g_Timer.Frame();
	g_Input.Frame();
	g_SoundMgr.Frame();
	g_CamMgr.PreFrame();
	g_CamMgr.Frame();
	g_ObjMgr.PreFrame();
	g_ObjMgr.Frame();
	Frame();
	g_CollisionMgr.Frame();
	PostFrame();
	return true;
}

bool myCore::PreRender()
{
	myDevice::PreRender();
	m_pSkyBox->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);
	m_pSkyBox->Render(g_pImmediateContext);



	//�����Ͷ����� ������Ʈ ����
	m_pd3dContext->RSSetState(myDxState::g_pRSEdit.Get());
	//�ȼ� ���̴��� ���÷� ����(������)
	m_pd3dContext->PSSetSamplers(0, 1, &myDxState::g_pSSWrapLinear);
	//���� ���Ľ� ������Ʈ ����(���̰� ����)
	m_pd3dContext->OMSetDepthStencilState(myDxState::g_pDSSDepthEnable, 0);
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
	m_pBasisLine->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);

	m_pBasisLine->Draw(g_pImmediateContext, Vector3(0, 0, 0), Vector3(50, 0, 0), Vector4(1, 0, 0, 1));
	m_pBasisLine->Draw(g_pImmediateContext, Vector3(0, 0, 0), Vector3(0, 50, 0), Vector4(0, 1, 0, 1));
	m_pBasisLine->Draw(g_pImmediateContext, Vector3(0, 0, 0), Vector3(0, 0, 50), Vector4(0, 0, 1, 1));
	g_Timer.Render();
	g_Input.Render();
	g_ObjMgr.Render();
	g_SoundMgr.Render();
	g_Draw.Render();
	//g_Draw.Draw(0, 0, g_Timer.m_szBuffer);
	myDevice::PostRender();
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
	if (GameInit())
	{
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
	}
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
	m_pSkyBoxObj->Release();
	SAFE_DEL(m_pSkyBoxObj);
	Release();
	g_FbxLoader.Release();
	g_CamMgr.Release();
	g_Draw.Release();
	g_Timer.Release();
	g_Input.Release();
	g_ObjMgr.Release();
	g_SoundMgr.Release();
	myDevice::Release();
	_CrtDumpMemoryLeaks();
	return true;
}

bool myCore::InitTool(HWND hWnd, HINSTANCE hInstance)
{
	HRESULT hr = S_OK;
	g_hWnd = m_hWnd = hWnd;					//���� �ڵ鿡 ����
	GetClientRect(m_hWnd, &m_rtClient);		//Ŭ���̾�Ʈ�� ũ�⸦ Ŭ���̾�Ʈ ��Ʈ�� ��´�
	GetWindowRect(m_hWnd, &m_rtWindow);
	g_rtClient = m_rtClient;
	g_hInstance = hInstance;
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	GameInit();
	return true;
}

bool myCore::ReleaseTool()
{
	GameRelease();
	//�̴ϼȶ����� ����
	CoUninitialize();
	return true;
}

bool myCore::RunTool()
{
	if (GameFrame() == false) return false;
	if (GameRender() == false) return false;
	return true;
}
