#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "Lock.h"
#include <map>
#include <vector>
#include <string>

// TODO need lock

class CGameServerInfo;
class CGateClientInfo;
class CGameInfo
{
public:
	CGameInfo(s32 id) : m_Id(id)
	{}
	~CGameInfo();

	s32 GameId() const;

	const CGateClientInfo* GetGateInfo(s32 gateId) const;
	void SetGateInfo(s32 gateId, const char *ip, s32 port, s32 curCount, s32 maxCount);
	void DelGateInfo(s32 gateId);
	void GetGateList(std::vector<s32> &vecId) const;

	const CGameServerInfo* GetGameServerInfo(s32 serverId) const;
	void SetGameServerInfo(s32 serverId, s32 curCount, u64 instanceId);
	void DelGameServerInfo(s32 serverId);
	void GetGameServerList(std::vector<s32> &vecId) const;

	s32 GetPlayerCount() const;
	bool GetOptimalGateInfo(std::string &ip, s32 &port) const;
	u64 GetOptimalGameServerInfo() const;

private:
	s32 m_Id;
	std::map<s32, CGameServerInfo*> m_GameServers;
	std::map<s32, CGateClientInfo*> m_GameGates;
};

class CGameManager
{
public:
	~CGameManager();
	static CGameManager& Instance()
	{
		if( m_pInstance == NULL )
		{
			m_pInstance = new CGameManager();
		}
		return *m_pInstance;
	}

	void SetGateInfo(s32 gameId, s32 gateId, const char *ip, s32 port, s32 curCount, s32 maxCount);
	void SetGameServerInfo(s32 gameId, s32 serverId, s32 curCount, u64 instanceId);

	void DelGameInfo(s32 gameId);
	void DelGateInfo(s32 gameId, s32 gateId);
	void DelGameServerInfo(s32 gameId, s32 serverId);

	const CGameInfo* GetGameInfo(s32 gameId);
	s32  GetPlayerCount(s32 gameId);
	void GetGameList(std::vector<s32> &vecId);

private:
	CGameManager() : 
		m_GameLock(),
		m_GameMap()
	{
	}

	CGameInfo* _GetGameInfo(s32 gameId);
	CGameInfo* _TryGetGameInfo(s32 gameId);

	static CGameManager*m_pInstance;

	SGLib::CLock m_GameLock;
	std::map<s32, CGameInfo*> m_GameMap;
};

#endif
