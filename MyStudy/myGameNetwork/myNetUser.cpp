#include "myNetUser.h"
myNetUser::myNetUser()
{
	iRecvSize = 0;
	iSendSize = 0;
	ZeroMemory(recvBuf, sizeof(char) * 10000);
}
myNetUser::~myNetUser()
{

}