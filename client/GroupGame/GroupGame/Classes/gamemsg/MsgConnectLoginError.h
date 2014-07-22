#ifndef _GAMEMSG_CONNECT_LOGIN_ERROR_H_
#define _GAMEMSG_CONNECT_LOGIN_ERROR_H_

#include "MsgBase.h"

class CMsgConnectLoginError : public CMsgBase
{
public:
	CMsgConnectLoginError();
	virtual void Process();
};

#endif
