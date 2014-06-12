#ifndef _GAME_INFO_MGR_H_
#define _GAME_INFO_MGR_H_

#include "Utils.h"
#include "Lock.h"
#include <map>

class CClient;
class CGameInfoMgr
{
	DEFINE_SINGLETON_CLASS(CGameInfoMgr);

public:
	struct sGameInfo
	{
		s32 game;
		s32 gameid;
		s32 count;
		CClient* client;
	};

	~CGameInfoMgr();
	void SetInfo(s32 gameid, s32 count, CClient* client);
	void RemoveInfo(CClient *client);

	sGameInfo* FindFreeGameManager(s32 game);

private:
	// <game, <gameid, gameinfo> >
	SGLib::CLock m_Lock;
	std::map<s32, std::map<s32, sGameInfo*> > m_GameInfo;
	std::map<CClient*, sGameInfo*> m_GameInfoIndex;
};


#endif
