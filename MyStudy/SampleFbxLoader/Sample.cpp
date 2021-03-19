#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	return true;
}

bool Sample::Frame()
{
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		myDxState::m_FillMode = D3D11_FILL_WIREFRAME;
		myDxState::SetRasterizerState();
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		myDxState::m_FillMode = D3D11_FILL_SOLID;
		myDxState::SetRasterizerState();
	}
	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		myDxState::m_CullMode = D3D11_CULL_BACK;
		myDxState::SetRasterizerState();
	}
	if (g_Input.GetKey('7') == KEY_PUSH)
	{
		myDxState::m_CullMode = D3D11_CULL_FRONT;
		myDxState::SetRasterizerState();
	}
	return true;
}

bool Sample::Render()
{
	//최종적으로는 모든 함수는 각자의 Render에서 돌아가게끔 해야함
	//그걸 호출하는건 obj매니저에 있는 메인gameobject



	return true;
}

bool Sample::Release()
{

	return true;
}

Matrix Sample::CreateMatrixShadow(Vector4 * pPlane, Vector4 * pLight)
{
	Matrix mat;
	Vector4 plane, light;
	pPlane->Normalize();
	plane.x = pPlane->x * -1.0f;
	plane.y = pPlane->y * -1.0f;
	plane.z = pPlane->z * -1.0f;
	plane.w = pPlane->w * -1.0f;
	light = *pLight;// * -1.0f;
	float D = -(plane.Dot(light));
	mat._11 = plane.x * light.x + D;	mat._12 = plane.x * light.y;	mat._13 = plane.x * light.z;	mat._14 = plane.x * light.w;
	mat._21 = plane.y * light.x;	mat._22 = plane.y * light.y + D;	mat._23 = plane.y * light.z;	mat._24 = plane.y * light.w;
	mat._31 = plane.z * light.x;	mat._32 = plane.z * light.y;	mat._33 = plane.z * light.z + D;	mat._34 = plane.z * light.w;
	mat._41 = plane.w * light.x;	mat._42 = plane.w * light.y;	mat._43 = plane.w * light.z;	mat._44 = plane.w * light.w + D;
	return mat;
}

LRESULT Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (g_CamMgr.m_pMainCamera == nullptr) return -1;
	int iRet = g_CamMgr.m_pMainCamera->WndProc(hWnd, message, wParam, lParam);
	return -1;
}

