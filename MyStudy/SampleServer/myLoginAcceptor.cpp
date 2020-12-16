#include "myLoginAcceptor.h"
#include "mySessionMgr.h"
#include "myIOCP.h"

bool myLoginAcceptor::AddSession(SOCKET sock, SOCKADDR_IN addr)
{
	{
		myLock lock(this);
		myChatUser* user = new myChatUser;
		user->m_Sock = sock;
		user->m_SockAddr = addr;
		I_Session.AddUser(user);
		//iocp�� �ڵ鿣 ���� Ű�� ���� �ּҰ��� �Ѱܼ� ����
		I_Iocp.SetBind(sock, (ULONG_PTR)user);
		user->WaitReceive();
		//�Ƹ��� �����尡 ù ����� Ȯ�� �� �� �ְ� �ϱ����� ����� ���ÿ� ���ú긦 �� �� ȣ��
		printf("\n����->%s:%d\n",
			inet_ntoa(addr.sin_addr),
			ntohs(addr.sin_port));
	}
	return true;
}
