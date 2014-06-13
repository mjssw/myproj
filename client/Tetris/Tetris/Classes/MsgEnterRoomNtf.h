#ifndef _MSG_ENTER_ROOM_NTF_H_
#define _MSG_ENTER_ROOM_NTF_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "GameList.h"
#include "NetClient.h"
#include "GameManager.h"

class CMsgEnterRoomNtf : public CMsgBase
{
public:
	CMsgEnterRoomNtf(const std::string &user) : m_EnterPlayer(user)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgEnterRoomNtf" );
		CGameManager::Instance().EnterRoom( m_EnterPlayer );	
	}

private:
	std::string m_EnterPlayer;
};

#endif