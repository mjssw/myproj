#ifndef _GAMEMSG_GROUP_MODULE_H_
#define _GAMEMSG_GROUP_MODULE_H_

#include "MsgBase.h"
#include "Define.h"
#include <string>
#include <vector>

class CGroup;
class CGroupMember;

class CMsgConnectGroup : public CMsgBase
{
public:
	CMsgConnectGroup();
	virtual void Process();
};

class CMsgLoginGroupResult : public CMsgBase
{
public:
	CMsgLoginGroupResult(s32 result); 
	virtual void Process();
private:
	s32 m_result;
};

class CMsgGroupListUpdate: public CMsgBase
{
public:
	CMsgGroupListUpdate(std::vector<CGroup*> *groups); 
	virtual void Process();
private:
	std::vector<CGroup*> *m_groups;
};

class CMsgGroupInfoUpdate : public CMsgBase
{
public:
	CMsgGroupInfoUpdate(u64 groupid, std::vector<CGroupMember*> *members); 
	virtual void Process();
private:
	u64 m_groupid;
	std::vector<CGroupMember*> *m_members;
};

class CMsgCreateGroupResult : public CMsgBase
{
public:
	CMsgCreateGroupResult(s32 result, const std::string &name, const std::string &head, u64 groupid);
	virtual void Process();
private:
	u64 m_groupid;
	s32 m_result;
	std::string m_name;
	std::string m_head;
};

class CMsgLeaveGroupResult : public CMsgBase
{
public:
	CMsgLeaveGroupResult(s32 result, u64 groupid);
	virtual void Process();
private:
	u64 m_groupid;
	s32 m_result;
};

class CMsgMemberLeaveGroupNotify : public CMsgBase
{
public:
	CMsgMemberLeaveGroupNotify(u64 groupid, const std::string &user);
	virtual void Process();
private:
	u64 m_groupid;
	std::string m_leaveuser;
};

class CMsgAddMemberRsp : public CMsgBase
{
public:
	CMsgAddMemberRsp(u64 groupid, s32 result);
	virtual void Process();
private:
	u64 m_groupid;
	s32 m_result;
};

class CMsgAskJoinGroup : public CMsgBase
{
public:
	CMsgAskJoinGroup(u64 groupid, const std::string &name);
	virtual void Process();
private:
	u64 m_groupid;
	std::string m_name;
};

class CMsgMemberJoinGroup : public CMsgBase
{
public:
	CMsgMemberJoinGroup(u64 groupid, const std::string &user, const std::string &name, const std::string &head);
	virtual void Process();
private:
	u64 m_groupid;
	std::string m_user;
	std::string m_name;
	std::string m_head;
};

class CMsgGroupMessage : public CMsgBase
{
public:
	CMsgGroupMessage(u64 groupid, const std::string &sender, const std::string &content);
	virtual void Process();
private:
	u64 m_groupid;
	std::string m_sender;
	std::string m_content;
};

class CMsgGroupCreateGameRoomRsp : public CMsgBase
{
public:
	CMsgGroupCreateGameRoomRsp(s32 result, u64 groupid, s32 game, const std::string &ip, s32 port, s32 roomid, const std::string &roompwd);
	virtual void Process();
private:
	s32 m_result;
	u64 m_groupid;
	s32 m_game;
	std::string m_ip;
	s32 m_port;
	s32 m_roomid;
	std::string m_roompwd;
};

class CMsgGroupCreateGameRoomNtf : public CMsgBase
{
public:
	CMsgGroupCreateGameRoomNtf(const std::string &creater, u64 groupid, s32 game, const std::string &ip, s32 port, s32 roomid, const std::string &roompwd);
	virtual void Process();
private:
	std::string m_creater;
	u64 m_groupid;
	s32 m_game;
	std::string m_ip;
	s32 m_port;
	s32 m_roomid;
	std::string m_roompwd;
};

#endif
