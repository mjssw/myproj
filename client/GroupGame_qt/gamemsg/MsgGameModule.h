#ifndef _MSG_GAME_MODULE_H_
#define _MSG_GAME_MODULE_H_

#include "MsgBase.h"
#include "Define.h"
#include <string>
#include <vector>

class CMsgConnectGame : public CMsgBase
{
public:
	CMsgConnectGame();
	virtual void Process();
};

class CMsgGameMessage : public CMsgBase
{
public:
	CMsgGameMessage(int msgid, int msglen, char *data);
	virtual void Process();

private:
	int m_msgid;
	int m_msglen;
    std::string m_msgData;
};


#endif
