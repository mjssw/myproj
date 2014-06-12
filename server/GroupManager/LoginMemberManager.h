#ifndef _LOGIN_MEMBER_MANAGER_H_
#define _LOGIN_MEMBER_MANAGER_H_

#include "Utils.h"
#include "GroupMemberPosManager.h"
#include "Lock.h"
#include <vector>

class CLoginMemberManager
{
	DEFINE_SINGLETON_CLASS(CLoginMemberManager);

public:
	~CLoginMemberManager();
	CGroupMemberPositionManager& MemberManager();

	void AddGroup(const std::string &user, u64 groupid);
	void AddGroup(const std::string &user, const std::vector<u64> &vecGroupid);
	void DelGroup(const std::string &user, u64 groupid);
	void DelAllGroup(const std::string &user);
	void GetGroups(const std::string &user, std::vector<u64> &vecGroups);

	void DisplayInfo();

private:
	void _AddGroup(const std::string &user, u64 groupid);

private:
	CGroupMemberPositionManager m_Members;

	SGLib::CLock m_Lock;
	std::map< std::string, std::map<u64, s32> > m_MembersGroup;
};

#endif
