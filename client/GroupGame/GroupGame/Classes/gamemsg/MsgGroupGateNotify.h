#ifndef _GAMEMSG_GROUP_GATE_NOTIFY_H_
#define _GAMEMSG_GROUP_GATE_NOTIFY_H_

#include "MsgBase.h"
#include "Define.h"
#include <string>

class CMsgGroupGateNotify : public CMsgBase
{
public:
	CMsgGroupGateNotify(const std::string &ip, s32 port);
	virtual void Process();

private:
	std::string m_ip;
	s32 m_port;
};

#endif
