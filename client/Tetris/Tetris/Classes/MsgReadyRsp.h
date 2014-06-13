#ifndef _MSG_READY_RSP_H_
#define _MSG_READY_RSP_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "TetrisScene.h"
#include "NetClient.h"
#include "GameManager.h"

class CMsgReadyRsp: public CMsgBase
{
public:
	CMsgReadyRsp(s32 ret) : m_Result(ret)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgReadyRsp" );
		if( m_Result == sglib::errorcode::E_ErrorCode_Success )
		{
			CGameManager::Instance().PlayerReady();
		}
		else
		{
		}
	}

private:
	s32 m_Result;
};

#endif