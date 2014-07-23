#include "GroupManager.h"
#include "ServerManager.h"
#include "GateClientManager.h"
#include "GroupClient.h"
#include "GroupRpcClient.h"
#include "group.pb.h"
#include "msgid.pb.h"
#include "errno.pb.h"
#include "BasicConfig.h"
using namespace std;
using namespace SGLib;

struct _groupManagerDBParam
{
	u64 groupid;
	void *data;
};

INIT_SIGNLETON_CLASS(CGroupManager);

CGroupManager::~CGroupManager()
{
}

CGroupInfo* CGroupManager::AddGroup(u64 id, const char *name, const char *icon)
{
	CGuardLock<CLock> g(m_Lock);
	map<u64, CGroupInfo*>::iterator it = m_Groups.find( id );
	if( it == m_Groups.end() )
	{
		CGroupInfo *group = new CGroupInfo( id, name, icon );
		if( group )
		{
			m_Groups[ id ] = group;
			return group;
		}
		else
		{
			SERVER_LOG_ERROR( "CGroupManager,AddGroup,new CGroupInfo" );
		}
	}

	return NULL;
}

void CGroupManager::DelGroup(u64 id)
{
	CGuardLock<CLock> g(m_Lock);
	map<u64, CGroupInfo*>::iterator it = m_Groups.find( id );
	if( it != m_Groups.end() )
	{
		SAFE_DELETE( it->second );
		m_Groups.erase( it );
	}
}

CGroupInfo* CGroupManager::FindGroup(u64 id)
{
	CGuardLock<CLock> g(m_Lock);
	map<u64, CGroupInfo*>::iterator it = m_Groups.find( id );
	if( it != m_Groups.end() )
	{
		return it->second;
	}

	return NULL;
}

s32 CGroupManager::TotalGroupCount()
{
	CGuardLock<CLock> g(m_Lock);
	return (s32)m_Groups.size();
}

s32 CGroupManager::TotalMemberCount()
{
	s32 count = 0;
	CGuardLock<CLock> g(m_Lock);
	map<u64, CGroupInfo*>::iterator it = m_Groups.begin();
	for( ; it != m_Groups.end(); ++it )
	{
		SG_ASSERT( it->second != NULL );
		count += it->second->MemberCount();
	}

	return count;
}

s32 CGroupManager::GroupMemberCount(u64 id)
{
	CGroupInfo *group = FindGroup( id );
	if( group )
	{
		return group->MemberCount();
	}

	return 0;
}

void CGroupManager::SetGroupReady()
{
	m_bIsReady = true;
}

void CGroupManager::SendMsgToClient(u64 gateInstanceId, u64 clientid, const ::google::protobuf::Message &msg, s32 msgid)
{
    CClient *client = CServerManager::Instance().FindClient( gateInstanceId );
    if( client != NULL )
    {
        CGroupClient *cli = (CGroupClient*)client;
        cli->SendMsgToClient( clientid, msg, msgid );
    }
    else
    {
        SERVER_LOG_ERROR( "CGroupManager,SendMsgToClient,FindClient,GateId:" << gateInstanceId << ",ClientId:" << clientid << ",MsgId:" << msgid );
    }
}

void CGroupManager::SendMsgToClient(s32 gateResId, u64 clientid, const ::google::protobuf::Message &msg, s32 msgid)
{
	const CGateClientInfo *info = CGateClientManager::Instance().GetGateInfo( gateResId );
    if( info )
    {
        u64 gateid = info->m_GateInstanceId;
        SendMsgToClient( gateid, clientid, msg, msgid );
    }
    else
    {
        SERVER_LOG_ERROR( "CGroupManager,SendMsgToClient,GetGateInfo,GateResId:" << gateResId << ",CliendId:" << clientid << ",MsgId:" << msgid );
    }
}

void CGroupManager::BroadcastMsgToGates(const ::google::protobuf::Message &msg, s32 msgid)
{
	vector<u64> vecClient;
	CServerManager::Instance().GetClientList( vecClient );
	vector<u64>::iterator it = vecClient.begin();
	for( ; it != vecClient.end(); ++it )
	{
		CClient *client = CServerManager::Instance().FindClient( *it );
		if( !client )
		{
			continue;
		}

		CGroupClient *pClient = (CGroupClient*)client;
		pClient->SendMsg( msg, msgid );
	}
}

void CGroupManager::StartReportStatusTimer(bool isDynamicStart)
{
	m_nReportTimerId = CServerManager::Instance().AddTimer(
			E_ReportStatusInfo_Timer,
			ReportStatusInforCallBack,
			NULL,
			0,
			true );

		// 设置动态启动
		m_bIsDynamicStart = isDynamicStart;
		m_bIsReady = (!m_bIsDynamicStart);
		if( m_bIsDynamicStart )
		{
			m_bHasReported = false;
		}
}

void CGroupManager::ReportStatusInforCallBack(void *pData, s32 nDataLen)
{
	if( !CGroupManager::Instance().m_bIsReady )
	{
		// 动态启动的如果其gate还没连接上来，则不要汇报自身的状态

		if( !CGroupManager::Instance().m_bHasReported )
		{
			// 如果还没有回报过，在汇报给GameManager
			CGroupManager::Instance().m_bHasReported = CGroupManager::Instance()._ReportDynamicStarted();
		}

		return;
	}

	vector<s32> vecGateId;
	CGateClientManager::Instance().GetGateIdList( vecGateId );

	if( vecGateId.size() > 0 )
	{
		sglib::groupproto::GroupGroupmanagerReportStatusInfo reportInfo;
		reportInfo.set_serverid( CServerManager::Instance().ServerId() );
		reportInfo.set_totalgroup( CGroupManager::Instance().TotalGroupCount() );	
		reportInfo.set_totalmember( CGroupManager::Instance().TotalMemberCount() );	

		vector<s32>::iterator it = vecGateId.begin();
		for( ; it != vecGateId.end(); ++it )
		{
			const CGateClientInfo *info = CGateClientManager::Instance().GetGateInfo( *it );
			if( info == NULL )
			{
				continue;
			}

			sglib::publicproto::GateServerInfo *pInfo = reportInfo.add_gateservers();
			pInfo->set_id( info->m_GateId );
			pInfo->set_ip( info->m_GateIp );
			pInfo->set_port( info->m_GatePort );
			pInfo->set_cur_count( info->m_GateCurConnCount );
			pInfo->set_max_count( info->m_GateMaxConnCount );
		}

		CServerManager::Instance().SendRpcMsg( 
			CServerManager::Instance().GetGroupManagerServerId(),
			reportInfo, sglib::msgid::GPGM_REPORT_STATUS_INFO );
	}
}

void CGroupManager::NotifyMemberJoinGroup(u64 groupid, const std::string &newMember)
{
	CGroupInfo *group = FindGroup( groupid );
	if( !group )
	{
		SG_ASSERT( false );
		return;
	}
	CGroupMember *member = group->FindMember( newMember.c_str() );
	SG_ASSERT( member );

	map<s32, vector<u64> > alluser;
	vector<string> vecUser;
	group->DumpMemberInfo(  alluser, newMember, vecUser );

	// 通知新玩家新的群详细信息 
	_NotifyGroupInfoToMember( *group, vecUser, *member );

	// 通知此群中的所有在线成员有新成员加入
	_NotifyNewMemberJoin( groupid, alluser, *member );
}

void CGroupManager::BroadcastGroupMessage(u64 groupid, const string &sender, const string &content)
{
	CGroupInfo *group = FindGroup( groupid );
	if( !group )
	{
		SERVER_LOG_ERROR( "CGroupManager,BroadcastGroupMessage,FindGroup," << groupid );
		return;
	}

	if( !group->FindMember( sender.c_str() ) )
	{
		SERVER_LOG_ERROR( "CGroupManager,BroadcastGroupMessage,SenderNotInGroup," << groupid << "," << sender );
		return;
	}

	sglib::groupproto::SCGroupMessageNtf ntf;
	ntf.set_groupid( groupid );
	ntf.set_content( content );
	ntf.set_sender( sender );

	BroadcastMsgInGroup( groupid, ntf, sglib::msgid::SC_GROUP_MESSAGE_NTF );
}

void CGroupManager::NotifyGroupInfoToMember(u64 groupid, const std::string &member)
{
	CGroupInfo *group = FindGroup( groupid );
	if( !group )
	{
		SG_ASSERT( false );
		return;
	}
	CGroupMember *pMember = group->FindMember( member.c_str() );
	SG_ASSERT( pMember );

	map<s32, vector<u64> > alluser;
	vector<string> vecUser;
	group->DumpMemberInfo(  alluser, member, vecUser );

	// 通知新玩家新的群详细信息 
	_NotifyGroupInfoToMember( *group, vecUser, *pMember );
}

void CGroupManager::NotifyAllGroupGateAddGroupInfo(u64 groupid, s32 serverid)
{
	sglib::groupproto::GroupGateAddGroupNtf ntf;
	ntf.set_groupid( groupid );
	ntf.set_serverid( serverid );

	CGroupManager::Instance().BroadcastMsgToGates( ntf, sglib::msgid::GPGG_ADD_GROUPID_NTF );
}

void CGroupManager::BroadcastMsgInGroup(u64 groupid, const ::google::protobuf::Message &msg, s32 msgid)
{
	CGroupInfo *group = FindGroup( groupid );
	if( !group )
	{
		SERVER_LOG_ERROR( "CGroupManager,BroadcastMsgInGroup,FindGroup," << groupid );
		return;
	}

	map<s32, vector<u64> > alluser;
	vector<string> vecUser;
	group->DumpMemberInfo(  alluser, "", vecUser );

	map<s32, vector<u64> >::iterator it = alluser.begin();
	for( ; it != alluser.end(); ++it )
	{
		const CGateClientInfo *pGate = CGateClientManager::Instance().GetGateInfo( it->first );
		if( pGate )
		{
			CGroupClient *client = (CGroupClient*)CServerManager::Instance().FindClient( pGate->m_GateInstanceId );
			SG_ASSERT( client != NULL );
			client->SendMsgToClients( it->second, msg, msgid );
		}
		else
		{
			SERVER_LOG_WARN( "CGroupManager,BroadcastMsgInGroup,GetGateInfo," << it->first );
		}
	}
}

void CGroupManager::UserLogin(CGroupClient &client, s32 gateresid, u64 clientid, const std::string &user, const std::string &token)
{
	SERVER_LOG_INFO( "GroupUserLogin,"<< gateresid << "," << clientid << "," << user.c_str() << "," << token.c_str() );

	s32 result = _CheckLoginValid( user, token );
	if( result == sglib::errorcode::E_ErrorCode_Success )
	{
		_NotifyManagerUserLogin( gateresid, clientid, user );
		_NotifyGroupgateLoginSuccess( client, clientid );
	}
	_NotifyUserLoginResult( client, result, clientid );
}

void CGroupManager::CreateGroup(CGroupClient &client, s32 gateresid, u64 gateid, u64 clientid, const std::string &groupName)
{
	SERVER_LOG_DEBUG( "CreateGroup,"<< gateresid << "," << gateid << "," << clientid << "," << groupName.c_str() );
	_NotifyManagerCreateGroup( gateid, gateresid, clientid, groupName );
}

void CGroupManager::AddGroupMembers(CGroupClient &client, s32 gateresid, u64 gateid, u64 clientid, sglib::groupproto::CSGroupAddMemberReq &req)
{
	SERVER_LOG_INFO( "AddGroupMembers,"<< gateresid << "," << clientid << "," <<\
		req.groupid() << "," << req.users_size() );

	CGroupInfo *group = CGroupManager::Instance().FindGroup( req.groupid() );
	if( !group )
	{
		_NotifyAddGroupMemberRsp( client, clientid, req.groupid(), sglib::errorcode::E_ErrorCode_NotFoundGroup );
		return;
	}

	// TODO 检查自己是否在此群

#if _DEBUG
	string allusers = "";
#endif
	vector<string> vecUser;
	for( s32 i=0; i<(s32)req.users_size(); ++i )
	{
#if _DEBUG
		allusers += req.users( i );
		allusers += ",";
#endif
		if( !group->FindMember( req.users(i).c_str() ) )
		{
			vecUser.push_back( req.users(i) );
		}
		else
		{
			SERVER_LOG_DEBUG( "AddGroupMembers," << req.users(i) << ",AleardyInGroup" );
		}
	}
#if _DEBUG
	SERVER_LOG_DEBUG( "AddGroupMembers," << allusers.c_str() );
#endif

	_NotifyManagerAddMemberToGroup( gateresid, gateid, clientid, req.groupid(), vecUser );
}

void CGroupManager::AgreeJoinGroup(CGroupClient &client, s32 gateresid, u64 gateid, u64 clientid, u64 groupid)
{
	SERVER_LOG_INFO( "AgreeJoinGroup,"<< gateresid << "," << clientid << "," << groupid );

	// TODO 检查是否受到邀请

	CGroupInfo *group = CGroupManager::Instance().FindGroup( groupid );
	if( !group )
	{
		SERVER_LOG_ERROR( "CGroupClient,_GroupAgreeJoinProc,FindGroup," << groupid );
		return;
	}

	_AskManagerInfoToAgree( gateresid, clientid, groupid );
}

void CGroupManager::LeaveGroup(CGroupClient &client, s32 gateresid, u64 gateid, u64 clientid, u64 groupid)
{
	SERVER_LOG_DEBUG( "LeaveGroup," << gateresid << "," << clientid << "," << groupid ); 
	_DoMemberLeaveGroup( client, gateresid, clientid, groupid );
}

void CGroupManager::DeleteGroup(CGroupClient &client, s32 gateresid, u64 gateid, u64 clientid, u64 groupid)
{		
	SERVER_LOG_DEBUG( "DeleteGroup," << gateresid << "," << clientid << "," << groupid ); 
	_DoMemberDeleteGroup( client, gateresid, clientid, groupid );
}

void CGroupManager::GroupMessage(s32 gateresid, u64 gateid, u64 clientid, u64 groupid, const std::string &content)
{
	SERVER_LOG_DEBUG( "GroupMessage," << gateresid << "," << clientid <<\
		"," << groupid << "," << content.c_str() );

	CGroupInfo *group = CGroupManager::Instance().FindGroup( groupid );
	if( !group )
	{
		SERVER_LOG_ERROR( "CGroupManager,_GroupMessageProc,FindGroup," << groupid );
		return;
	}

	CGroupMember *member = group->FindMember(
		gateresid, clientid );
	if( !member )
	{
		SERVER_LOG_ERROR( "CGroupManager,_GroupMessageProc,FindMember," << gateresid << "," << clientid );
		return;
	}

	BroadcastGroupMessage( groupid, member->GetUser(), content );
}

void CGroupManager::UserLogout(s32 gateresid, u64 clientid)
{
	sglib::groupproto::GroupGroupmanagerUserLogoutNtf ntf;
    ntf.set_gateresid( gateresid );
    ntf.set_clientid( clientid );
		
    CServerManager::Instance().SendRpcMsg( 
        CServerManager::Instance().GetGroupManagerServerId(),
        ntf, sglib::msgid::GPGM_USER_LOGOUT_NTF );
}

void CGroupManager::CreateGroupGame(CGroupClient &client, s32 gateresid, u64 gateid, u64 clientid, u64 groupid, s32 game)
{
	SERVER_LOG_DEBUG( "CreateGroupGame," << gateresid << "," << clientid <<\
		"," << groupid << "," << game );

	CGroupInfo *group = CGroupManager::Instance().FindGroup( groupid );
	if( !group )
	{
		SERVER_LOG_ERROR( "CGroupManager,CreateGroupGame,FindGroup," << groupid );
		return;
	}

	_NotifyGroupManagerCreateGameRoom( gateresid, clientid, groupid, game );
}

void CGroupManager::TryCreateGroup(u64 gateid, s32 gateresid, u64 clientid, const std::string &user, const std::string &name, u64 groupid, u64 groupserverid)
{
	SERVER_LOG_INFO( "RpcCreateGroupResult," << gateid << "," <<\
		gateresid << "," << clientid << "," << user.c_str() << "," <<\
		name.c_str() << "," << groupid <<\
		"," << groupserverid );

	s32 result = sglib::errorcode::E_ErrorCode_Success;
	CGroupInfo *group = CGroupManager::Instance().AddGroup(
		groupid, name.c_str(), "" );
	if( group == NULL )
	{
		result = sglib::errorcode::E_ErrorCode_CreateGroupError;
		SERVER_LOG_ERROR( "RpcCreateGroupResult," << groupid << "," << name.c_str() );
	}
	else
	{
		// TODO

		CGroupMember *member = group->AddMember( 
			user.c_str(), 
			user.c_str(), 
			"", 
			true );
		SG_ASSERT( member != NULL );
		member->SetOnline( true, gateresid, clientid );
	}

	_NotifyGroupmanagerCreateGroupResult( 
		result,
		gateid, clientid, user.c_str(),
		name.c_str(), groupid, groupserverid );
}

void CGroupManager::CreateGroupResult(s32 result, u64 gateid, u64 clientid, const std::string &name, u64 groupid, u64 groupserverid)
{
	SERVER_LOG_INFO( "CreateGroupResult," << result << "," << gateid << "," <<\
		clientid << "," << name.c_str() << "," << groupid << "," << groupserverid );

	if( result == sglib::errorcode::E_ErrorCode_Success )
	{
		// 创建成功，先通知所有gate
		CGroupManager::Instance().NotifyAllGroupGateAddGroupInfo(
			groupid, groupserverid );
	}

	_NotifyClientCreateGroupRsp(
		result, gateid, clientid,
		name.c_str(), groupid );
}

void CGroupManager::AddGroupMemberRsp(sglib::groupproto::GroupmanagerGroupAddMemberToGroupRsp &rsp)
{
	SERVER_LOG_INFO( "AddMemberToGroupRsp," << rsp.gateid() << "," << rsp.gateresid() << "," <<\
		rsp.clientid() << "," << rsp.groupid() << "," << rsp.users_size() );

	// 先通知原请求的玩家
	CGroupClient *client = (CGroupClient*)CServerManager::Instance().FindClient( rsp.gateid() );
	SG_ASSERT( client != NULL );
	_NotifyAddGroupMemberRsp( *client, rsp.clientid(), rsp.groupid(), sglib::errorcode::E_ErrorCode_Success );

	CGroupInfo *groupInfo = CGroupManager::Instance().FindGroup( rsp.groupid() );
	SG_ASSERT( groupInfo != NULL );

	// 发出请求给被邀请的玩家
	sglib::groupproto::SCGroupAskJoinNtf ntf;
	sglib::publicproto::GroupInfo *pInfo = ntf.mutable_group();
	pInfo->set_id( groupInfo->GetId() );
	pInfo->set_name( groupInfo->GetName() );
	pInfo->set_count( groupInfo->MemberCount() );
	pInfo->set_icon( "icon" );

	map<s32, vector<u64> > alluser;
	for( s32 i=0; i<(s32)rsp.users_size(); ++i )
	{
		const sglib::publicproto::GroupMemberPosInfo &info = rsp.users( i );
		alluser[ info.gateresid() ].push_back( info.clientid() );
	}
	map<s32, vector<u64> >::iterator it = alluser.begin();
	for( ; it != alluser.end(); ++it )
	{
		const CGateClientInfo *pGate = CGateClientManager::Instance().GetGateInfo( it->first );
		if( pGate )
		{
			client = (CGroupClient*)CServerManager::Instance().FindClient( pGate->m_GateInstanceId );
			SG_ASSERT( client != NULL );
			client->SendMsgToClients(
				it->second,
				ntf,
				sglib::msgid::SC_GROUP_ASK_JOIN_NTF );
		}
		else
		{
			SERVER_LOG_WARN( "CGroupRpcClient,_GroupmanagerGroupAddMemberRspProc,GetGateInfo," << it->first );
		}
	}
}

void CGroupManager::AgreeJoinGroupAskInfoRsp(sglib::groupproto::GroupmanagerGroupForAgreeJoinAskInfoRsp &rsp)
{
	SERVER_LOG_INFO( "AgreeJoinAskInfoRsp," << rsp.result() << "," << rsp.gateresid() << "," <<\
		rsp.clientid() << "," << rsp.groupid() << "," << rsp.user().c_str() );

	CGroupInfo *group = CGroupManager::Instance().FindGroup( rsp.groupid() );
	if( !group )
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_GroupAgreeJoinAskInfoRspProc,FindGroup," << rsp.groupid() );
		return;
	}

	if( rsp.result() == sglib::errorcode::E_ErrorCode_Success )
	{
		CGroupMember *member = group->AddMember( 
			rsp.user().c_str(), 
			rsp.user().c_str(), 
			"", 
			true );
		SG_ASSERT( member != NULL );
		member->SetOnline( true, rsp.gateresid(), rsp.clientid() );
		CGroupManager::Instance().NotifyMemberJoinGroup( rsp.groupid(), rsp.user() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_GroupAgreeJoinAskInfoRspProc,Failed," << rsp.result() );
	}
}

void CGroupManager::LoadGroup(sglib::groupproto::GroupmanagerGroupLoadGroupNtf &ntf)
{
	SERVER_LOG_INFO( "LoadGroup," << ntf.gateresid() << "," <<\
		ntf.clientid() << "," << ntf.groupid() << "," << ntf.user().c_str() );

	// 检查是否已经加载此群
	CGroupInfo *group = CGroupManager::Instance().FindGroup( ntf.groupid() );
	if( group )
	{
		_LoadGroupDone( ntf.groupid(), ntf.user() );
	}
	else
	{
		// 从DB加载群信息
		char *struser = new char[USER_MAX_LEN];
		memset( struser, 0, USER_MAX_LEN );
		strncpy( struser, ntf.user().c_str(), USER_MAX_LEN );
		_groupManagerDBParam _param = { ntf.groupid(), struser };

		char strid[128] = {0};
		sprintf( strid, "%llu", ntf.groupid() );
		string sql = string("select name,head from groups where id='") + strid + "';";
		s32 id = CServerManager::Instance().GetGroupDbId();
		bool ret = CServerManager::Instance().ExecSql( 
			id, sql, this, &CGroupManager::_GetGroupInfoCallback, &_param, sizeof(_param) );
		if( !ret )
		{
			SERVER_LOG_ERROR( "CGroupManager,LoadGroup,ExecSql," << ntf.user().c_str() << "," << sql.c_str() );
		}
		/*

		*/
	}

	/*
	// for debug
	s32 result = _DoLoadGroup( ntf.groupid(), ntf.gateresid(), ntf.clientid(), ntf.user().c_str() );
	_NotifyLoadGroupResult(
		result, 
		ntf.groupid(),
		CServerManager::Instance().ServerId(),
		ntf.user().c_str() );

	if( result == sglib::errorcode::E_ErrorCode_Success )
	{
		// 先通知所有的group gate更新信息
		CGroupManager::Instance().NotifyAllGroupGateAddGroupInfo(
			ntf.groupid(), CServerManager::Instance().ServerId() );

		// 到这里表示加载群成功了,通知client群信息更新
		CGroupManager::Instance().NotifyGroupInfoToMember(
			ntf.groupid(), ntf.user() );
	}
	//*/
}

void CGroupManager::GroupMemberOnline(sglib::groupproto::GroupmanagerGroupMemberOnlineNtf &ntf)
{
	SERVER_LOG_INFO( "_GroupmanagerGroupMemberOnlineProc," << ntf.gateresid() << "," <<\
		ntf.clientid() << "," << ntf.groupid() << "," << ntf.user().c_str() );

	_MemberOnline( ntf.groupid(), ntf.gateresid(), ntf.clientid(), ntf.user() );
}

void CGroupManager::GroupMemberOffline(sglib::groupproto::GroupmanagerGroupMemberOfflineNtf &ntf)
{
	SERVER_LOG_INFO( "_GroupmanagerGroupMemberOfflineProc," << ntf.gateresid() << "," <<\
		ntf.clientid() << "," << ntf.groupid() << "," << ntf.user().c_str() );

	_MemberOffline( ntf.groupid(), ntf.gateresid(), ntf.clientid(), ntf.user() );
}

void CGroupManager::CreateGroupGameResult(sglib::groupproto::GroupmanagerGroupCreateGameRoomRsp &rsp)
{
	SERVER_LOG_INFO( "_GroupmanagerGroupCreateGameResultProc," << rsp.result() << "," \
		<< rsp.groupid() << "," << rsp.gateresid() << "," << rsp.clientid() << "," \
		<< rsp.game() << "," << rsp.ip().c_str() << "," << rsp.port() << "," \
		<< rsp.roomid() << "," << rsp.password().c_str() );

	_NotifyCreateGameRoomResult( rsp.gateresid(), rsp.clientid(),
		rsp.result(), rsp.groupid(), rsp.game(), rsp.ip(), rsp.port(), rsp.roomid(), rsp.password() );
	if( rsp.result() == sglib::errorcode::E_ErrorCode_Success )
	{
		CGroupInfo *info = CGroupManager::Instance().FindGroup( rsp.groupid() );
		SELF_ASSERT( info, return; );
		CGroupMember *member = info->FindMember( rsp.gateresid(), rsp.clientid() );
		SELF_ASSERT( member, return; );

		sglib::groupproto::SCGroupCreateGameRoomNtf ntf;
		ntf.set_creater( member->GetUser() );
		ntf.set_game( rsp.game() );
		ntf.set_ip( rsp.ip() );
		ntf.set_port( rsp.port() );
		ntf.set_roomid( rsp.roomid() );
		ntf.set_password( rsp.password() );
		ntf.set_groupid( rsp.groupid() );

		CGroupManager::Instance().BroadcastMsgInGroup( 
			rsp.groupid(), ntf, sglib::msgid::SC_GROUP_CREATE_GAMEROOM_NTF );
	}
}

void CGroupManager::DisplayInfo()
{
#if _DEBUG
	printf( "========= Group Info ===========\n" );
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	std::map<u64, CGroupInfo*>::iterator it = m_Groups.begin();
	for( ; it != m_Groups.end(); ++it )
	{
		it->second->DisplayInfo();
		printf( "\n" );
	}
#endif
}

bool CGroupManager::_ReportDynamicStarted()
{
	sglib::groupproto::GroupGroupmanagerDynamicStartNtf ntf;
	
	const CServerConfig *cfg = CServerManager::Instance().GetServerConfig();
	if( !cfg )
	{
		SERVER_LOG_ERROR( "CGroupManager,_ReportDynamicStarted,GetServerConfig" );
		return false;
	}

	const ServerConfig &_cfg = cfg->GetServerConfig();
	ntf.set_id( _cfg.m_Id );
	ntf.set_ip( _cfg.m_Ip );
	ntf.set_port( _cfg.m_Port );
	
	CServerManager::Instance().SendRpcMsg(  
		CServerManager::Instance().GetGroupManagerServerId(),
		ntf, sglib::msgid::GPGM_DYNAMIC_START_NTF );

	return true;
}

void CGroupManager::_NotifyNewMemberJoin(u64 groupid, std::map<s32, std::vector<u64> > &alluser, CGroupMember &newMember)
{
	sglib::groupproto::SCGroupMemberJoinNtf ntf;
	ntf.set_groupid( groupid );
	sglib::publicproto::GroupMemberInfo *member = ntf.mutable_member();
	SG_ASSERT( member != NULL );
	member->set_user( newMember.GetUser() );
	member->set_nickname( newMember.GetNickName() );
	member->set_head( newMember.GetHead() );
	member->set_online( 1 );

	map<s32, vector<u64> >::iterator it = alluser.begin();
	for( ; it != alluser.end(); ++it )
	{
		const CGateClientInfo *pGate = CGateClientManager::Instance().GetGateInfo( it->first );
		if( pGate )
		{
			CGroupClient *client = (CGroupClient*)CServerManager::Instance().FindClient( pGate->m_GateInstanceId );
			SG_ASSERT( client != NULL );
			client->SendMsgToClients(
				it->second,
				ntf,
				sglib::msgid::SC_GROUP_MEMBER_JOIN_NTF );
		}
		else
		{
			SERVER_LOG_WARN( "CGroupManager,_NotifyNewMemberJoin,GetGateInfo," << it->first );
		}
	}
}

void CGroupManager::_NotifyGroupInfoToMember(CGroupInfo &group, vector<string> &vecUser, CGroupMember &member)
{
	const CGateClientInfo *pGate = CGateClientManager::Instance().GetGateInfo( member.GetGateResId() );
	SG_ASSERT( pGate != NULL );
	CGroupClient *client = (CGroupClient*)CServerManager::Instance().FindClient( pGate->m_GateInstanceId );
	SG_ASSERT( client != NULL );

	// 通知玩家群列表更新信息
	sglib::groupproto::SCGroupListUpdate groupListUpdate;
	sglib::publicproto::GroupInfo *info = groupListUpdate.add_groups();
	SG_ASSERT( info != NULL );
	info->set_id( group.GetId() );
	info->set_name( group.GetName() );
	info->set_icon( group.GetIcon() );
	info->set_count( group.MemberCount() );
	client->SendMsgToClient( member.GetClientId(), groupListUpdate, sglib::msgid::SC_GROUP_LIST_UPDATE );

	// 通知玩家群内成员列表信息
	sglib::groupproto::SCGroupInfoUpdate groupInfoUpdate;
	groupInfoUpdate.set_id( group.GetId() );
	vector<string>::iterator it = vecUser.begin();
	for( ; it != vecUser.end(); ++it )
	{
		CGroupMember *member = group.FindMember( it->c_str() );
		if( member )
		{
			sglib::publicproto::GroupMemberInfo *info = groupInfoUpdate.add_members();
			SG_ASSERT( info != NULL );
			info->set_user( member->GetUser() );
			info->set_nickname( member->GetNickName() );
			info->set_head( member->GetHead() );
			info->set_online( member->IsOnline() ? 1 : 0 );
		}
		else
		{
			// 有可能此玩家正好离开群
			SERVER_LOG_WARN( "CGroupManager,_NotifyGroupInfoToNewMember,FindMember," << it->c_str() );
		}
	}
	client->SendMsgToClient( member.GetClientId(), groupInfoUpdate, sglib::msgid::SC_GROUP_INFO_UPDATE );
}

s32 CGroupManager::_CheckLoginValid(const std::string &user, const std::string &token)
{
	// TODO
	return sglib::errorcode::E_ErrorCode_Success;
}

void CGroupManager::_NotifyManagerUserLogin(s32 gateResId, u64 clientid, const string &user)
{
    sglib::groupproto::GroupGroupmanagerUserLoginNtf ntf;
    ntf.set_gateresid( gateResId );
    ntf.set_clientid( clientid );
    ntf.set_user( user );
		
    CServerManager::Instance().SendRpcMsg( 
        CServerManager::Instance().GetGroupManagerServerId(),
        ntf, sglib::msgid::GPGM_USER_LOGIN_NTF );
}

void CGroupManager::_NotifyGroupgateLoginSuccess(CGroupClient &client, u64 clientid)
{
	sglib::groupproto::GroupGateLoginSuccessNtf ntf;
	ntf.set_clientid( clientid );
	client.SendMsg( ntf, sglib::msgid::GPGG_LOGIN_SUCCESS_NTF );
}

void CGroupManager::_NotifyUserLoginResult(CGroupClient &client, s32 result, u64 clientid)
{
    sglib::groupproto::SCGroupUserLoginRsp rsp;
    rsp.set_result( result );
	client.SendMsgToClient( clientid, rsp, sglib::msgid::SC_GROUP_LOGIN_RSP );
}

void CGroupManager::_NotifyManagerCreateGroup(u64 gateid, s32 gateresid, u64 clientid, const string &name)
{
	sglib::groupproto::GroupGroupmanageCreateGroupReq req;
	req.set_gateid( gateid );
	req.set_gateresid( gateresid );
	req.set_clientid( clientid );
	req.set_name( name );
	
	CServerManager::Instance().SendRpcMsg( 
        CServerManager::Instance().GetGroupManagerServerId(),
		req, sglib::msgid::GPGM_GROUP_CREATE_REQ );
}

void CGroupManager::_NotifyAddGroupMemberRsp(CGroupClient &client, u64 clientid, u64 groupid, s32 result)
{
	sglib::groupproto::SCGroupAddMemberRsp rsp;
	rsp.set_groupid( groupid );
	rsp.set_result( result );
	client.SendMsgToClient( clientid, rsp, sglib::msgid::SC_GROUP_ADD_MEMBER_RSP );
}

void CGroupManager::_NotifyManagerAddMemberToGroup(s32 gateresid, u64 gateid, u64 clientid, u64 groupid, std::vector<std::string> &vecUser)
{
	sglib::groupproto::GroupGroupmanagerAddMemberToGroupReq req;
	req.set_gateid( gateid );
	req.set_gateresid( gateresid );
	req.set_clientid( clientid );
	req.set_groupid( groupid );
	vector<string>::iterator it = vecUser.begin();
	for( ; it != vecUser.end(); ++it )
	{
		string *user = req.add_users();
		*user = *it;
	}
	
	CServerManager::Instance().SendRpcMsg( 
        CServerManager::Instance().GetGroupManagerServerId(),
		req, sglib::msgid::GPGM_GROUP_ADD_MEMBER_REQ );
}

void CGroupManager::_AskManagerInfoToAgree(s32 gateResId, u64 clientid, u64 groupid)
{
	sglib::groupproto::GroupGroupmanagerForAgreeJoinAskInfoReq req;
	req.set_gateresid( gateResId );
	req.set_clientid( clientid );
	req.set_groupid( groupid );
	
	CServerManager::Instance().SendRpcMsg( 
        CServerManager::Instance().GetGroupManagerServerId(),
		req, sglib::msgid::GPGM_AGREE_ASK_MEMBER_INFO_REQ );
}

void CGroupManager::_DoMemberLeaveGroup(CGroupClient &client, s32 gateResId, u64 clientid, u64 groupid)
{
	s32 result = sglib::errorcode::E_ErrorCode_Success;
		
	vector<string> leaveUser;
	CGroupInfo *group = CGroupManager::Instance().FindGroup( groupid );
	if( group )
	{
		CGroupMember *member = group->FindMember(
			gateResId, clientid );
		if( member )
		{
			leaveUser.push_back( member->GetUser() );
			member->SetOnline( false, member->GetGateResId(), member->GetClientId() );
			group->DelMember( member->GetUser().c_str() );
		}
		else
		{
			SERVER_LOG_ERROR( "CGroupManager,_DoMemberLeaveGroup,FindMember," << gateResId << "," << clientid );
			result = sglib::errorcode::E_ErrorCode_NotInThisGroup;
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManager,_DoMemberLeaveGroup,FindGroup," << groupid );
		result = sglib::errorcode::E_ErrorCode_NotFoundGroup;
	}

	_NotifyMemberLeaveGroupRsp( client, clientid, result, groupid );
	if( result == sglib::errorcode::E_ErrorCode_Success )
	{
		_NotifyGroupManagerMemberLeaveGroup( groupid, leaveUser );

		sglib::groupproto::SCGroupMemberLeaveNtf ntf;
		ntf.set_groupid( groupid );
		SG_ASSERT( leaveUser.size() == 1 );
		ntf.set_user( leaveUser[0] );
		CGroupManager::Instance().BroadcastMsgInGroup(
			groupid,
			ntf,
			sglib::msgid::SC_GROUP_MEMBER_LEAVE_NTF );
	}
}

void CGroupManager::_NotifyMemberLeaveGroupRsp(CGroupClient &client, u64 clientid, s32 result, u64 groupid)
{
	sglib::groupproto::SCGroupLeaveRsp rsp;
	rsp.set_result( result );
	rsp.set_groupid( groupid );

	client.SendMsgToClient( clientid, rsp, sglib::msgid::SC_GROUP_LEAVE_RSP );
}

void CGroupManager::_NotifyGroupManagerMemberLeaveGroup(u64 groupid, const vector<string> &user)
{
	// 分批发
	const int MAX_SEND = 100;
	for( s32 i=0; i<(s32)user.size(); )
	{
		sglib::groupproto::GroupGroupmanagerMemberLeaveNtf ntf;
		ntf.set_groupid( groupid );

		s32 count = 0;
		for( s32 j=i; j<(s32)user.size() && count<MAX_SEND; ++j, ++count )
		{
			string *pUser = ntf.add_user();
			SG_ASSERT( pUser );
			*pUser = user[j];
		}

		i += count;

		CServerManager::Instance().SendRpcMsg( 
			CServerManager::Instance().GetGroupManagerServerId(),
			ntf, sglib::msgid::GPGM_GROUP_MEMBER_LEAVE_NTF );
	}
}

void CGroupManager::_DoMemberDeleteGroup(CGroupClient &client, s32 gateResId, u64 clientid, u64 groupid)
{
	s32 result = sglib::errorcode::E_ErrorCode_Success;
		
	map<s32, vector<u64> > alluser;
	vector<string> vecUser;
	CGroupInfo *group = CGroupManager::Instance().FindGroup( groupid );
	if( group )
	{
		group->DumpMemberInfo(  alluser, "", vecUser );
		CGroupMember *member = group->FindMember(
			gateResId, clientid );
		if( !member )
		{
			SERVER_LOG_ERROR( "CGroupManager,_DoMemberDeleteGroup,FindMember," << gateResId << "," << clientid );
			result = sglib::errorcode::E_ErrorCode_NotInThisGroup;
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupClient,_DoMemberDeleteGroup,FindGroup," << groupid );
		result = sglib::errorcode::E_ErrorCode_NotFoundGroup;
	}

	_NotifyMemberDeleteGroupRsp( client, clientid, result, groupid );
	if( result == sglib::errorcode::E_ErrorCode_Success )
	{
		sglib::groupproto::SCGroupDeleteNtf ntf;
		ntf.set_groupid( groupid );
		CGroupManager::Instance().BroadcastMsgInGroup(
			groupid,
			ntf,
			sglib::msgid::SC_GROUP_DELETE_NTF );

		CGroupManager::Instance().DelGroup( groupid );

		_NotifyGroupGateDeleteGroup( groupid );
		_NotifyGroupManagerMemberLeaveGroup( groupid, vecUser );
		_NotifyGroupManagerDeleteGroup( groupid );
	}
}

void CGroupManager::_NotifyMemberDeleteGroupRsp(CGroupClient &client, u64 clientid, s32 result, u64 groupid)
{
	sglib::groupproto::SCGroupDeleteRsp rsp;
	rsp.set_result( result );
	rsp.set_groupid( groupid );

	client.SendMsgToClient( clientid, rsp, sglib::msgid::SC_GROUP_DELETE_RSP );
}

void CGroupManager::_NotifyGroupGateDeleteGroup(u64 groupid)
{
	sglib::groupproto::GroupGateDelGroupNtf ntf;
	ntf.set_groupid( groupid );

	CGroupManager::Instance().BroadcastMsgToGates(
		ntf,
		sglib::msgid::GPGG_DEL_GROUPID_NTF );
}

void CGroupManager::_NotifyGroupManagerDeleteGroup(u64 groupid)
{
	sglib::groupproto::GroupGroupmanagerGroupDeleteNtf ntf;
	ntf.set_groupid( groupid );
	
	CServerManager::Instance().SendRpcMsg( 
		CServerManager::Instance().GetGroupManagerServerId(),
		ntf, sglib::msgid::GPGM_GROUP_DELETE_NTF );
}

void CGroupManager::_NotifyGroupManagerCreateGameRoom(s32 gateResId, u64 clientid, u64 groupid, s32 game)
{
	sglib::groupproto::GroupGroupmanagerCreateGameRoomReq req;
	req.set_gateresid( gateResId );
	req.set_clientid( clientid );
	req.set_groupid( groupid );
	req.set_game( game );
	
	CServerManager::Instance().SendRpcMsg( 
		CServerManager::Instance().GetGroupManagerServerId(),
		req, sglib::msgid::GPGM_GROUP_CREATE_GAMEROOM_REQ );
}

void CGroupManager::_NotifyGroupmanagerCreateGroupResult(s32 result, u64 gateid, u64 clientid, const char *user, const char *name, u64 groupid, u64 serverid)
{
	sglib::groupproto::GroupGroupmanagerCreateGroupResult ret;
	ret.set_result( result );
	ret.set_gateid( gateid );
	ret.set_clientid( clientid );
	ret.set_user( user );
	ret.set_name( name );
	ret.set_groupid( groupid );
	ret.set_groupserverid( serverid );
	ret.set_serverid( CServerManager::Instance().ServerId() );

	CServerManager::Instance().SendRpcMsg(
		CServerManager::Instance().GetGroupManagerServerId(),
		ret,
		sglib::msgid::GPGM_GROUP_CREATE_RESULT );
}

void CGroupManager::_NotifyClientCreateGroupRsp(s32 result, u64 gateid, u64 clientid, const char *name, u64 groupid)
{
	sglib::groupproto::SCGroupCreateRsp rsp;
	rsp.set_result( result );
	rsp.set_name( name );
	rsp.set_groupid( groupid );

	CGroupManager::Instance().SendMsgToClient(
		gateid, clientid, rsp, sglib::msgid::SC_GROUP_CREATE_RSP );
}

#if _DEBUG
static void _DebugLoadGroup(u64 groupid, s32 gateresid, u64 clientid, const string &user)
{
	char groupName[32] = {0};
	sprintf( groupName, "gp_%llu", groupid );
	CGroupInfo *group = CGroupManager::Instance().AddGroup( groupid, groupName, "" );
	if( !group )
	{
		SGDEBUG( "_DebugLoadGroup addgroup:%llu failed\n", groupid );
		return;
	}

	if( groupid == 1 )
	{
		group->AddMember( "sjj1", "sjj1", "head1", false );
		group->AddMember( "sjj2", "sjj2", "head2", false );
		group->AddMember( "sjj3", "sjj3", "head3", false );
	}
	else if( groupid == 2 )
	{
		group->AddMember( "sjj1", "sjj1", "head1", false );
		group->AddMember( "sjj2", "sjj2", "head2", false );
	}
	else if( groupid == 3 )
	{
		group->AddMember( "sjj1", "sjj1", "head1", false );
		group->AddMember( "sjj3", "sjj3", "head3", false );
	}

	CGroupMember *member = group->FindMember( user.c_str() );
	if( !member )
	{
		SGDEBUG( "_DebugLoadGroup findMember:%s in group:%llu failed\n", user.c_str(), groupid );
		return;
	}

	member->SetOnline( true, gateresid, clientid );
}
#endif
s32 CGroupManager::_DoLoadGroup(u64 groupid, s32 gateresid, u64 clientid, const char *user)
{
	// 注意此group有可能已经加载了

	s32 ret = sglib::errorcode::E_ErrorCode_Success;
	CGroupInfo *group = CGroupManager::Instance().FindGroup( groupid );
	if( !group )
	{
		// 从DB加载一个群的完整信息是一个异步的过程,
		// 这里是测试，因此是同步处理
#if _DEBUG
		_DebugLoadGroup( groupid, gateresid, clientid, string(user) );
#endif
	}
	else
	{
		SERVER_LOG_DEBUG( "CGroupManager,_DoLoadGroup,AlreadyLoad," << groupid );
	}

	return ret;
}

void CGroupManager::_NotifyLoadGroupResult(s32 result, u64 groupid, s32 serverid, const char *user)
{
	sglib::groupproto::GroupGroupmanagerLoadGroupResult ret;
	ret.set_result( result );
	ret.set_groupid( groupid );
	ret.set_serverid( serverid );
	ret.set_user( user );
	ret.set_totalgroup( CGroupManager::Instance().TotalGroupCount() );	
	ret.set_totalmember( CGroupManager::Instance().TotalMemberCount() );	

	CServerManager::Instance().SendRpcMsg(
		CServerManager::Instance().GetGroupManagerServerId(),
		ret,
		sglib::msgid::GPGM_GROUP_LOAD_RESULT );
}

void CGroupManager::_MemberOnline(u64 groupid, s32 gateresid, u64 clientid, const std::string &user)
{
	CGroupInfo *groupInfo = CGroupManager::Instance().FindGroup( groupid );
	SG_ASSERT( groupInfo != NULL );
	CGroupMember *member = groupInfo->FindMember( user.c_str() );
	if( !member )
	{
		SERVER_LOG_ERROR( "CGroupManager,_MemberOnline,FindMember," << groupid << "," << user.c_str() );
		return;
	}

	member->SetOnline( true, gateresid, clientid );

	// 通知玩家群信息
	CGroupManager::Instance().NotifyGroupInfoToMember(
		groupid, user );

	// 群内广播玩家上线
	sglib::groupproto::SCGroupMemberOnlineNtf ntf;
	ntf.set_groupid( groupid );
	ntf.set_user( user );
	BroadcastMsgInGroup( groupid, ntf, sglib::msgid::SC_GROUP_MEMBER_ONLINE );
}

void CGroupManager::_MemberOffline(u64 groupid, s32 gateresid, u64 clientid, const std::string &user)
{
	CGroupInfo *groupInfo = CGroupManager::Instance().FindGroup( groupid );
	SG_ASSERT( groupInfo != NULL );
	CGroupMember *member = groupInfo->FindMember( user.c_str() );
	if( !member )
	{
		SERVER_LOG_ERROR( "CGroupManager,_MemberOffline,FindMember," << groupid << "," << user.c_str() );
		return;
	}

	member->SetOnline( false, gateresid, clientid );

	sglib::groupproto::SCGroupMemberOfflineNtf ntf;
	ntf.set_groupid( groupid );
	ntf.set_user( user );
	BroadcastMsgInGroup( groupid, ntf, sglib::msgid::SC_GROUP_MEMBER_OFFLINE );
}

void CGroupManager::_NotifyCreateGameRoomResult(s32 gateresid, u64 clientid, s32 result, u64 groupid, s32 game, const std::string &ip, s32 port, s32 roomid, const std::string &roompwd)
{
	sglib::groupproto::SCGroupCreateGameRoomRsp rsp;
	rsp.set_result( result );
	rsp.set_groupid( groupid );
	rsp.set_game( game );
	rsp.set_ip( ip );
	rsp.set_port( port );
	rsp.set_roomid( roomid );
	rsp.set_password( roompwd );

	CGroupManager::Instance().SendMsgToClient( 
		gateresid, clientid, rsp, sglib::msgid::SC_GROUP_CREATE_GAMEROOM_RSP );
}

void CGroupManager::_LoadGroupDone(u64 groupid, const std::string &user)
{
	_NotifyLoadGroupResult(
		sglib::errorcode::E_ErrorCode_Success, 
		groupid,
		CServerManager::Instance().ServerId(),
		user.c_str() );

	// 先通知所有的group gate更新信息
	CGroupManager::Instance().NotifyAllGroupGateAddGroupInfo(
		groupid, CServerManager::Instance().ServerId() );

	// 到这里表示加载群成功了,通知client群信息更新
	CGroupManager::Instance().NotifyGroupInfoToMember(
		groupid, user );
}

void CGroupManager::_GetGroupInfoCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len)
{
	SELF_ASSERT( param, return; );
	SELF_ASSERT( sizeof(_groupManagerDBParam)==len, return; );
	_groupManagerDBParam *_param = (_groupManagerDBParam*)param;
	string user = string( ((char*)_param->data) );
	u64 groupid = _param->groupid;
	
	string groupname="", grouphead="";
	while( RecordSet && RecordSet->GetRecord() )
	{
		// user,name,head,ismaster
		const char *val = RecordSet->GetFieldValue( 1 );
		if( val )
		{
			groupname = val;
		}

		val = RecordSet->GetFieldValue( 2 );
		if( val )
		{
			grouphead = val;
		}
		break;
	}
	
	// 在找一下是否存在此群了
	CGroupInfo *group = CGroupManager::Instance().FindGroup( groupid );
	if( !group )
	{
		group = CGroupManager::Instance().AddGroup( groupid, groupname.c_str(), grouphead.c_str() );
		if( !group )
		{
			SERVER_LOG_ERROR( "_GetGroupInfoCallback,AddGroup," << groupid << "," << user.c_str() );
			return;
		}
	}
	
	// 如果此群已经存在了，可能有两种情况,一个是正在加载群成员列表，还一个是已经加载完了
	if( group->MemberCount() > 0 )
	{
		SAFE_DELETE( _param->data );
		_LoadGroupDone( groupid, user );
		return;
	}

	_groupManagerDBParam dbparam = { groupid, _param->data };
	string sql = string("select user,name,head,ismaster from ") +
		CServerManager::Instance().BuildGroupTableName( groupid ) + ";";
	s32 id = CServerManager::Instance().GetGroupDbId();
	bool ret = CServerManager::Instance().ExecSql( 
		id, sql, this, &CGroupManager::_GetGroupMemberCallback, &dbparam, sizeof(dbparam) );
	if( !ret )
	{
		SERVER_LOG_ERROR( "CGroupManager,_GetGroupInfoCallback,ExecSql," << user.c_str() << "," << sql.c_str() );
	}
}

void CGroupManager::_GetGroupMemberCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len)
{
	SELF_ASSERT( param, return; );
	SELF_ASSERT( sizeof(_groupManagerDBParam)==len, return; );
	_groupManagerDBParam *_param = (_groupManagerDBParam*)param;
	string user = string( ((char*)_param->data) );
	u64 groupid = _param->groupid;
	SAFE_DELETE( _param->data );
	
	// 在找一下是否存在此群了
	CGroupInfo *group = CGroupManager::Instance().FindGroup( groupid );
	if( group )
	{
		if( group->MemberCount() > 0 )
		{
			_LoadGroupDone( groupid, user );
		}
		return;
	}
	
	// 添加群 TODO
	group = CGroupManager::Instance().AddGroup( groupid, "", "" );
	if( !group )
	{
		SERVER_LOG_ERROR( "_GetGroupDataCallback,AddGroup," << groupid << "," << user.c_str() );
		return;
	}
	while( RecordSet && RecordSet->GetRecord() )
	{
		// user,name,head,ismaster
		string member = "";
		const char *val = RecordSet->GetFieldValue( 1 );
		if( val )
		{
			member = val;
		}

		string name = "";
		val = RecordSet->GetFieldValue( 2 );
		if( val )
		{
			name = val;
		}

		string head = "";
		val = RecordSet->GetFieldValue( 3 );
		if( val )
		{
			head = val;
		}

		s32 ismaster = 0;
		val = RecordSet->GetFieldValue( 4 );
		if( val )
		{
			ismaster = atoi( val );
		}

		group->AddMember( member.c_str(), name.c_str(), head.c_str(), false );
		SERVER_LOG_DEBUG( "_GetGroupDataCallback group:" << groupid << " add member:(" <<\
			member.c_str() << "," << name.c_str() << "," << head.c_str() << "," << ismaster << ")" );
	}
		
	_LoadGroupDone( groupid, user );
}
