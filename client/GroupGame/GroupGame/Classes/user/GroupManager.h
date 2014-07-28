#ifndef _GROUP_MANAGER_H_
#define _GROUP_MANAGER_H_

#include "Define.h"
#include <string>
#include <map>

class CGroup;
class CGroupManager
{
public:
	CGroupManager();
	~CGroupManager();

	bool AddGroup(CGroup *group);
	void RemoveGroup(u64 id);
	CGroup* FindGroup(u64 id);

private:
	std::map<u64, CGroup*> m_groups;
};

#endif
