#include "cocos2d.h"
#include "MsgGameModule.h"
#include "net/NetManager.h"
#include "scene/ViewBase.h"
#include "scene/SceneManager.h"
#include "user/UserManager.h"
#include "CommDef.h"
#include "errno.pb.h"
using namespace cocos2d;
using namespace std;

CMsgConnectGame::CMsgConnectGame()
{
}
void CMsgConnectGame::Process()
{
	CCLog( "[CMsgConnectGame::Process] connect game(%s:%d) success", LOGIN_IP, LOGIN_PORT );
	CViewBase *view = CSceneManager::Instance().GetCurView();
	if( !view )
	{
		CCLog( "[CMsgConnectGame::Process] cur view is NULL" );
		return;
	}

	view->UpdateView( CSceneManager::E_UpdateType_ConnectGame );
}

CMsgGameMessage::CMsgGameMessage(int msgid, int msglen, char *data) :
	m_msgid( msgid ),
	m_msglen( msglen ),
	m_data( NULL )
{
	if( msglen>0 && data )
	{
		m_data = new char[msglen];
		CCAssert( m_data, "new msg data error" );
		memcpy( m_data, data, msglen );
	}
}
void CMsgGameMessage::Process()
{
	CNetManager::Instance().LuaProcessGameMessage( m_msgid, m_msglen, m_data );
	delete [] m_data;
}
