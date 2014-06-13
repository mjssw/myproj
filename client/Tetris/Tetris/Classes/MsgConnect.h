#ifndef _MSG_CONNECT_H_
#define _MSG_CONNECT_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "GameList.h"
#include "NetClient.h"

class CMsgConnect : public CMsgBase
{
public:
	CMsgConnect(void *client) : m_pClient(client)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "====> CMsgConnect::Proc : state:%d", CNetManager::Instance().GetState() );
		CNetManager::Instance().SetClient( (CNetClient*)m_pClient );
		if( CNetManager::Instance().GetState() == CNetManager::E_State_Connecting_LoginServer )
		{
			CNetManager::Instance().SetState( CNetManager::E_State_Connected_LoginServer );
			CNetManager::Instance().Login( CGameManager::Instance().GetUser() );
		}
		else if( CNetManager::Instance().GetState() == CNetManager::E_State_Connecting_GameServer )
		{
			cocos2d::CCLog( "====> connected to game server, user:%s",
				CGameManager::Instance().GetUser().c_str());
			CNetManager::Instance().SetState( CNetManager::E_State_Connected_GameServer );
			CNetManager::Instance().LoginGame( CGameManager::Instance().GetUser().c_str() );
		}
		else if( CNetManager::Instance().GetState() == CNetManager::E_State_Connecting_GameServer_ByGroup )
		{
			cocos2d::CCLog( "++++++ connected to game server, now login game" );
			// 通过群进入游戏房间
			CNetManager::Instance().SetState( CNetManager::E_State_Connected_GameServer_ByGroup );
			// 先登录游戏
			CNetManager::Instance().LoginGame( CGameManager::Instance().GetUser().c_str() );
		}
	}

private:
	void *m_pClient;
};

#endif
