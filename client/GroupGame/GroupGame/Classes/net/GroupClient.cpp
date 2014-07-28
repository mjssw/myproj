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
					//_CreateGroupRsp(rsp);
				}
			}break;
		case sglib::msgid::SC_GROUP_ADD_MEMBER_RSP:
			{
				sglib::groupproto::SCGroupAddMemberRsp msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					//_AddMemberRsp(rsp);
				}
			}break;
		case sglib::msgid::SC_GROUP_ASK_JOIN_NTF:
			{
				sglib::groupproto::SCGroupAskJoinNtf msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					//_AskJoinGroupNtf(ntf);
				}
			}break;
		case sglib::msgid::SC_GROUP_MEMBER_JOIN_NTF:
			{
				sglib::groupproto::SCGroupMemberJoinNtf msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					//_MemberJoinGroupNtf(ntf);
				}
			}break;
		case sglib::msgid::SC_GROUP_MESSAGE_NTF:
			{
				sglib::groupproto::SCGroupMessageNtf msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					//_GroupMessageNtf(ntf);
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
					//_LeaveGroupRsp(rsp);
				}
			}break;
		case sglib::msgid::SC_GROUP_MEMBER_LEAVE_NTF:
			{
				sglib::groupproto::SCGroupMemberLeaveNtf msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					//_MemberLeaveGroupNtf(ntf);
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
