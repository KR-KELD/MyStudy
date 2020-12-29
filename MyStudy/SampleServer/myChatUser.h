#pragma once
#include"myNetUser.h"
class myChatUser : public myNetUser
{
public:
	myChatUser(myServer* pServer);
	virtual ~myChatUser();
};

