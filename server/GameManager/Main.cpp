#include "ServerManager.h"
#include "GameMgrClient.h"
#include "GameMgrRpcClient.h"
#include "GameManager.h"
#include "DynamicServerManager.h"
#include "ReportServerManager.h"
#include "GameServerInfo.h"
#include "GateClientManager.h"
#include <vector>
using namespace std;

#include "gamemanager.pb.h"
#include "msgid.pb.h"

static void ReportGameInfoToCenter(const CGameInfo *pGame)
{
	SG_ASSERT( pGame != NULL );

	sglib::gamemanagerproto::GamemanagerCenterReportGameInfo info;
	info.set_gameid( pGame->GameId() );

	vector<s32> gateIds;
	pGame->GetGateList( gateIds );
	vector<s32>::iterator it = gateIds.begin();
	for( ; it != gateIds.end(); ++it )
	{
		const CGateClientInfo *_info = pGame->GetGateInfo( *it );
		SG_ASSERT( _info != NULL );
		sglib::publicproto::GateServerInfo *pInfo = info.add_gateservers();
		if( !pInfo )
		{
			continue;
		}

		pInfo->set_id( _info->m_GateId );
		pInfo->set_ip( _info->m_GateIp );
		pInfo->set_port( _info->m_GatePort );
		pInfo->set_cur_count( _info->m_GateCurConnCount );
		pInfo->set_max_count( _info->m_GateMaxConnCount );
	}

	vector<s32> serverIds;
	pGame->GetGameServerList( serverIds );
	it = serverIds.begin();
	for( ; it != serverIds.end(); ++it )
	{
		const CGameServerInfo *_info = pGame->GetGameServerInfo( *it );
		SG_ASSERT( _info != NULL );
		sglib::publicproto::GameServerInfo *pInfo = info.add_gameservers();
		if( !pInfo )
		{
			continue;
		}

		pInfo->set_serverid( _info->Id() );
		pInfo->set_cur_count( _info->CurCount() );
	}

	CServerManager::Instance().SendRpcMsg(
		CServerManager::Instance().GetCenterServerId(),
		info,
		sglib::msgid::GMCT_REPORT_GAME_INFO );
}

static void ReportGameInfoToGroupManager(const CGameInfo *pGame)
{
	SG_ASSERT( pGame != NULL );

	// 先检查是否已经连接成功了
	CRpcClient *rpc = CServerManager::Instance().FindRpcClient(
		CServerManager::Instance().GetGroupManagerServerId() );
	if( !rpc )
	{
		return;
	}

	sglib::gamemanagerproto::GamemanagerGroupmanagerReportGameInfo info;
	info.set_gameid( pGame->GameId() );
	info.set_count( pGame->GetPlayerCount() );

	CServerManager::Instance().SendRpcMsg(
		CServerManager::Instance().GetGroupManagerServerId(),
		info,
		sglib::msgid::GMGM_GAME_INFO_REPORT );
}

static void ReportGameInforCallBack(void *pData, s32 nDataLen)
{
	vector<s32> gameIds;
	CGameManager::Instance().GetGameList( gameIds );
	if( gameIds.size() == 0 )
	{
		return;
	}

	SG_ASSERT( gameIds.size() == 1 );
	s32 gameId = gameIds[0];
	const CGameInfo *pGame = CGameManager::Instance().GetGameInfo( gameId );
	SG_ASSERT( pGame != NULL );

	// 向 Center 汇报
	ReportGameInfoToCenter( pGame );

	// 检查是否需要向GroupMananger汇报
	if( CServerManager::Instance().GetGroupManagerServerId() != 0 )
	{
		ReportGameInfoToGroupManager( pGame );
	}
}

int main(int argc, char *argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	if( argc >= 5 )
	{
		CGameManager::Instance();
		CServerManager::Instance();
		CDynamicServerManager::Instance();
		CReportServerManager::Instance();
		bool bInit = CServerManager::Instance().InitServer<CGameMgrClient, CGameMgrRpcClient, CServerConfig>(
			argv[1], 
			argv[2], 
			argv[3], 
			argv[4] );
		if( bInit && CServerManager::Instance().Start() )
		{
			// 启动汇报游戏状态数据时钟
			s32 _timerId = CServerManager::Instance().AddTimer(
				10000,
				ReportGameInforCallBack,
				NULL,
				0,
				true );

			while( true )
			{
				char cmd[32] = {0};
				gets( cmd );
				if( strcmp( cmd, "quit" ) == 0 )
				{
					break;
				}
				else
				{
					SERVER_LOG_WARN( "unknown cmd" );
				}
			}

			CServerManager::Instance().DelTimer( _timerId );
			CServerManager::Instance().Stop();
		}
		else
		{
			printf("start failed\n");
		}
	}

	google::protobuf::ShutdownProtobufLibrary();
}
