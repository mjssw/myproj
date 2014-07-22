#ifndef _GAMEMSG_GAME_INFO_NOTIFY_H_
#define _GAMEMSG_GAME_INFO_NOTIFY_H_

#include "MsgBase.h"
#include "Define.h"
#include <string>
#include <vector>
#include "user/GameInfo.h"

class CMsgGameInfoNotify : public CMsgBase
{
public:
	CMsgGameInfoNotify(std::vector<CGameInfo> &games);
	virtual void Process();

private:
	std::vector<CGameInfo> m_games;
};

#endif
