#ifndef _MSG_LEAVE_ROOM_RSP_H_
#define _MSG_LEAVE_ROOM_RSP_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "TetrisScene.h"
#include "NetClient.h"
#include "GameManager.h"

class CMsgLeaveRoomRsp : public CMsgBase
{
public:
	CMsgLeaveRoomRsp(s32 ret) : m_Result(ret)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgLeaveRoomRsp" );
		if( m_Result == sglib::errorcode::E_ErrorCode_Success )
		{
			CGameManager::Instance().DelEnemy();
			CNetManager::Instance().PauseProcMsg();
			cocos2d::CCDirector::sharedDirector()->replaceScene( CTetrisScene::scene() );
		}
		else
		{
		}
	}

private:
	s32 m_Result;
};

#endif
