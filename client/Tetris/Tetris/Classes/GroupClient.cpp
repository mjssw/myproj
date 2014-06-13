#include "GroupClient.h"
#include "NetManager.h"
#include "GameManager.h"
#include "MsgGroup.h"
#include "MsgConnectError.h"
#include "cocos2d.h"
using namespace cocos2d;

int CGroupClient::OnRecv(char *buf, int len)
{
	int totalProc = 0;
	char *pbuf = buf;
	while(true)
	{
		int procLen = 0;
		if( len < sizeof(int) )
		{
			return totalProc;
		}

		int pkgLen = *((int*)pbuf);
		if( len < pkgLen )
		{
			return totalProc;
		}

		int _id = *((int*)(pbuf+sizeof(int)));
		switch(_id)
		{
		case sglib::msgid::SC_GROUP_LOGIN_RSP:
			{
				sglib::groupproto::SCGroupUserLoginRsp rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_LoginGroupResult(rsp);
				}
			}break;
		case sglib::msgid::SC_GROUP_CREATE_RSP:
			{
				sglib::groupproto::SCGroupCreateRsp rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_CreateGroupRsp(rsp);
				}
			}break;
		case sglib::msgid::SC_GROUP_ADD_MEMBER_RSP:
			{
				sglib::groupproto::SCGroupAddMemberRsp rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_AddMemberRsp(rsp);
				}
			}break;
		case sglib::msgid::SC_GROUP_ASK_JOIN_NTF:
			{
				sglib::groupproto::SCGroupAskJoinNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_AskJoinGroupNtf(ntf);
				}
			}break;
		case sglib::msgid::SC_GROUP_MEMBER_JOIN_NTF:
			{
				sglib::groupproto::SCGroupMemberJoinNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_MemberJoinGroupNtf(ntf);
				}
			}break;
		case sglib::msgid::SC_GROUP_MESSAGE_NTF:
			{
				sglib::groupproto::SCGroupMessageNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_GroupMessageNtf(ntf);
				}
			}break;
		case sglib::msgid::SC_GROUP_INFO_UPDATE:
			{
				sglib::groupproto::SCGroupInfoUpdate up;
				if( up.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_GroupInfoUpdate(up);
				}
			}break;
		case sglib::msgid::SC_GROUP_LIST_UPDATE:
			{
				sglib::groupproto::SCGroupListUpdate up;
				if( up.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_GroupListUpdate(up);
				}
			}break;
		case sglib::msgid::SC_GROUP_LEAVE_RSP:
			{
				sglib::groupproto::SCGroupLeaveRsp rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_LeaveGroupRsp(rsp);
				}
			}break;
		case sglib::msgid::SC_GROUP_MEMBER_LEAVE_NTF:
			{
				sglib::groupproto::SCGroupMemberLeaveNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_MemberLeaveGroupNtf(ntf);
				}
			}break;
		case sglib::msgid::SC_GROUP_DELETE_RSP:
			{
				sglib::groupproto::SCGroupDeleteRsp rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_DeleteGroupRsp(rsp);
				}
			}break;
		case sglib::msgid::SC_GROUP_DELETE_NTF:
			{
				sglib::groupproto::SCGroupDeleteNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_DeleteGroupNtf(ntf);
				}
			}break;
		case sglib::msgid::SC_GROUP_MEMBER_ONLINE:
			{
				sglib::groupproto::SCGroupMemberOnlineNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					CCLog( "GROUP:%llu\t%s online now!\n", ntf.groupid(), ntf.user().c_str() );
				}
			}break;
		case sglib::msgid::SC_GROUP_MEMBER_OFFLINE:
			{
				sglib::groupproto::SCGroupMemberOfflineNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					CCLog( "GROUP:%llu\t%s offline now!\n", ntf.groupid(), ntf.user().c_str() );
				}
			}break;
		case sglib::msgid::SC_GROUP_CREATE_GAMEROOM_RSP:
			{
				sglib::groupproto::SCGroupCreateGameRoomRsp rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_GroupCreateGameRoomResult( rsp );
				}
			}break;
		case sglib::msgid::SC_GROUP_CREATE_GAMEROOM_NTF:
			{
				sglib::groupproto::SCGroupCreateGameRoomNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_GroupMemberCreateGameRoomNtf( ntf );
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
	CNetManager::Instance().PushMessage( new CMsgGroupConnect(this) );
}

void CGroupClient::OnClose()
{
	CCLog("close\n");
}

void CGroupClient::OnConnectError()
{
	CNetManager::Instance().PushMessage( new CMsgConnectError() );
}

void CGroupClient::LoginGroup(const string &user)
{
	m_User = user;
	CCLog( "%s will login group\n", user.c_str() );
	sglib::groupproto::CSGroupUserLoginReq req;
	req.set_user( user );
	req.set_token( "xxx" );
	SendMsg( req, sglib::msgid::CS_GROUP_LOGIN_REQ );
}

void CGroupClient::CreateGroup(const std::string &gpname)
{
	sglib::groupproto::CSGroupCreateReq req;
	req.set_name( gpname );
	SendMsg( req, sglib::msgid::CS_GROUP_CREATE_REQ );
}

void CGroupClient::AddMemberToGroup(std::vector< std::string > &member)
{
	sglib::groupproto::CSGroupAddMemberReq req;
	req.set_groupid( atoi(member[1].c_str()) );
	for( int i=2; i<member.size(); ++i )
	{
		std::string *str = req.add_users();
		*str = member[i];
	}
	SendMsg( req, sglib::msgid::CS_GROUP_ADD_MEMBER_REQ );
}

void CGroupClient::AgreeJoinGroup(std::string &groupid)
{
	sglib::groupproto::CSGroupAgreeJoinReq req;
	req.set_groupid( atoi(groupid.c_str()) );
	SendMsg( req, sglib::msgid::CS_GROUP_AGREE_JOIN_REQ );
}

void CGroupClient::GroupMessage(std::string &groupid, std::string &msg)
{
	sglib::groupproto::CSGroupMessageReq req;
	req.set_groupid( atoi(groupid.c_str()) );
	req.set_content( msg );
	SendMsg( req, sglib::msgid::CS_GROUP_MESSAGE_REQ );
}

void CGroupClient::LeaveGroup(std::string &groupid)
{
	sglib::groupproto::CSGroupLeaveReq req;
	req.set_groupid( atoi(groupid.c_str()) );
	SendMsg( req, sglib::msgid::CS_GROUP_LEAVE_REQ );
}

void CGroupClient::DeleteGroup(std::string &groupid)
{
	sglib::groupproto::CSGroupDeleteReq req;
	req.set_groupid( atoi(groupid.c_str()) );
	SendMsg( req, sglib::msgid::CS_GROUP_DELETE_REQ );
}

void CGroupClient::AskCreateGameRoom(u64 groupid, s32 game)
{
	sglib::groupproto::CSGroupCreateGameRoomReq req;
	req.set_groupid( groupid );
	req.set_game( game );
	SendMsg( req, sglib::msgid::CS_GROUP_CREATE_GAMEROOM_REQ );
}

void CGroupClient::SendMsg(const ::google::protobuf::Message &Msg, s32 nMsgId)
{
	int total = Msg.ByteSize();
	char buf[MAX_MSG_LEN] = {0};
	*((s32*)(buf)) = total + MSG_ID_LEN + MSG_HEAD_LEN;
	*((s32*)(buf + MSG_HEAD_LEN)) = nMsgId;
	if( Msg.SerializeToArray(buf+MSG_ID_LEN+MSG_HEAD_LEN, MAX_MSG_LEN) )
	{
		if( !Send(buf, total+MSG_ID_LEN+MSG_HEAD_LEN) )
		{
			Fini();
		}
		else
		{
			CCLog("send msg: [%d] %d\n", GetSocket(), nMsgId);
		}
	}
	else
	{
		CCLog( "Msg:%d SerializeToArray failed\n", nMsgId );
	}
}


void CGroupClient::_LoginGroupResult(const sglib::groupproto::SCGroupUserLoginRsp &rsp)
{
	CCLog("%s login group result:%d\n",
		m_User.c_str(), rsp.result() );
	if( rsp.result() == sglib::errorcode::E_ErrorCode_Success )
	{
		CNetManager::Instance().PushMessage( new CMsgLoginGroupRsp(rsp.result()) );
	}
}

void CGroupClient::_CreateGroupRsp(const sglib::groupproto::SCGroupCreateRsp &rsp)
{
	CCLog( "%s create group:%s result:%d groupid:%llu\n",
		m_User.c_str(), rsp.name().c_str(), rsp.result(), rsp.groupid() );
}

void CGroupClient::_AddMemberRsp(const sglib::groupproto::SCGroupAddMemberRsp &rsp)
{
	CCLog( "%s add member to group:%llu rsp: %d\n",
		m_User.c_str(), rsp.groupid(), rsp.result() );
}

void CGroupClient::_AskJoinGroupNtf(const sglib::groupproto::SCGroupAskJoinNtf &ntf)
{
	CCLog( "Are you agree join group:%llu name:%s icon:%s count:%d\n",
		ntf.group().id(),
		ntf.group().name().c_str(),
		ntf.group().icon().c_str(),
		ntf.group().count()
		);
}

void CGroupClient::_MemberJoinGroupNtf(const sglib::groupproto::SCGroupMemberJoinNtf &ntf)
{
	CCLog( "Welcome new member join group:%llu\n\tUser:%s\n\tNickName:%s\n\tHead:%s\n",
		ntf.groupid(),
		ntf.member().user().c_str(),
		ntf.member().nickname().c_str(),
		ntf.member().head().c_str() );
}

void CGroupClient::_GroupMessageNtf(const sglib::groupproto::SCGroupMessageNtf &ntf)
{
	CCLog( "%s says:%s in group:%llu\n",
		ntf.sender().c_str(),
		ntf.content().c_str(),
		ntf.groupid() );

	CNetManager::Instance().PushMessage( new CMsgGroupMessageNtf(ntf.sender(), ntf.content(), ntf.groupid()) );
}

void CGroupClient::_GroupInfoUpdate(const sglib::groupproto::SCGroupInfoUpdate &up)
{
	vector<string> members;
	CCLog( "====== Group:%llu Info Update =======\n", up.id() );
	for( int i=0; i<up.members_size(); ++i )
	{
		CCLog( "Member user:%s\tnickname:%s\thead:%s\tonline:%d\n",
			up.members(i).user().c_str(),
			up.members(i).nickname().c_str(),
			up.members(i).head().c_str(),
			up.members(i).online() );
		
		members.push_back( up.members(i).user() );
	}
	
	CNetManager::Instance().PushMessage( new CMsgGroupInfoUpdate(up.id(), members) );
}

void CGroupClient::_GroupListUpdate(const sglib::groupproto::SCGroupListUpdate &up)
{
	vector<u64> vecId;
	vector<string> vecName;
	CCLog( "====== Group Update =========\n");
	for( int i=0; i<up.groups_size(); ++i )
	{
		CCLog( "Group Id:%llu\tName:%s\tCount:%d\n",
			up.groups(i).id(),
			up.groups(i).name().c_str(),
			up.groups(i).count() );

		vecId.push_back( up.groups(i).id() );
		vecName.push_back( up.groups(i).name() );
	}
	CNetManager::Instance().PushMessage( new CMsgGroupListUpdate(vecId, vecName) );
}

void CGroupClient::_LeaveGroupRsp(const sglib::groupproto::SCGroupLeaveRsp &rsp)
{
	CCLog( "%s leave group:%llu result:%d\n",
		m_User.c_str(),
		rsp.groupid(), rsp.result() );
}

void CGroupClient::_MemberLeaveGroupNtf(const sglib::groupproto::SCGroupMemberLeaveNtf &ntf)
{
	CCLog( "member:%s leave group:%llu\n",
		ntf.user().c_str(), ntf.groupid() );
}

void CGroupClient::_DeleteGroupRsp(const sglib::groupproto::SCGroupDeleteRsp &rsp)
{
	CCLog( "%s delete group:%llu result:%d\n",
		m_User.c_str(),
		rsp.groupid(), rsp.result() );
}

void CGroupClient::_DeleteGroupNtf(const sglib::groupproto::SCGroupDeleteNtf &ntf)
{
	CCLog( "group:%llu delete\n", ntf.groupid() );
}

void CGroupClient::_GroupCreateGameRoomResult( const sglib::groupproto::SCGroupCreateGameRoomRsp &rsp)
{
	CCLog( "create game room in group result:%d\n\tGroup:%llu\tGame:%d\tRoom:%d\tIp:%s\tPort:%d\n", 
		rsp.result(),
		rsp.groupid(), rsp.game(), rsp.roomid(), rsp.ip().c_str(), rsp.port() );
	
	CNetManager::Instance().PushMessage( new CMsgGroupCreateGameRoomResult(
		rsp.result(), rsp.groupid(), rsp.game(), rsp.roomid(), rsp.ip(), rsp.port()) );
}

void CGroupClient::_GroupMemberCreateGameRoomNtf(const sglib::groupproto::SCGroupCreateGameRoomNtf &ntf)
{
	CCLog( "group member:%s create game room in group:%llu\n\tGame:%d\tRoom:%d\tIp:%s\tPort:%d\n", 
		ntf.creater().c_str(), ntf.groupid(), 
		ntf.game(), ntf.roomid(), ntf.ip().c_str(), ntf.port() );
	
	CNetManager::Instance().PushMessage( new CMsgGroupCreateGameRoomNtf(
		ntf.creater().c_str(), ntf.groupid(),ntf.game(), ntf.roomid(), ntf.ip(), ntf.port()) );
}
