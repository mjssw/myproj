#include "UserManager.h"

SIGNLETON_CLASS_INIT(CUserManager);

void CUserManager::LoadFromConfig()
{
	// TODO 
	SetSavePwd( false );
	SetAutoLogin( false );
}

void CUserManager::SetSavePwd(bool isSavePwd)
{
	m_isSavePwd = isSavePwd;
}

bool CUserManager::GetSavePwd()
{
	return m_isSavePwd;
}

void CUserManager::SetAutoLogin(bool isAutoLogin)
{
	m_isAutoLogin = isAutoLogin;
}

bool CUserManager::GetAutoLogin()
{
	return m_isAutoLogin;
}

void CUserManager::SetUser(const std::string &user)
{
	m_user = user;
}

const std::string& CUserManager::GetUser()
{
	return m_user;
}

void CUserManager::SetPwd(const std::string &pwd)
{
	m_pwd = pwd;
}

const std::string& CUserManager::GetPwd()
{
	return m_pwd;
}
