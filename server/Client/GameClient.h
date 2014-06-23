#ifndef _GAME_CLIENT_H_
#define _GAME_CLIENT_H_

#include <stdio.h>
#include "ClientLib.h"
#include "timer.h"
using namespace SGLib;

#include <string>
#include <vector>

#include "login.pb.h"
#include "client.pb.h";
#include "msgid.pb.h"
#include "common.pb.h"
#include "errno.pb.h"
#include "tetris.pb.h"
#include "group.pb.h"


static void SplitCmd(const char *cmd, std::vector<std::string> &vecCmd)
{
	const char *ptr = cmd;
	while(ptr)
	{
		const char *pos = strchr(ptr, ' ');
		if( pos == NULL )
		{
			vecCmd.push_back(ptr);
			break;
		}

		vecCmd.push_back( string(ptr, pos) );
		ptr = pos + 1;
	}
}

class ctCLient : public CClientBase
{
public:
	enum
	{
		MSG_ID_LEN = sizeof(s32),
		MSG_HEAD_LEN = MSG_ID_LEN,
		MAX_MSG_LEN = 4096,
	};

    ctCLient(int id) : CClientBase(id), m_timer()
    {
		m_timer.Start();
		m_bHaveGroupInfo = false;
    }
	CTimer<1> m_timer;
	std::string m_User;
	std::string m_gameIp;
	s32 m_gamePort;
	std::string m_groupIp;
	s32 m_groupPort;
	bool m_bHaveGroupInfo;
	std::string m_token;
	bool m_bLoginResult;

	virtual int OnRecv(char *buf, int len)
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
			case sglib::msgid::LC_GROUP_GATE_NOTIFY:
				{
					sglib::loginproto::SCGroupGateNotify ntf;
					if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_GroupGateNotify(ntf);
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
			case sglib::msgid::SC_GROUP_LOGIN_RSP:
				{
					sglib::groupproto::SCGroupUserLoginRsp rsp;
					if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_LoginGroupResult(rsp);
					}
				}break;
			case sglib::msgid::SC_GROUP_CREATE_RSP:
				{
					sglib::groupproto::SCGroupCreateRsp rsp;
					if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_CreateGroupRsp(rsp);
					}
				}break;
			case sglib::msgid::SC_GROUP_ADD_MEMBER_RSP:
				{
					sglib::groupproto::SCGroupAddMemberRsp rsp;
					if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_AddMemberRsp(rsp);
					}
				}break;
			case sglib::msgid::SC_GROUP_ASK_JOIN_NTF:
				{
					sglib::groupproto::SCGroupAskJoinNtf ntf;
					if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_AskJoinGroupNtf(ntf);
					}
				}break;
			case sglib::msgid::SC_GROUP_MEMBER_JOIN_NTF:
				{
					sglib::groupproto::SCGroupMemberJoinNtf ntf;
					if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_MemberJoinGroupNtf(ntf);
					}
				}break;
			case sglib::msgid::SC_GROUP_MESSAGE_NTF:
				{
					sglib::groupproto::SCGroupMessageNtf ntf;
					if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_GroupMessageNtf(ntf);
					}
				}break;
			case sglib::msgid::SC_GROUP_INFO_UPDATE:
				{
					sglib::groupproto::SCGroupInfoUpdate up;
					if( up.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_GroupInfoUpdate(up);
					}
				}break;
			case sglib::msgid::SC_GROUP_LIST_UPDATE:
				{
					sglib::groupproto::SCGroupListUpdate up;
					if( up.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_GroupListUpdate(up);
					}
				}break;
			case sglib::msgid::SC_GROUP_LEAVE_RSP:
				{
					sglib::groupproto::SCGroupLeaveRsp rsp;
					if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_LeaveGroupRsp(rsp);
					}
				}break;
			case sglib::msgid::SC_GROUP_MEMBER_LEAVE_NTF:
				{
					sglib::groupproto::SCGroupMemberLeaveNtf ntf;
					if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_MemberLeaveGroupNtf(ntf);
					}
				}break;
			case sglib::msgid::SC_GROUP_DELETE_RSP:
				{
					sglib::groupproto::SCGroupDeleteRsp rsp;
					if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_DeleteGroupRsp(rsp);
					}
				}break;
			case sglib::msgid::SC_GROUP_DELETE_NTF:
				{
					sglib::groupproto::SCGroupDeleteNtf ntf;
					if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_DeleteGroupNtf(ntf);
					}
				}break;
			case sglib::msgid::SC_GROUP_MEMBER_ONLINE:
				{
					sglib::groupproto::SCGroupMemberOnlineNtf ntf;
					if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						printf( "GROUP:%llu\t%s online now!\n", ntf.groupid(), ntf.user().c_str() );
					}
				}break;
			case sglib::msgid::SC_GROUP_MEMBER_OFFLINE:
				{
					sglib::groupproto::SCGroupMemberOfflineNtf ntf;
					if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						printf( "GROUP:%llu\t%s offline now!\n", ntf.groupid(), ntf.user().c_str() );
					}
				}break;
			case sglib::msgid::SC_GROUP_CREATE_GAMEROOM_RSP:
				{
					sglib::groupproto::SCGroupCreateGameRoomRsp rsp;
					if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_GroupCreateGameRoomResult( rsp );
					}
				}break;
			case sglib::msgid::SC_GROUP_CREATE_GAMEROOM_NTF:
				{
					sglib::groupproto::SCGroupCreateGameRoomNtf ntf;
					if( ntf.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_GroupMemberCreateGameRoomNtf( ntf );
					}
				}break;
			case sglib::msgid::LC_USER_REGISTER_RSP:
				{
					sglib::loginproto::SCUserRegisterRsp rsp;
					if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						printf( "register result:%d\n", rsp.result() );
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

	virtual void OnConnect()
	{
		/*
		g_mapClient[GetSocket()] = this;
		//g_session->m_client = this;
		printf("sock %d connect\n", GetSocket());

		s64 thisAddr = (s64)this;
		//m_timer.AddTimer(1000, TimerCallBack, &thisAddr, sizeof(s64), true);

		//*/
	}

	virtual void OnClose()
	{
		/*
		map<int, ctCLient*>::iterator it = g_mapClient.find(GetSocket());
		if( it != g_mapClient.end() )
		{
			g_mapClient.erase( it );
		}
		//*/

		m_timer.Stop();
		printf("close");
	}

	static void TimerCallBack(void *pData, s32 nDataLen)
	{
		s64 addrThis = *((s64*)pData);
		ctCLient *pThis = (ctCLient*)(addrThis);
	}

	void UserLogin(const string &user, const string &pwd, const string &flag)
	{
		sglib::loginproto::CSUserLoginReq req;
		req.set_user(user);
		req.set_password(pwd);
		req.set_flag( flag );
		SendMsg( req, sglib::msgid::CL_USER_LOGIN_REQ );
		m_User = user;
	}

	void UserEnterGame(int gameid)
	{
		sglib::commonproto::CSEnterGameReq req;
		req.set_gameid( gameid );
		SendMsg( req, sglib::msgid::CS_USER_ENTER_GAME_REQ );
	}

	void LoginGame(const std::string &userId)
	{
		sglib::commonproto::CSLoginGameReq req;
		req.set_userid(userId);
		req.set_token("abc");
		SendMsg( req, sglib::msgid::CS_LOGIN_GAME_REQ );
	}

	void EnterRoom(s32 roomId)
	{
		sglib::commonproto::CSEnterRoomReq req;
		req.set_roomid(roomId);
		SendMsg( req, sglib::msgid::CS_ENTER_ROOM_REQ );
	}

	void LeaveRoom()
	{
		sglib::commonproto::CSLeaveRoomReq req;
		SendMsg( req, sglib::msgid::CS_LEAVE_ROOM_REQ );
	}

	void Ready()
	{
		sglib::tetrisproto::CSMsgReadyReq req;
		SendMsg( req, sglib::msgid::CS_TETRIS_READY_REQ );
	}

	void Lost()
	{
		sglib::tetrisproto::CSMsgGameLostReq req;
		SendMsg( req, sglib::msgid::CS_TETRIS_GAMELOST_REQ );
	}

	void PutBlock(s32 blockid, s32 column)
	{
		sglib::tetrisproto::CSMsgPutBlockReq req;
		req.set_blockid( blockid );
		req.set_column( column );
		SendMsg( req, sglib::msgid::CS_TETRIS_PUT_BLOCK_REQ );
	}

	void ClearBlock(vector<s32> &vecRows)
	{
		sglib::tetrisproto::CSMsgClearBlockReq req;
		vector<s32>::iterator it = vecRows.begin();
		for( ; it != vecRows.end(); ++it )
		{
			req.add_rows( *it );
		}
		SendMsg( req, sglib::msgid::CS_TETRIS_CLEAR_BLOCK_REQ );
	}

	void Register(const string &user, const string &pwd)
	{
		sglib::loginproto::CSUserRegisterReq req;
		req.set_user( user );
		req.set_password( pwd );
		SendMsg( req, sglib::msgid::CL_USER_REGISTER_REQ );
	}

	// ====================== group ==================================

	void LoginGroup(const std::string &userId, const std::string &token)
	{
		printf( "%s will login group\n", userId.c_str() );
		m_User = userId;
		sglib::groupproto::CSGroupUserLoginReq req;
		req.set_user( userId );
		req.set_token( token );
		//req.set_password( "xxx" );
		SendMsg( req, sglib::msgid::CS_GROUP_LOGIN_REQ );
	}

	void CreateGroup(const std::string &gpname)
	{
		printf( "%s create group %s\n", m_User.c_str(), gpname.c_str() );
		sglib::groupproto::CSGroupCreateReq req;
		req.set_name( gpname );
		SendMsg( req, sglib::msgid::CS_GROUP_CREATE_REQ );
	}
	
	void AddMemberToGroup(std::vector< std::string > &member)
	{
		printf( "%s add member to group : %s\n", m_User.c_str(), member[1].c_str() );
		sglib::groupproto::CSGroupAddMemberReq req;
		req.set_groupid( atoi(member[1].c_str()) );
		for( int i=2; i<member.size(); ++i )
		{
			std::string *str = req.add_users();
			*str = member[i];
		}
		SendMsg( req, sglib::msgid::CS_GROUP_ADD_MEMBER_REQ );
	}

	void AgreeJoinGroup(std::string &groupid)
	{
		printf( "%s agree join group : %s\n", m_User.c_str(), groupid.c_str() );
		sglib::groupproto::CSGroupAgreeJoinReq req;
		req.set_groupid( atoi(groupid.c_str()) );
		SendMsg( req, sglib::msgid::CS_GROUP_AGREE_JOIN_REQ );
	}

	void GroupMessage(std::string &groupid, std::string &msg)
	{
		sglib::groupproto::CSGroupMessageReq req;
		req.set_groupid( atoi(groupid.c_str()) );
		req.set_content( msg );
		SendMsg( req, sglib::msgid::CS_GROUP_MESSAGE_REQ );
	}

	void LeaveGroup(std::string &groupid)
	{
		sglib::groupproto::CSGroupLeaveReq req;
		req.set_groupid( atoi(groupid.c_str()) );
		SendMsg( req, sglib::msgid::CS_GROUP_LEAVE_REQ );
	}

	void DeleteGroup(std::string &groupid)
	{
		sglib::groupproto::CSGroupDeleteReq req;
		req.set_groupid( atoi(groupid.c_str()) );
		SendMsg( req, sglib::msgid::CS_GROUP_DELETE_REQ );
	}

	void AskCreateGameRoom(std::string &groupid, std::string &game)
	{
		sglib::groupproto::CSGroupCreateGameRoomReq req;
		req.set_groupid( atoi(groupid.c_str()) );
		req.set_game( atoi(game.c_str()) );
		SendMsg( req, sglib::msgid::CS_GROUP_CREATE_GAMEROOM_REQ );
	}

private:
	void _UserLoginRsp(const sglib::loginproto::SCUserLoginRsp &rsp)
	{
		printf("login rsp:%d %s\n", rsp.result(), rsp.token().c_str());
		m_token = rsp.token();
		m_bLoginResult = (rsp.result() == 0 );
	}

	void _GameInfoNotify(const sglib::loginproto::SCGameInfoNotify &ntf)
	{
		printf("game info notify, size=%d\n", ntf.games_size() );
		for(int i=0; i<ntf.games_size(); ++i )
		{
			const sglib::publicproto::GameInfo &info = ntf.games(i);
			printf("\tGAME:%d online=%d\n", info.gameid(), info.cur_count());
		}
	}

	void _GroupGateNotify(const sglib::loginproto::SCGroupGateNotify &ntf)
	{
		printf( "group info %s:%d\n", ntf.ip().c_str(), ntf.port() );
		m_groupIp = ntf.ip();
		m_groupPort = ntf.port();
		m_bHaveGroupInfo = true;
	}

	void _EnterGameRsp(const sglib::commonproto::SCEnterGameRsp &rsp)
	{
		printf( "enter game:%d result=%d, [ip,port]=[%s,%d]\n",
			rsp.gameid(), rsp.result(),
			rsp.ip().c_str(), rsp.port() );
		if( rsp.result() == sglib::errorcode::E_ErrorCode_Success )
		{
			m_gameIp = rsp.ip();
			m_gamePort = rsp.port();
		}
	}

	void _LoginGameRsp(const sglib::commonproto::SCLoginGameRsp &rsp)
	{
		printf( "login game result=%d\n",
			rsp.result() );
	}

	void _EnterRoomRsp(const sglib::commonproto::SCEnterRoomRsp &rsp)
	{
		printf( "enter room result=%d, room=%d\n",
			rsp.result(), rsp.roomid() );
	}

	void _LeaveRoomRsp(const sglib::commonproto::SCLeaveRoomRsp &rsp)
	{
		printf( "leave room result=%d\n",
			rsp.result() );
	}

	void _EnterRoomNtf(const sglib::commonproto::SCEnterRoomNtf &ntf)
	{
		printf( "player:%s enter room\n",
			ntf.user().c_str() );
	}

	void _LeaveRoomNtf(const sglib::commonproto::SCLeaveRoomNtf &ntf)
	{
		printf( "player:%s leave room\n",
			ntf.user().c_str() );
	}

	void _ReadyRsp(const sglib::tetrisproto::SCMsgReadyRsp &rsp)
	{
		printf( "ready result=%d\n", rsp.result() );
	}

	void _GameStart(const sglib::tetrisproto::SCMsgGameStartNtf &ntf)
	{
		printf( "game start\n");
	}

	void _GameEnd(const sglib::tetrisproto::SCMsgGameEndNtf &ntf)
	{
		printf( "game end, loser=%s\n", ntf.loser().c_str());
	}

	void _EnemyReady(const string &enemy)
	{
		printf( "enemy=%s ready\n", enemy.c_str());
	}

	void _RoomInfoNotify(const sglib::tetrisproto::SCMsgRoomInfoNtf &ntf)
	{
		printf("room info:\n");
		for( s32 i=0; i<ntf.players_size(); ++i )
		{
			printf( "\t%s\t%d\n", 
				ntf.players(i).user().c_str(), 
				ntf.players(i).isready() );
		}
	}

	void _EnemyPutBlockNotify(const sglib::tetrisproto::SCMsgPutBlockNtf &ntf)
	{
		printf(" enemy put block:%d at:%d\n",
			ntf.blockid(),
			ntf.column() );
	}

	void _EnemyClearBlockNotify(const sglib::tetrisproto::SCMsgClearBlockNtf &ntf)
	{
		string clearstr = "";
		for( s32 i=0; i<ntf.rows_size(); ++i )
		{
			char str[32] = {0};
			sprintf( str, "%d,", ntf.rows(i) );
			clearstr += str;
		}
		printf( "enemy clear row:%s\n", clearstr.c_str() );
	}

	void _LoginGroupResult(const sglib::groupproto::SCGroupUserLoginRsp &rsp)
	{
		printf("%s login group result:%d\n",
			m_User.c_str(), rsp.result() );
	}

	void _CreateGroupRsp(const sglib::groupproto::SCGroupCreateRsp &rsp)
	{
		printf( "%s create group:%s result:%d groupid:%llu\n",
			m_User.c_str(), rsp.name().c_str(), rsp.result(), rsp.groupid() );
	}

	void _AddMemberRsp(const sglib::groupproto::SCGroupAddMemberRsp &rsp)
	{
		printf( "%s add member to group:%llu rsp: %d\n",
			m_User.c_str(), rsp.groupid(), rsp.result() );
	}

	void _AskJoinGroupNtf(const sglib::groupproto::SCGroupAskJoinNtf &ntf)
	{
		printf( "Are you agree join group:%llu name:%s icon:%s count:%d\n",
			ntf.group().id(),
			ntf.group().name().c_str(),
			ntf.group().icon().c_str(),
			ntf.group().count()
		);
	}

	void _MemberJoinGroupNtf(const sglib::groupproto::SCGroupMemberJoinNtf &ntf)
	{
		printf( "Welcome new member join group:%llu\n\tUser:%s\n\tNickName:%s\n\tHead:%s\n",
			ntf.groupid(),
			ntf.member().user().c_str(),
			ntf.member().nickname().c_str(),
			ntf.member().head().c_str() );
	}

	void _GroupMessageNtf(const sglib::groupproto::SCGroupMessageNtf &ntf)
	{
		printf( "%s says:%s in group:%llu\n",
			ntf.sender().c_str(),
			ntf.content().c_str(),
			ntf.groupid() );
	}

	void _GroupInfoUpdate(const sglib::groupproto::SCGroupInfoUpdate &up)
	{
		printf( "====== Group:%llu Info Update =======\n", up.id() );
		for( int i=0; i<up.members_size(); ++i )
		{
			printf( "Member user:%s\tnickname:%s\thead:%s\tonline:%d\n",
				up.members(i).user().c_str(),
				up.members(i).nickname().c_str(),
				up.members(i).head().c_str(),
				up.members(i).online() );
		}
	}

	void _GroupListUpdate(const sglib::groupproto::SCGroupListUpdate &up)
	{
		printf( "====== Group Update =========\n");
		for( int i=0; i<up.groups_size(); ++i )
		{
			printf( "Group Id:%llu\tName:%s\tCount:%d\n",
				up.groups(i).id(),
				up.groups(i).name().c_str(),
				up.groups(i).count() );
		}
	}

	void _LeaveGroupRsp(const sglib::groupproto::SCGroupLeaveRsp &rsp)
	{
		printf( "%s leave group:%llu result:%d\n",
			m_User.c_str(),
			rsp.groupid(), rsp.result() );
	}

	void _MemberLeaveGroupNtf(const sglib::groupproto::SCGroupMemberLeaveNtf &ntf)
	{
		printf( "member:%s leave group:%llu\n",
			ntf.user().c_str(), ntf.groupid() );
	}

	void _DeleteGroupRsp(const sglib::groupproto::SCGroupDeleteRsp &rsp)
	{
		printf( "%s delete group:%llu result:%d\n",
			m_User.c_str(),
			rsp.groupid(), rsp.result() );
	}

	void _DeleteGroupNtf(const sglib::groupproto::SCGroupDeleteNtf &ntf)
	{
		printf( "group:%llu delete\n", ntf.groupid() );
	}

	void _GroupCreateGameRoomResult( const sglib::groupproto::SCGroupCreateGameRoomRsp &rsp)
	{
		printf( "create game room in group result:%d\n\tGroup:%llu\tGame:%d\tRoom:%d\tIp:%s\tPort:%d\n", 
			rsp.result(),
			rsp.groupid(), rsp.game(), rsp.roomid(), rsp.ip().c_str(), rsp.port() );
	}

	void _GroupMemberCreateGameRoomNtf(const sglib::groupproto::SCGroupCreateGameRoomNtf &ntf)
	{
		printf( "group member:%s create game room in group:%llu\n\tGame:%d\tRoom:%d\tIp:%s\tPort:%d\n", 
			ntf.creater().c_str(), ntf.groupid(), 
			ntf.game(), ntf.roomid(), ntf.ip().c_str(), ntf.port() );
	}

private:
	void SendMsg(const ::google::protobuf::Message &Msg, s32 nMsgId)
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
};

#endif
