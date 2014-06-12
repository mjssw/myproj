#include <stdio.h>
#include "TemplateClient.h"
#include "TemplateRpcClient.h"
#include "ServerManager.h"

int main(int argc, char *argv[])
{
	if( argc >= 5 )
	{
		CServerManager::Instance();

		bool bInit = CServerManager::Instance().InitServer<CTemplateClient, CTemplateRpcClient, CServerConfig>(
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
