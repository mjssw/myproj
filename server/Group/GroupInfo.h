#ifndef _GROUP_INFO_H_
#define _GROUP_INFO_H_

#include <string>
#include <map>
#include <vector>
#include "Lock.h"

class CGroupInfo;
class CGroupMember
{
public:
	CGroupMember(const char *user, const char *nickname, const char *head, bool online, CGroupInfo &group) : 
		m_User(user), 
		m_NickName(nickname), 
		m_Head(head), 
		m_bOnline(online),
		m_GateResId(0),
		m_ClientId(0),
		m_Group(group)
	{}

	const std::string& GetUser(){ return m_User; }
	const std::string& GetNickName(){ return m_NickName; }
	void SetNickName(const char *nickname){ m_NickName = nickname; }
	const std::string& GetHead(){ return m_Head; }
	void SetHead(const char *head){ m_Head = head; }
	bool IsOnline(){ return m_bOnline; }
	void SetOnline(bool bOnline, s32 gateResId, u64 clientId);

	s32 GetGateResId(){ return m_GateResId; }
	u64 GetClientId(){ return m_ClientId; }

	u64 GetGateId(){ return m_GateId; }
	void SetGateId(u64 gateid){ m_GateId = gateid; }

private:
	std::string m_User;
	std::string m_NickName;
	std::string m_Head;
	bool m_bOnline;
	s32 m_GateResId;
	u64 m_ClientId;
	CGroupInfo &m_Group;
	u64 m_GateId;

public:
	union TempData
	{
		u64 param1;
	} m_tmp;
};

class CGroupInfo
{
public:
	friend class CGroupMember;

	CGroupInfo(u64 id, const char *name, const char *icon);
	~CGroupInfo();

	u64 GetId();
	const std::string& GetName();
	const std::string& GetIcon();
	void SetName(const char *name);
	void SetIcon(const char *icon);

	CGroupMember* AddMember(const char *user, const char *nickname, const char *head, bool online);
	void DelMember(const char *user);
	CGroupMember* FindMember(const char *user);
	CGroupMember* FindMember(s32 gateresid, u64 clientid);
	s32 MemberCount();
	void DumpMemberInfo(std::map<s32, std::vector<u64> > &alluser, const std::string &excludeUser, std::vector<std::string> &vecUser);

	void DisplayInfo();

	void Dump(std::vector<std::string> &vec);

	const std::string& GetLastCheckTime();
	void SetLastCheckTime(const std::string &t);

private:
	void _AddMemberIndex(CGroupMember *member);
	void _DelMemberIndex(CGroupMember *member);

private:
	u64 m_Id;
	std::string m_Name;
	std::string m_Icon;

	SGLib::CLock m_Lock;
	std::map<std::string, CGroupMember*> m_Members;

	std::map<s32, std::map<u64, CGroupMember*> > m_MemberIndex;

	// 此变量是一个年月值,形如 201407 
	// 收到聊天消息时如果当前日期是1号，且当前年月与m_lastCheckTime不一致，
	// 则需要重新建新的group_message表,形如 group_123_201408
	std::string m_lastCheckTime;
};

#endif
