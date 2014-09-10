#ifndef _GAMEMSG_LOGIN_MODULE_H_
#define _GAMEMSG_LOGIN_MODULE_H_

#include "MsgBase.h"
#include "Define.h"
#include <string>
#include <vector>
#include "user/GameInfo.h"

class CMsgConnectLogin : public CMsgBase
{
public:
	CMsgConnectLogin();
	virtual void Process();
};

class CMsgCloseLogin : public CMsgBase
{
public:
	CMsgCloseLogin();
	virtual void Process();
};

class CMsgConnectLoginError : public CMsgBase
{
public:
	CMsgConnectLoginError();
	virtual void Process();
};

class CMsgGameInfoNotify : public CMsgBase
{
public:
	CMsgGameInfoNotify(std::vector<CGameInfo> &games);
	virtual void Process();

private:
	std::vector<CGameInfo> m_games;
};

class CMsgGroupGateNotify : public CMsgBase
{
public:
	CMsgGroupGateNotify(const std::string &ip, s32 port);
	virtual void Process();

private:
	std::string m_ip;
	s32 m_port;
};

class CMsgLoginResult : public CMsgBase
{
public:
	CMsgLoginResult(s32 result, const std::string &token);
	virtual void Process();

private:
	s32 m_result;
	std::string m_token;
};

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

class CMsgConnectRegister : public CMsgBase
{
public:
	CMsgConnectRegister();
	virtual void Process();
};

class CMsgResiterRsp : public CMsgBase
{
public:
	CMsgResiterRsp(s32 result);
	virtual void Process();
private:
	s32 m_result;
};

#endif
