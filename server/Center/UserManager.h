#ifndef _USER_MANAGER_H_
#define _USER_MANAGER_H_

#include "Define.h"
#include <map>
#include <string>

class CUser;
class CCenterClient;
class CUserManager
{
	SINGLETON_CLASS_DEFINE(CUserManager);
public:
	void AddUser(const std::string &user, const std::string &flag);
	void DelUser(const std::string &user);
	void NotifyGameListToUser(CCenterClient &client, u64 gateid, u64 clientid);
	void UserAskEnterGameInfo(CCenterClient &client, u64 gateid, u64 clientid, s32 gameid, u64 gsid);

private:
	std::map<std::string, CUser*> m_mapUsers;
};

#endif
