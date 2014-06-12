#include "GameMgrRpcClient.h"
#include "GameMgrClient.h"
#include "ServerManager.h"
#include "GameManager.h"
#include "msgid.pb.h"
#include "center.pb.h"
#include "gamemanager.pb.h"
#include "group.pb.h"

CGameMgrRpcClient::CGameMgrRpcClient(int nId) : CRpcClient(nId)
{
	_RegisterProc( sglib::msgid::CTS_ENTER_GAME_RSP,   
		&CGameMgrRpcClient::_CenterServerEnterGameRsp );
	_RegisterProc( sglib::msgid::GMGM_ASK_CREATE_GAMEROOM_REQ,   
		&CGameMgrRpcClient::_GroupManagerAskCreateGameRoomProc );
}

void CGameMgrRpcClient::OnConnect()
{
	CRpcClient::OnConnect();
}

void CGameMgrRpcClient::OnRecv(const byte *pPkg, s32 nPkgLen)
{
	s32 msgId = *((s32*)pPkg);

	map<int, RpcProtoProc>::iterator it = m_mapProtoProc.find( msgId );
	if( it != m_mapProtoProc.end() )
	{
		(this->*it->second)( pPkg+MSG_ID_LEN, nPkgLen-MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CGameMgrRpcClient,OnRecv," << GetId() << "," << msgId );
	}
}

void CGameMgrRpcClient::_RegisterProc(int id, RpcProtoProc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}

void CGameMgrRpcClient::_CenterServerEnterGameRsp(const byte *pkg, s32 len)
{
	sglib::centerproto::CenterServerEnterGameRsp rsp;
	if( rsp.ParseFromArray(pkg, len) )
	{
		sglib::gamemanagerproto::GamemanagerServerEnterGameRsp msg;
		msg.set_result( rsp.result() );
		msg.set_gameid( rsp.gameid() );
		msg.set_ip( rsp.ip() );
		msg.set_port( rsp.port() );

		CClient *client = CServerManager::Instance().FindClient( rsp.gsid() );
		if( client != NULL )
		{
			CGameMgrClient *pClient = (CGameMgrClient*)client;
			pClient->SendMessageToGameServer( msg, sglib::msgid::GMS_ENTER_GAME_RSP );
		}
		else
		{
			SERVER_LOG_ERROR( "CGameMgrRpcClient,_CenterServerEnterGameRsp,FindClient" );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGameMgrRpcClient,_CenterServerEnterGameRsp,ParseFromArray" );
	}
}
 
void CGameMgrRpcClient::_GroupManagerAskCreateGameRoomProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupmanagerGamemanagerCreateGameRoomReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "CGameMgrRpcClient,_GroupManagerAskCreateGameRoomProc," \
			<< req.groupid() << "," << req.gateresid() << "," << req.clientid() << "," \
			<< req.game() << "," << req.gameid() << "," << req.serverid() );

		const CGameInfo *game = CGameManager::Instance().GetGameInfo( req.gameid() );
		if( game )
		{
			u64 gameServerId = game->GetOptimalGameServerInfo();
			if( gameServerId != 0 )
			{
				CClient *client = CServerManager::Instance().FindClient( gameServerId );
				SELF_ASSERT( client, return; ); // TODO
				CGameMgrClient *pClient = (CGameMgrClient*)client;

				sglib::gamemanagerproto::GamemanagerServerCreateGameRoomReq _req;
				_req.set_groupid( req.groupid() );
				_req.set_gateresid( req.gateresid() );
				_req.set_clientid( req.clientid() );
				_req.set_game( req.game() );
				_req.set_serverid( req.serverid() );
				pClient->SendMessageToGameServer( _req, sglib::msgid::GMS_CREATE_GAMEROOM_REQ );
			}
			else
			{
				// TODO
			}
		}
		else
		{
			// TODO
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGameMgrRpcClient,_GroupManagerAskCreateGameRoomProc,ParseFromArray" );
	}
}
