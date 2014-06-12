#include "GameMgrClient.h"
#include "GameMgrRpcClient.h"
#include "ServerManager.h"
#include "GameManager.h"
#include "DynamicServerManager.h"
#include "ReportServerManager.h"
#include "msgid.pb.h"
#include "gamemanager.pb.h"
#include "center.pb.h"
#include "group.pb.h"
#include <vector>
using namespace std;

CGameMgrClient::CGameMgrClient(int nId) : CClient(nId)
{
	_RegisterProc( sglib::msgid::SGM_ENTER_GAME_REQ,	&CGameMgrClient::_AskEnterGame );
	_RegisterProc( sglib::msgid::SGM_REPORT_GAME_INFO,	&CGameMgrClient::_GameServerReportInfo );
	_RegisterProc( sglib::msgid::SGM_GATE_CLOSE_NTF,	&CGameMgrClient::_GameGateClose );
	_RegisterProc( sglib::msgid::SGM_DYNAMIC_START_NTF,	&CGameMgrClient::_GameServerDynamicStart );
	_RegisterProc( sglib::msgid::SGM_CREATE_GAMEROOM_RSP,	&CGameMgrClient::_GameServerCreateGameRoomResult );
}

void CGameMgrClient::OnClose()
{
	CClient::OnClose();

	CReportServerManager::Instance().DelServer( this );	
}

void CGameMgrClient::OnRecv(const byte *pPkg, s32 nPkgLen)
{
	s32 msgId = *((s32*)pPkg);

	map<int, ProtoProc>::iterator it = m_mapProtoProc.find( msgId );
	if( it != m_mapProtoProc.end() )
	{
		(this->*it->second)( pPkg+MSG_ID_LEN, nPkgLen-MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CGameMgrClient,OnRecv," << GetClientId() << "," << msgId );
	}
}

void CGameMgrClient::SendMessageToGameServer(const ::google::protobuf::Message &msg, s32 msgid)
{
	int count = msg.ByteSize();
	byte buf[MAX_MSG_LEN] = {0};
	*((s32*)buf) = msgid;
	if( msg.SerializeToArray(buf+MSG_ID_LEN, MAX_MSG_LEN) )
	{
		if( !SendPkg( buf, count+MSG_ID_LEN ) )
		{
			SERVER_LOG_ERROR( "CGameMgrClient,SendMessageToGameServer,SendPkg" );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGameMgrClient,SendMessageToGameServer,SerializeToArray" );
	}
}

void CGameMgrClient::_RegisterProc(s32 id, ProtoProc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}

void CGameMgrClient::_SendMsgToCenter(const ::google::protobuf::Message &Msg, s32 nMsgId)
{
	CServerManager::Instance().SendRpcMsg( 
		CServerManager::Instance().GetCenterServerId(),
		Msg, 
		nMsgId );
}

void CGameMgrClient::_AskEnterGame(const byte *pPkg, s32 nPkgLen)
{
	sglib::gamemanagerproto::ServerGamemanagerEnterGameReq req;
	if( req.ParseFromArray(pPkg, nPkgLen) )
	{
		sglib::centerproto::ServerCenterEnterGameReq enterReq;
		enterReq.set_clientid( req.clientid() );
		enterReq.set_gateid( req.gateid() );
		enterReq.set_gameid( req.gameid() );
		enterReq.set_gsid( GetClientId() );
		_SendMsgToCenter( enterReq, sglib::msgid::SCT_ENTER_GAME_REQ );
	}
	else
	{
		SERVER_LOG_ERROR( "CGameMgrClient,_AskEnterGame,ParseFromArray" );
	}
}

void CGameMgrClient::_GameServerReportInfo(const byte *pPkg, s32 nPkgLen)
{
	sglib::gamemanagerproto::ServerGamemanagerReportGameInfo info;
	if( info.ParseFromArray(pPkg, nPkgLen) )
	{
		CGameManager::Instance().SetGameServerInfo(
			info.gameid(),
			info.gameserver().serverid(),
			info.gameserver().cur_count(),
			GetClientId() );

		/*
		SERVER_LOG_DEBUG( "===> GameServerReport GameId:" << info.gameid() << ", ServerId:" <<\
			info.gameserver().serverid() << ", CurCount:" << info.gameserver().cur_count() );
		//*/

		for( int i=0; i<info.gateservers_size(); ++i )
		{
			const sglib::publicproto::GateServerInfo &_info = info.gateservers( i );

			CGameManager::Instance().SetGateInfo(
				info.gameid(),
				_info.id(),
				_info.ip().c_str(),
				_info.port(),
				_info.cur_count(),
				_info.max_count() );

			/*
			SERVER_LOG_DEBUG( "\t---> Gate Id:" << _info.id() << ", Ip:" << _info.ip().c_str() <<\
				", Port:" << _info.port() << ", CurCount:" << _info.cur_count() <<\
				", MaxCount:" << _info.max_count() );
			//*/
		}

		// 汇报信息了，从动态服务器列表里删除
		CReportServerManager::Instance().AddServer( this );
		CDynamicServerManager::Instance().DelServer( this );
	}
	else
	{
		SERVER_LOG_ERROR( "CGameMgrClient,_GameServerReportInfo,ParseFromArray" );
	}
}

void CGameMgrClient::_GameGateClose(const byte *pPkg, s32 nPkgLen)
{
	sglib::gamemanagerproto::ServerGamemanagerGateCloseNtf ntf;
	if( ntf.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_DEBUG( "GameGateClose GameId:" << ntf.gameid() << ", GateId:" << ntf.gateid() );

		CGameManager::Instance().DelGateInfo(
			ntf.gameid(), ntf.gateid() );
		
		_NotifyCenterGateClose( ntf.gameid(), ntf.gateid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGameMgrClient,_GameGateClose,ParseFromArray" );
	}
}

void CGameMgrClient::_NotifyCenterGateClose(s32 gameid, s32 gateid)
{
	sglib::gamemanagerproto::GamemanagerCenterGateCloseNtf ntf;
	ntf.set_gameid( gameid );
	ntf.set_gateid( gateid );
		
	_SendMsgToCenter( ntf, sglib::msgid::GMCT_GATE_CLOSE_NTF );
}

void CGameMgrClient::_GameServerDynamicStart(const byte *pPkg, s32 nPkgLen)
{
	sglib::gamemanagerproto::ServerGamemanagerDynamicStartNtf ntf;
	if( ntf.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_DEBUG( "GameServerDynamicStart ServerId:" << ntf.id() << ", ip:" << ntf.ip().c_str() \
			<< ", port:" << ntf.port() << ", GameId:" << ntf.gameid() );

		// 将此动态增加的server加入列表里
		CDynamicServerManager::Instance().AddServer( this );
		
		// 找一个不在 正在动态启动列表 里的server
		CGameMgrClient *client = _FindUnDynamicServerClient( ntf.gameid(), ntf.id() );
		if( client )
		{
			// 找到
			_NotifyNewServerDynamicStart( client, ntf.id(), ntf.ip().c_str(), ntf.port() );
		}
		else
		{
			SERVER_LOG_ERROR( "CGameMgrClient,_GameServerDynamicStart,_FindUnDynamicServerClient" );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGameMgrClient,_GameServerDynamicStart,ParseFromArray" );
	}
}
 
void CGameMgrClient::_GameServerCreateGameRoomResult(const byte *pPkg, s32 nPkgLen)
{
	sglib::gamemanagerproto::ServerGamemanagerCreateGameRoomRsp rsp;
	if( rsp.ParseFromArray(pPkg, nPkgLen) )
	{

		const CGameInfo *game = CGameManager::Instance().GetGameInfo( rsp.gameid() );
		SELF_ASSERT( game, return; ); // TODO
		string ip = "";
		s32 port = 0;
		bool ret = game->GetOptimalGateInfo( ip, port );
		SELF_ASSERT( ret, return; );

		SERVER_LOG_DEBUG( "_GameServerCreateGameRoomResult:" << rsp.result() << "," << rsp.groupid() \
			<< "," << rsp.roomid() << "," << ip.c_str() << "," << port );

		sglib::groupproto::GamemanagerGroupmanagerCreateGameRoomRsp _rsp;
		_rsp.set_result( rsp.result() );
		_rsp.set_groupid( rsp.groupid() );
		_rsp.set_gateresid( rsp.gateresid() );
		_rsp.set_clientid( rsp.clientid() );
		_rsp.set_game( rsp.game() );
		_rsp.set_serverid( rsp.serverid() );
		_rsp.set_ip( ip );
		_rsp.set_port( port );
		_rsp.set_roomid( rsp.roomid() );
		_rsp.set_password( rsp.password() );

		CServerManager::Instance().SendRpcMsg( 
			CServerManager::Instance().GetGroupManagerServerId(), _rsp, 
			sglib::msgid::GMGM_ASK_CREATE_GAMEROOM_RSP );
	}
	else
	{
		SERVER_LOG_ERROR( "CGameMgrClient,_GameServerCreateGameRoomResult,ParseFromArray" );
	}
}

CGameMgrClient* CGameMgrClient::_FindUnDynamicServerClient(s32 gameid, s32 serverid)
{
	vector<u64> vecClient;		
	CServerManager::Instance().GetClientList( vecClient );
	CGameMgrClient *client = NULL;
	for( vector<u64>::iterator it=vecClient.begin(); it!=vecClient.end(); ++it )
	{
		CClient *_tmp = CServerManager::Instance().FindClient( *it );
		if( !_tmp )
		{
			continue;
		}

		if( CDynamicServerManager::Instance().HasServer(_tmp) )
		{
			continue;
		}

		// 如果此服务器不在列表里，还要检查一下这个服务器是否曾经汇报会
		// 状态信息，如果没有汇报过，则可能是另一正在动态启动的服务器
		if( CReportServerManager::Instance().HasServer(_tmp) )
		{
			client = (CGameMgrClient*)_tmp;
			break;
		}
	}

	return client;
}

void CGameMgrClient::_NotifyNewServerDynamicStart(CGameMgrClient *client, s32 id, const char *ip, s32 port)
{
	SG_ASSERT( client != NULL );

	sglib::gamemanagerproto::GamemanagerServerDynamicServerStartNtf ntf;
	ntf.set_id( id );
	ntf.set_ip( ip );
	ntf.set_port( port );
	
	client->SendMessageToGameServer( ntf, sglib::msgid::GMS_DYNAMIC_SERVERSTART_NTF );
}

void CGameMgrClient::_NotifyGameServerClose(s32 gameid, s32 serverid)
{
	// 如果有游戏服动态关闭，只通知center去掉对应的消息，但是不通知游戏服的gate

	sglib::gamemanagerproto::GamemanagerCenterServerCloseNtf ntf;
	ntf.set_gameid( gameid );
	ntf.set_serverid( serverid );

	_SendMsgToCenter( ntf, sglib::msgid::GMCT_GAME_SERVER_CLOSE_NTF );
}
