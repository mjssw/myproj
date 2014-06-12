#ifndef _GROUP_INFO_MANAGER_H_
#define _GROUP_INFO_MANAGER_H_

#include <map>
#include <vector>
#include "Lock.h"
#include "Utils.h"

class CGateClientInfo;
struct SGroupInfo
{
	s32 m_TotalGroupCount;
	s32 m_TotalMemberCount;
	u64 m_InstanceId;
};

class CGroupInfoManager
{
	DEFINE_SINGLETON_CLASS(CGroupInfoManager);

public:
	~CGroupInfoManager();
	void SetGroupInfo(s32 id, s32 totalGroup, s32 totalMember, u64 serverid);
	void DelGroupInfo(s32 id);
	const SGroupInfo* GetGroupInfo(s32 id);	
	void SetGateInfo(s32 gateId, const char *ip, s32 port, s32 curCount, s32 maxCount);
	void DelGateInfo(s32 gateId);
	void GetGateList(std::vector<s32> &vecId);
	const CGateClientInfo* GetGroupGateInfo(s32 gateId);
	u64 FindProperGroup();

private:
	SGLib::CLock m_Lock;
	std::map<s32, SGroupInfo> m_mapGroupInfo;
	std::map<s32, CGateClientInfo*> m_mapGroupGateInfo;
};

#endif
