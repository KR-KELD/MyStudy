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
		//iocp에 핸들엔 소켓 키엔 유저 주소값을 넘겨서 연결
		I_Iocp.SetBind(sock, (ULONG_PTR)user);
		user->WaitReceive();
		//아마도 스레드가 첫 결과를 확인 할 수 있게 하기위해 연결과 동시에 리시브를 한 번 호출
		printf("\n접속->%s:%d\n",
			inet_ntoa(addr.sin_addr),
			ntohs(addr.sin_port));
	}
	return true;
}
