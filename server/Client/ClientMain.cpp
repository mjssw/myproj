#include <stdio.h>
#include "ClientLib.h"
#include "timer.h"
using namespace SGLib;

#include "login.pb.h"
#include "client.pb.h";
#include "msgid.pb.h"
#include "common.pb.h"
#include "errno.pb.h"
#include "tetris.pb.h"
#include "group.pb.h"

#include <map>
#include <vector>
#include <string>
using namespace std;

class ctCLient;
class CSessionMgr;
CSessionMgr *g_session;
static map<int, ctCLient*> g_mapClient;

static string g_gameIp;
static s32 g_gamePort;
static bool g_canEnterGame = false;
static string g_userId = "";

void SplitCmd(const char *cmd, vector<string> &vecCmd)
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

class ctCLient;

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
    }
	CTimer<1> m_timer;
	std::string m_User;

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
		g_mapClient[GetSocket()] = this;
		//g_session->m_client = this;
		printf("sock %d connect\n", GetSocket());

		s64 thisAddr = (s64)this;
		//m_timer.AddTimer(1000, TimerCallBack, &thisAddr, sizeof(s64), true);
	}

	virtual void OnClose()
	{
		map<int, ctCLient*>::iterator it = g_mapClient.find(GetSocket());
		if( it != g_mapClient.end() )
		{
			g_mapClient.erase( it );
		}

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

	void LoginGroup(const std::string &userId)
	{
		printf( "%s will login group\n", userId.c_str() );
		m_User = userId;
		sglib::groupproto::CSGroupUserLoginReq req;
		req.set_user( userId );
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

	void _EnterGameRsp(const sglib::commonproto::SCEnterGameRsp &rsp)
	{
		printf( "enter game:%d result=%d, [ip,port]=[%s,%d]\n",
			rsp.gameid(), rsp.result(),
			rsp.ip().c_str(), rsp.port() );
		if( rsp.result() == sglib::errorcode::E_ErrorCode_Success )
		{
			g_canEnterGame = true;
			g_gameIp = rsp.ip();
			g_gamePort = rsp.port();
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

class CSessionMgr
{
public:
	CSessionMgr() : m_client(NULL), m_LoginConn(1), m_GroupConn(1), m_GameConn(1)
	{
	}
	~CSessionMgr()
	{
		m_LoginConn.Stop();
		m_GroupConn.Stop();
		m_GameConn.Stop();
	}
	void Start(s32 loginPort, s32 groupPort)
	{
		//m_LoginConn.Start( "127.0.0.1", loginPort );
		m_GroupConn.Start( "127.0.0.1", groupPort );
	
		while(true)
		{
			char cmd[64] = {0};
			gets(cmd);
			vector<string> vCmd;
			SplitCmd(cmd, vCmd );
			if( vCmd.size() == 0 )
			{
				printf("no cmd here\n");
				continue;
			}

			if( vCmd[0] == "quit" )
			{
				break;
			}
			else if( vCmd[0] == "logingroup" )
			{
				if( vCmd.size() == 2 )
				{
					m_client->LoginGroup( vCmd[1] );
				}
			}
			else if( vCmd[0] == "creategroup" )
			{
				if( vCmd.size() == 2 )
				{
					m_client->CreateGroup( vCmd[1] );
				}
			}
			else if( vCmd[0] == "addgroupmember" )
			{
				if( vCmd.size() >= 3 )
				{
					m_client->AddMemberToGroup( vCmd );
				}
			}
			else if( vCmd[0] == "agree" )
			{
				if( vCmd.size() == 2 )
				{
					m_client->AgreeJoinGroup( vCmd[1] );
				}
			}
			else if( vCmd[0] == "groupmsg" )
			{
				if( vCmd.size() == 3 )
				{
					m_client->GroupMessage( vCmd[1], vCmd[2] );
				}
			}
			else if( vCmd[0] == "leavegroup" )
			{
				if( vCmd.size() == 2 )
				{
					m_client->LeaveGroup( vCmd[1] );
				}
			}
			else if( vCmd[0] == "deletegroup" )
			{
				if( vCmd.size() == 2 )
				{
					m_client->DeleteGroup( vCmd[1] );
				}
			}
			else if( vCmd[0] == "createroom" )
			{
				if( vCmd.size() == 3 )
				{
					m_client->AskCreateGameRoom( vCmd[1], vCmd[2] );
				}
			}
		}
	}
	ctCLient *m_client;
	CClientManager<ctCLient> m_LoginConn;
	CClientManager<ctCLient> m_GroupConn;
	CClientManager<ctCLient> m_GameConn;
};

void ClientLogin(const char *ip, int port, int clientNum)
{
	CClientManager<ctCLient> mgr(clientNum);
	mgr.Start(ip, port);

	while(true)
	{
		char cmd[64] = {0};
		gets(cmd);
		vector<string> vCmd;
		SplitCmd(cmd, vCmd );
		if( vCmd.size() == 0 )
		{
			printf("no cmd here\n");
			continue;
		}

		if( vCmd[0] == "quit" )
		{
			break;
		}
		else if( vCmd[0] == "login" )
		{
			if( vCmd.size() == 4 )
			{
				g_mapClient.begin()->second->UserLogin(vCmd[1], vCmd[2], vCmd[3]);
			}
		}
		else if( vCmd[0] == "entergame" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->UserEnterGame( atoi(vCmd[1].c_str()) );
			}
		}
		else if( vCmd[0] == "logingame" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->LoginGame( vCmd[1] );
			}
		}
		else if( vCmd[0] == "enterroom" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->EnterRoom( atoi(vCmd[1].c_str()) );
			}
		}
		else if( vCmd[0] == "leaveroom" )
		{
			{
				g_mapClient.begin()->second->LeaveRoom( );
			}
		}
		else if( vCmd[0] == "ready" )
		{
			g_mapClient.begin()->second->Ready( );
		}
		else if( vCmd[0] == "lost" )
		{
			g_mapClient.begin()->second->Lost( );
		}
		else if( vCmd[0] == "put" )
		{
			if( vCmd.size() == 3 )
			{
				g_mapClient.begin()->second->PutBlock( 
					atoi( vCmd[1].c_str() ),
					atoi( vCmd[2].c_str() ) );
			}
		}
		else if( vCmd[0] == "clear" )
		{
			if( vCmd.size() > 1 )
			{
				vector<s32> vecRows;
				for( size_t i=1; i<vCmd.size(); ++i )
				{
					vecRows.push_back(
						atoi( vCmd[i].c_str() ) );
				}
				g_mapClient.begin()->second->ClearBlock( vecRows );
			}
		}
		else if( vCmd[0] == "register" )
		{
			if( vCmd.size() == 3 )
			{
				g_mapClient.begin()->second->Register( vCmd[1], vCmd[2] );
			}
		}
	}

	mgr.Stop();
}

void ClientInGame(const char *ip, int port, int clientNum)
{
	CClientManager<ctCLient> mgr(clientNum);
	mgr.Start(ip, port);

	while(true)
	{
		char cmd[64] = {0};
		gets(cmd);
		vector<string> vCmd;
		SplitCmd(cmd, vCmd );
		if( vCmd.size() == 0 )
		{
			printf("no cmd here\n");
			continue;
		}

		if( vCmd[0] == "quit" )
		{
			break;
		}
		else if( vCmd[0] == "entergame" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->UserEnterGame( atoi(vCmd[1].c_str()) );
			}
		}
		else if( vCmd[0] == "logingame" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->LoginGame( vCmd[1] );
			}
		}
		else if( vCmd[0] == "enterroom" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->EnterRoom( atoi(vCmd[1].c_str()) );
			}
		}
		else if( vCmd[0] == "leaveroom" )
		{
			{
				g_mapClient.begin()->second->LeaveRoom( );
			}
		}
		else if( vCmd[0] == "ready" )
		{
			g_mapClient.begin()->second->Ready( );
		}	
		else if( vCmd[0] == "lost" )
		{
			g_mapClient.begin()->second->Lost( );
		}	
		else if( vCmd[0] == "put" )
		{
			if( vCmd.size() == 3 )
			{
				g_mapClient.begin()->second->PutBlock( 
					atoi( vCmd[1].c_str() ),
					atoi( vCmd[2].c_str() ) );
			}
		}
		else if( vCmd[0] == "clear" )
		{
			if( vCmd.size() > 1 )
			{
				vector<s32> vecRows;
				for( size_t i=1; i<vCmd.size(); ++i )
				{
					vecRows.push_back(
						atoi( vCmd[i].c_str() ) );
				}
				g_mapClient.begin()->second->ClearBlock( vecRows );
			}
		}
	}

	mgr.Stop();
}

void ClientInGroup(const char *ip, int port, int clientNum)
{
	CClientManager<ctCLient> mgr(clientNum);
	mgr.Start(ip, port);

	while(true)
	{
		char cmd[64] = {0};
		gets(cmd);
		vector<string> vCmd;
		SplitCmd(cmd, vCmd );
		if( vCmd.size() == 0 )
		{
			printf("no cmd here\n");
			continue;
		}

		if( vCmd[0] == "quit" )
		{
			break;
		}
		else if( vCmd[0] == "logingroup" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->LoginGroup( vCmd[1] );
			}
		}
		else if( vCmd[0] == "creategroup" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->CreateGroup( vCmd[1] );
			}
		}
		else if( vCmd[0] == "addgroupmember" )
		{
			if( vCmd.size() >= 3 )
			{
				g_mapClient.begin()->second->AddMemberToGroup( vCmd );
			}
		}
		else if( vCmd[0] == "agree" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->AgreeJoinGroup( vCmd[1] );
			}
		}
		else if( vCmd[0] == "groupmsg" )
		{
			if( vCmd.size() == 3 )
			{
				g_mapClient.begin()->second->GroupMessage( vCmd[1], vCmd[2] );
			}
		}
		else if( vCmd[0] == "leavegroup" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->LeaveGroup( vCmd[1] );
			}
		}
		else if( vCmd[0] == "deletegroup" )
		{
			if( vCmd.size() == 2 )
			{
				g_mapClient.begin()->second->DeleteGroup( vCmd[1] );
			}
		}
		else if( vCmd[0] == "createroom" )
		{
			if( vCmd.size() == 3 )
			{
				g_mapClient.begin()->second->AskCreateGameRoom( vCmd[1], vCmd[2] );
			}
		}
	}

	mgr.Stop();
}


void testtime()
{
	char strTime[256] = {0};
	time_t tNow;
	time( &tNow );
	strftime( strTime, sizeof(strTime), "%Y-%m-%d %X",localtime(&tNow) );
	printf( "%s\n", strTime );
}

void RunClientSession()
{ 
	CSessionMgr session;
	g_session = &session;
}

extern void TestForTestServer(const char *ip, int port, int clientNum, int freq);
extern void TestForIoService(const char *ip, int port, int clientNum, int freq);
int main(int argc, char *argv[])
{
	/*
	testtime();
	if( argc == 5 )
	{
		//TestForTestServer( argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]) );
		TestForIoService( argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]) );
	}
	return 0;
	//*/

	vector<string> vec;
	SplitCmd("login", vec);
	vec.clear();
	SplitCmd("entergame 10", vec);

	if( argc == 4 )
	{
		/*
		printf("====== player enter GROUP ======\n");
		ClientInGroup(argv[1], atoi(argv[2]), atoi(argv[3]));
		printf("====== player leave GROUP ======\n");
		//*/

		printf("====== player enter LOGIN ======\n");
		ClientLogin(argv[1], atoi(argv[2]), atoi(argv[3]));
		printf("====== player leave LOGIN ======\n");
		if( g_canEnterGame )
		{
			printf("====== player enter GAME ======\n");
			ClientInGame(g_gameIp.c_str(), g_gamePort, 1);
			printf("====== player leave GAME ======\n");
		}
		//*/
	}
    
	return 0;
}

