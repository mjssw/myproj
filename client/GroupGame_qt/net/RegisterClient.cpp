#include "RegisterClient.h"
#include "CommDef.h"
#include "net/NetManager.h"
#include "gamemsg/MsgLoginModule.h"
#include "login.pb.h"
#include "msgid.pb.h"
using namespace SGLib;
using namespace std;

CRegisterClient::CRegisterClient(s32 id) : 
	CProtoClientBase( id )
{
}

int CRegisterClient::OnRecv(char *buf, int len)
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
		case sglib::msgid::LC_USER_REGISTER_RSP:
			{
				sglib::loginproto::SCUserRegisterRsp msg;
				if( msg.ParseFromArray(pbuf+MSG_ID_LEN+MSG_HEAD_LEN, pkgLen-MSG_ID_LEN-MSG_HEAD_LEN) )
				{
					_UserRegisterRsp( msg );
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

void CRegisterClient::OnConnect()
{
	CNetManager::Instance().SetRegClientInstance( this );

	CMsgBase *msg = new CMsgConnectRegister();
	if( !msg )
	{
		CCLog( "[CRegisterClient::OnConnect] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( msg );
}

void CRegisterClient::OnClose()
{
}

void CRegisterClient::OnConnectError()
{
}

void CRegisterClient::Register(const string &user, const string &pwd)
{
	sglib::loginproto::CSUserRegisterReq req;
	req.set_user( user );
	req.set_password( pwd );
	// TODO
	req.set_sex( E_Sex_Male );
    req.set_name( "player" );
	req.set_head( "#0" );

	SendMsg( req, sglib::msgid::CL_USER_REGISTER_REQ );
}

void CRegisterClient::_UserRegisterRsp(sglib::loginproto::SCUserRegisterRsp &msg)
{
	CMsgBase *_msg = new CMsgResiterRsp( msg.result() );
	if( !_msg )
	{
		CCLog( "[CRegisterClient::_UserRegisterRsp] new msg failed" );
		return;
	}

	CNetManager::Instance().PushMessage( _msg );
}
