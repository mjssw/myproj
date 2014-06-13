#include "NetClient.h"
#include "NetManager.h"
#include "GameManager.h"
#include "MsgConnect.h"
#include "MsgLoginRsp.h"
#include "MsgGameInfo.h"
#include "MsgConnectError.h"
#include "MsgEnterGameRsp.h"
#include "MsgLoginGameRsp.h"
#include "MsgEnterRoomRsp.h"
#include "MsgEnterRoomNtf.h"
#include "MsgLeaveRoomNtf.h"
#include "MsgLeaveRoomRsp.h"
#include "MsgRoomInfoNtf.h"
#include "MsgReadyRsp.h"
#include "MsgEnemyReady.h"
#include "MsgGameStart.h"
#include "MsgGameEnd.h"
#include "MsgEnemyPutBlockNtf.h"
#include "MsgEnemyClearBlockNtf.h"

int CNetClient::OnRecv(char *buf, int len)
{
	int totalProc = 0;
	char *pbuf = buf;
	while(true)
	{
		int procLen = 0;
		if( len < sizeof(int) )
		{
			return totalProc;
		}

		int pkgLen = *((int*)pbuf);
		if( len < pkgLen )
		{
			return totalProc;
		}

		int _id = *((int*)(pbuf+sizeof(int)));
		switch(_id)
		{
		case sglib::msgid::LC_USER_LOGIN_RSP:
			{
				sglib::loginproto::SCUserLoginRsp rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_UserLoginRsp(rsp);
				}
			}
			break;
		case sglib::msgid::LC_GAME_INFO_NOTIFY:
			{
				sglib::loginproto::SCGameInfoNotify ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_GameInfoNotify(ntf);
				}
			}
			break;
		case sglib::msgid::SC_USER_ENTER_GAME_RSP:
			{
				sglib::commonproto::SCEnterGameRsp rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_EnterGameRsp(rsp);
				}
			}
			break;
		case sglib::msgid::SC_LOGIN_GAME_RSP:
			{
				sglib::commonproto::SCLoginGameRsp rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_LoginGameRsp(rsp);
				}
			}
			break;
		case sglib::msgid::SC_ENTER_ROOM_RSP:
			{
				sglib::commonproto::SCEnterRoomRsp rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_EnterRoomRsp(rsp);
				}
			}
			break;
		case sglib::msgid::SC_ENTER_ROOM_NTF:
			{
				sglib::commonproto::SCEnterRoomNtf rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_EnterRoomNtf(rsp);
				}
			}
			break;
		case sglib::msgid::SC_LEAVE_ROOM_RSP:
			{
				sglib::commonproto::SCLeaveRoomRsp rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_LeaveRoomRsp(rsp);
				}
			}
			break;
		case sglib::msgid::SC_LEAVE_ROOM_NTF:
			{
				sglib::commonproto::SCLeaveRoomNtf rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_LeaveRoomNtf(rsp);
				}
			}
			break;
		case sglib::msgid::SC_TETRIS_READY_RSP:
			{
				sglib::tetrisproto::SCMsgReadyRsp rsp;
				if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_ReadyRsp(rsp);
				}
			}
			break;
		case sglib::msgid::SC_TETRIS_GAMESTART_NOTIFY:
			{
				sglib::tetrisproto::SCMsgGameStartNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_GameStart(ntf);
				}
			}break;
		case sglib::msgid::SC_TETRIS_GAMEEND_NOTIFY:
			{
				sglib::tetrisproto::SCMsgGameEndNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_GameEnd(ntf);
				}
			}break;
		case sglib::msgid::SC_TETRIS_READY_NOTIFY:
			{
				sglib::tetrisproto::SCMsgReadyNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_EnemyReady(ntf.player());
				}
			}break;
		case sglib::msgid::SC_TETRIS_ROOM_INFO_NOTIFY:
			{
				sglib::tetrisproto::SCMsgRoomInfoNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_RoomInfoNotify(ntf);
				}
			}break;
		case sglib::msgid::SC_TETRIS_PUT_BLOCK_NOTIFY:
			{
				sglib::tetrisproto::SCMsgPutBlockNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_EnemyPutBlockNotify(ntf);
				}
			}break;
		case sglib::msgid::SC_TETRIS_CLEAR_BLOCK_NOTIFY:
			{
				sglib::tetrisproto::SCMsgClearBlockNtf ntf;
				if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
				{
					_EnemyClearBlockNotify(ntf);
				}
			}break;
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

void CNetClient::OnConnect()
{
	CNetManager::Instance().PushMessage( new CMsgConnect(this) );
}

void CNetClient::OnClose()
{
	printf("close\n");
}

void CNetClient::OnConnectError()
{
	CNetManager::Instance().PushMessage( new CMsgConnectError() );
}

void CNetClient::UserLogin(const std::string &user)
{
	sglib::loginproto::CSUserLoginReq req;
	req.set_user( user );
	req.set_password("123");
	SendMsg( req, sglib::msgid::CL_USER_LOGIN_REQ );
}

void CNetClient::UserEnterGame(int gameid)
{
	sglib::commonproto::CSEnterGameReq req;
	req.set_gameid( gameid );
	SendMsg( req, sglib::msgid::CS_USER_ENTER_GAME_REQ );
}

void CNetClient::LoginGame(const std::string &userId)
{
	sglib::commonproto::CSLoginGameReq req;
	req.set_userid(userId);
	req.set_token("abc");
	SendMsg( req, sglib::msgid::CS_LOGIN_GAME_REQ );
}

void CNetClient::EnterRoom(s32 roomId)
{
	sglib::commonproto::CSEnterRoomReq req;
	req.set_roomid(roomId);
	SendMsg( req, sglib::msgid::CS_ENTER_ROOM_REQ );
}

void CNetClient::LeaveRoom()
{
	sglib::commonproto::CSLeaveRoomReq req;
	SendMsg( req, sglib::msgid::CS_LEAVE_ROOM_REQ );
}

void CNetClient::Ready()
{
	sglib::tetrisproto::CSMsgReadyReq req;
	SendMsg( req, sglib::msgid::CS_TETRIS_READY_REQ );
}

void CNetClient::Lost()
{
	sglib::tetrisproto::CSMsgGameLostReq req;
	SendMsg( req, sglib::msgid::CS_TETRIS_GAMELOST_REQ );
}

void CNetClient::PutBlock(s32 blockid, s32 column)
{
	sglib::tetrisproto::CSMsgPutBlockReq req;
	req.set_blockid( blockid );
	req.set_column( column );
	SendMsg( req, sglib::msgid::CS_TETRIS_PUT_BLOCK_REQ );
}

void CNetClient::ClearBlock(vector<s32> &vecRows)
{
	sglib::tetrisproto::CSMsgClearBlockReq req;
	vector<s32>::iterator it = vecRows.begin();
	for( ; it != vecRows.end(); ++it )
	{
		req.add_rows( *it );
	}
	SendMsg( req, sglib::msgid::CS_TETRIS_CLEAR_BLOCK_REQ );
}

void CNetClient::_UserLoginRsp(const sglib::loginproto::SCUserLoginRsp &rsp)
{
	cocos2d::CCLog( "---> CMsgLoginRsp" );
	CNetManager::Instance().PushMessage( new CMsgLoginRsp(rsp.result()) );
}

void CNetClient::_GameInfoNotify(const sglib::loginproto::SCGameInfoNotify &ntf)
{
	cocos2d::CCLog( "---> CMsgGameInfo" );
	int count = 0;
	CGameInfo ** gameInfo = new CGameInfo*[ ntf.games_size() ];
	for(int i=0; i<ntf.games_size(); ++i )
	{
		const sglib::publicproto::GameInfo &info = ntf.games(i);
		//printf("\tGAME:%d online=%d\n", info.gameid(), info.count());
		CGameInfo *game = new CGameInfo();
		game->m_Id = info.gameid();
		game->m_Count = info.cur_count();
		gameInfo[ count++ ] = game;
	}

	CNetManager::Instance().PushMessage( new CMsgGameInfo(gameInfo, count) );
}

void CNetClient::_EnterGameRsp(const sglib::commonproto::SCEnterGameRsp &rsp)
{
	cocos2d::CCLog( "---> CMsgEnterGameRsp" );
	if( rsp.result() == sglib::errorcode::E_ErrorCode_Success )
	{
		CNetManager::Instance().PushMessage( new CMsgEnterGameRsp(rsp.ip().c_str(), rsp.port()) );
	}
}

void CNetClient::_LoginGameRsp(const sglib::commonproto::SCLoginGameRsp &rsp)
{
	cocos2d::CCLog( "---> CMsgLoginGameRsp" );
	if( rsp.result() == sglib::errorcode::E_ErrorCode_Success )
	{
		CNetManager::Instance().PushMessage( new CMsgLoginGameRsp(rsp.result()) );
	}
}

void CNetClient::_EnterRoomRsp(const sglib::commonproto::SCEnterRoomRsp &rsp)
{
	cocos2d::CCLog( "---> CMsgEnterRoomRsp" );
	CNetManager::Instance().PushMessage( new CMsgEnterRoomRsp(rsp.result()) );
}

void CNetClient::_LeaveRoomRsp(const sglib::commonproto::SCLeaveRoomRsp &rsp)
{
	cocos2d::CCLog( "---> CMsgLeaveRoomRsp" );
	CNetManager::Instance().PushMessage( new CMsgLeaveRoomRsp(rsp.result()) );
}

void CNetClient::_EnterRoomNtf(const sglib::commonproto::SCEnterRoomNtf &ntf)
{
	cocos2d::CCLog( "---> CMsgEnterRoomNtf" );
	CNetManager::Instance().PushMessage( new CMsgEnterRoomNtf(ntf.user()) );
}

void CNetClient::_LeaveRoomNtf(const sglib::commonproto::SCLeaveRoomNtf &ntf)
{
	cocos2d::CCLog( "---> CMsgLeaveRoomNtf" );
	CNetManager::Instance().PushMessage( new CMsgLeaveRoomNtf(ntf.user()) );
}

void CNetClient::_ReadyRsp(const sglib::tetrisproto::SCMsgReadyRsp &rsp)
{
	cocos2d::CCLog( "---> CMsgReadyRsp" );
	CNetManager::Instance().PushMessage( new CMsgReadyRsp(rsp.result()) );
}

void CNetClient::_GameStart(const sglib::tetrisproto::SCMsgGameStartNtf &ntf)
{
	cocos2d::CCLog( "---> CMsgGameStart" );
	CNetManager::Instance().PushMessage( new CMsgGameStart(ntf.blockseed()) );
}

void CNetClient::_GameEnd(const sglib::tetrisproto::SCMsgGameEndNtf &ntf)
{
	cocos2d::CCLog( "---> CMsgGameEnd" );
	CNetManager::Instance().PushMessage( new CMsgGameEnd(ntf.loser()) );
}

void CNetClient::_EnemyReady(const string &enemy)
{
	cocos2d::CCLog( "---> CMsgEnemyReady" );
	CNetManager::Instance().PushMessage( new CMsgEnemyReady(enemy) );
}

void CNetClient::_RoomInfoNotify(const sglib::tetrisproto::SCMsgRoomInfoNtf &ntf)
{
	cocos2d::CCLog( "---> CMsgRoomInfoNtf" );
	int count = 0;
	CPlayerInfo ** playerInfo = new CPlayerInfo*[ ntf.players_size() ];
	for(int i=0; i<ntf.players_size(); ++i )
	{
		const sglib::publicproto::RoomPlayerInfo &info = ntf.players(i);
		CPlayerInfo *player = new CPlayerInfo();
		player->m_User = info.user();
		player->m_bReady = (info.isready()==1);
		playerInfo[ count++ ] = player;
	}

	CNetManager::Instance().PushMessage( new CMsgRoomInfoNtf(count, playerInfo) );
}

void CNetClient::_EnemyPutBlockNotify(const sglib::tetrisproto::SCMsgPutBlockNtf &ntf)
{
	cocos2d::CCLog( "---> CMsgEnemyPutBlockNtf" );
	CNetManager::Instance().PushMessage( new CMsgEnemyPutBlockNtf(
		ntf.blockid(),
		ntf.column()) );
}

void CNetClient::_EnemyClearBlockNotify(const sglib::tetrisproto::SCMsgClearBlockNtf &ntf)
{
	cocos2d::CCLog( "---> CMsgEnemyClearBlockNtf" );
	CNetManager::Instance().PushMessage( new CMsgEnemyClearBlockNtf(
		ntf.rows_size(), (int*)ntf.rows().data() ) );
}

void CNetClient::SendMsg(const ::google::protobuf::Message &Msg, s32 nMsgId)
{
	int total = Msg.ByteSize();
	char buf[MAX_MSG_LEN] = {0};
	*((s32*)(buf)) = total + MSG_ID_LEN + MSG_HEAD_LEN;
	*((s32*)(buf + MSG_HEAD_LEN)) = nMsgId;
	if( Msg.SerializeToArray(buf+MSG_ID_LEN+MSG_HEAD_LEN, MAX_MSG_LEN) )
	{
		if( !Send(buf, total+MSG_ID_LEN+MSG_HEAD_LEN) )
		{
			Fini();
		}
		else
		{
			printf("send msg: [%d] %d\n", GetSocket(), nMsgId);
		}
	}
	else
	{
		printf( "Msg:%d SerializeToArray failed\n", nMsgId );
	}
}
