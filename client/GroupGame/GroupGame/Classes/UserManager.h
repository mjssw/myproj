#ifndef _USER_MANAGER_H_
#define _USER_MANAGER_H_

#include "Define.h"
#include <string>

class CUserManager
{
	SINGLETON_CLASS_DEFINE(CUserManager);

public:
	void LoadFromConfig();

	void SetSavePwd(bool isSavePwd);
	bool GetSavePwd();
	void SetAutoLogin(bool isAutoLogin);
	bool GetAutoLogin();

	void SetUser(const std::string &user);
	const std::string& GetUser();
	void SetPwd(const std::string &pwd);
	const std::string& GetPwd();



private:
	bool m_isSavePwd;
	bool m_isAutoLogin;
	std::string m_user;
	std::string m_pwd;
};

#endif
