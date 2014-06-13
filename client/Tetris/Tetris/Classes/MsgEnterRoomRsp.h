#ifndef _MSG_ENTER_ROOM_RSP_H_
#define _MSG_ENTER_ROOM_RSP_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "TetrisRoomScene.h"
#include "NetClient.h"
#include "GameManager.h"

class CMsgEnterRoomRsp : public CMsgBase
{
public:
	CMsgEnterRoomRsp(s32 ret) : m_Result(ret)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgEnterRoomRsp" );
		if( m_Result == sglib::errorcode::E_ErrorCode_Success )
		{
			CNetManager::Instance().PauseProcMsg();
			cocos2d::CCLog( "change to tetris room scene" );
			cocos2d::CCDirector::sharedDirector()->replaceScene( CTetrisRoomScene::scene() );
		}
		else
		{
			CGameManager::Instance().EnterRoomFailed();
		}
	}

private:
	s32 m_Result;
};

#endif