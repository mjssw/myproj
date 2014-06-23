#include <map>
#include <vector>
#include <string>
using namespace std;

#include "GameManager.h"

class ctCLient;
static map<int, ctCLient*> g_mapClient;

static string g_gameIp;
static s32 g_gamePort;
static bool g_canEnterGame = false;
static string g_userId = "";


void ClientLogin(const char *ip, int port, int clientNum)
{
	CClientManager<ctCLient> mgr(clientNum);
	mgr.Start(ip, port);

	while(true)
	{
		char cmd[64] = {0};
		gets(cmd);
		vector<string> vCmd;
		SplitCmd(cmd, vCmd );
		if( vCmd.size() == 0 )
		{
			printf("no cmd here\n");
			continue;
		}

		if( vCmd[0] == "quit" )
		{
			break;
		}
		else if( vCmd[0] == "login" )
		{
			if( vCmd.size() == 4 )
			{
				g_mapClient.begin()->second->UserLogin(vCmd[1], vCmd[2], vCmd[3]);
			}
		}
		else if( vCmd[0] == "entergame" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->UserEnterGame( atoi(vCmd[1].c_str()) );
			}
		}
		else if( vCmd[0] == "logingame" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->LoginGame( vCmd[1] );
			}
		}
		else if( vCmd[0] == "enterroom" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->EnterRoom( atoi(vCmd[1].c_str()) );
			}
		}
		else if( vCmd[0] == "leaveroom" )
		{
			{
				g_mapClient.begin()->second->LeaveRoom( );
			}
		}
		else if( vCmd[0] == "ready" )
		{
			g_mapClient.begin()->second->Ready( );
		}
		else if( vCmd[0] == "lost" )
		{
			g_mapClient.begin()->second->Lost( );
		}
		else if( vCmd[0] == "put" )
		{
			if( vCmd.size() == 3 )
			{
				g_mapClient.begin()->second->PutBlock( 
					atoi( vCmd[1].c_str() ),
					atoi( vCmd[2].c_str() ) );
			}
		}
		else if( vCmd[0] == "clear" )
		{
			if( vCmd.size() > 1 )
			{
				vector<s32> vecRows;
				for( size_t i=1; i<vCmd.size(); ++i )
				{
					vecRows.push_back(
						atoi( vCmd[i].c_str() ) );
				}
				g_mapClient.begin()->second->ClearBlock( vecRows );
			}
		}
		else if( vCmd[0] == "register" )
		{
			if( vCmd.size() == 3 )
			{
				g_mapClient.begin()->second->Register( vCmd[1], vCmd[2] );
			}
		}
	}

	mgr.Stop();
}

void ClientInGame(const char *ip, int port, int clientNum)
{
	CClientManager<ctCLient> mgr(clientNum);
	mgr.Start(ip, port);

	while(true)
	{
		char cmd[64] = {0};
		gets(cmd);
		vector<string> vCmd;
		SplitCmd(cmd, vCmd );
		if( vCmd.size() == 0 )
		{
			printf("no cmd here\n");
			continue;
		}

		if( vCmd[0] == "quit" )
		{
			break;
		}
		else if( vCmd[0] == "entergame" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->UserEnterGame( atoi(vCmd[1].c_str()) );
			}
		}
		else if( vCmd[0] == "logingame" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->LoginGame( vCmd[1] );
			}
		}
		else if( vCmd[0] == "enterroom" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->EnterRoom( atoi(vCmd[1].c_str()) );
			}
		}
		else if( vCmd[0] == "leaveroom" )
		{
			{
				g_mapClient.begin()->second->LeaveRoom( );
			}
		}
		else if( vCmd[0] == "ready" )
		{
			g_mapClient.begin()->second->Ready( );
		}	
		else if( vCmd[0] == "lost" )
		{
			g_mapClient.begin()->second->Lost( );
		}	
		else if( vCmd[0] == "put" )
		{
			if( vCmd.size() == 3 )
			{
				g_mapClient.begin()->second->PutBlock( 
					atoi( vCmd[1].c_str() ),
					atoi( vCmd[2].c_str() ) );
			}
		}
		else if( vCmd[0] == "clear" )
		{
			if( vCmd.size() > 1 )
			{
				vector<s32> vecRows;
				for( size_t i=1; i<vCmd.size(); ++i )
				{
					vecRows.push_back(
						atoi( vCmd[i].c_str() ) );
				}
				g_mapClient.begin()->second->ClearBlock( vecRows );
			}
		}
	}

	mgr.Stop();
}

void ClientInGroup(const char *ip, int port, int clientNum)
{
	CClientManager<ctCLient> mgr(clientNum);
	mgr.Start(ip, port);

	while(true)
	{
		char cmd[64] = {0};
		gets(cmd);
		vector<string> vCmd;
		SplitCmd(cmd, vCmd );
		if( vCmd.size() == 0 )
		{
			printf("no cmd here\n");
			continue;
		}

		if( vCmd[0] == "quit" )
		{
			break;
		}
		else if( vCmd[0] == "logingroup" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->LoginGroup( vCmd[1] );
			}
		}
		else if( vCmd[0] == "creategroup" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->CreateGroup( vCmd[1] );
			}
		}
		else if( vCmd[0] == "addgroupmember" )
		{
			if( vCmd.size() >= 3 )
			{
				g_mapClient.begin()->second->AddMemberToGroup( vCmd );
			}
		}
		else if( vCmd[0] == "agree" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->AgreeJoinGroup( vCmd[1] );
			}
		}
		else if( vCmd[0] == "groupmsg" )
		{
			if( vCmd.size() == 3 )
			{
				g_mapClient.begin()->second->GroupMessage( vCmd[1], vCmd[2] );
			}
		}
		else if( vCmd[0] == "leavegroup" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->LeaveGroup( vCmd[1] );
			}
		}
		else if( vCmd[0] == "deletegroup" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->DeleteGroup( vCmd[1] );
			}
		}
		else if( vCmd[0] == "createroom" )
		{
			if( vCmd.size() == 3 )
			{
				g_mapClient.begin()->second->AskCreateGameRoom( vCmd[1], vCmd[2] );
			}
		}
	}

	mgr.Stop();
}


void testtime()
{
	char strTime[256] = {0};
	time_t tNow;
	time( &tNow );
	strftime( strTime, sizeof(strTime), "%Y-%m-%d %X",localtime(&tNow) );
	printf( "%s\n", strTime );
}

void TestGroupGame(const char *ip, s32 port)
{
	CGameMgr mgr;
	mgr.Start( ip, port );
}

extern void TestForTestServer(const char *ip, int port, int clientNum, int freq);
extern void TestForIoService(const char *ip, int port, int clientNum, int freq);
int main(int argc, char *argv[])
{
	if( argc == 3 )
	{
		TestGroupGame( argv[1], atoi(argv[2]) );
		return 0;
	}
	return 0;

	/*
	testtime();
	if( argc == 5 )
	{
		//TestForTestServer( argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]) );
		TestForIoService( argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]) );
	}
	return 0;
	//*/

	vector<string> vec;
	SplitCmd("login", vec);
	vec.clear();
	SplitCmd("entergame 10", vec);

	if( argc == 4 )
	{
		/*
		printf("====== player enter GROUP ======\n");
		ClientInGroup(argv[1], atoi(argv[2]), atoi(argv[3]));
		printf("====== player leave GROUP ======\n");
		//*/

		printf("====== player enter LOGIN ======\n");
		ClientLogin(argv[1], atoi(argv[2]), atoi(argv[3]));
		printf("====== player leave LOGIN ======\n");
		if( g_canEnterGame )
		{
			printf("====== player enter GAME ======\n");
			ClientInGame(g_gameIp.c_str(), g_gamePort, 1);
			printf("====== player leave GAME ======\n");
		}
		//*/
	}
    
	return 0;
}

