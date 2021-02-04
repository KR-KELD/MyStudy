#include "myCore.h"

HRESULT myCore::DeleteDXResource()
{
	//�������� ���ҽ� ����
	g_Draw.DeleteDependentResource();
	return E_NOTIMPL;
}

HRESULT myCore::CreateDXResource(UINT w, UINT h)
{
	//����� �ʱ�ȭ
	IDXGISurface1* pBackBuffer = nullptr;
	//����� ��������
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&pBackBuffer);
	//���ҽ����� ����ۿ� ���������ؼ� �ܴ�
	g_Draw.ResizeDevice(w, h, pBackBuffer);
	//����� ����
	if (pBackBuffer) pBackBuffer->Release();
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
	//g_Draw.Init();
	//m_Graphics.Init();
	g_SoundMgr.Init();

	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),
		(LPVOID*)&pBackBuffer);
	//dx2d ��ο� ����
	g_Draw.Set(m_hWnd,
		g_rtClient.right,
		g_rtClient.bottom, pBackBuffer);
	if (pBackBuffer) pBackBuffer->Release();

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
	g_SoundMgr.Frame();
	Frame();
	g_Draw.Frame();
	PostFrame();
	return true;
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
	g_Timer.Render();
	g_Input.Render();
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

bool myCore::GameRelease()
{
	Release();
	g_Draw.Release();
	g_Timer.Release();
	g_Input.Release();
	g_SoundMgr.Release();
	myDevice::Release();
	//m_Graphics.Release();
	return true;
}