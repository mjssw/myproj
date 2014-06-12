#ifndef _GROUP_MEMBER_POS_MANAGER_H_
#define _GROUP_MEMBER_POS_MANAGER_H_

#include <string>
#include <map>
#include "Utils.h"
#include "Lock.h"

class CGroupMemberPosition
{
public:
	CGroupMemberPosition(const char *user) : 
		m_User(user), 
		m_GateResId(0),
		m_ClientId(0)
	{}

	~CGroupMemberPosition(){}

	const std::string& User(){ return m_User; }
	void SetGateResId(s32 id){ m_GateResId = id; }
	s32  GetGateResId(){ return m_GateResId; }
	void SetClientId(u64 id){ m_ClientId = id; }
	u64  GetClientId(){ return m_ClientId; } 

private:
	std::string m_User;
	s32 m_GateResId;
	u64 m_ClientId;
};

class CGroupMemberPositionManager
{
public:
	CGroupMemberPositionManager();
	~CGroupMemberPositionManager();

	void SetMember(s32 gateResId, u64 clientid, const char *user);
	void DelMember(s32 gateResId, u64 clientid);
	CGroupMemberPosition* FindMember(s32 gateResId, u64 clientid);
	CGroupMemberPosition* FindMember(const std::string &user);

	void DisplayInfo();
private:
	SGLib::CLock m_Lock;
	std::map< s32, std::map<u64, CGroupMemberPosition*> > m_Members;
	std::map< std::string, CGroupMemberPosition* > m_MembersIndex;
};

#endif
