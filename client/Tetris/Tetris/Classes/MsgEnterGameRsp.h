#ifndef _MSG_ENTER_GAME_RSP_H_
#define _MSG_ENTER_GAME_RSP_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "NetClient.h"
#include "GameManager.h"

class CMsgEnterGameRsp : public CMsgBase
{
public:
	CMsgEnterGameRsp(const char *ip, int port) : m_Ip(ip), m_Port(port)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgEnterGameRsp" );
		CNetManager::Instance().Stop();
		if( CNetManager::Instance().Start( m_Ip.c_str(), m_Port ) )
		{
			CNetManager::Instance().SetState( CNetManager::E_State_Connecting_GameServer );
		}
		else
		{
			CGameManager::Instance().ConnectLoginError();
		}
	}

private:
	std::string m_Ip;
	int m_Port;
};

#endif