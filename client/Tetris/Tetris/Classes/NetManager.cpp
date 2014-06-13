#include "NetManager.h"
#include "NetClient.h"
#include "MsgBase.h"
using namespace std;
using namespace SGLib;

//#include "support/CCNotificationCenter.h"
#include "CCDirector.h"
#include "CCScheduler.h"
using namespace cocos2d;

INIT_SIGNLETON_CLASS(CNetManager);

void CNetManager::Init()
{
	m_pClientMgr = NULL;
	m_pClient = NULL;
	m_bPauseProcMsg = false;

	m_pGroupClientMgr = NULL;
	m_pGroupClient = NULL;

	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget( this, 0, false );
}

bool CNetManager::Start(const char *ip, s32 port)
{
	if( m_pClientMgr == NULL && m_pClient == NULL )
	{
		m_pClientMgr = new CClientManager<CNetClient>( 1 );
		if( m_pClientMgr )
		{
			return m_pClientMgr->Start( ip, port );
		}
	}

	return false;
}

void CNetManager::Stop()
{
	if( m_pClientMgr )
	{
		m_pClientMgr->Stop();
		SAFE_DELETE( m_pClientMgr );
		m_pClient = NULL;
	}
}

bool CNetManager::StartGroup(const char *ip, s32 port)
{
	if( m_pGroupClientMgr == NULL && m_pGroupClient == NULL )
	{
		m_pGroupClientMgr = new CClientManager<CGroupClient>( 1 );
		if( m_pGroupClientMgr )
		{

			return m_pGroupClientMgr->Start( ip, port );
		}
	}

	return false;
}

void CNetManager::StopGroup()
{
	if( m_pGroupClientMgr )
	{
		m_pGroupClientMgr->Stop();
		SAFE_DELETE( m_pGroupClientMgr );
		m_pGroupClientMgr = NULL;
	}
}

void CNetManager::SetGroupClient(CGroupClient *client)
{
	m_pGroupClient = client;
}

void CNetManager::ProcMessage()
{
	while( !m_bPauseProcMsg )
	{
		CMsgBase *msg = NULL;
		{
			CGuardLock<CLock> g(m_Lock);
			if( !m_MsgQueue.empty() )
			{
				msg = m_MsgQueue.front();
				m_MsgQueue.pop_front();
			}
		}
		if( msg )
		{
			msg->Proc();
			SAFE_DELETE( msg );
		}
		else
		{
			break;
		}
	}
}

void CNetManager::PushMessage(CMsgBase *msg)
{
	if( msg )
	{
		CGuardLock<CLock> g(m_Lock);
		m_MsgQueue.push_back( msg );
	}
}

void CNetManager::SetClient(CNetClient *client)
{
	m_pClient = client;
}

void CNetManager::SetState(int state)
{
	m_nState = state;
}

int CNetManager::GetState()
{
	return m_nState;
}

void CNetManager::update(float dt)
{
	ProcMessage();
}

void CNetManager::PauseProcMsg()
{ 
	cocos2d::CCLog( "------ PauseProcMsg ------" );
	m_bPauseProcMsg = true;
}

void CNetManager::ResumeProcMsg()
{ 
	cocos2d::CCLog( "------ ResumeProcMsg ------" );
	m_bPauseProcMsg = false; 
}

void CNetManager::Login(const std::string &user)
{
	if( m_pClient )
	{
		m_pClient->UserLogin( user );
	}
}

void CNetManager::EnterGame(int gameId)
{
	if( m_pClient )
	{
		m_pClient->UserEnterGame( gameId );
	}
}

void CNetManager::LoginGame(const char *user)
{
	if( m_pClient )
	{
		m_pClient->LoginGame( string(user) );
	}
}

void CNetManager::EnterRoom(int roomId)
{
	if( m_pClient )
	{
		m_pClient->EnterRoom( roomId );
	}
}

void CNetManager::LeaveRoom()
{
	if( m_pClient )
	{
		m_pClient->LeaveRoom();
	}
}

void CNetManager::Ready()
{
	if( m_pClient )
	{
		m_pClient->Ready();
	}
}

void CNetManager::Lost()
{
	if( m_pClient )
	{
		m_pClient->Lost();
	}
}

void CNetManager::PutBlock(s32 blockid, s32 column)
{
	if( m_pClient )
	{
		m_pClient->PutBlock( blockid, column );
	}
}

void CNetManager::ClearBlock(std::vector<s32> &vecRows)
{
	if( m_pClient )
	{
		m_pClient->ClearBlock( vecRows );
	}
}

///////////////////////////////////////////////////////

void CNetManager::LoginGroup(const std::string &user)
{
	if( m_pGroupClient )
	{
		m_pGroupClient->LoginGroup( user );
	}
}

void CNetManager::CreateGameRoom(u64 groupid, s32 game)
{
	if( m_pGroupClient )
	{
		m_pGroupClient->AskCreateGameRoom( groupid, game );
	}
}
