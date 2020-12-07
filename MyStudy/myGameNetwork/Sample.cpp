#include "Sample.h"

GAMERUN;

bool Sample::Init()
{
	m_Net.InitNetwork("192.168.0.169", 10000);
	//m_Net.InitNetwork("175.194.89.26", 10000);
	return true;
}

bool Sample::Frame()
{

	//if (m_Net.Accept() == false)
	//{
	//	//break;
	//}
	//if (m_Net.RecvDataList() == false)
	//{
	//	//break;
	//}
	//if (m_Net.Process() == false)
	//{
	//	//break;
	//}
	return true;
}

bool Sample::Render()
{
	return true;
}

bool Sample::Release()
{
	m_Net.DeleteNetwork();
	return true;
}
