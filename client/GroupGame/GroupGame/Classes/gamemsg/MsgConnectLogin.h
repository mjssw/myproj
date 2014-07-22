#ifndef _GAMEMSG_CONNECT_LOGIN_H_
#define _GAMEMSG_CONNECT_LOGIN_H_

#include "MsgBase.h"

class CMsgConnectLogin : public CMsgBase
{
public:
	CMsgConnectLogin();
	virtual void Process();
};

#endif
