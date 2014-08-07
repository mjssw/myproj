#include "NetManager.h"
#include "GroupClient.h"
#include "CommDef.h"
#include "gamemsg/MsgGroupModule.h"
#include "msgid.pb.h"
#include "user/Group.h"
#include "user/GroupMember.h"
using namespace SGLib;
using namespace cocos2d;
using namespace std;

CGroupClient::CGroupClient(s32 id) : 
	CProtoClientBase( id )
{
}

int CGroupClient::OnRecv(char *buf, int len)
{
	int totalProc = 0;
	char *pbuf = buf;
	while( true )
	{
		int procLen = 0;
		if( len < MSG_HEAD_LEN )
		{
			return totalProc;
		}

		MsgHeadType pkgLen = *((MsgHeadType*)pbuf);
		if( len < (int)pkgLen )
		{
			return totalProc;
		}

		int _id = *((int*)( pbuf + MSG_HEAD_LEN ));
		switch( _id )
		{
		case sglib::msgid::SC_GROUP_LOGIN_RSP:
			{
				sglib::groupproto::SCGroupUserLoginRsp msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_LoginGroupResult( msg );
				}
			}break;
		case sglib::msgid::SC_GROUP_CREATE_RSP:
			{
				sglib::groupproto::SCGroupCreateRsp msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_CreateGroupRsp( msg );
				}
			}break;
		case sglib::msgid::SC_GROUP_ADD_MEMBER_RSP:
			{
				sglib::groupproto::SCGroupAddMemberRsp msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_AddMemberRsp( msg );
				}
			}break;
		case sglib::msgid::SC_GROUP_ASK_JOIN_NTF:
			{
				sglib::groupproto::SCGroupAskJoinNtf msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_AskJoinGroupNtf( msg );
				}
			}break;
		case sglib::msgid::SC_GROUP_MEMBER_JOIN_NTF:
			{
				sglib::groupproto::SCGroupMemberJoinNtf msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_MemberJoinGroupNtf( msg );
				}
			}break;
		case sglib::msgid::SC_GROUP_MESSAGE_NTF:
			{
				sglib::groupproto::SCGroupMessageNtf msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_GroupMessageNtf( msg );
				}
			}break;
		case sglib::msgid::SC_GROUP_INFO_UPDATE:
			{
				sglib::groupproto::SCGroupInfoUpdate msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_GroupInfoUpdate( msg );
				}
			}break;
		case sglib::msgid::SC_GROUP_LIST_UPDATE:
			{
				sglib::groupproto::SCGroupListUpdate msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_GroupListUpdate( msg );
				}
			}break;
		case sglib::msgid::SC_GROUP_LEAVE_RSP:
			{
				sglib::groupproto::SCGroupLeaveRsp msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_LeaveGroupRsp( msg );
				}
			}break;
		case sglib::msgid::SC_GROUP_MEMBER_LEAVE_NTF:
			{
				sglib::groupproto::SCGroupMemberLeaveNtf msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_MemberLeaveGroupNtf( msg );
				}
			}break;
		case sglib::msgid::SC_GROUP_DELETE_RSP:
			{
				sglib::groupproto::SCGroupDeleteRsp msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					//_DeleteGroupRsp(rsp);
				}
			}break;
		case sglib::msgid::SC_GROUP_DELETE_NTF:
			{
				sglib::groupproto::SCGroupDeleteNtf msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					//_DeleteGroupNtf(ntf);
				}
			}break;
		case sglib::msgid::SC_GROUP_MEMBER_ONLINE:
			{
				sglib::groupproto::SCGroupMemberOnlineNtf msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					CCLog( "GROUP:%llu\t%s online now!", msg.groupid(), msg.user().c_str() );
				}
			}break;
		case sglib::msgid::SC_GROUP_MEMBER_OFFLINE:
			{
				sglib::groupproto::SCGroupMemberOfflineNtf msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					CCLog( "GROUP:%llu\t%s offline now!", msg.groupid(), msg.user().c_str() );
				}
			}break;
		case sglib::msgid::SC_GROUP_CREATE_GAMEROOM_RSP:
			{
				sglib::groupproto::SCGroupCreateGameRoomRsp msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					//_GroupCreateGameRoomResult( rsp );
				}
			}break;
		case sglib::msgid::SC_GROUP_CREATE_GAMEROOM_NTF:
			{
				sglib::groupproto::SCGroupCreateGameRoomNtf msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					//_GroupMemberCreateGameRoomNtf( ntf );
				}
			}break;
		default:
			break;
		}

		procLen = (pkgLen);
		pbuf += procLen;
		len -= procLen;
		totalProc += procLen;
	}

	return totalProc;
}

void CGroupClient::OnConnect()
{
	CNetManager::Instance().SetGroupClientInstance( this );
	
	CMsgBase *msg = new CMsgConnectGroup();
	if( !msg )
	{
		CCLog( "[CGroupClient::OnConnect] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( msg );
}

void CGroupClient::OnClose()
{
}

void CGroupClient::OnConnectError()
{
}

void CGroupClient::Login(const string &user, const string &token)
{
	sglib::groupproto::CSGroupUserLoginReq req;
	req.set_user( user );
	req.set_token( token );
	
	SendMsg( req, sglib::msgid::CS_GROUP_LOGIN_REQ );
}
 
void CGroupClient::CreateGroup(const std::string &name, const std::string &head)
{
	sglib::groupproto::CSGroupCreateReq req;
	req.set_name( name );
	req.set_head( head );

	SendMsg( req, sglib::msgid::CS_GROUP_CREATE_REQ );
}

void CGroupClient::LeaveGroup(u64 groupid)
{
	sglib::groupproto::CSGroupLeaveReq req;
	req.set_groupid( groupid );
	
	SendMsg( req, sglib::msgid::CS_GROUP_LEAVE_REQ );
}

void CGroupClient::InviteMember(u64 groupid, const std::string &user)
{
	sglib::groupproto::CSGroupAddMemberReq req;
	req.set_groupid( groupid );
	req.add_users( user );
	
	SendMsg( req, sglib::msgid::CS_GROUP_ADD_MEMBER_REQ );
}

void CGroupClient::AgreeJoinGroup(u64 groupid)
{
	sglib::groupproto::CSGroupAgreeJoinReq req;
	req.set_groupid( groupid );

	SendMsg( req, sglib::msgid::CS_GROUP_AGREE_JOIN_REQ );
}

void CGroupClient::ChatMessage(u64 groupid, const string &text)
{
	sglib::groupproto::CSGroupMessageReq req;
	req.set_groupid( groupid );
	req.set_content( text );

	SendMsg( req, sglib::msgid::CS_GROUP_MESSAGE_REQ );
}

void CGroupClient::_LoginGroupResult(sglib::groupproto::SCGroupUserLoginRsp &msg)
{
	CMsgBase *gamemsg = new CMsgLoginGroupResult( msg.result() ); 
	if( !gamemsg )
	{
		CCLog( "[CGroupClient::_LoginGroupResult] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( gamemsg );
}

void CGroupClient::_CreateGroupRsp(sglib::groupproto::SCGroupCreateRsp &msg)
{
	CCLog( "CGroupClient::_CreateGroupRsp result:%d groupid:%llu",
		msg.result(), msg.groupid() );

	CMsgBase *gamemsg = new CMsgCreateGroupResult( msg.result(), msg.name(), msg.head(), msg.groupid() ); 
	if( !gamemsg )
	{
		CCLog( "[CGroupClient::_CreateGroupRsp] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( gamemsg );
}

void CGroupClient::_LeaveGroupRsp(sglib::groupproto::SCGroupLeaveRsp &msg)
{
	CCLog( "CGroupClient::_LeaveGroupRsp result:%d groupid:%llu",
		msg.result(), msg.groupid() );

	CMsgBase *gamemsg = new CMsgLeaveGroupResult( msg.result(), msg.groupid() ); 
	if( !gamemsg )
	{
		CCLog( "[CGroupClient::_LeaveGroupRsp] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( gamemsg );
}

void CGroupClient::_MemberLeaveGroupNtf(sglib::groupproto::SCGroupMemberLeaveNtf &msg)
{
	CCLog( "CGroupClient::_MemberLeaveGroupNtf member:%s in groupid:%llu leave",
		msg.user().c_str(), msg.groupid() );

	CMsgBase *gamemsg = new CMsgMemberLeaveGroupNotify( msg.groupid(), msg.user() ); 
	if( !gamemsg )
	{
		CCLog( "[CGroupClient::_MemberLeaveGroupNtf] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( gamemsg );
}

void CGroupClient::_AddMemberRsp(sglib::groupproto::SCGroupAddMemberRsp &msg)
{
	CCLog( "CGroupClient::_AddMemberRsp result:%s in groupid:%llu",
		msg.result(), msg.groupid() );

	CMsgBase *gamemsg = new CMsgAddMemberRsp( msg.groupid(), msg.result() ); 
	if( !gamemsg )
	{
		CCLog( "[CGroupClient::_AddMemberRsp] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( gamemsg );
}

void CGroupClient::_AskJoinGroupNtf(sglib::groupproto::SCGroupAskJoinNtf &msg)
{
	CCLog( "CGroupClient::_AskJoinGroupNtf groupname:%s curcount:%d",
		msg.group().name().c_str(), msg.group().count() );

	CMsgBase *gamemsg = new CMsgAskJoinGroup( msg.group().id(), msg.group().name() );
	if( !gamemsg )
	{
		CCLog( "[CGroupClient::_AskJoinGroupNtf] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( gamemsg );
}

void CGroupClient::_MemberJoinGroupNtf(sglib::groupproto::SCGroupMemberJoinNtf &msg)
{
	CCLog( "CGroupClient::_MemberJoinGroupNtf group:%llu member:%s:%s", msg.groupid(),
		msg.member().user().c_str(), msg.member().nickname().c_str() );

	CMsgBase *gamemsg = new CMsgMemberJoinGroup( 
		msg.groupid(), 
		msg.member().user(),
		msg.member().nickname(), 
		msg.member().head() );
	if( !gamemsg )
	{
		CCLog( "[CGroupClient::_MemberJoinGroupNtf] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( gamemsg );
}

void CGroupClient::_GroupMessageNtf(sglib::groupproto::SCGroupMessageNtf &msg)
{
	CCLog( "CGroupClient::_GroupMessageNtf group:%llu member:%s text:%s", msg.groupid(),
		msg.sender().c_str(), msg.content().c_str() );

	CMsgBase *gamemsg = new CMsgGroupMessage( 
		msg.groupid(), 
		msg.sender(),
		msg.content() );
	if( !gamemsg )
	{
		CCLog( "[CGroupClient::_GroupMessageNtf] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( gamemsg );
}

#define _DESTORY_VECTOR_ELEM(vec, type) \
	vector<type*>::iterator it = vec->begin();\
	for( ; it != vec->end(); ++it )\
	{\
		SAFE_DELETE( (*it) );\
	}\
	vec->clear();\
	SAFE_DELETE( vec );


void CGroupClient::_GroupListUpdate(sglib::groupproto::SCGroupListUpdate &msg)
{
	vector<CGroup*> *groups = new vector<CGroup*>();
	if( !groups )
	{
		CCLog( "[CGroupClient::_GroupListUpdate] new groups ERROR" );
		return;
	}

	for( s32 i=0; i<msg.groups_size(); ++i )
	{
		CGroup *group = new CGroup(
			msg.groups(i).id(),
			msg.groups(i).name(),
			msg.groups(i).icon() );
		if( !group )
		{
			CCLog( "[CGroupClient::_GroupListUpdate] new group ERROR" );
			break;
		}

		groups->push_back( group );
	}

	CMsgBase *gamemsg = new CMsgGroupListUpdate( groups );
	if( !gamemsg )
	{
		CCLog( "[CGroupClient::_GroupListUpdate] new msg failed" );
		_DESTORY_VECTOR_ELEM( groups, CGroup );
		return;
	}

	CNetManager::Instance().PushMessage( gamemsg );
}

void CGroupClient::_GroupInfoUpdate(sglib::groupproto::SCGroupInfoUpdate &msg)
{
	vector<CGroupMember*> *members = new vector<CGroupMember*>();
	if( !members )
	{
		CCLog( "[CGroupClient::_GroupInfoUpdate] new members ERROR" );
		return;
	}

	for( s32 i=0; i<msg.members_size(); ++i )
	{
		CGroupMember *member = new CGroupMember( msg.members(i).user() );
		if( !member )
		{
			CCLog( "[CGroupClient::_GroupInfoUpdate] new member ERROR" );
			break;
		}

		member->SetName( msg.members(i).nickname() );
		member->SetHead( msg.members(i).head() );
		member->SetOnline( msg.members(i).online() == CGroupMember::E_Member_Online );

		members->push_back( member );
	}

	CMsgBase *gamemsg = new CMsgGroupInfoUpdate( msg.id(), members );
	if( !gamemsg )
	{
		CCLog( "[CGroupClient::_GroupInfoUpdate] new msg failed" );
		_DESTORY_VECTOR_ELEM( members, CGroupMember );
		return;
	}

	CNetManager::Instance().PushMessage( gamemsg );
}
