#include "cocos2d.h"
#include "MsgGameInfoNotify.h"
#include "scene/ViewBase.h"
#include "scene/SceneManager.h"
#include "user/UserManager.h"
using namespace std;
using namespace cocos2d;

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
