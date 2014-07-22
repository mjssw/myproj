#include "NetManager.h"
#include "gamemsg/MsgBase.h"
using namespace cocos2d;
using namespace SGLib;
using namespace std;

SIGNLETON_CLASS_INIT(CNetManager);

bool CNetManager::Init()
{
	m_loginClient = NULL;
	m_groupClient = NULL;
	m_gameClient = NULL;

	m_loginConn = NULL;
	m_groupConn = NULL;
	m_gameConn = NULL;

	m_isPauseProcessMessage = false;

	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget( this, 0, false );
	return m_timer.Start();
}

void CNetManager::update(float delta)
{
	ProcessMessage();
}

void CNetManager::Fini()
{
	CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget( this );
	m_timer.Stop();
	_CloseAll();
}

bool CNetManager::StartLogin(const char *ip, int port)
{
	_CloseAll();

	m_loginConn = new SGLib::CClientManager<CLoginClient>( 1 );
	if( !m_loginConn )
	{
		return false;
	}

	return m_loginConn->Start( ip, port );
}

bool CNetManager::StartGroup(const char *ip, int port)
{
	m_groupConn = new SGLib::CClientManager<CGroupClient>( 1 );
	if( !m_groupConn )
	{
		return false;
	}

	return m_groupConn->Start( ip, port );
}

bool CNetManager::StartGame(const char *ip, int port)
{
	CloseGameConn();

	m_gameConn = new SGLib::CClientManager<CGameClient>( 1 );
	if( !m_gameConn )
	{
		return false;
	}

	return m_gameConn->Start( ip, port );
}

void CNetManager::CloseGameConn()
{
	if( m_gameConn )
	{
		m_gameClient = NULL;
		m_gameConn->Stop();
		SAFE_DELETE( m_gameConn );
	}
}

void CNetManager::CloseLoginConn()
{
	if( m_loginConn )
	{
		m_loginClient = NULL;
		m_loginConn->Stop();
		SAFE_DELETE( m_loginConn );
	}
}

void CNetManager::PushMessage(CMsgBase *msg)
{
	if( msg )
	{
		CGuardLock<CLock> g( m_msgLock );
		m_msgQueue.push_back( msg );
	}
}

void CNetManager::ProcessMessage()
{
	while( !m_isPauseProcessMessage )
	{
		CMsgBase *msg = NULL;
		{
			CGuardLock<CLock> g( m_msgLock );
			if( !m_msgQueue.empty() )
			{
				msg = m_msgQueue.front();
				m_msgQueue.pop_front();
			}
		}
		if( msg )
		{
			msg->Process();
			SAFE_DELETE( msg );
		}
		else
		{
			break;
		}
	}
}

void CNetManager::PauseProcessMessage()
{
	m_isPauseProcessMessage = true;
}

void CNetManager::ResumeProcessMessage()
{
	m_isPauseProcessMessage = false;
}
	
void CNetManager::SetLoginClientInstance(CLoginClient *client)
{
	m_loginClient = client;
}

CLoginClient* CNetManager::GetLoginClientInstance()
{
	return m_loginClient;
}

void CNetManager::SetGroupClientInstance(CGroupClient *client)
{
	m_groupClient = client;
}

CGroupClient* CNetManager::GetGroupClientInstance()
{
	return m_groupClient;
}

void CNetManager::SetGameClientInstance(CGameClient *client)
{
	m_gameClient = client;
}

CGameClient* CNetManager::GetGameClientInstance()
{
	return m_gameClient;
}

void CNetManager::_CloseAll()
{
	m_loginClient = NULL;
	m_groupClient = NULL;
	m_gameClient = NULL;

#define _CLOSE_CONN(conn) \
	if( conn )\
	{\
		conn->Stop();\
		SAFE_DELETE( conn );\
	}

	_CLOSE_CONN( m_loginConn );
	_CLOSE_CONN( m_groupConn );
	_CLOSE_CONN( m_gameConn );

#undef _CLOSE_CONN
}

