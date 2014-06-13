#ifndef _GROUP_CLIENT_H_
#define _GROUP_CLIENT_H_

#include "ClientLib.h"
using namespace SGLib;

#include "client.pb.h";
#include "msgid.pb.h"
#include "common.pb.h"
#include "errno.pb.h"
#include "group.pb.h"

#include <map>
#include <vector>
#include <string>
using namespace std;


class CGroupClient : public SGLib::CClientBase
{
public:
	enum
	{
		MSG_ID_LEN = sizeof(s32),
		MSG_HEAD_LEN = MSG_ID_LEN,
		MAX_MSG_LEN = 4096,
	};

	CGroupClient(int id) : SGLib::CClientBase(id) {}

	virtual int OnRecv(char *buf, int len);
	virtual void OnConnect();
	virtual void OnClose();
	virtual void OnConnectError();

public:
	void LoginGroup(const std::string &);
	void CreateGroup(const std::string &gpname);
	void AddMemberToGroup(std::vector< std::string > &member);
	void AgreeJoinGroup(std::string &groupid);
	void GroupMessage(std::string &groupid, std::string &msg);
	void LeaveGroup(std::string &groupid);
	void DeleteGroup(std::string &groupid);
	void AskCreateGameRoom(u64 groupid, s32 game);

private:
	void SendMsg(const ::google::protobuf::Message &Msg, s32 nMsgId);

private:
	void _LoginGroupResult(const sglib::groupproto::SCGroupUserLoginRsp &rsp);
	void _CreateGroupRsp(const sglib::groupproto::SCGroupCreateRsp &rsp);
	void _AddMemberRsp(const sglib::groupproto::SCGroupAddMemberRsp &rsp);
	void _AskJoinGroupNtf(const sglib::groupproto::SCGroupAskJoinNtf &ntf);
	void _MemberJoinGroupNtf(const sglib::groupproto::SCGroupMemberJoinNtf &ntf);
	void _GroupMessageNtf(const sglib::groupproto::SCGroupMessageNtf &ntf);
	void _GroupInfoUpdate(const sglib::groupproto::SCGroupInfoUpdate &up);
	void _GroupListUpdate(const sglib::groupproto::SCGroupListUpdate &up);
	void _LeaveGroupRsp(const sglib::groupproto::SCGroupLeaveRsp &rsp);
	void _MemberLeaveGroupNtf(const sglib::groupproto::SCGroupMemberLeaveNtf &ntf);
	void _DeleteGroupRsp(const sglib::groupproto::SCGroupDeleteRsp &rsp);
	void _DeleteGroupNtf(const sglib::groupproto::SCGroupDeleteNtf &ntf);
	void _GroupCreateGameRoomResult( const sglib::groupproto::SCGroupCreateGameRoomRsp &rsp);
	void _GroupMemberCreateGameRoomNtf(const sglib::groupproto::SCGroupCreateGameRoomNtf &ntf);

private:
	std::string m_User;
};

#endif
