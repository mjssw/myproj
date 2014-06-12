#include "GameManager.h"
#include "StackTrace.h"
#include "GameServerInfo.h"
#include "GateClientManager.h"
using namespace SGLib;
using namespace std;

CGameInfo::~CGameInfo()
{
	std::map<s32, CGateClientInfo*>::iterator it = m_GameGates.begin();
	for( ; it != m_GameGates.end(); ++it )
	{
		if( it->second != NULL )
		{
			SAFE_DELETE( it->second );
		}
	}
	m_GameGates.clear();
}

s32 CGameInfo::GameId() const
{
	return m_Id;
}

const CGateClientInfo* CGameInfo::GetGateInfo(s32 gateId) const
{
	std::map<s32, CGateClientInfo*>::const_iterator it = m_GameGates.find( gateId );
	if( it != m_GameGates.end() )
	{
		return it->second;
	}

	return NULL;
}

void CGameInfo::SetGateInfo(s32 gateId, const char *ip, s32 port, s32 curCount, s32 maxCount)
{
	CGateClientInfo *pInfo = NULL;
	std::map<s32, CGateClientInfo*>::iterator it = m_GameGates.find( gateId );
	if( it != m_GameGates.end() )
	{
		pInfo = it->second;
	}
	else
	{
		pInfo = new CGateClientInfo( gateId );
		if( pInfo == NULL )
		{
			SG_ASSERT( false );
			return;
		}
		m_GameGates[ gateId ] = pInfo;
	}

	pInfo->m_GateCurConnCount = curCount;	
	pInfo->m_GateMaxConnCount = maxCount;
	strncpy( pInfo->m_GateIp, ip, MAX_IP_LEN );
	pInfo->m_GatePort = port;
}

void CGameInfo::DelGateInfo(s32 gateId)
{
	std::map<s32, CGateClientInfo*>::iterator it = m_GameGates.find( gateId );
	if( it != m_GameGates.end() )
	{
		if( it->second != NULL )
		{
			SAFE_DELETE( it->second );
		}
		m_GameGates.erase( it );
	}
}

void CGameInfo::GetGateList(std::vector<s32> &vecId) const
{
	vecId.clear();
	std::map<s32, CGateClientInfo*>::const_iterator it = m_GameGates.begin();
	for( ; it != m_GameGates.end(); ++it )
	{
		vecId.push_back( it->first );
	}
}

const CGameServerInfo* CGameInfo::GetGameServerInfo(s32 serverId) const
{
	std::map<s32, CGameServerInfo*>::const_iterator it = m_GameServers.find( serverId );
	if( it != m_GameServers.end() )
	{
		return it->second;
	}

	return NULL;
}

void CGameInfo::SetGameServerInfo(s32 serverId, s32 curCount, u64 instanceId)
{
	CGameServerInfo *pInfo = NULL;
	std::map<s32, CGameServerInfo*>::iterator it = m_GameServers.find( serverId );
	if( it != m_GameServers.end() )
	{
		pInfo = it->second;
	}
	else
	{
		pInfo = new CGameServerInfo( serverId );
		if( pInfo == NULL )
		{
			SG_ASSERT( false );
			return;
		}
		m_GameServers[ serverId ] = pInfo;
	}

	pInfo->SetCount( curCount );	
	pInfo->SetServerInstanceId( instanceId );
}

void CGameInfo::DelGameServerInfo(s32 serverId)
{
	std::map<s32, CGameServerInfo*>::iterator it = m_GameServers.find( serverId );
	if( it != m_GameServers.end() )
	{
		if( it->second != NULL )
		{
			SAFE_DELETE( it->second );
		}
		m_GameServers.erase( it );
	}
}

void CGameInfo::GetGameServerList(std::vector<s32> &vecId) const
{
	vecId.clear();
	std::map<s32, CGameServerInfo*>::const_iterator it = m_GameServers.begin();
	for( ; it != m_GameServers.end(); ++it )
	{
		vecId.push_back( it->first );
	}
}

s32 CGameInfo::GetPlayerCount() const
{
	s32 count = 0;
	std::map<s32, CGameServerInfo*>::const_iterator it = m_GameServers.begin();
	for( ; it != m_GameServers.end(); ++it )
	{
		SG_ASSERT( it->second != NULL );
		count += it->second->CurCount();
	}

	return count;
}

bool CGameInfo::GetOptimalGateInfo(std::string &ip, s32 &port) const
{
	ip = "";
	port = INVALID_VAL;
	
	s32 minCount = -1;
	s32 gateId = -1;
	std::map<s32, CGateClientInfo*>::const_iterator it = m_GameGates.begin();
	for( ; it != m_GameGates.end(); ++it )
	{
		SG_ASSERT( it->second != NULL );
		if( it->second->m_GateCurConnCount >= it->second->m_GateMaxConnCount )
		{
			continue;
		}

		if( minCount == -1 )
		{
			minCount = it->second->m_GateCurConnCount;
			gateId = it->first;
		}
		else
		{
			if( it->second->m_GateCurConnCount < minCount )
			{
				minCount = it->second->m_GateCurConnCount;
				gateId = it->first;
			}
		}
	}

	if( gateId == -1 )
	{
		return false;
	}

	const CGateClientInfo *pGate = GetGateInfo( gateId );
	if( pGate == NULL )
	{
		return false;
	}

	ip = pGate->m_GateIp;
	port = pGate->m_GatePort;

	return true;
}

u64 CGameInfo::GetOptimalGameServerInfo() const
{
	const CGameServerInfo* game = NULL;
	map<s32, CGameServerInfo*>::const_iterator cit = m_GameServers.begin();
	for( ; cit != m_GameServers.end(); ++cit )
	{
		if( game == NULL )
		{
			game = cit->second;
			continue;
		}

		if( game->CurCount() > cit->second->CurCount() )
		{
			game = cit->second;
		}
	}

	if( game )
	{
		return game->GetServerInstanceId();
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGameManager* CGameManager::m_pInstance = NULL;

CGameManager::~CGameManager()
{
	//SGLib::CGuardLock<SGLib::CLock> g(m_GameLock);
	map<s32, CGameInfo*>::iterator it = m_GameMap.begin();
	for( ; it != m_GameMap.end(); ++it )
	{
		if( it->second != NULL )
		{
			SAFE_DELETE( it->second );
		}
	}
}

void CGameManager::SetGateInfo(s32 gameId, s32 gateId, const char *ip, s32 port, s32 curCount, s32 maxCount)
{
	CGameInfo *pGame = _TryGetGameInfo( gameId );
	SG_ASSERT( pGame != NULL );
	pGame->SetGateInfo( gateId, ip, port, curCount, maxCount );
}

void CGameManager::SetGameServerInfo(s32 gameId, s32 serverId, s32 curCount, u64 instanceId)
{
	CGameInfo *pGame = _TryGetGameInfo( gameId );
	SG_ASSERT( pGame != NULL );
	pGame->SetGameServerInfo( serverId, curCount, instanceId );
}

void CGameManager::DelGameInfo(s32 gameId)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_GameLock);
	map<s32, CGameInfo*>::iterator it = m_GameMap.find( gameId );
	if( it != m_GameMap.end() )
	{
		SAFE_DELETE( it->second );
		m_GameMap.erase( it );
	}
}

void CGameManager::DelGateInfo(s32 gameId, s32 gateId)
{
	CGameInfo *pGame = _GetGameInfo( gameId );
	if( pGame != NULL )
	{
		pGame->DelGateInfo( gateId );
	}
}

void CGameManager::DelGameServerInfo(s32 gameId, s32 serverId)
{
	CGameInfo *pGame = _GetGameInfo( gameId );
	if( pGame != NULL )
	{
		pGame->DelGameServerInfo( serverId );
	}
}

const CGameInfo* CGameManager::GetGameInfo(s32 gameId)
{
	return _GetGameInfo( gameId );
}

s32 CGameManager::GetPlayerCount(s32 gameId)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_GameLock);
	map<s32, CGameInfo*>::iterator it = m_GameMap.begin();
	for( ; it != m_GameMap.end(); ++it )
	{
		SG_ASSERT( it->second != NULL );
		return it->second->GetPlayerCount();
	}

	return 0;
}

void CGameManager::GetGameList(std::vector<s32> &vecId)
{
	vecId.clear();
	SGLib::CGuardLock<SGLib::CLock> g(m_GameLock);
	map<s32, CGameInfo*>::iterator it = m_GameMap.begin();
	for( ; it != m_GameMap.end(); ++it )
	{
		vecId.push_back( it->first );
	}
}

CGameInfo* CGameManager::_GetGameInfo(s32 gameId)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_GameLock);
	map<s32, CGameInfo*>::iterator it = m_GameMap.find( gameId );
	if( it != m_GameMap.end() )
	{
		return it->second;
	}

	return NULL;
}

CGameInfo* CGameManager::_TryGetGameInfo(s32 gameId)
{
	CGameInfo *pGame = NULL;
	{
		SGLib::CGuardLock<SGLib::CLock> g(m_GameLock);
		pGame = _GetGameInfo( gameId );
		if( pGame == NULL )
		{
			pGame = new CGameInfo( gameId );
			if( pGame == NULL )
			{
				SG_ASSERT( false );
				return NULL;
			}
			m_GameMap[ gameId ] = pGame;
		}
	}

	return pGame;
}
