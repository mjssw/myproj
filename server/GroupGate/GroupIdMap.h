#ifndef _GROUP_ID_MAP_H_
#define _GROUP_ID_MAP_H_

#include "Lock.h"
#include "Utils.h"
#include <map>

class CGroupIdMap
{
DEFINE_SINGLETON_CLASS(CGroupIdMap);

public:
	void AddGroupId(u64 groupid, s32 serverid);
	void DelGroupId(u64 groupid);
	s32 GetServerIdByGroupId(u64 groupid);

	void DisplayInfo();
private:
	SGLib::CLock m_Lock;
	std::map<u64, s32> m_GroupIdMap;
};

#endif
