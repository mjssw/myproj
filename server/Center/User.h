#ifndef __USER_H_
#define __USER_H_

#include <string>

class CUser
{
public:
	CUser();
	CUser(const std::string &user);
	~CUser();

	void SetUser(const std::string &user);
	const char* User();
	void SetFlag(const std::string &flag);
	const std::string& GetFlag();

private:
	std::string m_user;
	std::string m_flag;
};

#endif
