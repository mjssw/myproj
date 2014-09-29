#include "TetrisRpcClient.h"
#include "ServerManager.h"
#include "TetrisClient.h"
#include "Hall.h"
#include "msgid.pb.h"
#include "center.pb.h"
#include "common.pb.h"
#include "gamemanager.pb.h"
#include "errno.pb.h"

CTetrisRpcClient::CTetrisRpcClient(int nId) : 
	CRpcClient(nId)
{
	_RegisterProc( sglib::msgid::GMS_ENTER_GAME_RSP, 
		&CTetrisRpcClient::_GamemanagerServerEnterGameRsp );
	_RegisterProc( sglib::msgid::GMS_DYNAMIC_SERVERSTART_NTF, 
		&CTetrisRpcClient::_GamemanagerServerDynamicServerStart );
	_RegisterProc( sglib::msgid::GMS_CREATE_GAMEROOM_REQ, 
		&CTetrisRpcClient::_GamemanagerServerCreateGameRoom );
}

CTetrisRpcClient::~CTetrisRpcClient()
{
}

void CTetrisRpcClient::OnConnect()
{
	CRpcClient::OnConnect();
}

void CTetrisRpcClient::OnRecv(const byte *pPkg, s32 nPkgLen)
{
	s32 msgId = *((s32*)pPkg);

	SERVER_LOG_DEBUG( "client:" << GetId() << " recv rpc pkg, len=" << nPkgLen << " ,msgId=" << msgId );

	map<int, ProtoProc>::iterator it = m_mapProtoProc.find( msgId );
	if( it != m_mapProtoProc.end() )
	{
		(this->*it->second)( pPkg+MSG_ID_LEN, nPkgLen-MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CTetrisRpcClient,OnRecv," << GetId() << "," << msgId );
	}
}

void CTetrisRpcClient::_RegisterProc(int id, ProtoProc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}
	
void CTetrisRpcClient::_GamemanagerServerEnterGameRsp(const byte *pPkg, s32 nPkgLen)
{
	sglib::gamemanagerproto::GamemanagerServerEnterGameRsp rsp;
	if( rsp.ParseFromArray(pPkg, nPkgLen) )
	{
		sglib::commonproto::SCEnterGameRsp msg;
		msg.set_result( rsp.result() );
		msg.set_gameid( rsp.gameid() );
		msg.set_ip( rsp.ip() );
		msg.set_port( rsp.port() );

		_SendMsgToClient( rsp.gateid(), rsp.clientid(), msg, sglib::msgid::SC_USER_ENTER_GAME_RSP );
	}
	else
	{
		SERVER_LOG_ERROR( "CTetrisRpcClient,_GamemanagerServerEnterGameRsp,ParseFromArray" );
	}
}

void CTetrisRpcClient::_SendMsgToClient(u64 gateid, u64 clientid, const ::google::protobuf::Message &Msg, s32 nMsgId)
{
	CClient *client = CServerManager::Instance().FindClient( gateid );
	if( client != NULL )
	{
		CTetrisClient *cli = (CTetrisClient*)client;
		cli->SendMsgToClient( clientid, Msg, nMsgId );
	}
	else
	{
		SERVER_LOG_ERROR( "CTetrisRpcClient,_SendMsgToClient," << gateid );
	}
}

void CTetrisRpcClient::_GamemanagerServerDynamicServerStart(const byte *pPkg, s32 nPkgLen)
{
	sglib::gamemanagerproto::GamemanagerServerDynamicServerStartNtf ntf;
	if( ntf.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_DEBUG( "_GamemanagerServerDynamicServerStart id:" << ntf.id() << ", ip" <<\
			ntf.ip().c_str() << " , port:" << ntf.port() );

		sglib::gateproto::ServerGateConnectNewServerNtf cmd;
		cmd.set_id( ntf.id() );
		cmd.set_ip( ntf.ip() );
		cmd.set_port( ntf.port() );

		// 将此消息广播给其所有的gate
		vector<u64> vecClient;
		CServerManager::Instance().GetClientList( vecClient );
		vector<u64>::iterator it = vecClient.begin();
		for( ; it != vecClient.end(); ++it )
		{
			CClient *client = CServerManager::Instance().FindClient( *it );
			if( !client )
			{
				continue;
			}

			CTetrisClient *pClient = (CTetrisClient*)client;
			pClient->SendMsg( cmd, sglib::msgid::SG_CONNECT_NEW_SERVER_NTF );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CTetrisRpcClient,_GamemanagerServerDynamicServerStart,ParseFromArray" );
	}
}
 
void CTetrisRpcClient::_GamemanagerServerCreateGameRoom(const byte *pPkg, s32 nPkgLen)
{
	sglib::gamemanagerproto::GamemanagerServerCreateGameRoomReq req;
	if( req.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_DEBUG( "_GamemanagerServerCreateGameRoom," << req.groupid() << "," <<\
			req.gateresid() << "," << req.clientid() << "," << req.game() << "," << req.serverid() );

		// TODO
		s32 roomid = CHall::Instance().FindFreeRoom( req.groupid() );
		s32 result = (roomid!=INVALID_VAL) ? \
			sglib::errorcode::E_ErrorCode_Success : sglib::errorcode::E_ErrorCode_NotEnoughRoom;

		sglib::gamemanagerproto::ServerGamemanagerCreateGameRoomRsp rsp;
		rsp.set_result( result );
		rsp.set_groupid( req.groupid() );
		rsp.set_gateresid( req.gateresid() );
		rsp.set_clientid( req.clientid() );
		rsp.set_game( req.game() );
		rsp.set_serverid( req.serverid() );
		rsp.set_roomid( roomid );
		rsp.set_password( "" );
		rsp.set_gameid( CHall::Instance().GameId() );
		
		CServerManager::Instance().SendRpcMsg( 
			CServerManager::Instance().GetGameManagerServerId(),
			rsp, sglib::msgid::SGM_CREATE_GAMEROOM_RSP );
	}
	else
	{
		SERVER_LOG_ERROR( "CTetrisRpcClient,_GamemanagerServerCreateGameRoom,ParseFromArray" );
	}
}
