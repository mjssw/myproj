#include "cocos2d.h"
#include "GameClient.h"
#include "CommDef.h"
#include "gamemsg/MsgGameModule.h"
#include "NetManager.h"
using namespace SGLib;
using namespace cocos2d;

CGameClient::CGameClient(s32 id) : 
	CProtoClientBase( id )
{
}

int CGameClient::OnRecv(char *buf, int len)
{
	int totalProc = 0;
	char *pbuf = buf;
	while( true )
	{
		int procLen = 0;
		if( len < MSG_HEAD_LEN )
		{
			return totalProc;
		}

		MsgHeadType pkgLen = *((MsgHeadType*)pbuf);
		if( len < (int)pkgLen )
		{
			return totalProc;
		}

		int _id = *((int*)( pbuf + MSG_HEAD_LEN ));

		CMsgBase *msg = new CMsgGameMessage(_id, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN, pbuf+MSG_ID_LEN+MSG_HEAD_LEN );
		if( msg )
		{
			CNetManager::Instance().PushMessage( msg );
		}
		else
		{
			CCLog( "[ERROR][CMsgGameMessage] new msg failed. msgid=%d", _id );
		}

		procLen = (pkgLen);
		pbuf += procLen;
		len -= procLen;
		totalProc += procLen;
	}

	return totalProc;
}

void CGameClient::OnConnect()
{
	CCLog( "[CGameClient::OnConnect] success" );
	CNetManager::Instance().SetGameClientInstance( this );

	CMsgBase *msg = new CMsgConnectGame();
	if( !msg )
	{
		CCLog( "[CGameClient::OnConnect] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( msg );
}

void CGameClient::OnClose()
{
}

void CGameClient::OnConnectError()
{
}
