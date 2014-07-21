#ifndef _GROUP_CLIENT_H_
#define _GROUP_CLIENT_H_

#include "ClientLib.h"

class CGroupClient : public SGLib::CClientBase
{
public:
	CGroupClient(s32 id);
	virtual int OnRecv(char *buf, int len);
	virtual void OnConnect();
	virtual void OnClose();
	virtual void OnConnectError();
};

#endif
