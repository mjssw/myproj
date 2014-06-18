#ifndef _LOGIN_MANAGER_H_
#define _LOGIN_MANAGER_H_

#include "Define.h"
#include "DBQuery.h"
#include <map>
#include <string>

class CLoginClient;

class CUser
{
public:
	enum 
	{
		E_State_Free,
		E_State_Logining,
		E_State_LoginSuccess,
		E_State_LoginFailed,
	};

	CUser() : m_user(""), m_state(E_State_Free), m_token("")
	{}
	CUser(const std::string &user) : m_user(user), m_state(E_State_Free), m_token("")
	{}

	void SetUser(const std::string &user) { m_user = user; }
	const char* User() { return m_user.c_str(); }
	void SetState(s32 state) { m_state = state; }
	s32 GetState() { return m_state; }
	void SetToken(const std::string &token) { m_token = token; }
	const std::string& GetToken() { return m_token; }
	void SetFlag(const std::string &flag) { m_flag = flag; }
	const std::string& GetFlag() { return m_flag; }

private:
	std::string m_user;
	s32 m_state;
	std::string m_token;
	std::string m_flag;
};

class CLoginManager
{
	SINGLETON_CLASS_DEFINE(CLoginManager);

public:
	static s32 HashUser(const std::string &user);
	void UserLogin(CLoginClient &client, u64 clientId, const std::string &user, const std::string &pwd, const std::string &flag);
	void UserRegister(CLoginClient &client, u64 clientId, const std::string &user, const std::string &pwd);
	void UserAskEnterGame(u64 clientid, u64 gateid, s32 gameid);

private:
	void _NotifyLoginResult(CLoginClient &client, u64 clientId, s32 result, const std::string &token);
	std::string _BuildToken(const std::string &user, const std::string &flag);
	void _NotifyCenterUserLogin(u64 clientId, u64 gateid, const std::string &user, const std::string &flag);
	void _NotifyRegisterResult(CLoginClient &client, u64 clientId, s32 result);

private:
	void _RegisterCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len);
	void _UserLoginCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len);

private:
	std::map<std::string, CUser*> m_userLogin;
};

#endif
