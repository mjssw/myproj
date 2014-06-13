#include "GameManager.h"
#include "Room.h"
#include "GameScene.h"
#include "Block.h"
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

INIT_SIGNLETON_CLASS(CGameManager);

void CGameManager::Test()
{
	int count = 20;
	int seed = 300;
	vector<u32> nums;
	{
		m_DownBlockRand.SetSeed(seed);
		for( int i=0; i<count; ++i )
		{
			nums.push_back( m_DownBlockRand.NextUInt( 100) );
		}
	}

	vector<u32> nums2;
	{
		m_DownBlockRand.SetSeed(seed);
		for( int i=0; i<count; ++i )
		{
			nums2.push_back( m_DownBlockRand.NextUInt( 100) );
		}
	}
}

void CGameManager::Init()
{
	_Register( GAME_ID_TETRIS, PATCH_RES_DIR("tetris.png") );
	m_isOfflineGame = false;
}

void CGameManager::SetGameInfo(s32 gameid, CGameInfo *info)
{
	map<s32, CGameInfo*>::iterator it = m_GameList.find( gameid );
	if( it != m_GameList.end() )
	{
		SAFE_DELETE( it->second );
	}

	m_GameList[ gameid ] = info;
}

void CGameManager::SetGameWorld(CGameWorldBase *pWorld)
{
	m_pGameWorld = pWorld;
}

void CGameManager::ConnectLoginError()
{
	if( m_pGameWorld )
	{
		m_pGameWorld->ConnectError();
	}
}

s32 CGameManager::GameCount()
{
	return (s32)m_GameList.size();
}

void CGameManager::DumpGameIdList(std::vector<int> &vecGameId)
{
	std::map<s32, CGameInfo*>::iterator it = m_GameList.begin();
	for( ; it != m_GameList.end(); ++it )
	{
		vecGameId.push_back( it->first );
	}
}

void CGameManager::_Register(s32 gameId, const char *icon)
{
	m_GameIcon[ gameId ] = string(icon);
}

string CGameManager::GetGameIcon(s32 gameId)
{
	map<s32, std::string>::iterator it = m_GameIcon.find( gameId );
	if( it != m_GameIcon.end() )
	{
		return it->second;
	}
	return "";
}

CGameInfo* CGameManager::GetGameInfo(s32 gameId)
{
	map<s32, CGameInfo*>::iterator it = m_GameList.find( gameId );
	if( it != m_GameList.end() )
	{
		return it->second;
	}
	return NULL;
}

void CGameManager::InitRoomList(int count)
{
	_ReleaseOldRooms();
	int id = ROOM_ID_FROM;
	for( int i=0; i<count; ++i )
	{
		CRoom *room = new CRoom( id++ );
		if( room )
		{
			m_RoomList[ room->Id() ] = room;
		}
		else
		{
			break;
		}
	}
}

void CGameManager::_ReleaseOldRooms()
{
	map<s32, CRoomBase*>::iterator it = m_RoomList.begin();
	for( ; it != m_RoomList.end(); ++it )
	{
		SAFE_DELETE( it->second );
	}
	m_RoomList.clear();
}

CRoomBase* CGameManager::GetRoom(int id)
{
	map<s32, CRoomBase*>::iterator it = m_RoomList.find( id );
	if( it != m_RoomList.end() )
	{
		return it->second;
	}

	return NULL;
}

void CGameManager::EnterRoomFailed()
{
	if( m_pGameWorld )
	{
		m_pGameWorld->EnterRoomFailed();
	}
}

void CGameManager::EnterRoom(const std::string &user)
{
	if( m_pGameWorld )
	{
		m_pGameWorld->EnterRoom( user );
	}
}

void CGameManager::LeaveRoom(const std::string &user)
{
	if( m_pGameWorld )
	{
		m_pGameWorld->LeaveRoom( user );
	}
}

void CGameManager::PlayerReady()
{
	if( m_pGameWorld )
	{
		m_pGameWorld->PlayerReady();
	}
}

void CGameManager::EnemyReady()
{
	if( m_pGameWorld )
	{
		m_pGameWorld->EnemyReady();
	}
}

void CGameManager::RoomInfo(const std::string &enemy, bool isReady)
{
	if( m_pGameWorld )
	{
		m_pGameWorld->RoomInfo( enemy, isReady );
	}
}

void CGameManager::GameStart(s32 seed)
{
	m_BlockSeed = seed;
	m_BattleScene.Init();
	m_DownBlockRand.SetSeed( seed );
}

void CGameManager::GameEnd(const std::string &loser)
{
	if( m_pGameWorld )
	{
		m_pGameWorld->GameEnd( !(m_User == loser) );
	}
}

s32 CGameManager::NextBlockId()
{
	/*
	static s32 gid = 0;
	++gid;
	if( gid > CBlockBase::E_Block_Count ) gid = 1;
	return gid;//*/
#ifdef _TEST_BLOCK
	//return 18;
	return (s32)(m_DownBlockRand.NextUInt(CBlockBase::E_Block_Count) + 1);
#else
	return (s32)(m_DownBlockRand.NextUInt(CBlockBase::E_Block_Count) + 1);
#endif
}

bool CGameManager::TryDown(CBlock *block, cocos2d::CCLayer *layer)
{
	return m_BattleScene.TryDown( block, layer );
}

bool CGameManager::TryLeft(CBlock *block, cocos2d::CCLayer *layer)
{
	return m_BattleScene.TryLeft( block, layer );
}

bool CGameManager::TryRight(CBlock *block, cocos2d::CCLayer *layer)
{
	return m_BattleScene.TryRight( block, layer );
}

s32 CGameManager::TryRotateClockwise(CBlock *block, cocos2d::CCLayer *layer)
{
	return m_BattleScene.TryRotateClockwise( block, layer );
}

bool CGameManager::PutBlock(CBlock *block, s32 col)
{
	return m_BattleScene.PutBlock( block, col );
}

void CGameManager::InitGameScene()
{
	m_BattleScene.Init();
}

void CGameManager::CheckCleanUp(CBlock *block, cocos2d::CCLayer *layer)
{
	m_BattleScene.CheckCleanUp( block, layer );
}

void CGameManager::EnemyClearRows(std::vector<int> &clearRows)
{
	if( m_pGameWorld )
	{
		m_pGameWorld->EnemyClearRows( clearRows );
	}
}

bool CGameManager::RiseBlock(int count, CBlock *block, cocos2d::CCLayer *layer)
{
	if( count > 1 )
	{
		return m_BattleScene.RiseBlock( count-1, block, layer );
	}
	return true;
}

void CGameManager::AddGroup(u64 id, std::string &name)
{
	std::map<u64, CGroupInfo*>::iterator it = m_Groups.find( id );
	if( it != m_Groups.end() )
	{
		it->second->m_Name = name;
		return;
	}

	CGroupInfo *group = new CGroupInfo();
	group->m_Id = id;
	group->m_Name = name;
	m_Groups[ id ] = group;
}

void CGameManager::AddGroupMember(u64 id, std::string &member)
{
	std::map<u64, CGroupInfo*>::iterator it = m_Groups.find( id );
	if( it != m_Groups.end() )
	{
		it->second->m_Members[ member ] = 1;
		return;
	}

	CCLog( "####### error ###########" );

	CGroupInfo *group = new CGroupInfo();
	group->m_Id = id;
	group->m_Members[ member ] = 1;
	m_Groups[ id ] = group;
}

void CGameManager::DumpGroupNameList(std::vector<u64> &vecName)
{
	vecName.clear();
	std::map<u64, CGroupInfo*>::iterator it = m_Groups.begin();
	for( ; it != m_Groups.end(); ++it )
	{
		vecName.push_back( it->first );
	}
}

CGroupInfo* CGameManager::GetGroupInfo(u64 id)
{
	std::map<u64, CGroupInfo*>::iterator it = m_Groups.find( id );
	if( it != m_Groups.end() )
	{
		return it->second;
	}

	return NULL;
}
	
void CGameManager::SetCurGroup(CGroupInfo *group)
{ 
	m_curGroup = group; 
}

CGroupInfo* CGameManager::GetCurGroup()
{ 
	return m_curGroup; 
} 
	
void CGameManager::AddGroupGameRoom(u64 groupid, s32 game, s32 roomid, const std::string &ip, s32 port, const std::string &creater)
{
	CGameRoomInfo info;
	info.creater = creater;
	info.game = game;
	info.groupid = groupid;
	info.ip = ip;
	info.port = port;
	info.roomid = roomid;

	m_GameRooms.push_back( info );
}

void CGameManager::DelGroupGameRoom(u64 groupid, s32 game, s32 roomid)
{
	vector<CGameRoomInfo>::iterator it = m_GameRooms.begin();
	for( ; it != m_GameRooms.end(); )
	{
		vector<CGameRoomInfo>::iterator next = it + 1;
		if( it->groupid == groupid &&
			it->game == game &&
			it->roomid == roomid )
		{
			m_GameRooms.erase( it );
		}
		it = next;
	}
}

std::vector<CGameRoomInfo>& CGameManager::GetGameRoomInfo()
{
	return m_GameRooms;
}

void CGameManager::CreateGroupGameRoom()
{
	m_pGameWorld->CreateGroupGameRoom();
}
