#include "ServerManager.h"
#include "TetrisClient.h"
#include "TetrisRpcClient.h"
#include "Hall.h"
#include "GateClientManager.h"
#include "Room.h"
#include "TetrisConfig.h"
#include "Player.h"
#include "StateManager.h"
#include "PlayerState.h"
#include "Block.h"
#include "GameId.h"
#include "msgid.pb.h"
#include "PlayerStateBase.h"
#include "RoomStateBase.h"
#include "PlayerStateManager.h"
#include "RoomStateManager.h"

class CTBase
{
public:
	void Test(){ printf("CTBase::Test\n"); }
};
typedef void (CTBase::*Proc)();
class CTD1 : public CTBase
{
public:
	void Test2(){ printf("CTD1::Test\n"); }
};

class CA
{
public:
	~CA(){ printf("~CA()\n"); }
};

class CB
{
public:
	~CB(){ printf("~CB()\n"); }
};

class CTest
{
public:
	~CTest(){ printf( "~CTest()\n" ); }
	CA ca;
	CB cb;
};

template<class T1>
class CTemp1
{
};

template<class T1>
class CUser : public CTemp1<T1>
{
};

class CTestUser
{
public:
	CUser<CTestUser> aa;
};
static CTestUser g_tt;

void TestArrayParam(int **arr)
{
}

/////////////////////////////////////////////////////////////////////

void RegisterPlayerState()
{
	CPlayerStateManager::Instance();
	CPlayerStateManager::Instance().InitPlayerStateManager();

	// 其他state在这里注册 

	// 注册状态可以处理的消息
	CPlayerStateManager::Instance().AddStateProcMessage( E_PlayerState_InRoom_Free, sglib::msgid::CS_TETRIS_READY_REQ );
	CPlayerStateManager::Instance().AddStateProcMessage( E_PlayerState_InGame, sglib::msgid::CS_TETRIS_PUT_BLOCK_REQ );
	CPlayerStateManager::Instance().AddStateProcMessage( E_PlayerState_InGame, sglib::msgid::CS_TETRIS_CLEAR_BLOCK_REQ );
	CPlayerStateManager::Instance().AddStateProcMessage( E_PlayerState_InGame, sglib::msgid::CS_TETRIS_GAMELOST_REQ );
}

void RegisterRoomState()
{
	CRoomStateManager::Instance();
	CRoomStateManager::Instance().InitRoomStateManager();

	// 其他state在这里注册 
	CRoomStateManager::Instance().AddStateProcMessage( E_RoomState_Wait, sglib::msgid::CS_TETRIS_READY_REQ );
	CRoomStateManager::Instance().AddStateProcMessage( E_RoomState_InGame, sglib::msgid::CS_TETRIS_PUT_BLOCK_REQ );
	CRoomStateManager::Instance().AddStateProcMessage( E_RoomState_InGame, sglib::msgid::CS_TETRIS_CLEAR_BLOCK_REQ );
	CRoomStateManager::Instance().AddStateProcMessage( E_RoomState_InGame, sglib::msgid::CS_TETRIS_GAMELOST_REQ );
}

int main(int argc, char *argv[])
{
	CTBase cb1;
	Proc pf1 = (Proc)&CTBase::Test;

	CTD1 c1;
	Proc pf = (Proc)&CTD1::Test2;

	(cb1.*pf1)();
	(c1.*pf)();

	{
		CTest t;
	}



	if( argc >= 5 )
	{
		CGateClientManager::Instance();
		CHall::Instance();
		CServerManager::Instance();
		RegisterPlayerState();
		RegisterRoomState();
		CBlockManager::Instance();
		CBlockManager::Instance().Init();

		bool bInit = CServerManager::Instance().InitServer<CTetrisClient, CTetrisRpcClient, CTetrisConfig>(
			argv[1], 
			argv[2], 
			argv[3], 
			argv[4] );
		if( bInit && CServerManager::Instance().Start() )
		{
			CTetrisConfig *pConfig = (CTetrisConfig*)CServerManager::Instance().GetServerConfig(); 
			CHall::Instance().Init( GAME_ID_TETRIS, new CPlayer(), pConfig->GetHallConfig() );
			CHall::Instance().InitRoomList( new CRoom(0) );

			while( true )
			{
				char cmd[32] = {0};
				gets( cmd );
				if( strcmp( cmd, "quit" ) == 0 )
				{
					break;
				}
			}

			CHall::Instance().Fini();
			CServerManager::Instance().Stop();
		}
		else
		{
			printf("start failed\n");
		}
	}
	
	google::protobuf::ShutdownProtobufLibrary();
}
