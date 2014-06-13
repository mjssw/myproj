#ifndef _MSG_LOGIN_RSP_H_
#define _MSG_LOGIN_RSP_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "GameList.h"
#include "NetClient.h"

class CMsgLoginRsp : public CMsgBase
{
public:
	CMsgLoginRsp(s32 ret) : m_Result(ret)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgLoginRsp" );
		if( m_Result == sglib::errorcode::E_ErrorCode_Success )
		{
			cocos2d::CCDirector::sharedDirector()->replaceScene( CGameList::scene() );
		}
	}

private:
	s32 m_Result;
};

#endif
