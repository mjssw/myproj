#include "LoginClient.h"
#include "LoginRpcClient.h"
#include "ServerManager.h"
#include "LoginManager.h"
#include "gate.pb.h"
#include "client.pb.h"
#include "center.pb.h"
#include "msgid.pb.h"
#include "errno.pb.h"
#include "common.pb.h"
using namespace std;

CLoginClient::CLoginClient(int nId) : CGateClientBase(nId)
{
	// Client msg
	_RegisterClientProc( sglib::msgid::CL_USER_LOGIN_REQ,		&CLoginClient::_UserLoginProc );
	_RegisterClientProc( sglib::msgid::CS_USER_ENTER_GAME_REQ,	&CLoginClient::_UserEnterGameProc );
	_RegisterClientProc( sglib::msgid::CL_USER_REGISTER_REQ,	&CLoginClient::_UserRegisterProc );
}

void CLoginClient::_RegisterClientProc(s32 id, ClientProtoProcFunc proc)
{
	if( proc != NULL )
	{
		m_mapLoginProtoProc[ id ] = proc;
	}
}

void CLoginClient::ProcPlayerMessage(u64 gateid, u64 clientid, s32 msgid, const byte *pkg, s32 len)
{
	SERVER_LOG_DEBUG( "CLoginClient::ProcPlayerMessage msgId=" << msgid << " from client=" << clientid << " msgLen=" << len );

	map<int, ClientProtoProcFunc>::iterator it = m_mapLoginProtoProc.find( msgid );
	if( it != m_mapLoginProtoProc.end() )
	{
		(this->*it->second)( clientid, pkg, len );
	}
	else
	{
		SERVER_LOG_ERROR( "CLoginClient,ProcPlayerMessage," << msgid );
	}
}

void CLoginClient::_UserLoginProc(u64 clientId, const byte *pPkg, s32 nPkgLen)
{
	sglib::loginproto::CSUserLoginReq req;
	if( req.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_DEBUG( "recv client CLUserLoginReq: user=" << req.user() << ", pwd=" << req.password() \
			<< "clientid:" << clientId );
		
		CLoginManager::Instance().UserLogin( *this, clientId, req.user(), req.password(), req.flag() );
	}
	else
	{
		SERVER_LOG_ERROR( "CLoginClient,_UserLoginProc,ParseFromArray" );
	}
}

void CLoginClient::_UserRegisterProc(u64 clientId, const byte *pPkg, s32 nPkgLen)
{
	sglib::loginproto::CSUserRegisterReq req;
	if( req.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_DEBUG( "recv client CSUserRegisterReq : user=" << req.user() << ", pwd=" << req.password() \
			<< "clientid:" << clientId );

		CLoginManager::Instance().UserRegister( *this, clientId, req.user(), req.password() );
	}
	else
	{
		SERVER_LOG_ERROR( "CLoginClient,_UserRegisterProc,ParseFromArray" );
	}
}

void CLoginClient::_UserEnterGameProc(u64 clientId, const byte *pPkg, s32 nPkgLen)
{
	sglib::commonproto::CSEnterGameReq req;
	if( req.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_DEBUG( "recv client CSEnterGameReq : gameid=" << req.gameid() << " clientid=" << clientId );

		CLoginManager::Instance().UserAskEnterGame( clientId, GetClientId(), req.gameid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CLoginClient,_UserEnterGameProc,ParseFromArray" );
	}
}
