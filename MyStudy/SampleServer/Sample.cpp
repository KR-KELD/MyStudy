#include "myLoginServer.h"

void main()
{
	myLoginServer server;
	server.Init();
	server.Run();
	server.Release();
}