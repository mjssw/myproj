#ifndef _USER_BASIC_INFO_H_
#define _USER_BASIC_INFO_H_

#include "Define.h"
#include <string>

class CUserBasic
{
public:
	enum EMoneyType
	{
		E_Money_Gold,
		E_Money_Diamond,

		E_Money_Type_Count,
	};
	enum ESex
	{
		E_Sex_Male,
		E_Sex_Female,
	};
	
	CUserBasic();
	~CUserBasic();

	static ESex ConvertSex(s32 sex);

	void SetName(const std::string &name);
	const std::string& GetName();
	void SetHead(const std::string &head);
	const std::string& GetHead();
	void SetSex(ESex sex);
	ESex GetSex();
	void SetExp(u64 exp);
	u64 GetExp();
	void SetLevel(u64 level);
	u64 GetLevel();
	void SetMoney(EMoneyType type, u64 money);
	u64 GetMoney(EMoneyType type);

private:
	std::string m_name;
	std::string m_head;
	ESex m_sex;
	u64 m_exp;
	u64 m_level;
	u64 m_money[E_Money_Type_Count];
};

#endif
