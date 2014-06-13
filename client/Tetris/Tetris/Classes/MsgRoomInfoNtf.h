#ifndef _MSG_ROOM_INFO_NTF_H_
#define _MSG_ROOM_INFO_NTF_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "TetrisScene.h"
#include "NetClient.h"
#include "GameManager.h"

class CMsgRoomInfoNtf : public CMsgBase
{
public:
	CMsgRoomInfoNtf(int count, CPlayerInfo **info) : m_Count(count), m_OtherPlayers(info)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgRoomInfoNtf" );
		for( int i=0; i<m_Count; ++i )
		{
			CGameManager::Instance().RoomInfo(
				m_OtherPlayers[i]->m_User,
				m_OtherPlayers[i]->m_bReady );
		}
		SAFE_DELETE_ARRAY( m_OtherPlayers );
	}

private:
	int m_Count;
	CPlayerInfo **m_OtherPlayers;
};

#endif