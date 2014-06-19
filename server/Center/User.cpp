#include "User.h"
using namespace std;

CUser::CUser() : m_user(""), m_flag("")
{
}

CUser::CUser(const std::string &user) : m_user(user), m_flag("")
{
}

CUser::~CUser()
{
}

void CUser::SetUser(const std::string &user)
{ 
	m_user = user; 
}

const char* CUser::User()
{ 
	return m_user.c_str(); 
}

void CUser::SetFlag(const std::string &flag)
{ 
	m_flag = flag; 
}

const std::string& CUser::GetFlag()
{ 
	return m_flag; 
}
