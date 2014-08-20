#include "GameInfo.h"
using namespace std;

CGameList::CGameList()
{
}

CGameList::~CGameList()
{
}

void CGameList::AddGameInfo(CGameInfo &game)
{
	m_games[ game.m_gameid ] = game;
}

void CGameList::Dump(std::vector<s32> &games)
{
	games.clear();
	map<s32, CGameInfo>::iterator it = m_games.begin();
	for( ; it != m_games.end(); ++it )
	{
		games.push_back( it->first );
	}
}

CGameInfo* CGameList::GetGameInfo(s32 gameid)
{
	map<s32, CGameInfo>::iterator it = m_games.find( gameid );
	if( it != m_games.end() )
	{
		return &(it->second);
	}
	return NULL;
}

