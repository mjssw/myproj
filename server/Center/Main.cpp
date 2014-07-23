#include "ServerManager.h"
#include "CenterClient.h"
#include "CenterRpcClient.h"
#include "GameManager.h"
#include "GroupInfoManager.h"
#include "UserManager.h"
#include "common.h"
#include "BasicConfig.h"
#include "DBQuery.h"

struct SGroups
{
	u64 count;
	u64 id[USER_GROUP_MAX_COUNT];
};
struct SMember
{
	char name[USER_MAX_LEN];
};
struct SMembers
{
	u64 count;

};
void _TestDbCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len)
{
	if( ErrMsg )
	{
		printf( "err:%s\n", ErrMsg );
		return;
	}
	
	s32 count = (s32)RecordSet->GetRecordCount();
	while( RecordSet && RecordSet->GetRecord() )
	{
		s32 count = (s32)RecordSet->GetRecordCount();
		const char *user = RecordSet->GetFieldValue( 1 );
		const char *group = RecordSet->GetFieldValue( 2 );
		SGroups *gps = (SGroups*)group; 

		printf( "user=%s groupcount=%llu\n", user, gps->count );
	}
}

int main(int argc, char *argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	if( argc >= 5 )
	{
		CServerManager::Instance();
		CGameManager::Instance();
		CGroupInfoManager::Instance();
		CUserManager::Instance();
		bool bInit = CServerManager::Instance().InitServer<CCenterClient, CCenterRpcClient, CServerConfig>(
			argv[1], 
			argv[2], 
			argv[3], 
			argv[4] );
		if( bInit && CServerManager::Instance().Start() )
		{
			while( true )
			{
				char cmd[32] = {0};
				gets( cmd );
				if( strcmp( cmd, "quit" ) == 0 )
				{
					break;
				}
				else if( strcmp( cmd, "tdb" ) == 0 )
				{
					CMysqlManager *mysql = new CMysqlManager( 3, 128, NULL);
					if( mysql )
					{
						bool ret = mysql->Start( "172.18.5.55", "ud1", "root", "123456", 3306 );
						if( ret )
						{
							IDBQuery *dbquery = mysql->GetDBQuery();

							// test 1 update
							/*
							SGroups gps;
							memset( &gps, 0, sizeof(gps) );
							gps.count = 3;
							for(s32 i=0; i<gps.count; ++i) gps.id[i] = i+1;
							string sql = "update user set Groups=";

							char *blob = new char[sizeof(gps) + 1];
							memset( blob, 0, sizeof(gps) + 1 );
							dbquery->AppendStream( blob, (char*)&gps, sizeof(u64)*4 );
							sql += blob;
							sql += " where User='sjj1';";
							ret = mysql->Execute( sql.c_str(), _TestDbCallback, NULL, 0 );
							printf( "excute sql:%s ret=%d\n", sql.c_str(), ret?1:0 );
							//*/

							// test 2 select
							string sql = "select User,Groups from user where User='sjj1';";
							bool ret = mysql->Execute( sql.c_str(), _TestDbCallback, NULL, 0 );
							printf( "excute sql:%s ret=%d\n", sql.c_str(), ret?1:0 );
							//*/

						}

						mysql->Stop();
						SAFE_DELETE( mysql );
					}
				}
			}

			CServerManager::Instance().Stop();
		}
		else
		{
			printf("start failed\n");
		}
	}

	google::protobuf::ShutdownProtobufLibrary();
}
