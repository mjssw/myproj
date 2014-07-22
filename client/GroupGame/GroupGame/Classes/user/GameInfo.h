#ifndef _GAME_INFO_H_
#define _GAME_INFO_H_

#include "Define.h"
#include <map>

class CGameInfo
{
public:
	CGameInfo(s32 gameid=0, s32 count=0) : 
		m_gameid( gameid ),
		m_count( count )
	{}

	s32 m_gameid;
	s32 m_count;
};

class CGameList
{
public:
	CGameList();
	~CGameList();

	void AddGameInfo(CGameInfo &game);

private:
	std::map<s32, CGameInfo> m_games;
};

#endif
