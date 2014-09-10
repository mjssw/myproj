#include "Basic.h"
using namespace std;

CUserBasic::CUserBasic() : 
	m_user( "" ),
	m_pwd( "" ),
	m_name( "" ),
	m_head( "" ),
	m_exp( 0 ),
	m_level( 0 )
{
	memset( m_money, 0, sizeof(m_money) );
}

CUserBasic::~CUserBasic()
{
}

void CUserBasic::SetUser(const string &user)
{
	m_user = user;
}

const string& CUserBasic::GetUser()
{
	return m_user;
}

void CUserBasic::SetPwd(const string &pwd)
{
	m_pwd = pwd;
}

const string& CUserBasic::GetPwd()
{
	return m_pwd;
}

void CUserBasic::SetName(const string &name)
{
	m_name = name;
}

const string& CUserBasic::GetName()
{
	return m_name;
}

void CUserBasic::SetSex(s32 sex)
{
	m_sex = sex;
}

s32 CUserBasic::GetSex()
{
	return m_sex;
}

void CUserBasic::SetHead(const string &head)
{
	m_head = head;
}

const string& CUserBasic::GetHead()
{
	return m_head;
}

void CUserBasic::SetExp(u64 exp)
{
	m_exp = exp;
}

u64 CUserBasic::GetExp()
{
	return m_exp;
}

void CUserBasic::SetLevel(u64 level)
{
	m_level = level;
}

u64 CUserBasic::GetLevel()
{
	return m_level;
}

void CUserBasic::SetMoney(EMoneyType type, u64 money)
{
	if( type != E_Money_Type_Count )
	{
		m_money[ type ] = money;
	}
}

u64 CUserBasic::GetMoney(EMoneyType type)
{
	if( type != E_Money_Type_Count )
	{
		return m_money[ type ];
	}
	return 0;
}

void CUserBasic::SetRegUser(const string &user)
{
	m_regUser = user;
}

const string& CUserBasic::GetRegUser()
{
	return m_regUser;
}

void CUserBasic::SetRegPwd(const string &pwd)
{
	m_regPwd = pwd;
}

const string& CUserBasic::GetRegPwd()
{
	return m_regPwd;
}

void CUserBasic::SetRegResult(s32 regRet)
{
	m_regResult = regRet;
}

s32 CUserBasic::GetRegResult()
{
	return m_regResult;
}
