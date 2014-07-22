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
