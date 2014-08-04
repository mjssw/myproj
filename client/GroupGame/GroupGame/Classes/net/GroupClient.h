#ifndef _GROUP_CLIENT_H_
#define _GROUP_CLIENT_H_

#include "ProtoClientBase.h"
#include "group.pb.h"
#include <string>

class CGroupClient : public CProtoClientBase
{
public:
	CGroupClient(s32 id);
	virtual int OnRecv(char *buf, int len);
	virtual void OnConnect();
	virtual void OnClose();
	virtual void OnConnectError();

	void Login(const std::string &user, const std::string &token);
	void CreateGroup(const std::string &name, const std::string &head);
	void LeaveGroup(u64 groupid);
	void InviteMember(u64 groupid, const std::string &user);
	void AgreeJoinGroup(u64 groupid);

private:
	void _LoginGroupResult(sglib::groupproto::SCGroupUserLoginRsp &msg);
	void _GroupListUpdate(sglib::groupproto::SCGroupListUpdate &msg);
	void _GroupInfoUpdate(sglib::groupproto::SCGroupInfoUpdate &msg);
	void _CreateGroupRsp(sglib::groupproto::SCGroupCreateRsp &msg);
	void _LeaveGroupRsp(sglib::groupproto::SCGroupLeaveRsp &msg);
	void _MemberLeaveGroupNtf(sglib::groupproto::SCGroupMemberLeaveNtf &msg);
	void _AddMemberRsp(sglib::groupproto::SCGroupAddMemberRsp &msg);
	void _AskJoinGroupNtf(sglib::groupproto::SCGroupAskJoinNtf &msg);
	void _MemberJoinGroupNtf(sglib::groupproto::SCGroupMemberJoinNtf &msg);
};

#endif
