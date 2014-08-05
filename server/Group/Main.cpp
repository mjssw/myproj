#include <stdio.h>
#include "GroupClient.h"
#include "GroupRpcClient.h"
#include "ServerManager.h"
#include "GroupManager.h"
#include "GroupConfig.h"

void test()
{
	time_t now;
	time( &now );
	char strtime[64] = {0};
	strftime( strtime, sizeof(strtime), "%Y-%m-%d",localtime(&now) );
	printf( "%s\n", strtime );
}

int main(int argc, char *argv[])
{
	test();
	if( argc >= 5 )
	{
		CServerManager::Instance();
		CGroupManager::Instance();

		bool bInit = CServerManager::Instance().InitServer<CGroupClient, CGroupRpcClient, CGroupConfig>(
			argv[1], 
			argv[2], 
			argv[3], 
			argv[4] );
		if( bInit && CServerManager::Instance().Start() )
		{
			const CGroupConfig *pConfig = (const CGroupConfig*)CServerManager::Instance().GetServerConfig();
			SG_ASSERT( pConfig != NULL );
			CGroupManager::Instance().StartReportStatusTimer( pConfig->IsDynamicStart() );

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
