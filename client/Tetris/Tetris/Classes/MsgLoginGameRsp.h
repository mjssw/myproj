#ifndef _MSG_LOGIN_GAME_RSP_H_
#define _MSG_LOGIN_GAME_RSP_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "TetrisScene.h"
#include "NetClient.h"

class CMsgLoginGameRsp : public CMsgBase
{
public:
	CMsgLoginGameRsp(s32 ret) : m_Result(ret)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgLoginGameRsp" );
		if( m_Result == sglib::errorcode::E_ErrorCode_Success )
		{
			if( CNetManager::Instance().GetState() == CNetManager::E_State_Connected_GameServer_ByGroup )
			{
				cocos2d::CCLog( "++++++ login game success, now enter room" );
				// 通过群进游戏房间，登录成功后直接进房间
				CGameRoomInfo *room = CGameManager::Instance().GetCurGameRoom();
				CNetManager::Instance().EnterRoom( room->roomid );
			}
			else
			{
				cocos2d::CCDirector::sharedDirector()->replaceScene( CTetrisScene::scene() );
			}
		}
	}

private:
	s32 m_Result;
};

#endif