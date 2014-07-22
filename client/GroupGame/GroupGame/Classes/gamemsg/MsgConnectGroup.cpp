#include "cocos2d.h"
#include "MsgConnectGroup.h"
#include "net/NetManager.h"
#include "scene/ViewBase.h"
#include "scene/SceneManager.h"
#include "user/UserManager.h"
using namespace cocos2d;

CMsgConnectGroup::CMsgConnectGroup()
{
}

void CMsgConnectGroup::Process()
{
	CCLog( "[CMsgConnectGroup::Process] connect group(%s:%d) success", 
		CUserManager::Instance().GetGroupGateIp().c_str(), 
		CUserManager::Instance().GetGroupGatePort() );
	CViewBase *view = CSceneManager::Instance().GetCurView();
	if( !view )
	{
		CCLog( "[CMsgConnectGroup::Process] cur view is NULL" );
		return;
	}

	view->UpdateView( CSceneManager::E_UpdateType_ConnectGroup );
}
