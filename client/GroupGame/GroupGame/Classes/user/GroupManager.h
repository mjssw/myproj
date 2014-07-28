#ifndef _GROUP_MANAGER_H_
#define _GROUP_MANAGER_H_

#include "Define.h"
#include <string>
#include <map>
#include <vector>

class CGroup;
class CGroupManager
{
public:
	CGroupManager();
	~CGroupManager();

	bool AddGroup(CGroup *group);
	void RemoveGroup(u64 id);
	CGroup* FindGroup(u64 id);
	void Dump(std::vector<u64> &vec);

private:
	std::map<u64, CGroup*> m_groups;
};

#endif
