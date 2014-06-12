#include "ServerManager.h"
#include "CenterClient.h"
#include "CenterRpcClient.h"
#include "GameManager.h"
#include "GroupInfoManager.h"
#include "common.h"

int main(int argc, char *argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	if( argc >= 5 )
	{
		CServerManager::Instance();
		CGameManager::Instance();
		CGroupInfoManager::Instance();
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
