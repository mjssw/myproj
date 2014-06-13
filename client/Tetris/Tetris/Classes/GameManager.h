#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "utils.h"
#include <map>
#include <vector>
#include <string>
#include "Define.h"
#include "layers_scenes_transitions_nodes/CCLayer.h"
#include "GameWorldBase.h"
#include "GameScene.h"
#include "MTRandom.h"

class CGameInfo
{
public:
	s32 m_Id;
	s32 m_Count;
};

class CPlayerInfo
{
public:
	std::string m_User;
	bool m_bReady;
};

class CGroupInfo
{
public:
	u64 m_Id;
	std::string m_Name;
	std::map<std::string, int> m_Members;
};

class CGameRoomInfo
{
public:
	s32 game;
	s32 roomid;
	std::string ip;
	s32 port;
	u64 groupid;
	std::string creater;
};

class CRoomBase;
class CGameManager
{
	DEFINE_SINGLETON_CLASS(CGameManager);
public:
	~CGameManager(){}

	void Init();
	void SetGameInfo(s32 gameid, CGameInfo *info);
	void SetGameWorld(CGameWorldBase *pWorld);
	void ConnectLoginError();
	s32 GameCount();
	void DumpGameIdList(std::vector<int> &vecGameId);
	std::string GetGameIcon(s32 gameId);
	CGameInfo* GetGameInfo(s32 gameId);
	void InitRoomList(int count);
	CRoomBase* GetRoom(int id);
	void EnterRoomFailed();
	void SetUser(const char *user){ m_User = user; }
	const std::string& GetUser(){ return m_User; }
	void EnterRoom(const std::string &user);
	void LeaveRoom(const std::string &user);
	void PlayerReady();
	void EnemyReady();
	void RoomInfo(const std::string &enemy, bool isReady);
	void GameStart(s32 seed);
	void GameEnd(const std::string &loser);
	s32 NextBlockId();
	bool TryDown(CBlock *block, cocos2d::CCLayer *layer);
	bool TryLeft(CBlock *block, cocos2d::CCLayer *layer);
	bool TryRight(CBlock *block, cocos2d::CCLayer *layer);
	s32  TryRotateClockwise(CBlock *block, cocos2d::CCLayer *layer);
	bool PutBlock(CBlock *block, s32 col);
	void InitGameScene();
	void CheckCleanUp(CBlock *block, cocos2d::CCLayer *layer);

	const std::string& GetEnemy(){ return m_EnemyUser; }
	void SetEnemy(const std::string &enemy){ m_EnemyUser = enemy; }
	void DelEnemy(){ m_EnemyUser = ""; }
		
	void EnemyClearRows(std::vector<int> &clearRows);
	bool RiseBlock(int count, CBlock *block, cocos2d::CCLayer *layer);

	bool IsOfflineGame(){ return m_isOfflineGame; }
	void SetOfflineGame(){ m_isOfflineGame = true; }
	void Test();

	void AddGroup(u64 id, std::string &name);
	void AddGroupMember(u64 id, std::string &member);
	s32 GroupCount(){ return (s32)m_Groups.size(); }
	void DumpGroupNameList(std::vector<u64> &vecName);
	CGroupInfo* GetGroupInfo(u64);
	void SetCurGroup(CGroupInfo *group);
	CGroupInfo* GetCurGroup();
	void AddGroupGameRoom(u64 groupid, s32 game, s32 roomid, const std::string &ip, s32 port, const std::string &creater);
	void DelGroupGameRoom(u64 groupid, s32 game, s32 roomid);
	std::vector<CGameRoomInfo>& GetGameRoomInfo();
	void CreateGroupGameRoom();
	void SetCurGameRoom(CGameRoomInfo *room){ m_pCurGameRoom = room; }
	CGameRoomInfo* GetCurGameRoom(){ return m_pCurGameRoom; }
private:
	void _Register(s32 gameId, const char *icon);
	void _ReleaseOldRooms();

private:
	std::map<s32, CGameInfo*> m_GameList;
	std::map<s32, std::string> m_GameIcon;
	CGameWorldBase *m_pGameWorld;
	std::map<s32, CRoomBase*> m_RoomList;
	std::string m_User;
	s32 m_BlockSeed;
	CGameScene m_BattleScene;

	std::string m_EnemyUser;
	CMTRandom m_DownBlockRand;

	std::map<u64, CGroupInfo*> m_Groups;
	CGroupInfo *m_curGroup;

	std::vector<CGameRoomInfo> m_GameRooms;
	CGameRoomInfo *m_pCurGameRoom;
	bool m_isOfflineGame;
};

#endif
