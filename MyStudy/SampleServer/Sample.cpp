#include "myLoginServer.h"
#include "myIOCP.h"
void main()
{
	I_Iocp.Init();
	myLoginServer server;
	server.Init();
	server.Run();
	server.Release();
}