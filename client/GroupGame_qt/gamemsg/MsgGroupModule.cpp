#include "MsgGroupModule.h"
#include "net/NetManager.h"
#include "user/UserManager.h"
#include "errno.pb.h"
#include "user/Group.h"
#include "user/GroupMember.h"
#include "user/GameRoomInfo.h"
#include "utils.h"
using namespace std;

CMsgConnectGroup::CMsgConnectGroup()
{
}
void CMsgConnectGroup::Process()
{
	CCLog( "[CMsgConnectGroup::Process] connect group(%s:%d) success", 
		CUserManager::Instance().GetGroupGateIp().c_str(), 
		CUserManager::Instance().GetGroupGatePort() );

}

CMsgLoginGroupResult::CMsgLoginGroupResult(s32 result) :
	m_result( result )
{
}
void CMsgLoginGroupResult::Process()
{
	CCLog( "[CMsgLoginGroupResult::Process] login group result:%d", m_result );

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
            (*it)->GetId(), (*it)->GetName().c_str(), ret?1:0, (*it)->GetName().c_str() );
	}
	SAFE_DELETE( m_groups );
	

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

            // TODO
		}
	}
	else
	{
		CCLog( "[CMsgCreateGroupResult::Process] Create Group ERROR errorcode:%d", m_result );
	}
}

CMsgLeaveGroupResult::CMsgLeaveGroupResult(s32 result, u64 groupid) : 
	m_result( result ),
	m_groupid( groupid )
{
}
void CMsgLeaveGroupResult::Process()
{
	CCLog( "[CMsgLeaveGroupResult::Process] result:%d groupid:%llu", 
		m_result, m_groupid );
	if( m_result == sglib::errorcode::E_ErrorCode_Success )
	{
		CUserManager::Instance().GetGroupManager().RemoveGroup( m_groupid );
		CUserManager::Instance().GetViewData().SetSelectGroup( NULL );
        // TODO
	}
}

CMsgMemberLeaveGroupNotify::CMsgMemberLeaveGroupNotify(u64 groupid, const std::string &user) :
	m_groupid( groupid ),
	m_leaveuser( user )
{
}
void CMsgMemberLeaveGroupNotify::Process()
{
	CCLog( "[CMsgMemberLeaveGroupNotify::Process] groupid:%llu member:%s", 
		m_groupid, m_leaveuser.c_str() );
	CGroup *group = CUserManager::Instance().GetGroupManager().FindGroup( m_groupid );
	if( !group )
	{
		CCLog( "[CMsgMemberLeaveGroupNotify::Process] [ERROR] not found group:%llu", m_groupid );
		return;
	}

	group->RemoveMember( m_leaveuser );
}

CMsgAddMemberRsp::CMsgAddMemberRsp(u64 groupid, s32 result) :
	m_groupid( groupid ),
	m_result( result )
{
}
void CMsgAddMemberRsp::Process()
{
	CCLog( "[CMsgAddMemberRsp::Process] groupid:%llu result:%d", 
		m_groupid, m_result );
}

CMsgAskJoinGroup::CMsgAskJoinGroup(u64 groupid, const std::string &name) : 
	m_groupid( groupid ),
	m_name( name )
{
}
void CMsgAskJoinGroup::Process()
{
	CCLog( "[CMsgAskJoinGroup::Process] groupid:%llu groupname:%s", 
        m_groupid, m_name.c_str() );
	CUserManager::Instance().GetViewData().SetJoinGroupInfo( m_groupid, m_name );
    // TODO
}

CMsgMemberJoinGroup::CMsgMemberJoinGroup(u64 groupid, const std::string &user, const std::string &name, const std::string &head) : 
	m_groupid( groupid ),
	m_user( user ),
	m_name( name ),
	m_head( head )
{
}
void CMsgMemberJoinGroup::Process()
{
	CCLog( "[CMsgMemberJoinGroup::Process] groupid:%llu user:%s name:%s", 
        m_groupid, m_user.c_str(), m_name.c_str() );

	CGroup *group = CUserManager::Instance().GetGroupManager().FindGroup( m_groupid );
	if( !group )
	{
		CCLog( "[CMsgMemberJoinGroup::Process] [ERROR] not found group:%llu", m_groupid );
		return;
	}

	CGroupMember *member = new CGroupMember( m_user );
	if( !member )
	{
		CCLog( "[CMsgMemberJoinGroup::Process] [ERROR] new member failed, group:%llu user:%s", 
			m_groupid, m_user.c_str() );
		return;
	}
	member->SetName( m_name );
	member->SetHead( m_head );
	member->SetOnline( true );
	group->AddMember( member );
}

CMsgGroupMessage::CMsgGroupMessage(u64 groupid, const std::string &sender, const std::string &content) :
	m_groupid( groupid ),
	m_sender( sender ),
	m_content( content )
{
}
void CMsgGroupMessage::Process()
{
	CUserManager::Instance().GetViewData().PushChatMsg( 
		m_groupid,
		m_sender,
		m_content );
    // TODO
}

CMsgGroupCreateGameRoomRsp::CMsgGroupCreateGameRoomRsp(s32 result, u64 groupid, s32 game, const std::string &ip, s32 port, s32 roomid, const std::string &roompwd) :
	m_result( result ),
	m_groupid( groupid ),
	m_game( game ),
	m_ip( ip ),
	m_port( port ),
	m_roomid( roomid ),
	m_roompwd( roompwd )
{
}
void CMsgGroupCreateGameRoomRsp::Process()
{
	CCLog( "[CreateGameRoomResult]:%d groupid:%llu game:%d ip:%s port:%d roomid:%d roompwd:%s",
		m_result, m_groupid, m_game, m_ip.c_str(), m_port, m_roomid, m_roompwd.c_str() );
}
	
CMsgGroupCreateGameRoomNtf::CMsgGroupCreateGameRoomNtf(const std::string &creater, u64 groupid, s32 game, const std::string &ip, s32 port, s32 roomid, const std::string &roompwd) :
	m_creater( creater ),
	m_groupid( groupid ),
	m_game( game ),
	m_ip( ip ),
	m_port( port ),
	m_roomid( roomid ),
	m_roompwd( roompwd )
{
}
void CMsgGroupCreateGameRoomNtf::Process()
{
	CCLog( "[CreateGameRoomNotify] creater:%s groupid:%llu game:%d ip:%s port:%d roomid:%d roompwd:%s",
		m_creater.c_str(), m_groupid, m_game, m_ip.c_str(), m_port, m_roomid, m_roompwd.c_str() );

	if( CUserManager::Instance().GetGameRoomList().AddGameRoomInfo(
		m_game, m_groupid, m_creater, m_ip, m_port, m_roomid, m_roompwd ) )
	{
        // TODO
	}
	else
	{
		CCLog( "[CMsgGroupCreateGameRoomNtf::Process] AddGameRoomInfo failed" );
	}
}
