#ifndef _MSG_LEAVE_ROOM_NTF_H_
#define _MSG_LEAVE_ROOM_NTF_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "TetrisScene.h"
#include "NetClient.h"
#include "GameManager.h"

class CMsgLeaveRoomNtf : public CMsgBase
{
public:
	CMsgLeaveRoomNtf(const std::string &user) : m_LeaveUser(user)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgLeaveRoomNtf" );
		CGameManager::Instance().LeaveRoom( m_LeaveUser );
	}

private:
	std::string m_LeaveUser;
};

#endif