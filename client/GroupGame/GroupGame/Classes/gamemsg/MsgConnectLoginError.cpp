#include "cocos2d.h"
#include "MsgConnectLoginError.h"
#include "scene/ViewBase.h"
#include "scene/SceneManager.h"
#include "net/NetManager.h"
using namespace cocos2d;

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