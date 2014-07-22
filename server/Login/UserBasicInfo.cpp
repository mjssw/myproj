#include "UserBasicInfo.h"
using namespace std;

CUserBasic::CUserBasic() : 
	m_name( "" ),
	m_head( "" ),
	m_sex( E_Sex_Male ),
	m_exp( 0 ),
	m_level( 0 )
{
	memset( m_money, 0, sizeof(m_money) );
}

CUserBasic::~CUserBasic()
{
}

CUserBasic::ESex CUserBasic::ConvertSex(s32 sex)
{
	switch( sex )
	{
	case 0: return E_Sex_Male;
	case 1: return E_Sex_Female;
	default:
		return E_Sex_Female;	
		break;
	};
}

void CUserBasic::SetName(const string &name)
{
	m_name = name;
}

const string& CUserBasic::GetName()
{
	return m_name;
}

void CUserBasic::SetHead(const string &head)
{
	m_head = head;
}

const string& CUserBasic::GetHead()
{
	return m_head;
}

void CUserBasic::SetSex(ESex sex)
{
	m_sex = sex;
}

CUserBasic::ESex CUserBasic::GetSex()
{
	return m_sex;
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