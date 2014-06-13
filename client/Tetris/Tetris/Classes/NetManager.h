#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_

#include "utils.h"
#include <deque>
#include "Lock.h"
#include "ClientLib.h"
#include "NetClient.h"
#include "GroupClient.h"
#include "cocoa/CCObject.h"

class CMsgBase;
class CNetManager : public cocos2d::CCObject
{
	DEFINE_SINGLETON_CLASS(CNetManager);

public:
	enum
	{
		E_State_Connecting_LoginServer,
		E_State_Connected_LoginServer,
		E_State_Connecting_GameServer,
		E_State_Connected_GameServer,
		E_State_Connecting_GroupServer,
		E_State_Connected_GroupServer,
		E_State_Connecting_GameServer_ByGroup,
		E_State_Connected_GameServer_ByGroup,
	};

public:
	~CNetManager(){ Stop(); }

public:
	void Init();
	bool Start(const char *ip, s32 port);
	void Stop();

	bool StartGroup(const char *ip, s32 port);
	void StopGroup();
	void SetGroupClient(CGroupClient *client);

	void ProcMessage();
	void PushMessage(CMsgBase *msg);

	void SetClient(CNetClient *client);
	void SetState(int state);
	int GetState();
    
	virtual void update(float dt);

	void PauseProcMsg();
	void ResumeProcMsg();
public:
	void Login(const std::string &user);
	void EnterGame(int gameId);
	void LoginGame(const char *user);
	void EnterRoom(int roomId);
	void LeaveRoom();
	void Ready();
	void Lost();
	void PutBlock(s32 blockid, s32 column);
	void ClearBlock(std::vector<s32> &vecRows);

public:
	// Group
	void LoginGroup(const std::string &user);
	void CreateGameRoom(u64 groupid, s32 game);

private:
	SGLib::CLock m_Lock;
	std::deque<CMsgBase*> m_MsgQueue;

	CClientManager<CNetClient> *m_pClientMgr;
	CNetClient *m_pClient;

	CClientManager<CGroupClient> *m_pGroupClientMgr;
	CGroupClient *m_pGroupClient;

	int m_nState;
	bool m_bPauseProcMsg;
};

#endif
