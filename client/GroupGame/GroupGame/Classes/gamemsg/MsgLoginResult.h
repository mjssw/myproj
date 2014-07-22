#ifndef _GAMEMSG_LOGIN_RESULT_H_
#define _GAMEMSG_LOGIN_RESULT_H_

#include "MsgBase.h"
#include "Define.h"
#include <string>

class CMsgLoginResult : public CMsgBase
{
public:
	CMsgLoginResult(s32 result, const std::string &token);
	virtual void Process();

private:
	s32 m_result;
	std::string m_token;
};

#endif
