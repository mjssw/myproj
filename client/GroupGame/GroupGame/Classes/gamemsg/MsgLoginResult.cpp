#include "cocos2d.h"
#include "MsgLoginResult.h"
#include "net/NetManager.h"
#include "scene/ViewBase.h"
#include "scene/SceneManager.h"
#include "user/UserManager.h"
#include "errno.pb.h"
using namespace std;
using namespace cocos2d;

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
