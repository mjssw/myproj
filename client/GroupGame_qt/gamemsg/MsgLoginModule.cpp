#include "MsgLoginModule.h"
#include "net/NetManager.h"
#include "user/UserManager.h"
#include "net/LoginClient.h"
#include "CommDef.h"
#include "errno.pb.h"
#include <QThread>
#include "wrapper/QtWrapper.h"

using namespace std;

CMsgConnectLogin::CMsgConnectLogin()
{
}

void CMsgConnectLogin::Process()
{
    CCLog( "[T:%d][CMsgConnectLogin::Process] connect login(%s:%d) success", \
        QThread::currentThreadId(), LOGIN_IP, LOGIN_PORT );

    CUserManager::Instance().GetViewData().UpdateLoginProgress();

    CLoginClient *client = CNetManager::Instance().GetLoginClientInstance();
    if( client )
    {
        client->Login(
            CUserManager::Instance().GetBasic().GetUser(),
            CUserManager::Instance().GetBasic().GetPwd() );
    }
}

CMsgCloseLogin::CMsgCloseLogin()
{
}

void CMsgCloseLogin::Process()
{
    CCLog( "[CMsgCloseLogin::Process]" );
    //CNetManager::Instance().CloseLoginConn();
}

CMsgConnectLoginError::CMsgConnectLoginError()
{
}

void CMsgConnectLoginError::Process()
{
    CCLog( "[CMsgConnectLoginError::Process] connect login(%s:%d) error", LOGIN_IP, LOGIN_PORT );
    emit CQtWrapper::Instance().userLoginFailed("connect login error");
}

CMsgGameInfoNotify::CMsgGameInfoNotify(std::vector<CGameInfo> &games)
{
	m_games = games;
}

void CMsgGameInfoNotify::Process()
{
    CCLog( "[T:%d][[CMsgGameInfoNotify::Process] get game info", QThread::currentThreadId() );

	vector<CGameInfo>::iterator it = m_games.begin();
	for( ; it != m_games.end(); ++it )
	{
		CUserManager::Instance().GetGameList().AddGameInfo( *it );
	}
    CUserManager::Instance().GetViewData().UpdateLoginProgress();
}

CMsgGroupGateNotify::CMsgGroupGateNotify(const std::string &ip, s32 port) : 
	m_ip( ip ),
	m_port( port )
{
}

void CMsgGroupGateNotify::Process()
{
    CCLog( "[T:%d][CMsgGroupGateNotify::Process] get group gate info %s:%d", \
           QThread::currentThreadId(), m_ip.c_str(), m_port );

    CUserManager::Instance().GetViewData().UpdateLoginProgress();
    CUserManager::Instance().SetGroupConnInfo( m_ip, m_port );

	bool ret = CNetManager::Instance().StartGroup( m_ip.c_str(), m_port );
	if( !ret )
	{
		CCLog( "[CMsgGroupGateNotify::Process] start group %s:%d FAILED",
			m_ip.c_str(), m_port );
        emit CQtWrapper::Instance().userLoginFailed("try start group error");
        return;
    }
}

CMsgLoginResult::CMsgLoginResult(s32 result, const string &token) : 
	m_result( result ),
	m_token( token )
{
}

void CMsgLoginResult::Process()
{
    CCLog( "[T:%d][CMsgLoginResult::Process] login result:%d token:%s", \
           QThread::currentThreadId(), m_result, m_token.c_str() );

	CUserManager::Instance().SetToken( m_token );
	if( m_result == sglib::errorcode::E_ErrorCode_Success )
	{
        emit CQtWrapper::Instance().userLoginSuccess();
        CUserManager::Instance().GetViewData().UpdateLoginProgress();
    }
	else
	{
        emit CQtWrapper::Instance().userLoginFailed("user or pwd wrong");
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
    CCLog( "[T:%d][CMsgUserBasicInfoNotify::Process] get user basic info name:%s head:%s sex:%d exp:%llu level:%llu gold:%llu diamond:%llu",\
        QThread::currentThreadId(), m_name.c_str(), m_head.c_str(), m_sex, m_exp, m_level, m_gold, m_diamond );

	CUserManager::Instance().GetBasic().SetName( m_name );
	CUserManager::Instance().GetBasic().SetHead( m_head );
	CUserManager::Instance().GetBasic().SetSex( m_sex );
	CUserManager::Instance().GetBasic().SetExp( m_exp );
	CUserManager::Instance().GetBasic().SetLevel( m_level );
	CUserManager::Instance().GetBasic().SetMoney( E_Money_Gold, m_gold );
	CUserManager::Instance().GetBasic().SetMoney( E_Money_Diamond, m_diamond );

    CUserManager::Instance().GetViewData().UpdateLoginProgress();
}

CMsgConnectRegister::CMsgConnectRegister()
{
}
void CMsgConnectRegister::Process()
{
    CCLog( "[T:%d][CMsgConnectRegister::Process] connect register(%s:%d) success", \
           QThread::currentThreadId(), LOGIN_IP, LOGIN_PORT );

    CRegisterClient *client = CNetManager::Instance().GetRegClientInstance();
    if( client )
    {
        client->Register(
            CUserManager::Instance().GetBasic().GetRegUser(),
            CUserManager::Instance().GetBasic().GetRegPwd() );
    }
}
	

CMsgResiterRsp::CMsgResiterRsp(s32 result) : m_result(result)
{
}
void CMsgResiterRsp::Process()
{
    CCLog( "[T:%d][CMsgResiterRsp::Process] register result:%d", QThread::currentThreadId(), m_result );

    CUserManager::Instance().GetBasic().SetRegResult( m_result );

    emit CQtWrapper::Instance().registerResult( m_result==0?true:false );
}
