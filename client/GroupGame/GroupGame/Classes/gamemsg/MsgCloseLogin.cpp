#include "cocos2d.h"
#include "MsgCloseLogin.h"
#include "net/NetManager.h"

CMsgCloseLogin::CMsgCloseLogin()
{
}

void CMsgCloseLogin::Process()
{
	cocos2d::CCLog( "[CMsgCloseLogin::Process]" );
	CNetManager::Instance().CloseLoginConn();
}
