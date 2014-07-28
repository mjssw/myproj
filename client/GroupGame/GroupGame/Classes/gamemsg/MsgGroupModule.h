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

class CMsgGroupInfoUpdate: public CMsgBase
{
public:
	CMsgGroupInfoUpdate(u64 groupid, std::vector<CGroupMember*> *members); 
	virtual void Process();
private:
	u64 m_groupid;
	std::vector<CGroupMember*> *m_members;
};

#endif
