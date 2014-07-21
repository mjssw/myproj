#ifndef _LOGIN_CLIENT_H_
#define _LOGIN_CLIENT_H_

#include "ClientLib.h"

class CLoginClient : public SGLib::CClientBase
{
public:
	CLoginClient(s32 id);
	virtual int OnRecv(char *buf, int len);
	virtual void OnConnect();
	virtual void OnClose();
	virtual void OnConnectError();
};

#endif
