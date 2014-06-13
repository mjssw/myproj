#ifndef _MSG_CONNECT_ERROR_H_
#define _MSG_CONNECT_ERROR_H_

#include "MsgBase.h"
#include "GameManager.h"

class CMsgConnectError : public CMsgBase
{
public:
	CMsgConnectError()
	{}

	virtual void Proc()
	{
		CGameManager::Instance().ConnectLoginError();
	}

};

#endif