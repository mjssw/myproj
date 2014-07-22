#include "cocos2d.h"
#include "MsgGroupGateNotify.h"
#include "net/NetManager.h"
#include "scene/ViewBase.h"
#include "scene/SceneManager.h"
#include "user/UserManager.h"
using namespace std;
using namespace cocos2d;
	
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
}
