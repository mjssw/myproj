#ifndef _GAME_CLIENT_H_
#define _GAME_CLIENT_H_

#include "ClientLib.h"

class CGameClient : public SGLib::CClientBase
{
public:
	CGameClient(s32 id);
	virtual int OnRecv(char *buf, int len);
	virtual void OnConnect();
	virtual void OnClose();
	virtual void OnConnectError();
};

#endif