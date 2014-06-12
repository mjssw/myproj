#ifndef _GROUP_MANAGER_H_
#define _GROUP_MANAGER_H_

#include "Utils.h"
#include "Lock.h"
#include <map>

class CGroupManager
{
	DEFINE_SINGLETON_CLASS(CGroupManager);

public:
	~CGroupManager();
	void AddGroup(u64 groupid, s32 serverid);
	void DelGroup(u64 groupid);
	s32 FindServerId(u64 groupid);

	void DisplayInfo();
private:
	SGLib::CLock m_Lock;
	std::map<u64, s32> m_GroupMap;
};

#endif
