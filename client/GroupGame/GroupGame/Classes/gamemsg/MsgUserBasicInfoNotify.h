#ifndef _GAMEMSG_USER_BASIC_INFO_NOTIFY_H_
#define _GAMEMSG_USER_BASIC_INFO_NOTIFY_H_

#include "MsgBase.h"
#include "Define.h"
#include <string>

class CMsgUserBasicInfoNotify : public CMsgBase
{
public:
	CMsgUserBasicInfoNotify(const std::string &name, const std::string &head, s32 sex, u64 exp, u64 level, u64 gold, u64 diamond);
	virtual void Process();

private:
	std::string m_name;
	s32 m_sex;
	std::string m_head;
	u64 m_exp;
	u64 m_level;
	u64 m_gold;
	u64 m_diamond;
};

#endif
