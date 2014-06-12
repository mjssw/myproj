#include "TetrisClient.h"
#include "ServerManager.h"
#include "Hall.h"
#include "Player.h"
#include "Room.h"
#include "msgid.pb.h"
#include "common.pb.h"
#include "errno.pb.h"
#include "tetris.pb.h"
#include "gamemanager.pb.h"
using namespace std;

CTetrisClient::CTetrisClient(int nId) : CGateClientBase(nId)
{
	_RegisterClientProc( sglib::msgid::CS_LOGIN_GAME_REQ, &CTetrisClient::_LoginGameProc );
}

void CTetrisClient::ProcPlayerMessage(u64 gateid, u64 clientid, s32 msgid, const byte *pkg, s32 len)
{
	map<int, ClientProtoProcFunc>::iterator it = m_mapProtoProc.find( msgid );
	if( it != m_mapProtoProc.end() )
	{
		(this->*it->second)(
			clientid,
			pkg, 
			len);
	}
	else
	{
		CHall::Instance().ProcPlayerMessage(
			gateid,
			clientid,
			msgid,
			pkg,	
			len );
	}
}

void CTetrisClient::OnClientClose(u64 clientid)
{
	_PlayerClose( GetClientId(), clientid );
	CGateClientBase::OnClientClose( clientid );
}

void CTetrisClient::OnClose()
{
	CGateClientBase::OnClose();

	// 通知 GameManager 有游戏的gate关闭了
	_NotifyGateClose();
}

void CTetrisClient::OnAccept()
{
	CGateClientBase::OnAccept();

	// 有Gate连接上来了，设置此标志
	CHall::Instance().SetGameServerReady();
}

void CTetrisClient::_RegisterClientProc(s32 id, ClientProtoProcFunc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}

void CTetrisClient::_LoginGameProc(u64 clientid, const byte *pkg, s32 len)
{
	sglib::commonproto::CSLoginGameReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_DEBUG( "client:"<< clientid << " CSLoginGameReq: userid=" <<
			req.userid() << " token=" << req.token() );
		_LoginGame( clientid, req.userid(), req.token() );
	}
	else
	{
		SERVER_LOG_ERROR( "CTetrisClient,_LoginGameProc,ParseFromArray" );
	}
}

void CTetrisClient::_LoginGame(u64 clientid, const std::string &userId, const std::string &token)
{
	s32 ret = sglib::errorcode::E_ErrorCode_Success;
	CPlayerBase *tmp = CHall::Instance().FindPlayer( GetClientId(), clientid );
	if( tmp != NULL )
	{
		ret = sglib::errorcode::E_ErrorCode_AleardyLoginGame;
	}
	else
	{
		CHall::Instance().EnterHall( GetClientId(), clientid, userId );
	}
	_NotifyLoginGameResult( clientid, ret );
}

void CTetrisClient::_NotifyLoginGameResult(u64 clientid, s32 result)
{
	sglib::commonproto::SCLoginGameRsp rsp;
	rsp.set_result( result );
	SendMsgToClient( clientid, rsp, sglib::msgid::SC_LOGIN_GAME_RSP );	
}

void CTetrisClient::_PlayerClose(u64 gateid, u64 clientid)
{
	CPlayerBase *pPlayer = CHall::Instance().FindPlayer( gateid, clientid );
	if( pPlayer != NULL )
	{
		CRoomBase *pRoom = pPlayer->GetRoom();
		if( pRoom != NULL )
		{
			CRoom *room = (CRoom*)pRoom;
			CPlayer *player = (CPlayer*)pPlayer;
			room->PlayerForceLeave( player );
		}
		CHall::Instance().LeaveHall( GetClientId(), clientid );
	}
	else
	{
		SERVER_LOG_ERROR( "CTetrisClient,_PlayerClose," << gateid << "," << clientid );
	}
}

void CTetrisClient::_NotifyGateClose()
{
	sglib::gamemanagerproto::ServerGamemanagerGateCloseNtf ntf;
	ntf.set_gameid( CHall::Instance().GameId() );
	ntf.set_gateid( m_GateId );

	CServerManager::Instance().SendRpcMsg( 
		CServerManager::Instance().GetGameManagerServerId(),
		ntf, sglib::msgid::SGM_GATE_CLOSE_NTF );
}
