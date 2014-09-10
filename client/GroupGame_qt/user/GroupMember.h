#ifndef _GROUP_MEMBER_H_
#define _GROUP_MEMBER_H_

#include "Define.h"
#include <string>

class CGroupMember
{
public:
	enum
	{
		E_Member_Offline = 0,
		E_Member_Online,
	};

	CGroupMember(const std::string &user);
	~CGroupMember();

	const std::string& GetUser();
	void SetName(const std::string &name);
	const std::string& GetName();
	void SetHead(const std::string &head);
	const std::string& GetHead();
	void SetOnline(bool online);
	bool IsOnline();

private:
	std::string m_user;	
	std::string m_name;
	std::string m_head;
	bool m_isonline;
};

#endif
