#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_

#include "cocos2d.h"
#include "Define.h"
#include "LoginClient.h"
#include "GroupClient.h"
#include "GameClient.h"
#include "Timer.h"
#include <deque>

#define LOGIN_IP	"172.18.5.55"
#define LOGIN_PORT	10003

class CMsgBase;
class CNetManager : public cocos2d::Node
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
	void CloseGameConn();
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

private:
	void _CloseAll();

private:
	CLoginClient *m_loginClient;
	CGroupClient *m_groupClient;
	CGameClient  *m_gameClient;

	SGLib::CClientManager<CLoginClient> *m_loginConn;
	SGLib::CClientManager<CGroupClient> *m_groupConn;
	SGLib::CClientManager<CGameClient> *m_gameConn;

	SGLib::CTimer<E_Max_Timer_Count>  m_timer;

	SGLib::CLock m_msgLock;
	std::deque<CMsgBase*> m_msgQueue;
	bool m_isPauseProcessMessage;
};

#endif
