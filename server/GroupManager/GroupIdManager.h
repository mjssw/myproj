#ifndef _GROUP_ID_MANAGER_H_
#define _GROUP_ID_MANAGER_H_

#include "Utils.h"
#include "Lock.h"

class CGroupIdManager
{
	DEFINE_SINGLETON_CLASS(CGroupIdManager);

public:
	~CGroupIdManager();
	void Init(u64 from);
	u64 GetNextGroupId();

private:
	SGLib::CLock m_Lock;
	u64 m_NextId;
};

#endif
