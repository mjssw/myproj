#include "cocos2d.h"
#include "MsgUserBasicInfoNotify.h"
#include "net/NetManager.h"
#include "scene/ViewBase.h"
#include "scene/SceneManager.h"
#include "user/UserManager.h"
using namespace std;
using namespace cocos2d;
	
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
	CCLog( "[CMsgUserBasicInfoNotify::Process] get user basic info name:%s head:%s sex:%d exp:%llu level:%llu gold:%llu diamond:%llu",
		m_name.c_str(), m_head.c_str(), m_sex, m_exp, m_level, m_gold, m_diamond );
	CViewBase *view = CSceneManager::Instance().GetCurView();
	if( !view )
	{
		CCLog( "[CMsgUserBasicInfoNotify::Process] cur view is NULL" );
		return;
	}

	CUserManager::Instance().GetBasic().SetName( m_name );
	CUserManager::Instance().GetBasic().SetHead( m_head );
	CUserManager::Instance().GetBasic().SetSex( m_sex );
	CUserManager::Instance().GetBasic().SetExp( m_exp );
	CUserManager::Instance().GetBasic().SetLevel( m_level );
	CUserManager::Instance().GetBasic().SetMoney( CUserBasic::E_Money_Gold, m_gold );
	CUserManager::Instance().GetBasic().SetMoney( CUserBasic::E_Money_Diamond, m_diamond );
	view->UpdateView( CSceneManager::E_UpdateType_LoadingProgress );
}
