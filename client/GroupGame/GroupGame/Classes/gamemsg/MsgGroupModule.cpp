#include "cocos2d.h"
#include "MsgGroupModule.h"
#include "net/NetManager.h"
#include "scene/ViewBase.h"
#include "scene/SceneManager.h"
#include "user/UserManager.h"
#include "errno.pb.h"
#include "user/Group.h"
#include "user/GroupMember.h"
#include "utils.h"
using namespace std;
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

CMsgLoginGroupResult::CMsgLoginGroupResult(s32 result) :
	m_result( result )
{
}
void CMsgLoginGroupResult::Process()
{
	CCLog( "[CMsgLoginGroupResult::Process] login group result:%d", m_result );
	CViewBase *view = CSceneManager::Instance().GetCurView();
	if( !view )
	{
		CCLog( "[CMsgLoginGroupResult::Process] cur view is NULL" );
		return;
	}

	if( m_result == sglib::errorcode::E_ErrorCode_Success )
	{
		view->UpdateView( CSceneManager::E_UpdateType_LoginGroup );
		view->UpdateView( CSceneManager::E_UpdateType_LoadingProgress );
	}
	else
	{
		view->UpdateView( CSceneManager::E_UpdateType_LoginFailed );
	}
}

CMsgGroupListUpdate::CMsgGroupListUpdate(vector<CGroup*> *groups) : 
	m_groups( groups )
{
}
void CMsgGroupListUpdate::Process()
{
	if( !m_groups )
	{
		CCLog( "[CMsgGroupListUpdate::Process][ERROR] m_groups is NULL" );
		return;
	}
	vector<CGroup*>::iterator it = m_groups->begin();
	for( ; it != m_groups->end(); ++it )
	{
		bool ret = CUserManager::Instance().GetGroupManager().AddGroup( *it );
		CCLog( "[CMsgGroupListUpdate::Process][DEBUG] add group:%llu:%s result:%d name:%s",
			(*it)->GetId(), (*it)->GetName().c_str(), ret?1:0, u2a((*it)->GetName().c_str()) );
	}
	SAFE_DELETE( m_groups );
	
	CViewBase *view = CSceneManager::Instance().GetCurView();
	if( !view )
	{
		CCLog( "[CMsgGroupListUpdate::Process] cur view is NULL" );
		return;
	}
	view->UpdateView( CSceneManager::E_UpdateType_UpdateGroupList );
}

CMsgGroupInfoUpdate::CMsgGroupInfoUpdate(u64 groupid, vector<CGroupMember*> *members) : 
	m_groupid( groupid ),
	m_members( members )
{
}
void CMsgGroupInfoUpdate::Process()
{
	if( !m_members )
	{
		CCLog( "[CMsgGroupInfoUpdate::Process][ERROR] m_members is NULL" );
		return;
	}

	CGroupManager &mgr = CUserManager::Instance().GetGroupManager();
	CGroup *group = mgr.FindGroup( m_groupid );
	if( !group )
	{
		CCAssert( false, "[CMsgGroupInfoUpdate::Process][ERROR] m_members is NULL" );
		return;
	}

	vector<CGroupMember*>::iterator it = m_members->begin();
	for( ; it != m_members->end(); ++it )
	{
		bool ret = group->AddMember( *it );
		CCLog( "[CMsgGroupInfoUpdate::Process][DEBUG] add member:%s to group:%llu:%s result:%d",
			(*it)->GetUser().c_str(), m_groupid, group->GetName().c_str(), ret?1:0 );
	}
	SAFE_DELETE( m_members );
}

CMsgCreateGroupResult::CMsgCreateGroupResult(s32 result, const std::string &name, const std::string &head, u64 groupid) :
	m_result( result ),
	m_name( name ),
	m_head( head ),
	m_groupid( groupid )
{
}
void CMsgCreateGroupResult::Process()
{
	CCLog( "[CMsgCreateGroupResult::Process] result:%d", m_result );
	if( m_result == sglib::errorcode::E_ErrorCode_Success )
	{
		CGroup *group = new CGroup( m_groupid, m_name, m_head );
		if( !group )
		{
			CCLog( "[CMsgCreateGroupResult::Process] [ERROR] new group failed" );
			return;
		}
		
		bool ret = CUserManager::Instance().GetGroupManager().AddGroup( group );
		CCLog( "[CMsgCreateGroupResult::Process][DEBUG] add group:%llu:%s ret:%d",
			m_groupid, m_name.c_str(), ret?1:0 );
		if( ret )
		{
			CGroupMember *self = new CGroupMember(
				CUserManager::Instance().GetBasic().GetUser() );
			CCAssert( self, "CMsgCreateGroupResult::Process new group member" );
			self->SetName( CUserManager::Instance().GetBasic().GetName() );
			self->SetHead( CUserManager::Instance().GetBasic().GetHead() );
			group->AddMember( self );
			CViewBase *view = CSceneManager::Instance().GetCurView();
			if( !view )
			{
				CCLog( "[CMsgCreateGroupResult::Process] cur view is NULL" );
				return;
			}
			view->UpdateView( CSceneManager::E_UpdateType_UpdateGroupList );
		}
	}
	else
	{
		CCLog( "[CMsgCreateGroupResult::Process] Create Group ERROR errorcode:%d", m_result );
	}
}
