#ifndef _LOGIN_MANAGER_H_
#define _LOGIN_MANAGER_H_

#include "Define.h"
#include "DBQuery.h"
#include <map>
#include <string>

class CLoginClient;
class CLoginManager
{
	SINGLETON_CLASS_DEFINE(CLoginManager);

public:
	static s32 HashUser(const std::string &user);
	void UserLogin(CLoginClient &client, u64 clientId, const std::string &user, const std::string &pwd, const std::string &flag);
	void UserRegister(CLoginClient &client, u64 clientId, const std::string &user, const std::string &pwd);
	void UserAskEnterGame(u64 clientid, u64 gateid, s32 gameid);

private:
	s32 _UserLogin(const std::string &user, const std::string &pwd, const std::string &flag);
	void _NotifyLoginResult(CLoginClient &client, u64 clientId, s32 result, const std::string &token);
	std::string _BuildToken(const std::string &user, const std::string &flag);
	void _NotifyCenterUserLogin(u64 clientId, u64 gateid, const std::string &user, const std::string &flag);
	void _NotifyRegisterResult(CLoginClient &client, u64 clientId, s32 result);

private:
	void _RegisterCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len);
};

#endif
