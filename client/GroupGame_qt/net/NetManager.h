#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_

#include "Define.h"
#include "LoginClient.h"
#include "GroupClient.h"
#include "GameClient.h"
#include "RegisterClient.h"
#include <deque>

/*
#ifdef _LOCAL_SERVER
#define LOGIN_IP	"127.0.0.1"
#else
//#define LOGIN_IP	"172.18.5.55"
#define LOGIN_IP	"192.168.1.106"
#endif
//*/
//#define LOGIN_IP	"192.168.1.106"
#define LOGIN_IP	"172.18.5.55"
#define LOGIN_PORT	10003

class CMsgBase;
class CNetManager
{
	SINGLETON_CLASS_DEFINE(CNetManager);

public:
	enum
	{
		E_Max_Timer_Count = 100,
	};
	
    virtual void update(float delta);

	bool Init();
	void Fini();
	bool StartLogin(const char *ip, int port);
	bool StartGroup(const char *ip, int port);
	bool StartGame(const char *ip, int port);
	bool StartRegister(const char *ip, int port);
	void CloseGameConn();
	void CloseLoginConn();
	void CloseRegisterConn();
	void PushMessage(CMsgBase *msg);
	void ProcessMessage();
	void PauseProcessMessage();
	void ResumeProcessMessage();
	void SetLoginClientInstance(CLoginClient *client);
	CLoginClient* GetLoginClientInstance();
	void SetGroupClientInstance(CGroupClient *client);
	CGroupClient* GetGroupClientInstance();
	void SetGameClientInstance(CGameClient *client);
	CGameClient* GetGameClientInstance();
	void SetRegClientInstance(CRegisterClient *client);
	CRegisterClient* GetRegClientInstance();
	
//	static int SendGameMessage(lua_State *ls);
//	void LuaProcessGameMessage(int msgid, int msglen, void *data);

private:
	void _CloseAll();

private:
	CLoginClient *m_loginClient;
	CGroupClient *m_groupClient;
	CGameClient  *m_gameClient;
	CRegisterClient *m_registerClient;

	SGLib::CClientManager<CLoginClient> *m_loginConn;
	SGLib::CClientManager<CGroupClient> *m_groupConn;
	SGLib::CClientManager<CGameClient> *m_gameConn;
	SGLib::CClientManager<CRegisterClient> *m_regConn;

	SGLib::CLock m_msgLock;
	std::deque<CMsgBase*> m_msgQueue;
	bool m_isPauseProcessMessage;
};

#endif