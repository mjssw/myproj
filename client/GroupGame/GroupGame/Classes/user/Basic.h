#ifndef _USER_BASIC_H_
#define _USER_BASIC_H_

#include "Define.h"
#include <string>
#include "CommDef.h"

class CUserBasic
{
public:

	CUserBasic();
	~CUserBasic();

	void SetUser(const std::string &user);
	const std::string& GetUser();
	void SetPwd(const std::string &pwd);
	const std::string& GetPwd();
	void SetName(const std::string &name);
	const std::string& GetName();
	void SetSex(s32 sex);
	s32 GetSex();
	void SetHead(const std::string &head);
	const std::string& GetHead();
	void SetExp(u64 exp);
	u64 GetExp();
	void SetLevel(u64 level);
	u64 GetLevel();
	void SetMoney(EMoneyType type, u64 money);
	u64 GetMoney(EMoneyType type);

	void SetRegUser(const std::string &user);
	const std::string& GetRegUser();
	void SetRegPwd(const std::string &pwd);
	const std::string& GetRegPwd();
	void SetRegResult(s32 regRet);
	s32 GetRegResult();

private:
	std::string m_user;
	std::string m_pwd;
	std::string m_name;
	s32 m_sex;
	std::string m_head;
	u64 m_exp;
	u64 m_level;
	u64 m_money[E_Money_Type_Count];

	std::string m_regUser;
	std::string m_regPwd;
	s32 m_regResult;
};

#endif
