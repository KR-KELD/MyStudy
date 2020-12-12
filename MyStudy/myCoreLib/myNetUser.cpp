#include "myNetUser.h"

myNetUser::myNetUser()
{
	iRecvSize = 0;
	iSendSize = 0;
	ZeroMemory(recvBuf, sizeof(char) * MAX_BUFFER_SIZE);
}

myNetUser::~myNetUser()
{

}