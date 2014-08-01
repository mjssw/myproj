#include <stdio.h>
#include "GroupManagerClient.h"
#include "GroupManagerRpcClient.h"
#include "ServerManager.h"
#include "GroupInfoManager.h"
#include "GateClientManager.h"
#include "LoginMemberManager.h"
#include "GroupIdManager.h"
#include "GroupManager.h"
#include "GameInfoMgr.h"
#include "LuaModule.h"
#include "group.pb.h"
#include "msgid.pb.h"
using namespace std;

#include "mysql.h"
static void ProcessRecord(MYSQL_RES *res)
{		
	MYSQL_ROW row = NULL;
	while( row = mysql_fetch_row( res ) )
	{
		const char *val = row[0];
		if( val )
		{
			printf( "val:%s\n", val );
		}
	}

	mysql_free_result( res );
}
static void TestMysqlQueryResult()
{
	MYSQL *conn = mysql_init(NULL);
	MYSQL *mCnn = mysql_real_connect( conn, 
		"172.18.5.55", "root", "123456", "ud1", 3306, NULL, CLIENT_MULTI_STATEMENTS );
	if( conn )
	{
		int ret = mysql_query(conn, "SET NAMES 'utf8';");
		printf( "query result:%d\n", ret );

		string sql = "call TestProc()";
		ret = mysql_query(conn, sql.c_str());
		if( ret == 0 )
		{
			MYSQL_RES *res = mysql_store_result( conn );
			ProcessRecord( res );
			while( !mysql_next_result( conn ) )
			{
				res = mysql_store_result( conn );
				if( res != NULL )
				{
					ProcessRecord( res );
				}
			}
		}

		mysql_close( conn );
	}
}

static void ReportGroupInforCallBack(void *pData, s32 nDataLen)
{
	sglib::groupproto::GroupmanagerCenterReportGroupGateInfo info;
	vector<s32> vecId;
	CGroupInfoManager::Instance().GetGateList( vecId );
	vector<s32>::iterator it = vecId.begin();
	for( ; it != vecId.end(); ++it )
	{
		const CGateClientInfo *_info = CGroupInfoManager::Instance().GetGroupGateInfo( *it );
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
	
	CServerManager::Instance().SendRpcMsg(
		CServerManager::Instance().GetCenterServerId(),
		info,
		sglib::msgid::GMCT_GP_GROUP_GATE_INFO );
}

int main(int argc, char *argv[])
{
	//TestMysqlQueryResult();return 0;

	CLuaModule::Instance().Init();
	CLuaModule::Instance().LoadScripts( "../Scripts/GetGameById.lua" );

	if( argc >= 5 )
	{
		CServerManager::Instance();
		CGroupInfoManager::Instance();
		CLoginMemberManager::Instance();
		CGroupIdManager::Instance();
		CGroupIdManager::Instance().Init( 1000 );
		CGroupManager::Instance();
		CGameInfoMgr::Instance();

		bool bInit = CServerManager::Instance().InitServer<CGroupManagerClient, CGroupManagerRpcClient, CServerConfig>(
			argv[1], 
			argv[2], 
			argv[3], 
			argv[4] );
		if( bInit && CServerManager::Instance().Start() )
		{
			bool ret = CServerManager::Instance().InitNextGroupId();
			if( !ret )
			{
				printf( "CServerManager::Instance().InitNextGroupId failed\n" );
				goto _EndProgress;
			}
			CServerManager::Instance().WaitInitGroupId();
			CGroupIdManager::Instance().Init( CServerManager::Instance().GetNextGroupId() );
			SERVER_LOG_INFO( "InitNextGroupIdDone," << CServerManager::Instance().GetNextGroupId() );

			// 启动汇报游戏状态数据时钟
			s32 _timerId = CServerManager::Instance().AddTimer(
				10000,
				ReportGroupInforCallBack,
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
				else if( strcmp(cmd, "groupinfo") == 0 )
				{
					CGroupManager::Instance().DisplayInfo();
					CLoginMemberManager::Instance().MemberManager().DisplayInfo();
					CLoginMemberManager::Instance().DisplayInfo();
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
_EndProgress:
	CLuaModule::Instance().Fini();
	google::protobuf::ShutdownProtobufLibrary();
}
