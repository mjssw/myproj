#include "LoginManager.h"
#include "LoginClient.h"
#include "ServerManager.h"
#include "errno.pb.h"
#include "login.pb.h"
#include "center.pb.h"
#include "msgid.pb.h"
using namespace std;

SIGNLETON_CLASS_INIT(CLoginManager);

s32 CLoginManager::HashUser(const std::string &user)
{
	// TODO 
	return 1;
}

void CLoginManager::UserLogin(CLoginClient &client, u64 clientId, const std::string &user, const std::string &pwd, const std::string &flag)
{
	s32 ret = _UserLogin( user, pwd, flag );
	string token = "";
	if( ret == sglib::errorcode::E_ErrorCode_Success )
	{
		token = _BuildToken( user, flag );
		_NotifyCenterUserLogin( clientId, client.GetClientId(), user, flag );
	}
	
	_NotifyLoginResult( client, clientId, ret, token );
}

struct _LoginManagerDBParam
{
	CLoginClient *client;
	u64 clientid;
};

void CLoginManager::UserRegister(CLoginClient &client, u64 clientId, const std::string &user, const std::string &pwd)
{
	_LoginManagerDBParam _param = { &client, clientId };

	s32 id = HashUser( user );
	string sql = "call UserRegister('" + user + "','" + pwd + "','abc')";
	bool ret = CServerManager::Instance().ExecSql( id, sql, this, &CLoginManager::_RegisterCallback, &_param, sizeof(_param) );
	if( !ret )
	{
		SERVER_LOG_ERROR( "CLoginManager,UserRegister,ExecSql," << sql.c_str() );
	}
}

void CLoginManager::UserAskEnterGame(u64 clientid, u64 gateid, s32 gameid)
{		
	sglib::centerproto::ServerCenterEnterGameReq enterReq;
	enterReq.set_clientid( clientid );
	enterReq.set_gateid( gateid );
	enterReq.set_gameid( gameid );
	CServerManager::Instance().SendRpcMsg( CServerManager::Instance().GetCenterServerId(),
		enterReq, sglib::msgid::SCT_ENTER_GAME_REQ );
}

s32 CLoginManager::_UserLogin(const std::string &user, const std::string &pwd, const std::string &flag)
{
	// TODO 
	return sglib::errorcode::E_ErrorCode_Success;
}

void CLoginManager::_NotifyLoginResult(CLoginClient &client, u64 clientId, s32 result, const std::string &token)
{
	sglib::loginproto::SCUserLoginRsp rsp;
	rsp.set_result( result );
	rsp.set_token( token );
	client.SendMsgToClient( clientId, rsp, sglib::msgid::LC_USER_LOGIN_RSP );
}

string CLoginManager::_BuildToken(const std::string &user, const std::string &flag)
{
	// TODO
	return "token_abc";
}

void CLoginManager::_NotifyCenterUserLogin(u64 clientId, u64 gateid, const std::string &user, const std::string &flag)
{
	sglib::loginproto::LoginCenterUserLoginReq req;
	req.set_clientid( clientId );
	req.set_gateid( gateid );
	req.set_user( user );
	req.set_flag( flag );

	CServerManager::Instance().SendRpcMsg( CServerManager::Instance().GetCenterServerId(),
		req, sglib::msgid::LCT_USER_LOGIN_REQ );
}

void CLoginManager::_NotifyRegisterResult(CLoginClient &client, u64 clientId, s32 result)
{
	sglib::loginproto::SCUserRegisterRsp rsp;
	rsp.set_result( result );
	client.SendMsgToClient( clientId, rsp, sglib::msgid::LC_USER_REGISTER_RSP );
}

void CLoginManager::_RegisterCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len)
{
	s32 result = sglib::errorcode::E_ErrorCode_Unknown;
	while( RecordSet && RecordSet->GetRecord() )
	{
		s32 count = (s32)RecordSet->GetRecordCount();
		SELF_ASSERT( count == 1, break; );
		result = sglib::errorcode::E_ErrorCode_RegError;
		const char *val = RecordSet->GetFieldValue( 1 );
		if( atoi(val) == 1 )
		{
			result = sglib::errorcode::E_ErrorCode_Success;
		}

		break;
	}
	
	SERVER_LOG_INFO( "CLoginManager,RegisterCallback," << result );

	SELF_ASSERT( param && len==sizeof(_LoginManagerDBParam), return; );
	_LoginManagerDBParam *_param = (_LoginManagerDBParam*)param;
	SELF_ASSERT( _param->client, return; );
	_NotifyRegisterResult( *(_param->client), _param->clientid, result );
}
