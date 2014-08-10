#include "cocos2d.h"
#include "LoginClient.h"
#include "CommDef.h"
#include "gamemsg/MsgLoginModule.h"
#include "net/NetManager.h"
#include "user/GameInfo.h"
#include "login.pb.h"
#include "msgid.pb.h"
#include <vector>
using namespace SGLib;
using namespace cocos2d;
using namespace std;

CLoginClient::CLoginClient(s32 id) : 
	CProtoClientBase( id )
{
}

int CLoginClient::OnRecv(char *buf, int len)
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
		switch( _id )
		{
		case sglib::msgid::LC_USER_LOGIN_RSP:
			{
				sglib::loginproto::SCUserLoginRsp msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_UserLoginRsp( msg );
				}
			}
			break;
		case sglib::msgid::LC_GAME_INFO_NOTIFY:
			{
				sglib::loginproto::SCGameInfoNotify msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_GameInfoNotify( msg );
				}
			}
			break;
		case sglib::msgid::LC_GROUP_GATE_NOTIFY:
			{
				sglib::loginproto::SCGroupGateNotify msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_GroupGateNotify( msg );
				}
			}
			break;
		case sglib::msgid::LC_USER_BASIC_INFO_NOTIFY:
			{
				sglib::loginproto::SCUserBasicInfoNotify msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_UserBasicInfoNotify( msg );
				}
			}
			break;
		default:
			break;
		}

		procLen = (pkgLen);
		pbuf += procLen;
		len -= procLen;
		totalProc += procLen;
	}

	return totalProc;
}

void CLoginClient::OnConnect()
{
    CCLog( "[CLoginClient::OnConnect] success" );
	CNetManager::Instance().SetLoginClientInstance( this );

	CMsgBase *msg = new CMsgConnectLogin();
	if( !msg )
	{
		CCLog( "[CLoginClient::OnConnect] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( msg );
}

void CLoginClient::OnClose()
{
    CCLog( "[CLoginClient::OnClose] success" );
	CMsgBase *msg = new CMsgCloseLogin();
	if( !msg )
	{
		CCLog( "[CLoginClient::OnClose] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( msg );
}

void CLoginClient::OnConnectError()
{
    CCLog( "[CLoginClient::OnConnectError] success" );
	CMsgBase *msg = new CMsgConnectLoginError();
	if( !msg )
	{
		CCLog( "[CLoginClient::OnConnectError] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( msg );
}

void CLoginClient::Login(const std::string &user, const std::string &pwd)
{
	sglib::loginproto::CSUserLoginReq req;
	req.set_user( user );
	req.set_password( pwd );
	req.set_flag( "abc" );

	SendMsg( req, sglib::msgid::CL_USER_LOGIN_REQ );
}

void CLoginClient::_UserLoginRsp(sglib::loginproto::SCUserLoginRsp &msg)
{
	CMsgBase *_msg = new CMsgLoginResult( msg.result(), msg.token() );
	if( !_msg )
	{
		CCLog( "[CLoginClient::_UserLoginRsp] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( _msg );
}

void CLoginClient::_GameInfoNotify(sglib::loginproto::SCGameInfoNotify &msg)
{
	vector<CGameInfo> games;
	for( int i=0; i<msg.games_size(); ++i )
	{
		games.push_back( CGameInfo( msg.games(i).gameid(), msg.games(i).cur_count() ) );
	}

	CMsgBase *_msg = new CMsgGameInfoNotify( games );
	if( !_msg )
	{
		CCLog( "[CLoginClient::_GameInfoNotify] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( _msg );
}

void CLoginClient::_GroupGateNotify(sglib::loginproto::SCGroupGateNotify &msg)
{
	CMsgBase *_msg = new CMsgGroupGateNotify( msg.ip(), msg.port() );
	if( !_msg )
	{
		CCLog( "[CLoginClient::_GroupGateNotify] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( _msg );
}

void CLoginClient::_UserBasicInfoNotify(sglib::loginproto::SCUserBasicInfoNotify &msg)
{
	CMsgBase *_msg = new CMsgUserBasicInfoNotify(
		msg.name(), msg.head(), msg.sex(), msg.exp(), msg.level(), msg.gold(), msg.diamond() );
	if( !_msg )
	{
		CCLog( "[CLoginClient::_UserBasicInfoNotify] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( _msg );
}
