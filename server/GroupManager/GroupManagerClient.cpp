#include "GroupManagerClient.h"
#include "ServerManager.h"
#include "ReportServerManager.h"
#include "DynamicServerManager.h"
#include "GroupInfoManager.h"
#include "LoginMemberManager.h"
#include "GroupIdManager.h"
#include "GroupManager.h"
#include "GameInfoMgr.h"
#include "group.pb.h"
#include "msgid.pb.h"
#include "errno.pb.h"
#include "gamemanager.pb.h"
using namespace std;

struct _GroupManagerDBParam
{
	s32 gateresid;
	u64 clientid;
	void *data;
};

CGroupManagerClient::CGroupManagerClient(s32 nId) : CClient(nId)
{
	_RegisterProc( sglib::msgid::GPGM_REPORT_STATUS_INFO,	&CGroupManagerClient::_GroupServerReportInfo );
	_RegisterProc( sglib::msgid::GPGM_GP_GATE_CLOSE_NTF,	&CGroupManagerClient::_GroupGateClose );
	_RegisterProc( sglib::msgid::GPGM_DYNAMIC_START_NTF,	&CGroupManagerClient::_GroupServerDynamicStart );
	_RegisterProc( sglib::msgid::GPGM_USER_LOGIN_NTF,		&CGroupManagerClient::_UserLoginGroupProc );
	_RegisterProc( sglib::msgid::GPGM_USER_LOGOUT_NTF,		&CGroupManagerClient::_UserLogoutGroupProc );
	_RegisterProc( sglib::msgid::GPGM_GROUP_CREATE_REQ,		&CGroupManagerClient::_CreateGroupProc );
	_RegisterProc( sglib::msgid::GPGM_GROUP_CREATE_RESULT,	&CGroupManagerClient::_CreateGroupResultProc );
	_RegisterProc( sglib::msgid::GPGM_GROUP_ADD_MEMBER_REQ,	&CGroupManagerClient::_AddMemberToGroupProc );
	_RegisterProc( sglib::msgid::GPGM_AGREE_ASK_MEMBER_INFO_REQ,	&CGroupManagerClient::_AgreeJoinAskInfoProc );
	_RegisterProc( sglib::msgid::GPGM_GROUP_LOAD_RESULT,	&CGroupManagerClient::_LoadGroupResultProc );
	_RegisterProc( sglib::msgid::GPGM_GROUP_MEMBER_LEAVE_NTF,		&CGroupManagerClient::_GroupMemberLeaveProc );
	_RegisterProc( sglib::msgid::GPGM_GROUP_DELETE_NTF,		&CGroupManagerClient::_GroupDeleteProc );
	_RegisterProc( sglib::msgid::GMGM_GAME_INFO_REPORT,		&CGroupManagerClient::_GameManagerReportInfo );
	_RegisterProc( sglib::msgid::GPGM_GROUP_CREATE_GAMEROOM_REQ,	&CGroupManagerClient::_GroupCreateGameRoomProc );
	_RegisterProc( sglib::msgid::GMGM_ASK_CREATE_GAMEROOM_RSP,		&CGroupManagerClient::_GameManagerCreateRoomResultProc );
}

void CGroupManagerClient::OnClose()
{
	CClient::OnClose();
	CReportServerManager::Instance().DelServer( this );
	CGameInfoMgr::Instance().RemoveInfo( this );
}

void CGroupManagerClient::OnRecv(const byte *pkg, s32 len)
{
	s32 msgId = *((s32*)pkg);

	map<int, ProtoProc>::iterator it = m_mapProtoProc.find( msgId );
	if( it != m_mapProtoProc.end() )
	{
		(this->*it->second)( pkg+MSG_ID_LEN, len-MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,OnRecv," << GetClientId() << "," << msgId );
	}
}

void CGroupManagerClient::SendMessageToClient(const ::google::protobuf::Message &msg, s32 msgid)
{
	int count = msg.ByteSize();
	byte buf[MAX_MSG_LEN] = {0};
	*((s32*)buf) = msgid;
	if( msg.SerializeToArray(buf+MSG_ID_LEN, MAX_MSG_LEN) )
	{
		if( !SendPkg( buf, count+MSG_ID_LEN ) )
		{
			SERVER_LOG_ERROR( "CGroupManagerClient,SendMessageToGroupServer,SendPkg" );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,SendMessageToGroupServer,SerializeToArray" );
	}
}

void CGroupManagerClient::SendMessageToGroupServer(const ::google::protobuf::Message &msg, s32 msgid)
{
	SendMessageToClient( msg, msgid );
}

void CGroupManagerClient::_RegisterProc(s32 id, ProtoProc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}

void CGroupManagerClient::_SendMsgToCenter(const ::google::protobuf::Message &Msg, s32 nMsgId)
{
	CServerManager::Instance().SendRpcMsg( 
		CServerManager::Instance().GetCenterServerId(),
		Msg, 
		nMsgId );
}

void CGroupManagerClient::_GroupServerReportInfo(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupGroupmanagerReportStatusInfo info;
	if( info.ParseFromArray(pkg, len) )
	{
		CGroupInfoManager::Instance().SetGroupInfo(
			info.serverid(),
			info.totalgroup(),
			info.totalmember(),
			GetClientId() );

		/*
		SERVER_LOG_DEBUG( "===> GroupServerReport ServerId:" << info.serverid() << \
			", totalGroup:" << info.totalgroup() << ", totalMember:" << info.totalmember() );
		//*/

		for( int i=0; i<info.gateservers_size(); ++i )
		{
			const sglib::publicproto::GateServerInfo &_info = info.gateservers( i );

			CGroupInfoManager::Instance().SetGateInfo(
				_info.id(),
				_info.ip().c_str(),
				_info.port(),
				_info.cur_count(),
				_info.max_count() );

			/*
			SERVER_LOG_DEBUG( "\t---> Gate Id:" << _info.id() << ", Ip:" << _info.ip().c_str() <<\
				", Port:" << _info.port() << ", CurCount:" << _info.cur_count() <<\
				", MaxCount:" << _info.max_count() );
			//*/
		}

		// 汇报信息了，从动态服务器列表里删除
		CReportServerManager::Instance().AddServer( this );
		CDynamicServerManager::Instance().DelServer( this );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_GroupServerReportInfo,ParseFromArray" );
	}
}

void CGroupManagerClient::_GroupGateClose(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupGroupmanagerGateCloseNtf ntf;
	if( ntf.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "_GroupGateClose GateId:" << ntf.gateid() );

		CGroupInfoManager::Instance().DelGateInfo( ntf.gateid() );
		_NotifyCenterGateClose( ntf.gateid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_GroupGateClose,ParseFromArray" );
	}
}

void CGroupManagerClient::_NotifyCenterGateClose(s32 gateid)
{
	sglib::groupproto::GroupmanagerCenterGateCloseNtf ntf;
	ntf.set_gateid( gateid );
		
	_SendMsgToCenter( ntf, sglib::msgid::GMCT_GP_GATE_CLOSE_NTF );
}

void CGroupManagerClient::_GroupServerDynamicStart(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupmanagerGroupDynamicServerStartNtf ntf;
	if( ntf.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "GroupServerDynamicStart," << ntf.id() << "," << ntf.ip().c_str() << "," << ntf.port() );

		// 将此动态增加的server加入列表里
		CDynamicServerManager::Instance().AddServer( this );
		
		// 找一个不在 正在动态启动列表 里的server
		CGroupManagerClient *client = _FindUnDynamicServerClient( ntf.id() );
		if( client )
		{
			// 找到
			_NotifyNewServerDynamicStart( client, ntf.id(), ntf.ip().c_str(), ntf.port() );
		}
		else
		{
			SERVER_LOG_ERROR( "CGroupManagerClient,_GroupServerDynamicStart,_FindUnDynamicServerClient" );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_GroupServerDynamicStart,ParseFromArray" );
	}
}

#if _DEBUG
static void _DebugLoadUserGroups(const string &user)
{
	if( user == "sjj1" )
	{
		CLoginMemberManager::Instance().AddGroup( user, 1 );
		CLoginMemberManager::Instance().AddGroup( user, 2 );
		CLoginMemberManager::Instance().AddGroup( user, 3 );
	}
	else if( user == "sjj2" )
	{
		CLoginMemberManager::Instance().AddGroup( user, 1 );
		CLoginMemberManager::Instance().AddGroup( user, 2 );
	}
	else if( user == "sjj3" )
	{
		CLoginMemberManager::Instance().AddGroup( user, 1 );
		CLoginMemberManager::Instance().AddGroup( user, 3 );
	}
}
#endif

void CGroupManagerClient::_UserLoginGroupProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupGroupmanagerUserLoginNtf ntf;
	if( ntf.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "UserLoginGroup," << ntf.gateresid() << "," << ntf.clientid() \
			<< "," << ntf.user().c_str() );

		CLoginMemberManager::Instance().MemberManager().SetMember(
			ntf.gateresid(), ntf.clientid(), ntf.user().c_str() );
		
		_GroupManagerDBParam _param = { ntf.gateresid(), ntf.clientid(), NULL };
		string sql = string("select groupid from ") + 
			CServerManager::Instance().BuildUserGroupTableName(ntf.user()) + ";";
		s32 id = CServerManager::Instance().HashUser( ntf.user() );
		bool ret = CServerManager::Instance().ExecSql( 
			id, sql, this, &CGroupManagerClient::_GetUserGroupsCallback, &_param, sizeof(_param) );
		if( !ret )
		{
			SERVER_LOG_ERROR( "CGroupManagerClient,_UserLoginGroupProc,ExecSql," << sql.c_str() );
		}
		/*
		// 下面是测试代码，加载登录玩家的所有group信息,
		// 如果是从DB中获取，则是一个异步的过程,
		// 这里的测试代码依然是同步的
#if _DEBUG
		_DebugLoadUserGroups( ntf.user() );
		_DoLoadMemberGroups( ntf.user(), ntf.gateresid(), ntf.clientid() );
#endif
		//*/
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_UserLoginGroupProc,ParseFromArray" );
	}
}

void CGroupManagerClient::_UserLogoutGroupProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupGroupmanagerUserLogoutNtf ntf;
	if( ntf.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "UserLogoutGroup," << ntf.gateresid() << "," << ntf.clientid() );

		CGroupMemberPosition *member = CLoginMemberManager::Instance().MemberManager().FindMember(
			ntf.gateresid(), ntf.clientid() );
		if( !member )
		{
			SERVER_LOG_ERROR( "CGroupManagerClient,_UserLogoutGroupProc,FindMember," << ntf.gateresid() \
				<< "," << ntf.clientid() );
			return;
		}

		// 先通知玩家下线
		_DoMemberLogout( member->User(), ntf.gateresid(), ntf.clientid() );

		// 在删除玩家信息
		CLoginMemberManager::Instance().MemberManager().DelMember(
			ntf.gateresid(), ntf.clientid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_UserLogoutGroupProc,ParseFromArray" );
	}
}

void CGroupManagerClient::_CreateGroupProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupGroupmanageCreateGroupReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		u64 gpid = CGroupIdManager::Instance().GetNextGroupId();

		SERVER_LOG_INFO( "CreateGroup," << GetClientId() << "," << req.gateid() << ","\
			<< req.gateresid() << "," << req.clientid() << "," << gpid << "," << req.name().c_str() << "," \
			<< req.head().c_str() );
		
		CGroupMemberPosition *member = CLoginMemberManager::Instance().MemberManager().FindMember(
			req.gateresid(), req.clientid() );
		if( member == NULL )
		{
			SERVER_LOG_ERROR( "CGroupManagerClient,_CreateGroupProc,FindMember," << req.gateresid() << "," << req.clientid() );
			return;
		}

		_NotifyGroupServerCreateGroup(
			req.gateid(), req.gateresid(), req.clientid(), 
			member->User().c_str(), member->GetName().c_str(), member->GetHead().c_str(),
			req.name().c_str(), gpid, req.head().c_str() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_CreateGroupProc,ParseFromArray" );
	}
}

void CGroupManagerClient::_CreateGroupResultProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupGroupmanagerCreateGroupResult ret;
	if( ret.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "CreateGroupResult," << ret.result() << "," << ret.gateid() << ","\
			<< ret.clientid() << "," << ret.user().c_str() << "," << ret.name().c_str() << "," \
			<< ret.groupid() << "," << ret.head().c_str() << "," << ret.groupserverid() << "," << ret.serverid() );

		if( ret.result() == sglib::errorcode::E_ErrorCode_Success )
		{
			CGroupManager::Instance().AddGroup( ret.groupid(), ret.serverid() );
		}

		_NotifyGroupCreateResult(
			ret.result(), ret.gateid(), ret.clientid(),
			ret.name().c_str(), ret.groupid(), ret.head().c_str(), ret.serverid(),
			ret.groupserverid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_CreateGroupResultProc,ParseFromArray" );
	}
}

void CGroupManagerClient::_AddMemberToGroupProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupGroupmanagerAddMemberToGroupReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "AddMemberToGroup," << req.gateid() << "," << req.gateresid() << ","\
			<< req.clientid() << "," << req.groupid() << "," << req.users_size() );

		sglib::groupproto::GroupmanagerGroupAddMemberToGroupRsp rsp;
		rsp.set_gateid( req.gateid() );
		rsp.set_gateresid( req.gateresid() );
		rsp.set_clientid( req.clientid() );
		rsp.set_groupid( req.groupid() );
		for( s32 i=0; i<(s32)req.users_size(); ++i )
		{
			CGroupMemberPosition *member = CLoginMemberManager::Instance().MemberManager().FindMember( req.users(i) );
			if( member )
			{
				sglib::publicproto::GroupMemberPosInfo *info = rsp.add_users();
				if( info )
				{
					info->set_user( member->User().c_str() );
					info->set_gateresid( member->GetGateResId() );
					info->set_clientid( member->GetClientId() );

					// 增加被邀请的标记
					member->AddInviteGroup( req.groupid() );
				}
				else
				{
					SERVER_LOG_ERROR( "CGroupManagerClient,AddUsers,GroupMemberPosInfo" );
				}
			}
			else
			{
				// TODO 玩家可能已经下线
				SERVER_LOG_WARN( "CGroupManagerClient,FindMember," << req.users(i) );
			}
		}

		SendMessageToGroupServer( rsp, sglib::msgid::GMGP_GROUP_ADD_MEMBER_RSP );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_AddMemberToGroupProc,ParseFromArray" );
	}
}
 
void CGroupManagerClient::_AgreeJoinAskInfoProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupGroupmanagerForAgreeJoinAskInfoReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "AgreeJoinAskInfo," << req.gateresid() << ","\
			<< req.clientid() << "," << req.groupid() );

		sglib::groupproto::GroupmanagerGroupForAgreeJoinAskInfoRsp *rsp = new sglib::groupproto::GroupmanagerGroupForAgreeJoinAskInfoRsp();
		if( !rsp )
		{
			SERVER_LOG_ERROR( "AgreeJoinAskInfo,NEWMsg FAILED" );
			return;
		}
		rsp->set_gateresid( req.gateresid() );
		rsp->set_clientid( req.clientid() );
		rsp->set_groupid( req.groupid() );

		rsp->set_result( sglib::errorcode::E_ErrorCode_Success );
		CGroupMemberPosition *member = CLoginMemberManager::Instance().MemberManager().FindMember(
			req.gateresid(), req.clientid() );
		if( !member )
		{
			rsp->set_result( sglib::errorcode::E_ErrorCode_NotFoundLoginGroup );	
		}
		else
		{
			rsp->set_user( member->User() );
			rsp->set_name( member->GetName() );
			rsp->set_head( member->GetHead() );
			if( member->IsInvited(req.groupid()) )
			{
				// 去掉邀请标记
				member->DelInviteGroup( req.groupid() );

				// 数据写入DB
				_TryAddMemberToGroup( *rsp, *member, req.groupid() );
				return;
			}
			else
			{
				rsp->set_result( sglib::errorcode::E_ErrorCode_NotInvitedJoinGroup );	
			}
		}

		SERVER_LOG_INFO( "CGroupManagerClient,_AgreeJoinAskInfoProc,JoinGroup," << 
			req.groupid() << "," << member->User().c_str() << "," << rsp->result() );
		SendMessageToGroupServer( *rsp, sglib::msgid::GMGP_AGREE_ASK_MEMBER_INFO_RSP );
		SAFE_DELETE( rsp );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_AgreeJoinAskInfoProc,ParseFromArray" );
	}
}

void CGroupManagerClient::_LoadGroupResultProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupGroupmanagerLoadGroupResult ret;
	if( ret.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "CGroupManagerClient,_LoadGroupResultProc," << ret.result() << ","\
			<< ret.groupid() << "," << ret.serverid() << "," << ret.user().c_str() << ","\
			<< ret.totalgroup() << "," << ret.totalmember() );

		if( ret.result() == sglib::errorcode::E_ErrorCode_Success )
		{
			CGroupManager::Instance().AddGroup( ret.groupid(), ret.serverid() );
		
			// 修改群汇报信息，利于分配加载群是的动态负载
			CGroupInfoManager::Instance().SetGroupInfo(
				ret.serverid(),
				ret.totalgroup(),
				ret.totalmember(),
				GetClientId() );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_LoadGroupResultProc,ParseFromArray" );
	}
}

void CGroupManagerClient::_GroupMemberLeaveProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupGroupmanagerMemberLeaveNtf ntf;
	if( ntf.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "CGroupManagerClient,_GroupMemberLeaveProc," << ntf.groupid() << ","\
			<< ntf.user_size() );
		for( s32 i=0; i<(s32)ntf.user_size(); ++i )
		{
			SERVER_LOG_INFO( "CGroupManagerClient,_GroupMemberLeaveProc,LeaveGroup," << ntf.groupid() << ","\
				<< ntf.user(i).c_str() );

			CLoginMemberManager::Instance().DelGroup( ntf.user(i), ntf.groupid() );	
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_GroupMemberLeaveProc,ParseFromArray" );
	}
}

void CGroupManagerClient::_GroupDeleteProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupGroupmanagerGroupDeleteNtf ntf;
	if( ntf.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "CGroupManagerClient,_GroupDeleteProc," << ntf.groupid() );

		CGroupManager::Instance().DelGroup( ntf.groupid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_GroupDeleteProc,ParseFromArray" );
	}
}

void CGroupManagerClient::_GameManagerReportInfo(const byte *pkg, s32 len)
{
	sglib::gamemanagerproto::GamemanagerGroupmanagerReportGameInfo info;
	if( info.ParseFromArray(pkg, len) )
	{
		//SERVER_LOG_DEBUG( "===> GameManagerReportInfo gameId:" << info.gameid() << " Count:" << info.count() );

		CGameInfoMgr::Instance().SetInfo(
			info.gameid(), info.count(), this );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_GameManagerReportInfo,ParseFromArray" );
	}
}

void CGroupManagerClient::_GroupCreateGameRoomProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupGroupmanagerCreateGameRoomReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "CGroupManagerClient,_GroupCreateGameRoomProc," << req.groupid() \
			<< "," << req.game() << "," << req.gateresid() << "," << req.clientid() );

		CGameInfoMgr::sGameInfo *info = CGameInfoMgr::Instance().FindFreeGameManager( req.game() );
		if( info )
		{
			_NotifyGameManagerCreateGameRoom(
				(CGroupManagerClient*)info->client,
				req.groupid(), req.gateresid(), req.clientid(), req.game(), info->gameid );
		}
		else
		{
			_NotifyCreateGameRoomResult( GetClientId(),
				sglib::errorcode::E_ErrorCode_NoThisGroupGame,
				req.groupid(), req.gateresid(), req.clientid(), req.game(), "", 0, 0, "" );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_GroupCreateGameRoomProc,ParseFromArray" );
	}
}

void CGroupManagerClient::_GameManagerCreateRoomResultProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GamemanagerGroupmanagerCreateGameRoomRsp rsp;
	if( rsp.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "CGroupManagerClient,_GameManagerCreateRoomResultProc," << rsp.result() \
			<< "," << rsp.groupid() << "," << rsp.gateresid() << "," << rsp.clientid() << "," \
			<< rsp.game() << "," << rsp.serverid() << "," << rsp.ip().c_str() << "," \
			<< rsp.port() << "," << rsp.roomid() << "," << rsp.password() );
		
		_NotifyCreateGameRoomResult( rsp.serverid(),
			rsp.result(), rsp.groupid(), rsp.gateresid(), rsp.clientid(), rsp.game(),
			rsp.ip().c_str(), rsp.port(), rsp.roomid(), rsp.password().c_str() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_GameManagerCreateRoomResultProc,ParseFromArray" );
	}
}

CGroupManagerClient* CGroupManagerClient::_FindUnDynamicServerClient(s32 serverid)
{
	vector<u64> vecClient;		
	CServerManager::Instance().GetClientList( vecClient );
	CGroupManagerClient *client = NULL;
	for( vector<u64>::iterator it=vecClient.begin(); it!=vecClient.end(); ++it )
	{
		CClient *_tmp = CServerManager::Instance().FindClient( *it );
		if( !_tmp )
		{
			continue;
		}

		if( CDynamicServerManager::Instance().HasServer(_tmp) )
		{
			continue;
		}

		// 如果此服务器不在列表里，还要检查一下这个服务器是否曾经汇报会
		// 状态信息，如果没有汇报过，则可能是另一正在动态启动的服务器
		if( CReportServerManager::Instance().HasServer(_tmp) )
		{
			client = (CGroupManagerClient*)_tmp;
			break;
		}
	}

	return client;
}

void CGroupManagerClient::_NotifyNewServerDynamicStart(CGroupManagerClient *client, s32 id, const char *ip, s32 port)
{
	SG_ASSERT( client != NULL );

	sglib::groupproto::GroupmanagerGroupDynamicServerStartNtf ntf;
	ntf.set_id( id );
	ntf.set_ip( ip );
	ntf.set_port( port );
	
	client->SendMessageToGroupServer( ntf, sglib::msgid::GMGP_DYNAMIC_SERVERSTART_NTF );
}

void CGroupManagerClient::_NotifyGroupServerClose(s32 serverid)
{
	// 如果有group服动态关闭，只通知center去掉对应的消息，但是不通知group gate

	sglib::groupproto::GroupmanagerCenterGroupCloseNtf ntf;
	ntf.set_serverid( serverid );

	_SendMsgToCenter( ntf, sglib::msgid::GMCT_GP_SERVER_CLOSE_NTF );
}

void CGroupManagerClient::_NotifyGroupServerCreateGroup(u64 gateid, s32 gateresid, u64 clientid, const char *user, const char *username, const char *userhead, const char *name, u64 groupid, const char *head)
{
	u64 serverid = CGroupInfoManager::Instance().FindProperGroup();
	CGroupManagerClient *client = (CGroupManagerClient*)CServerManager::Instance().FindClient( serverid );
	if( client )
	{
		sglib::groupproto::GroupmanageGroupCreateGroupNtf ntf;
		ntf.set_gateid( gateid );
		ntf.set_gateresid( gateresid );
		ntf.set_clientid( clientid );
		ntf.set_user( user );
		ntf.set_name( name );
		ntf.set_head( head );
		ntf.set_groupid( groupid );
		ntf.set_groupserverid( GetClientId() );
		ntf.set_username( username );
		ntf.set_userhead( userhead );

		client->SendMessageToGroupServer( ntf, sglib::msgid::GMGP_GROUP_CREATE_NTF );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_NotifyGroupServerCreateGroup,FindClient," << serverid );
	}
}

void CGroupManagerClient::_DoLoadMemberGroups(const string &user, s32 gateresid, u64 clientid)
{
	vector<u64> vecGroup;
	CLoginMemberManager::Instance().GetGroups( user, vecGroup );
	vector<u64>::iterator it = vecGroup.begin();
	for( ; it != vecGroup.end(); ++it )
	{
		u64 groupid = *it;
		s32 groupServerId = CGroupManager::Instance().FindServerId( groupid );
		if( groupServerId != INVALID_VAL )
		{
			SERVER_LOG_INFO( "CGroupManagerClient,_DoLoadMemberGroups,_NotifyMemberOnline," << groupServerId << ","\
				<< user.c_str() << "," << gateresid << "," << clientid << "," << groupid );

			_NotifyMemberOnline(
				groupServerId,
				user,
				gateresid,
				clientid,
				*it );
		}
		else
		{
			SERVER_LOG_INFO( "CGroupManagerClient,_DoLoadMemberGroups,_NotifyLoadGroup," << \
				user.c_str() << "," << gateresid << "," << clientid << "," << *it );

			_NotifyLoadGroup( 
				user,
				gateresid,
				clientid,
				*it );
		}
	}

	// 补全玩家的基本信息
	_DoLoadUserBasicInfo( user );
}

void CGroupManagerClient::_NotifyGroupCreateResult(s32 result, u64 gateid, u64 clientid, const char *name, u64 groupid, const char *head, s32 serverid, u64 instanceId)
{
	sglib::groupproto::GroupmanagerGroupCreateGroupRsp rsp;
	rsp.set_result( result );
	rsp.set_gateid( gateid );
	rsp.set_clientid( clientid );
	rsp.set_name( name );
	rsp.set_groupid( groupid );
	rsp.set_head( head );
	rsp.set_serverid( serverid );

	CServerManager::Instance().SendClientMsg(
		instanceId, rsp, sglib::msgid::GMGP_GROUP_CREATE_RSP );
}

void CGroupManagerClient::_NotifyLoadGroup(const std::string &user, s32 gateresid, u64 clientid, u64 groupid)
{
	u64 serverid = CGroupInfoManager::Instance().FindProperGroup();
	CGroupManagerClient *client = (CGroupManagerClient*)CServerManager::Instance().FindClient( serverid );
	if( client )
	{
		sglib::groupproto::GroupmanagerGroupLoadGroupNtf ntf;
		ntf.set_clientid( clientid );
		ntf.set_gateresid( gateresid );
		ntf.set_groupid( groupid );
		ntf.set_user( user );

		client->SendMessageToGroupServer( ntf, sglib::msgid::GMGP_GROUP_LOAD_NTF );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_NotifyLoadGroup,FindClient," << serverid );
	}
}

void CGroupManagerClient::_NotifyMemberOnline(s32 groupServerId, const std::string &user, s32 gateresid, u64 clientid, u64 groupid)
{
	const SGroupInfo *pInfo = CGroupInfoManager::Instance().GetGroupInfo( groupServerId );
	if( pInfo == NULL )
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_NotifyMemberOnline,GetGroupInfo," << groupServerId );
		return;
	}

	u64 serverId = pInfo->m_InstanceId;
	CGroupManagerClient *client = (CGroupManagerClient*)CServerManager::Instance().FindClient( serverId );
	if( client )
	{
		sglib::groupproto::GroupmanagerGroupMemberOnlineNtf ntf;
		ntf.set_clientid( clientid );
		ntf.set_gateresid( gateresid );
		ntf.set_groupid( groupid );
		ntf.set_user( user );

		client->SendMessageToGroupServer( ntf, sglib::msgid::GMGP_GROUP_MEMBER_ONLINE_NTF );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_NotifyMemberOnline,FindClient," << serverId );
	}
}

void CGroupManagerClient::_DoMemberLogout(const std::string &user, s32 gateresid, u64 clientid)
{
	vector<u64> vecGroup;
	CLoginMemberManager::Instance().GetGroups( user, vecGroup );
	vector<u64>::iterator it = vecGroup.begin();
	for( ; it != vecGroup.end(); ++it )
	{
		u64 groupid = *it;
		s32 groupServerId = CGroupManager::Instance().FindServerId( groupid );
		if( groupServerId != INVALID_VAL )
		{
			_NotifyMemberOffline(
				groupServerId,
				user,
				gateresid,
				clientid,
				groupid );
		}
		else
		{
			SERVER_LOG_ERROR( "CGroupManagerClient,_DoMemberLogout,FindServerId," << groupid );
		}
	
		//CLoginMemberManager::Instance().DelGroup( user, groupid );
	}

	CLoginMemberManager::Instance().DelAllGroup( user );
}

void CGroupManagerClient::_NotifyMemberOffline(s32 groupServerId, const std::string &user, s32 gateresid, u64 clientid, u64 groupid)
{
	const SGroupInfo *pInfo = CGroupInfoManager::Instance().GetGroupInfo( groupServerId );
	if( pInfo == NULL )
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_NotifyMemberOffline,GetGroupInfo," << groupServerId );
		return;
	}

	u64 serverId = pInfo->m_InstanceId;
	CGroupManagerClient *client = (CGroupManagerClient*)CServerManager::Instance().FindClient( serverId );
	if( client )
	{
		sglib::groupproto::GroupmanagerGroupMemberOfflineNtf ntf;
		ntf.set_clientid( clientid );
		ntf.set_gateresid( gateresid );
		ntf.set_groupid( groupid );
		ntf.set_user( user );

		client->SendMessageToGroupServer( ntf, sglib::msgid::GMGP_GROUP_MEMBER_OFFLINE_NTF );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_NotifyMemberOffline,FindClient," << serverId );
	}
}

void CGroupManagerClient::_NotifyCreateGameRoomResult(u64 serverId, s32 result, u64 groupid, s32 gateresid, u64 clientid, s32 game, const char *ip, s32 port, s32 roomid, const char *roompwd)
{
	CGroupManagerClient *client = (CGroupManagerClient*)CServerManager::Instance().FindClient( serverId );
	if( client )
	{
		sglib::groupproto::GroupmanagerGroupCreateGameRoomRsp rsp;
		rsp.set_result( result );
		rsp.set_groupid( groupid );
		rsp.set_gateresid( gateresid );
		rsp.set_clientid( clientid );
		rsp.set_game( game );
		rsp.set_ip( ip );
		rsp.set_port( port );
		rsp.set_roomid( roomid );
		rsp.set_password( roompwd );

		client->SendMessageToGroupServer( rsp, sglib::msgid::GMGP_GROUP_CREATE_GAMEROOM_RSP );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_NotifyCreateGameRoomResult,FindClient," << serverId );
	}
}

void CGroupManagerClient::_NotifyGameManagerCreateGameRoom(CGroupManagerClient *gameManager, u64 groupid, s32 gateresid, u64 clientid, s32 game, s32 gameid)
{
	SELF_ASSERT( gameManager, return; );
	sglib::groupproto::GroupmanagerGamemanagerCreateGameRoomReq req;
	req.set_groupid( groupid );
	req.set_gateresid( gateresid );
	req.set_clientid( clientid );
	req.set_game( game );
	req.set_gameid( gameid );
	req.set_serverid( GetClientId() );
	gameManager->SendMessageToClient( req, sglib::msgid::GMGM_ASK_CREATE_GAMEROOM_REQ );
}

void CGroupManagerClient::_DoLoadUserBasicInfo(const std::string &user)
{
	s32 id = CServerManager::Instance().HashUser( user );
	string sql = "select User,Name,Head from user where user='";
	sql += user;
	sql += "';";
	bool ret = CServerManager::Instance().ExecSql( 
		id, sql, this, &CGroupManagerClient::_GetUserBasicInfoCallback, NULL, 0 );
	if( !ret )
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_DoLoadUserBasicInfo,ExecSql," << sql.c_str() );
	}
}

void CGroupManagerClient::_TryAddMemberToGroup(google::protobuf::Message &msg, CGroupMemberPosition &member, u64 groupid)
{
	SERVER_LOG_INFO( "CGroupManagerClient,_TryAddMemberToGroup," << groupid << "," << member.User().c_str() );

	_GroupManagerDBParam _param = { 0, 0, &msg };

	s32 id = CServerManager::Instance().GetGroupDbId();
	char strId[128] = {0};
	sprintf( strId, "%llu", groupid );
	string udname = CServerManager::Instance().HashUserDBName( member.User() );
	string sql = string("call AddMemberToGroup('") + member.User() + "','" + member.GetName() + "','"\
		+ member.GetHead() + "'," + string(strId) + ",'" + udname + "');";
	bool ret = CServerManager::Instance().ExecSql( 
		id, sql, this, &CGroupManagerClient::_AddMemberToGroupCallback, &_param, sizeof(_param) );
	if( !ret )
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_TryAddMemberToGroup,ExecSql," << sql.c_str() );
	}
}

void CGroupManagerClient::_GetUserGroupsCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len)
{
	SELF_ASSERT( param, return; );
	SELF_ASSERT( len==sizeof(_GroupManagerDBParam), return; ); 
	_GroupManagerDBParam *_param = (_GroupManagerDBParam*)param;
	CGroupMemberPosition *member = CLoginMemberManager::Instance().MemberManager().FindMember( _param->gateresid, _param->clientid );
	if( !member )
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_GetUserGroupsCallback,FindMember," << _param->gateresid << "," << _param->clientid );
		return;
	}

	string user = member->User();
	while( RecordSet && RecordSet->GetRecord() )
	{
		u64 groupid = 0;
		const char *val = RecordSet->GetFieldValue( 1 );
		if( val )
		{
			sscanf( val, "%llu", &groupid );
			CLoginMemberManager::Instance().AddGroup( user, groupid );
			SERVER_LOG_DEBUG( "_GetUserGroupsCallback user:" << user.c_str() << " in group:" << groupid );
		}
	}
		
	_DoLoadMemberGroups( user, _param->gateresid, _param->clientid );
}

void CGroupManagerClient::_GetUserBasicInfoCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len)
{
	string user(""), name(""), head("");
	while( RecordSet && RecordSet->GetRecord() )
	{
		// User,Name,Head
		const char *val = RecordSet->GetFieldValue( 1 );
		if( val )
		{
			user = val;
		}

		val = RecordSet->GetFieldValue( 2 );
		if( val )
		{
			name = val;
		}
	
		val = RecordSet->GetFieldValue( 3 );
		if( val )
		{
			head = val;
		}

		break;
	}

	if( user != "" )
	{
		CGroupMemberPosition *member = CLoginMemberManager::Instance().MemberManager().FindMember( user );
		if( !member )
		{
			SERVER_LOG_INFO( "CGroupManagerClient,_GetUserBasicInfoCallback,FindMember," << user.c_str()\
				<< "NotFound maybe logout" );
			return;
		}

		member->SetName( name );
		member->SetHead( head );
		SERVER_LOG_INFO( "CGroupManagerClient,_GetUserBasicInfoCallback,SetMemberBasic," << user.c_str()\
			<< "," << name.c_str() << "," << head.c_str() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerClient,_GetUserBasicInfoCallback,UserEmpty" );
	}
}

void CGroupManagerClient::_AddMemberToGroupCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len)
{
	SELF_ASSERT( param, return; );
	SELF_ASSERT( len==sizeof(_GroupManagerDBParam), return; ); 
	_GroupManagerDBParam *_param = (_GroupManagerDBParam*)param;
	sglib::groupproto::GroupmanagerGroupForAgreeJoinAskInfoRsp *rsp = 
		(sglib::groupproto::GroupmanagerGroupForAgreeJoinAskInfoRsp*)_param->data;
	SELF_ASSERT( rsp, return; );

	bool flag = false;
	while( RecordSet && RecordSet->GetRecord() )
	{
		const char *val = RecordSet->GetFieldValue( 1 );
		if( val && atoi(val) == 1 )
		{
			flag = true;
		}
		break;
	}

	if( flag )
	{
		CLoginMemberManager::Instance().AddGroup( rsp->user(), rsp->groupid() );
	}
	else
	{
		rsp->set_result( sglib::errorcode::E_ErrorCode_JoinGroupFailed );
	}

	SERVER_LOG_INFO( "CGroupManagerClient,_AddMemberToGroupCallback," << rsp->groupid() << "," << 
		rsp->user().c_str() << "," << rsp->result() );

	SendMessageToGroupServer( *rsp, sglib::msgid::GMGP_AGREE_ASK_MEMBER_INFO_RSP );
	SAFE_DELETE( rsp );
}
