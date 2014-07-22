#ifndef _GAMEMSG_CONNECT_GROUP_H_
#define _GAMEMSG_CONNECT_GROUP_H_

#include "MsgBase.h"

class CMsgConnectGroup : public CMsgBase
{
public:
	CMsgConnectGroup();
	virtual void Process();
};

#endif
