#include "cocos2d.h"
#include "MsgLoginModule.h"
#include "net/NetManager.h"
#include "scene/ViewBase.h"
#include "scene/SceneManager.h"
#include "user/UserManager.h"
#include "CommDef.h"
#include "errno.pb.h"
using namespace cocos2d;
using namespace std;

CMsgConnectLogin::CMsgConnectLogin()
{
}

void CMsgConnectLogin::Process()
{
	CCLog( "[CMsgConnectLogin::Process] connect login(%s:%d) success", LOGIN_IP, LOGIN_PORT );
	CViewBase *view = CSceneManager::Instance().GetCurView();
	if( !view )
	{
		CCLog( "[CMsgConnectLogin::Process] cur view is NULL" );
		return;
	}

	view->UpdateView( CSceneManager::E_UpdateType_ConnectLogin );
}

CMsgCloseLogin::CMsgCloseLogin()
{
}

void CMsgCloseLogin::Process()
{
	cocos2d::CCLog( "[CMsgCloseLogin::Process]" );
	CNetManager::Instance().CloseLoginConn();
}

CMsgConnectLoginError::CMsgConnectLoginError()
{
}

void CMsgConnectLoginError::Process()
{
	CCLog( "[CMsgConnectLoginError::Process] connect login(%s:%d) error", LOGIN_IP, LOGIN_PORT );
	CViewBase *view = CSceneManager::Instance().GetCurView();
	if( !view )
	{
		CCLog( "[CMsgConnectLoginError::Process] cur view is NULL" );
		return;
	}

	view->UpdateView( CSceneManager::E_UpdateType_ConnectLoginError );
}

CMsgGameInfoNotify::CMsgGameInfoNotify(std::vector<CGameInfo> &games)
{
	m_games = games;
}

void CMsgGameInfoNotify::Process()
{
	CCLog( "[CMsgGameInfoNotify::Process] get game info" );
	CViewBase *view = CSceneManager::Instance().GetCurView();
	if( !view )
	{
		CCLog( "[CMsgGameInfoNotify::Process] cur view is NULL" );
		return;
	}

	vector<CGameInfo>::iterator it = m_games.begin();
	for( ; it != m_games.end(); ++it )
	{
		CUserManager::Instance().GetGameList().AddGameInfo( *it );
	}
	view->UpdateView( CSceneManager::E_UpdateType_LoadingProgress );
}

CMsgGroupGateNotify::CMsgGroupGateNotify(const std::string &ip, s32 port) : 
	m_ip( ip ),
	m_port( port )
{
}

void CMsgGroupGateNotify::Process()
{
	CCLog( "[CMsgGroupGateNotify::Process] get group gate info %s:%d", m_ip.c_str(), m_port );
	CViewBase *view = CSceneManager::Instance().GetCurView();
	if( !view )
	{
		CCLog( "[CMsgGroupGateNotify::Process] cur view is NULL" );
		return;
	}

	CUserManager::Instance().SetGroupConnInfo( m_ip, m_port );
	view->UpdateView( CSceneManager::E_UpdateType_LoadingProgress );
	bool ret = CNetManager::Instance().StartGroup( m_ip.c_str(), m_port );
	if( !ret )
	{
		CCLog( "[CMsgGroupGateNotify::Process] start group %s:%d FAILED",
			m_ip.c_str(), m_port );
		view->UpdateView( CSceneManager::E_UpdateType_LoginFailed );
	}
}

CMsgLoginResult::CMsgLoginResult(s32 result, const string &token) : 
	m_result( result ),
	m_token( token )
{
}

void CMsgLoginResult::Process()
{
	CCLog( "[CMsgLoginResult::Process] login result:%d token:%s", m_result, m_token.c_str() );
	CViewBase *view = CSceneManager::Instance().GetCurView();
	if( !view )
	{
		CCLog( "[CMsgLoginResult::Process] cur view is NULL" );
		return;
	}

	CUserManager::Instance().SetToken( m_token );
	if( m_result == sglib::errorcode::E_ErrorCode_Success )
	{
		view->UpdateView( CSceneManager::E_UpdateType_LoadingProgress );
	}
	else
	{
		view->UpdateView( CSceneManager::E_UpdateType_LoginFailed );
	}
}

CMsgUserBasicInfoNotify::CMsgUserBasicInfoNotify(const std::string &name, const std::string &head, s32 sex, u64 exp, u64 level, u64 gold, u64 diamond) : 
	m_name( name ),
	m_head( head ),
	m_sex( sex ),
	m_exp( exp ),
	m_level( level ),
	m_gold( gold ),
	m_diamond( diamond )
{
}

void CMsgUserBasicInfoNotify::Process()
{
	CCLog( "[CMsgUserBasicInfoNotify::Process] get user basic info name:%s head:%s sex:%d exp:%llu level:%llu gold:%llu diamond:%llu",
		m_name.c_str(), m_head.c_str(), m_sex, m_exp, m_level, m_gold, m_diamond );
	CViewBase *view = CSceneManager::Instance().GetCurView();
	if( !view )
	{
		CCLog( "[CMsgUserBasicInfoNotify::Process] cur view is NULL" );
		return;
	}

	CUserManager::Instance().GetBasic().SetName( m_name );
	CUserManager::Instance().GetBasic().SetHead( m_head );
	CUserManager::Instance().GetBasic().SetSex( m_sex );
	CUserManager::Instance().GetBasic().SetExp( m_exp );
	CUserManager::Instance().GetBasic().SetLevel( m_level );
	CUserManager::Instance().GetBasic().SetMoney( E_Money_Gold, m_gold );
	CUserManager::Instance().GetBasic().SetMoney( E_Money_Diamond, m_diamond );
	view->UpdateView( CSceneManager::E_UpdateType_LoadingProgress );
}

CMsgConnectRegister::CMsgConnectRegister()
{
}
void CMsgConnectRegister::Process()
{
	CCLog( "[CMsgConnectRegister::Process] connect register(%s:%d) success", LOGIN_IP, LOGIN_PORT );
	CViewBase *view = CSceneManager::Instance().GetCurView();
	if( !view )
	{
		CCLog( "[CMsgConnectRegister::Process] cur view is NULL" );
		return;
	}

	view->UpdateView( CSceneManager::E_UpdateType_ConnectRegister );
}
	

CMsgResiterRsp::CMsgResiterRsp(s32 result) : m_result(result)
{
}
void CMsgResiterRsp::Process()
{
	CCLog( "[CMsgResiterRsp::Process] register result:%d", m_result );
	CViewBase *view = CSceneManager::Instance().GetCurView();
	if( !view )
	{
		CCLog( "[CMsgResiterRsp::Process] cur view is NULL" );
		return;
	}

	CUserManager::Instance().GetBasic().SetRegResult( m_result );
	view->UpdateView( CSceneManager::E_UpdateType_RegisterResult );
}
