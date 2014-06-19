#include "LoginRpcClient.h"
#include "LoginClient.h"
#include "msgid.pb.h"
#include "login.pb.h"
#include "center.pb.h"
#include "common.pb.h"
#include "ServerManager.h"

CLoginRpcClient::CLoginRpcClient(int nId) : CRpcClient(nId)
{
	_RegisterProc( sglib::msgid::CTL_USER_LOGIN_RSP,   &CLoginRpcClient::CenterLoginUserLoginRsp );
	_RegisterProc( sglib::msgid::CTL_GAME_INFO_NOTIFY, &CLoginRpcClient::CenterLoginGameInfoNotify );
	_RegisterProc( sglib::msgid::CTS_ENTER_GAME_RSP,   &CLoginRpcClient::CenterServerEnterGameRsp );
}

void CLoginRpcClient::OnConnect()
{
	CRpcClient::OnConnect();
}

void CLoginRpcClient::OnRecv(const byte *pPkg, s32 nPkgLen)
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
		SERVER_LOG_ERROR( "client:" << GetId() << " not found msg process, msgId=" << msgId );
	}
}

void CLoginRpcClient::_RegisterProc(int id, ProtoProc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}
	
void CLoginRpcClient::_SendMsgToClient(u64 gateid, u64 clientid, const ::google::protobuf::Message &Msg, s32 nMsgId)
{
	CClient *client = CServerManager::Instance().FindClient( gateid );
	if( client != NULL )
	{
		CLoginClient *cli = (CLoginClient*)client;
		cli->SendMsgToClient( clientid, Msg, nMsgId );
	}
	else
	{
		SERVER_LOG_ERROR( "CLoginRpcClient can not find client:" << gateid << " failed." );
	}
}

void CLoginRpcClient::CenterLoginUserLoginRsp(const byte *pPkg, s32 nPkgLen)
{
	sglib::centerproto::CenterLoginUserLoginRsp rsp;
	if( rsp.ParseFromArray(pPkg, nPkgLen) )
	{
		// TODO
		sglib::loginproto::SCUserLoginRsp msg;
		msg.set_result( rsp.result() );
		msg.set_token( "abc" );
		//_SendMsgToClient( rsp.gateid(), rsp.clientid(), msg, sglib::msgid::LC_USER_LOGIN_RSP );
	}
	else
	{
		SERVER_LOG_ERROR( "CenterLoginUserLoginRsp ParseFromArray failed." );
	}
}

void CLoginRpcClient::CenterLoginGameInfoNotify(const byte *pPkg, s32 nPkgLen)
{
	sglib::centerproto::CenterLoginGameInfoNotify ntf;
	if( ntf.ParseFromArray(pPkg, nPkgLen) )
	{
		// TODO
		sglib::loginproto::SCGameInfoNotify msg;
		msg.mutable_games()->CopyFrom( ntf.games() );
		_SendMsgToClient( ntf.gateid(), ntf.clientid(), msg, sglib::msgid::LC_GAME_INFO_NOTIFY );
	}
	else
	{
		SERVER_LOG_ERROR( "CenterLoginGameInfoNotify ParseFromArray failed." );
	}
}

void CLoginRpcClient::CenterServerEnterGameRsp(const byte *pPkg, s32 nPkgLen)
{
	sglib::centerproto::CenterServerEnterGameRsp rsp;
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
		SERVER_LOG_ERROR( "CenterServerEnterGameRsp ParseFromArray failed." );
	}
}
