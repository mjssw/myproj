#include "cocos2d.h"
#include "MsgConnectLogin.h"
#include "net/NetManager.h"
#include "scene/ViewBase.h"
#include "scene/SceneManager.h"
using namespace cocos2d;

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
