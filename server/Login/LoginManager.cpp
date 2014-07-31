#include "LoginManager.h"
#include "LoginClient.h"
#include "LoginRpcClient.h"
#include "ServerManager.h"
#include "errno.pb.h"
#include "login.pb.h"
#include "center.pb.h"
#include "msgid.pb.h"
#include "common.pb.h"
#include "Utils.h"
using namespace std;

SIGNLETON_CLASS_INIT(CLoginManager);

struct _LoginManagerDBParam
{
	CLoginClient *client;
	u64 clientid;
};

void CLoginManager::UserLogin(CLoginClient &client, u64 clientId, const std::string &user, const std::string &pwd, const std::string &flag)
{
	CUser *puser = NULL;
	map<string, CUser*>::iterator it = m_userLogin.find( user );
	if( it != m_userLogin.end() )
	{
		if( it->second->GetState() == CUser::E_State_Logining )
		{
			// 忽略正在登录中的请求
			return;
		}

		if( it->second->GetState() == CUser::E_State_LoginSuccess )
		{
			// 已经登录成功
			_NotifyLoginResult( client, clientId, sglib::errorcode::E_ErrorCode_Success, it->second->GetToken() );
			return;
		}
		puser = it->second;
	}
	else
	{
		puser = new CUser( user );
		SELF_ASSERT( puser, return; );
		m_userLogin[ user ] = puser;
		m_userIndex[ client.GetClientId() ][ clientId ] = puser;
	}
	puser->SetState( CUser::E_State_Logining );
	puser->SetFlag( flag );

	SERVER_LOG_DEBUG( "User:" << user << "Login now, check valid from db" );

	_LoginManagerDBParam _param = { &client, clientId };
	s32 id = CServerManager::Instance().HashUser( user );
	string sql = "call UserLogin('" + user + "','" + pwd + "');";
	bool ret = CServerManager::Instance().ExecSql( id, sql, this, &CLoginManager::_UserLoginCallback, &_param, sizeof(_param) );
	if( !ret )
	{
		SERVER_LOG_ERROR( "CLoginManager,UserLogin,ExecSql," << sql.c_str() );
	}
}

void CLoginManager::UserRegister(CLoginClient &client, u64 clientId, const std::string &user, const std::string &pwd, const std::string &name, s32 sex, const std::string &head)
{
	SERVER_LOG_INFO( "CLoginManager,UserRegister," << user.c_str() << "," << pwd.c_str() << "," << name.c_str() \
		<< "," << sex << "," << head.c_str() );

	_LoginManagerDBParam _param = { &client, clientId };

	CUserBasic::ESex _sex = CUserBasic::ConvertSex( sex );
	char strSex[8] = {0};
	sprintf( strSex, "%d", (s32)_sex );
	s32 id = CServerManager::Instance().HashUser( user );
	string sql = "call UserRegister('" + user + "','" + pwd + "','" + name + "'," + strSex + ",'" + head + "');";
	bool ret = CServerManager::Instance().ExecSql( id, sql, this, &CLoginManager::_RegisterCallback, &_param, sizeof(_param) );
	if( !ret )
	{
		SERVER_LOG_ERROR( "CLoginManager,UserRegister,ExecSql," << sql.c_str() );
	}
}

void CLoginManager::UserAskEnterGame(u64 clientid, u64 gateid, s32 gameid)
{
	// 检查是否已经登录了
	CUser *user = _FindUser( gateid, clientid );
	if( !user || user->GetState() != CUser::E_State_LoginSuccess )
	{
		return;
	}

	sglib::centerproto::ServerCenterEnterGameReq enterReq;
	enterReq.set_clientid( clientid );
	enterReq.set_gateid( gateid );
	enterReq.set_gameid( gameid );
	CServerManager::Instance().SendRpcMsg( CServerManager::Instance().GetCenterServerId(),
		enterReq, sglib::msgid::SCT_ENTER_GAME_REQ );
}

void CLoginManager::UserClose(u64 clientid, u64 gateid)
{
	map<u64, map<u64, CUser*> >::iterator it = m_userIndex.find( gateid );
	if( it == m_userIndex.end() )
	{
		return;
	}
	map<u64, CUser*>::iterator _it = it->second.find( clientid );
	if( _it == it->second.end() || !_it->second )
	{
		return;
	}

	CUser *puser = _it->second;
	SERVER_LOG_INFO( "CLoginManager,UserClose," << puser->User() << "," << puser->GetState() );
	it->second.erase( _it );
	{
		map<string, CUser*>::iterator it = m_userLogin.find( puser->User() );
		SELF_ASSERT( it != m_userLogin.end(), ; );
		m_userLogin.erase( it );
	}
	SAFE_DELETE( puser );
}

void CLoginManager::UserLogout(u64 gateid, u64 clientid, const std::string &user)
{
	CUser *puser = _FindUser( gateid, clientid );
	if( !puser )
	{
		SERVER_LOG_ERROR( "CLoginManager,UserLogout," << gateid << "," << clientid << "," << user.c_str() );
		return;
	}

	SERVER_LOG_INFO( "CLoginManager,UserLogout," << puser->User() << "," << puser->GetState() );

	if( puser->GetState() == CUser::E_State_LoginSuccess ||
		puser->GetState() == CUser::E_State_Logining )
	{
		_NotifyCenterUserLogout( user );
	}
	puser->SetState( CUser::E_State_Logout );
}

void CLoginManager::GameInfoNotifyToUser(CLoginRpcClient &rpcClient, sglib::centerproto::CenterLoginGameInfoNotify &ntf)
{
	sglib::loginproto::SCGameInfoNotify msg;
	msg.mutable_games()->CopyFrom( ntf.games() );
	rpcClient.SendMsgToClient( ntf.gateid(), ntf.clientid(), msg, sglib::msgid::LC_GAME_INFO_NOTIFY );
}

void CLoginManager::GroupGateNotifyToUser(CLoginRpcClient &rpcClient, u64 gateid, u64 clientid, const std::string &ip, s32 port)
{
	sglib::loginproto::SCGroupGateNotify msg;
	msg.set_ip( ip );
	msg.set_port( port );
	rpcClient.SendMsgToClient( gateid, clientid, msg, sglib::msgid::LC_GROUP_GATE_NOTIFY );
}

void CLoginManager::EnterGameRspToUser(CLoginRpcClient &rpcClient, u64 gateid, u64 clientid, s32 result, s32 gameid, const std::string &ip, s32 port)
{
	sglib::commonproto::SCEnterGameRsp msg;
	msg.set_result( result );
	msg.set_gameid( gameid );
	msg.set_ip( ip );
	msg.set_port( port );

	rpcClient.SendMsgToClient( gateid, clientid, msg, sglib::msgid::SC_USER_ENTER_GAME_RSP );
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
	string token = user + "|" + flag + "|" + CServerManager::TimeNow();
	return token;
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

CUser* CLoginManager::_FindUser(u64 gateid, u64 clientid)
{
	map<u64, map<u64, CUser*> >::iterator it = m_userIndex.find( gateid );
	if( it == m_userIndex.end() )
	{
		return NULL;
	}
	map<u64, CUser*>::iterator _it = it->second.find( clientid );
	if( _it == it->second.end() || !_it->second )
	{
		return NULL;
	}

	return _it->second;
}

void CLoginManager::_NotifyCenterUserLogout(const std::string &user)
{
	sglib::centerproto::ServerCenterUserLogoutNotify ntf;
	ntf.set_user( user );
	CServerManager::Instance().SendRpcMsg( CServerManager::Instance().GetCenterServerId(),
		ntf, sglib::msgid::SCT_USER_LOGOUT_NOTIFY );
}

void CLoginManager::_GetUserBasicInfo(const char *user, CLoginClient &client, u64 clientId)
{
	SERVER_LOG_DEBUG( "User:" << user << "Login success, try get basic info from db" );

	_LoginManagerDBParam _param = { &client, clientId };

	s32 id = CServerManager::Instance().HashUser( user );
	string sql = "select Name,Head,Sex,Exp,Level,Gold,Diamond from user where user='";
	sql += user;
	sql += "';";
	bool ret = CServerManager::Instance().ExecSql( id, sql, this, &CLoginManager::_GetUserBasicInfoCallback, &_param, sizeof(_param) );
	if( !ret )
	{
		SERVER_LOG_ERROR( "CLoginManager,_GetUserBasicInfo,ExecSql," << sql.c_str() );
	}
}

void CLoginManager::_NotifyUserBasicInfo(CUser &user, CLoginClient &client, u64 clientId)
{
	sglib::loginproto::SCUserBasicInfoNotify ntf;
	ntf.set_name( user.GetBasic().GetName() );
	ntf.set_sex( user.GetBasic().GetSex() );
	ntf.set_head( user.GetBasic().GetHead() );
	ntf.set_exp( user.GetBasic().GetExp() );
	ntf.set_level( user.GetBasic().GetLevel() );
	ntf.set_gold( user.GetBasic().GetMoney( CUserBasic::E_Money_Gold ) );
	ntf.set_diamond( user.GetBasic().GetMoney( CUserBasic::E_Money_Diamond ) );

	client.SendMsgToClient( clientId, ntf, sglib::msgid::LC_USER_BASIC_INFO_NOTIFY );
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

void CLoginManager::_UserLoginCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len)
{
	s32 result = sglib::errorcode::E_ErrorCode_Unknown;
	while( RecordSet && RecordSet->GetRecord() )
	{
		s32 count = (s32)RecordSet->GetRecordCount();
		SELF_ASSERT( count == 1, break; );
		result = sglib::errorcode::E_ErrorCode_LoginError;
		const char *val = RecordSet->GetFieldValue( 1 );
		if( atoi(val) == 1 )
		{
			result = sglib::errorcode::E_ErrorCode_Success;
		}

		break;
	}

	SELF_ASSERT( param && len==sizeof(_LoginManagerDBParam), return; );
	_LoginManagerDBParam *_param = (_LoginManagerDBParam*)param;
	SELF_ASSERT( _param->client, return; );

	CUser *puser = _FindUser( _param->client->GetClientId(), _param->clientid );
	if( !puser )
	{
		SERVER_LOG_ERROR( "CLoginManager,_UserLoginCallback," << _param->client->GetClientId() << "," \
			<< _param->clientid << ",AleardyClose"  );
		return;
	}
	
	SERVER_LOG_DEBUG( "User:" << puser->User() << "Login, check valid from db, result:" << result );

	SELF_ASSERT( puser && puser->GetState() == CUser::E_State_Logining, return; );

	string token = "";
	if( result == sglib::errorcode::E_ErrorCode_Success )
	{
		puser->SetState( CUser::E_State_LoginSuccess );
		token = _BuildToken( puser->User(), puser->GetFlag() );
		_NotifyCenterUserLogin( _param->clientid, _param->client->GetClientId(), puser->User(), puser->GetFlag() );
	}
	else
	{
		puser->SetState( CUser::E_State_LoginFailed );
	}
	
	_NotifyLoginResult( *_param->client, _param->clientid, result, token );
	if( result == sglib::errorcode::E_ErrorCode_Success )
	{
		_GetUserBasicInfo( puser->User(), *_param->client, _param->clientid );
	}
}

void CLoginManager::_GetUserBasicInfoCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len)
{
	SELF_ASSERT( param && len==sizeof(_LoginManagerDBParam), return; );
	_LoginManagerDBParam *_param = (_LoginManagerDBParam*)param;
	SELF_ASSERT( _param->client, return; );

	CUser *puser = _FindUser( _param->client->GetClientId(), _param->clientid );
	if( !puser )
	{
		SERVER_LOG_ERROR( "CLoginManager,_GetUserBasicInfoCallback," << _param->client->GetClientId() << "," \
			<< _param->clientid << ",AleardyClose"  );
		return;
	}

	SERVER_LOG_DEBUG( "User:" << puser->User() << " get basic info from db" );

	while( RecordSet && RecordSet->GetRecord() )
	{
		// Name,Head,Sex,Exp,Level,Gold,Diamond
		s32 count = (s32)RecordSet->GetRecordCount();
		SELF_ASSERT( count == 1, break; );

		const char *val = RecordSet->GetFieldValue( 1 );
		if( val )
		{
			puser->GetBasic().SetName( val );
			SERVER_LOG_DEBUG( "\t" << "BasicInfo Name:" << val );
			string gbkname = CUtils::utf82s( val );
			SERVER_LOG_DEBUG( "\t" << "BasicInfo Name(gbk):" << gbkname.c_str() );
		}

		val = RecordSet->GetFieldValue( 2 );
		if( val )
		{
			puser->GetBasic().SetHead( val );
			SERVER_LOG_DEBUG( "\t" << "BasicInfo Head:" << val );
		}
		
		val = RecordSet->GetFieldValue( 3 );
		if( val )
		{
			puser->GetBasic().SetSex( CUserBasic::ConvertSex(atoi(val)) );
			SERVER_LOG_DEBUG( "\t" << "BasicInfo Sex:" << puser->GetBasic().GetSex() );
		}

		val = RecordSet->GetFieldValue( 4 );
		if( val )
		{
			u64 data = 0;
			sscanf( val, "%llu", &data );
			puser->GetBasic().SetExp( data );
			SERVER_LOG_DEBUG( "\t" << "BasicInfo Exp:" << puser->GetBasic().GetExp() );
		}

		val = RecordSet->GetFieldValue( 5 );
		if( val )
		{
			u64 data = 0;
			sscanf( val, "%llu", &data );
			puser->GetBasic().SetLevel( data );
			SERVER_LOG_DEBUG( "\t" << "BasicInfo Level:" << puser->GetBasic().GetLevel() );
		}

		val = RecordSet->GetFieldValue( 6 );
		if( val )
		{
			u64 data = 0;
			sscanf( val, "%llu", &data );
			puser->GetBasic().SetMoney( CUserBasic::E_Money_Gold, data );
			SERVER_LOG_DEBUG( "\t" << "BasicInfo Gold:" << data );
		}

		val = RecordSet->GetFieldValue( 7 );
		if( val )
		{
			u64 data = 0;
			sscanf( val, "%llu", &data );
			puser->GetBasic().SetMoney( CUserBasic::E_Money_Diamond, data );
			SERVER_LOG_DEBUG( "\t" << "BasicInfo Diamond:" << data );
		}

		break;
	}

	_NotifyUserBasicInfo( *puser, *_param->client, _param->clientid );
}
