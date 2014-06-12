#include "GroupRpcClient.h"
#include "ServerManager.h"
#include "GroupClient.h"
#include "GroupManager.h"
#include "GateClientManager.h"
#include "msgid.pb.h"
#include "group.pb.h"
#include "errno.pb.h"

CGroupRpcClient::CGroupRpcClient(s32 nId) : 
	CRpcClient(nId)
{
	_RegisterProc( sglib::msgid::GMGP_DYNAMIC_SERVERSTART_NTF, 
		&CGroupRpcClient::_GroupmanagerServerDynamicServerStart );
	_RegisterProc( sglib::msgid::GMGP_GROUP_CREATE_NTF, 
		&CGroupRpcClient::_GroupmanagerGroupCreateProc );
	_RegisterProc( sglib::msgid::GMGP_GROUP_CREATE_RSP, 
		&CGroupRpcClient::_GroupmanagerGroupCreateRspProc );
	_RegisterProc( sglib::msgid::GMGP_GROUP_ADD_MEMBER_RSP, 
		&CGroupRpcClient::_GroupmanagerGroupAddMemberRspProc );
	_RegisterProc( sglib::msgid::GMGP_AGREE_ASK_MEMBER_INFO_RSP, 
		&CGroupRpcClient::_GroupmanagerGroupAgreeJoinAskInfoRspProc );
	_RegisterProc( sglib::msgid::GMGP_GROUP_LOAD_NTF, 
		&CGroupRpcClient::_GroupmanagerGroupLoadGroupProc );
	_RegisterProc( sglib::msgid::GMGP_GROUP_MEMBER_ONLINE_NTF, 
		&CGroupRpcClient::_GroupmanagerGroupMemberOnlineProc );
	_RegisterProc( sglib::msgid::GMGP_GROUP_MEMBER_OFFLINE_NTF, 
		&CGroupRpcClient::_GroupmanagerGroupMemberOfflineProc );
	_RegisterProc( sglib::msgid::GMGP_GROUP_CREATE_GAMEROOM_RSP, 
		&CGroupRpcClient::_GroupmanagerGroupCreateGameResultProc );
}

CGroupRpcClient::~CGroupRpcClient()
{
}

void CGroupRpcClient::OnRecv(const byte *pkg, s32 len)
{
	s32 msgId = *((s32*)pkg);

	map<s32, ProtoProc>::iterator it = m_mapProtoProc.find( msgId );
	if( it != m_mapProtoProc.end() )
	{
		(this->*it->second)( pkg+MSG_ID_LEN, len-MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,OnRecv," << GetId() << "," << msgId );
	}
}

void CGroupRpcClient::_RegisterProc(s32 id, ProtoProc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}

void CGroupRpcClient::_SendMsgToClient(u64 gateid, u64 userid, const ::google::protobuf::Message &msg, s32 msgid)
{
	CClient *client = CServerManager::Instance().FindClient( gateid );
	if( client != NULL )
	{
		CGroupClient *cli = (CGroupClient*)client;
		cli->SendMsgToClient( userid, msg, msgid );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_SendMsgToClient," << gateid );
	}
}

void CGroupRpcClient::_GroupmanagerServerDynamicServerStart(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupmanagerGroupDynamicServerStartNtf ntf;
	if( ntf.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "DynamicServerStart," << ntf.id() << "," <<\
			ntf.ip().c_str() << "," << ntf.port() );

		sglib::gateproto::ServerGateConnectNewServerNtf cmd;
		cmd.set_id( ntf.id() );
		cmd.set_ip( ntf.ip() );
		cmd.set_port( ntf.port() );

		// 将此消息广播给其所有的gate
		CGroupManager::Instance().BroadcastMsgToGates( 
			cmd, sglib::msgid::SG_CONNECT_NEW_SERVER_NTF );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_GroupmanagerServerDynamicServerStart,ParseFromArray" );
	}
}

void CGroupRpcClient::_GroupmanagerGroupCreateProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupmanageGroupCreateGroupNtf ntf;
	if( ntf.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "CreateGroupNotify," << ntf.gateid() << "," <<\
			ntf.gateresid() << "," << ntf.clientid() << "," << ntf.user().c_str() << "," <<\
			ntf.name().c_str() << "," << ntf.groupid() <<\
			"," << ntf.groupserverid() );

		s32 result = sglib::errorcode::E_ErrorCode_Success;
		CGroupInfo *group = CGroupManager::Instance().AddGroup(
			ntf.groupid(), ntf.name().c_str(), "" );
		if( group == NULL )
		{
			result = sglib::errorcode::E_ErrorCode_CreateGroupError;
			SERVER_LOG_ERROR( "CreateGroup," << ntf.groupid() << "," << ntf.name().c_str() );
		}
		else
		{
			// TODO

			CGroupMember *member = group->AddMember( 
				ntf.user().c_str(), 
				ntf.user().c_str(), 
				"", 
				true );
			SG_ASSERT( member != NULL );
			member->SetOnline( true, ntf.gateresid(), ntf.clientid() );
		}

		_NotifyGroupmanagerCreateGroupResult( 
			result,
			ntf.gateid(), ntf.clientid(), ntf.user().c_str(),
			ntf.name().c_str(), ntf.groupid(), ntf.groupserverid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_GroupmanagerGroupCreateProc,ParseFromArray" );
	}
}

void CGroupRpcClient::_GroupmanagerGroupCreateRspProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupmanagerGroupCreateGroupRsp rsp;
	if( rsp.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "CreateGroupRsp," << rsp.result() << "," << rsp.gateid() << "," <<\
			rsp.clientid() << "," << rsp.name().c_str() << "," << rsp.groupid() << "," << rsp.serverid() );

		if( rsp.result() == sglib::errorcode::E_ErrorCode_Success )
		{
			// 创建成功，先通知所有gate
			CGroupManager::Instance().NotifyAllGroupGateAddGroupInfo(
				rsp.groupid(), rsp.serverid() );
		}

		_NotifyClientCreateGroupRsp(
			rsp.result(), rsp.gateid(), rsp.clientid(),
			rsp.name().c_str(), rsp.groupid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_GroupmanagerGroupCreateRspProc,ParseFromArray" );
	}
}

void CGroupRpcClient::_GroupmanagerGroupAddMemberRspProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupmanagerGroupAddMemberToGroupRsp rsp;
	if( rsp.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "AddMemberToGroupRsp," << rsp.gateid() << "," << rsp.gateresid() << "," <<\
			rsp.clientid() << "," << rsp.groupid() << "," << rsp.users_size() );

		// 先通知原请求的玩家
		CGroupClient *client = (CGroupClient*)CServerManager::Instance().FindClient( rsp.gateid() );
		SG_ASSERT( client != NULL );
		client->NotifyAddGroupMemberRsp( rsp.clientid(), rsp.groupid(), sglib::errorcode::E_ErrorCode_Success );

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
	else
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_GroupmanagerGroupAddMemberRspProc,ParseFromArray" );
	}
}

void CGroupRpcClient::_GroupmanagerGroupAgreeJoinAskInfoRspProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupmanagerGroupForAgreeJoinAskInfoRsp rsp;
	if( rsp.ParseFromArray(pkg, len) )
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
	else
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_GroupmanagerGroupAgreeJoinAskInfoRspProc,ParseFromArray" );
	}
}

void CGroupRpcClient::_GroupmanagerGroupLoadGroupProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupmanagerGroupLoadGroupNtf ntf;
	if( ntf.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "LoadGroup," << ntf.gateresid() << "," <<\
			ntf.clientid() << "," << ntf.groupid() << "," << ntf.user().c_str() );

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
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_GroupmanagerGroupLoadGroupProc,ParseFromArray" );
	}
}

void CGroupRpcClient::_GroupmanagerGroupMemberOnlineProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupmanagerGroupMemberOnlineNtf ntf;
	if( ntf.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "_GroupmanagerGroupMemberOnlineProc," << ntf.gateresid() << "," <<\
			ntf.clientid() << "," << ntf.groupid() << "," << ntf.user().c_str() );

		_MemberOnline( ntf.groupid(), ntf.gateresid(), ntf.clientid(), ntf.user() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_GroupmanagerGroupMemberOnlineProc,ParseFromArray" );
	}
}

void CGroupRpcClient::_GroupmanagerGroupMemberOfflineProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupmanagerGroupMemberOfflineNtf ntf;
	if( ntf.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "_GroupmanagerGroupMemberOfflineProc," << ntf.gateresid() << "," <<\
			ntf.clientid() << "," << ntf.groupid() << "," << ntf.user().c_str() );
		
		_MemberOffline( ntf.groupid(), ntf.gateresid(), ntf.clientid(), ntf.user() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_GroupmanagerGroupMemberOfflineProc,ParseFromArray" );
	}
}

void CGroupRpcClient::_GroupmanagerGroupCreateGameResultProc(const byte *pkg, s32 len)
{
	sglib::groupproto::GroupmanagerGroupCreateGameRoomRsp rsp;
	if( rsp.ParseFromArray(pkg, len) )
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
	else
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_GroupmanagerGroupMemberOfflineProc,ParseFromArray" );
	}
}

void CGroupRpcClient::_NotifyGroupmanagerCreateGroupResult(s32 result, u64 gateid, u64 clientid, const char *user, const char *name, u64 groupid, u64 serverid)
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

void CGroupRpcClient::_NotifyClientCreateGroupRsp(s32 result, u64 gateid, u64 clientid, const char *name, u64 groupid)
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
s32 CGroupRpcClient::_DoLoadGroup(u64 groupid, s32 gateresid, u64 clientid, const char *user)
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
		SERVER_LOG_DEBUG( "CGroupRpcClient,_DoLoadGroup,AlreadyLoad," << groupid );
	}

	return ret;
}

void CGroupRpcClient::_NotifyLoadGroupResult(s32 result, u64 groupid, s32 serverid, const char *user)
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

void CGroupRpcClient::_MemberOnline(u64 groupid, s32 gateresid, u64 clientid, const std::string &user)
{
	CGroupInfo *groupInfo = CGroupManager::Instance().FindGroup( groupid );
	SG_ASSERT( groupInfo != NULL );
	CGroupMember *member = groupInfo->FindMember( user.c_str() );
	if( !member )
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_MemberOnline,FindMember," << groupid << "," << user.c_str() );
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
	CGroupManager::Instance().BroadcastMsgInGroup( groupid, ntf, sglib::msgid::SC_GROUP_MEMBER_ONLINE );
}

void CGroupRpcClient::_MemberOffline(u64 groupid, s32 gateresid, u64 clientid, const std::string &user)
{
	CGroupInfo *groupInfo = CGroupManager::Instance().FindGroup( groupid );
	SG_ASSERT( groupInfo != NULL );
	CGroupMember *member = groupInfo->FindMember( user.c_str() );
	if( !member )
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_MemberOffline,FindMember," << groupid << "," << user.c_str() );
		return;
	}

	member->SetOnline( false, gateresid, clientid );

	sglib::groupproto::SCGroupMemberOfflineNtf ntf;
	ntf.set_groupid( groupid );
	ntf.set_user( user );
	CGroupManager::Instance().BroadcastMsgInGroup( groupid, ntf, sglib::msgid::SC_GROUP_MEMBER_OFFLINE );
}

void CGroupRpcClient::_NotifyCreateGameRoomResult(s32 gateresid, u64 clientid, s32 result, u64 groupid, s32 game, const std::string &ip, s32 port, s32 roomid, const std::string &roompwd)
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
