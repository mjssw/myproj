#include "GameInfoMgr.h"
#include "GroupManagerClient.h"
#include "LuaModule.h"
using namespace std;
using namespace SGLib;

INIT_SIGNLETON_CLASS(CGameInfoMgr);
	
CGameInfoMgr::~CGameInfoMgr()
{
	map<CClient*, sGameInfo*>::iterator it = m_GameInfoIndex.begin();
	for( ; it != m_GameInfoIndex.end(); ++it )
	{
		if( it->second )
		{
			SAFE_DELETE( it->second );
		}
	}

	m_GameInfoIndex.clear();
	m_GameInfo.clear();
}

void CGameInfoMgr::SetInfo(s32 gameid, s32 count, CClient* client)
{
	// TODO 加上时间条件

	s32 game = CLuaModule::Instance().GetGameById( gameid );
	SG_ASSERT( game != GAME_NOUSE );

	CGuardLock<CLock> g(m_Lock);
	map<CClient*, sGameInfo*>::iterator it = m_GameInfoIndex.find( client );
	if( it == m_GameInfoIndex.end() )
	{
		sGameInfo *info = new sGameInfo();
		SG_ASSERT( info != NULL );
		info->game = game;
		info->gameid = gameid;
		info->count = count;
		info->client = client;
		
		m_GameInfoIndex[ client ] = info;
		m_GameInfo[ game ][ gameid ] = info;
	}
	else
	{
		SG_ASSERT( it->second != NULL );
		SG_ASSERT( it->second->client == client );
		SG_ASSERT( it->second->game == game );
		SG_ASSERT( it->second->gameid == gameid );
		it->second->count = count;
	}
}

void CGameInfoMgr::RemoveInfo(CClient *client)
{
	CGuardLock<CLock> g(m_Lock);
	map<CClient*, sGameInfo*>::iterator it = m_GameInfoIndex.find( client );
	if( it != m_GameInfoIndex.end() )
	{
		SG_ASSERT( it->second != NULL );
		sGameInfo *info = it->second;
		m_GameInfoIndex.erase( it );
	
		map<s32, map<s32, sGameInfo*> >::iterator it_1 = m_GameInfo.find( info->game );
		SG_ASSERT( it_1 != m_GameInfo.end() );
		map<s32, sGameInfo*>::iterator it_2 = it_1->second.find( info->gameid );
		SG_ASSERT( it_2 != it_1->second.end() );
		it_1->second.erase( it_2 );

		SAFE_DELETE( info );
	}
}

CGameInfoMgr::sGameInfo* CGameInfoMgr::FindFreeGameManager(s32 game)
{
	CGuardLock<CLock> g(m_Lock);
	map<s32, map<s32, sGameInfo*> >::iterator it_1 = m_GameInfo.find( game );
	if( it_1 == m_GameInfo.end() )
	{
		return NULL;
	}

	sGameInfo *info = NULL;
	map<s32, sGameInfo*>::iterator it_2 = it_1->second.begin();
	for( ; it_2 != it_1->second.end(); ++it_2 )
	{
		if( info == NULL )
		{
			info = it_2->second;
			continue;
		}

		if( info->count > it_2->second->count )
		{
			info = it_2->second;
		}
	}

	return info;
}
