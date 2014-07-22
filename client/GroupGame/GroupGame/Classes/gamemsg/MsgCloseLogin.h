#ifndef _GAMEMSG_CLOSE_LOGIN_H_
#define _GAMEMSG_CLOSE_LOGIN_H_

#include "MsgBase.h"

class CMsgCloseLogin : public CMsgBase
{
public:
	CMsgCloseLogin();
	virtual void Process();
};

#endif
