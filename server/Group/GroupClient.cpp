#include "GroupClient.h"
#include "ServerManager.h"
#include "GroupManager.h"
#include "msgid.pb.h"
#include "group.pb.h"
#include "errno.pb.h"
using namespace std;

CGroupClient::CGroupClient(s32 nId) : CGateClientBase(nId)
{
    _RegisterClientProc( sglib::msgid::CS_GROUP_LOGIN_REQ,      &CGroupClient::_GroupUserLoginProc );
	_RegisterClientProc( sglib::msgid::CS_GROUP_CREATE_REQ,     &CGroupClient::_GroupCreateProc );
	_RegisterClientProc( sglib::msgid::CS_GROUP_SEARCH_REQ,     &CGroupClient::_GroupSearchProc );
	_RegisterClientProc( sglib::msgid::CS_GROUP_JOIN_REQ,       &CGroupClient::_GroupJoinProc );
	_RegisterClientProc( sglib::msgid::CS_GROUP_ADD_MEMBER_REQ, &CGroupClient::_GroupAddMemberProc );
	_RegisterClientProc( sglib::msgid::CS_GROUP_AGREE_JOIN_REQ, &CGroupClient::_GroupAgreeJoinProc );
	_RegisterClientProc( sglib::msgid::CS_GROUP_LEAVE_REQ,      &CGroupClient::_GroupLeaveProc );
	_RegisterClientProc( sglib::msgid::CS_GROUP_DELETE_REQ,     &CGroupClient::_GroupDeleteProc );
	_RegisterClientProc( sglib::msgid::CS_GROUP_MESSAGE_REQ,    &CGroupClient::_GroupMessageProc );
	_RegisterClientProc( sglib::msgid::CS_GROUP_CREATE_GAMEROOM_REQ, &CGroupClient::_GroupCreateGameRoomProc );
}

void CGroupClient::ProcPlayerMessage(u64 gateid, u64 clientid, s32 msgid, const byte *pkg, s32 len)
{
	map<int, ClientProtoProcFunc>::iterator it = m_mapProtoProc.find( msgid );
	if( it != m_mapProtoProc.end() )
	{
		(this->*it->second)(
			clientid,
			pkg, 
			len);
	}
}

void CGroupClient::OnClientClose(u64 clientid)
{
	CGateClientBase::OnClientClose( clientid );
    
    _NotifyManagerUserLogout( GateResId(), clientid );
}

void CGroupClient::OnClose()
{
	CGateClientBase::OnClose();
	
	_NotifyGateClose();
}

void CGroupClient::OnAccept()
{
	CGateClientBase::OnAccept();
	CGroupManager::Instance().SetGroupReady();
}

void CGroupClient::NotifyAddGroupMemberRsp(u64 clientid, u64 groupid, s32 result)
{
	sglib::groupproto::SCGroupAddMemberRsp rsp;
	rsp.set_groupid( groupid );
	rsp.set_result( result );
	SendMsgToClient( clientid, rsp, sglib::msgid::SC_GROUP_ADD_MEMBER_RSP );
}

void CGroupClient::_RegisterClientProc(s32 id, ClientProtoProcFunc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}

void CGroupClient::_NotifyGateClose()
{
	sglib::groupproto::GroupGroupmanagerGateCloseNtf ntf;
	ntf.set_gateid( m_GateId );

	CServerManager::Instance().SendRpcMsg( 
		CServerManager::Instance().GetGroupManagerServerId(),
		ntf, sglib::msgid::GPGM_GP_GATE_CLOSE_NTF );
}

void CGroupClient::_GroupUserLoginProc(u64 clientid, const byte *pkg, s32 len)
{
    sglib::groupproto::CSGroupUserLoginReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "GroupUserLogin,"<< GateResId() << "," << clientid << "," << req.user().c_str() << "," << req.token().c_str() );
        
		s32 result = _CheckLoginValid( req.user().c_str() );
        if( result == sglib::errorcode::E_ErrorCode_Success )
        {
            _NotifyManagerUserLogin( GateResId(), clientid, req.user().c_str() );
			_NotifyGroupgateLoginSuccess( clientid );
        }
        _NotifyUserLoginResult( result, clientid );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupClient,_GroupUserLoginProc,ParseFromArray" );
	}
}

void CGroupClient::_GroupCreateProc(u64 clientid, const byte *pkg, s32 len)
{
    sglib::groupproto::CSGroupCreateReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_DEBUG( "GroupCreateReq,"<< GateResId() << "," << clientid << "," << req.name().c_str() );
        
        _NotifyManagerCreateGroup( GetClientId(), clientid, req.name().c_str() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupClient,_GroupUserLoginProc,ParseFromArray" );
	}
}

void CGroupClient::_GroupSearchProc(u64 clientid, const byte *pkg, s32 len)
{
}

void CGroupClient::_GroupJoinProc(u64 clientid, const byte *pkg, s32 len)
{
}

void CGroupClient::_GroupAddMemberProc(u64 clientid, const byte *pkg, s32 len)
{
	sglib::groupproto::CSGroupAddMemberReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "GroupAddMember,"<< GateResId() << "," << clientid << "," <<\
			req.groupid() << "," << req.users_size() );

		CGroupInfo *group = CGroupManager::Instance().FindGroup( req.groupid() );
		if( !group )
		{
			NotifyAddGroupMemberRsp( clientid, req.groupid(), sglib::errorcode::E_ErrorCode_NotFoundGroup );
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
				SERVER_LOG_DEBUG( "GroupAddMember," << req.users(i) << ",AleardyInGroup" );
			}
		}
#if _DEBUG
		SERVER_LOG_DEBUG( "GroupAddMember," << allusers.c_str() );
#endif

		_NotifyManagerAddMemberToGroup( clientid, req.groupid(), vecUser );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupClient,_GroupAddMemberProc,ParseFromArray" );
	}
}

void CGroupClient::_GroupAgreeJoinProc(u64 clientid, const byte *pkg, s32 len)
{
	sglib::groupproto::CSGroupAgreeJoinReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_INFO( "AgreeJoinGroup,"<< GateResId() << "," << clientid << "," << req.groupid() );
		
		// TODO 检查是否受到邀请

		CGroupInfo *group = CGroupManager::Instance().FindGroup( req.groupid() );
		if( !group )
		{
			SERVER_LOG_ERROR( "CGroupClient,_GroupAgreeJoinProc,FindGroup," << req.groupid() );
			return;
		}

		_AskManagerInfoToAgree( GateResId(), clientid, req.groupid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupClient,_GroupAgreeJoinProc,ParseFromArray" );
	}
}

void CGroupClient::_GroupLeaveProc(u64 clientid, const byte *pkg, s32 len)
{
	sglib::groupproto::CSGroupLeaveReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_DEBUG( "GroupLeave," << GateResId() << "," << clientid <<\
			"," << req.groupid() ); 
		_DoMemberLeaveGroup( GateResId(), clientid, req.groupid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupClient,_GroupLeaveProc,ParseFromArray" );
	}
}

void CGroupClient::_GroupDeleteProc(u64 clientid, const byte *pkg, s32 len)
{
	sglib::groupproto::CSGroupDeleteReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_DEBUG( "GroupDelete," << GateResId() << "," << clientid <<\
			"," << req.groupid() ); 
		_DoMemberDeleteGroup( GateResId(), clientid, req.groupid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupClient,_GroupDeleteProc,ParseFromArray" );
	}
}

void CGroupClient::_GroupMessageProc(u64 clientid, const byte *pkg, s32 len)
{
	sglib::groupproto::CSGroupMessageReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_DEBUG( "GroupMessage," << GateResId() << "," << clientid <<\
			"," << req.groupid() << "," << req.content().c_str() );

		CGroupInfo *group = CGroupManager::Instance().FindGroup( req.groupid() );
		if( !group )
		{
			SERVER_LOG_ERROR( "CGroupClient,_GroupMessageProc,FindGroup," << req.groupid() );
			return;
		}

		CGroupMember *member = group->FindMember(
			GateResId(), clientid );
		if( !member )
		{
			SERVER_LOG_ERROR( "CGroupClient,_GroupMessageProc,FindMember," << GateResId() << "," << clientid );
			return;
		}

		CGroupManager::Instance().BroadcastGroupMessage(
			req.groupid(),
			member->GetUser(),
			req.content() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupClient,_GroupMessageProc,ParseFromArray" );
	}
}

void CGroupClient::_GroupCreateGameRoomProc(u64 clientid, const byte *pkg, s32 len)
{
	sglib::groupproto::CSGroupCreateGameRoomReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_DEBUG( "GroupCreateGameRoom," << GateResId() << "," << clientid <<\
			"," << req.groupid() << "," << req.game() );

		CGroupInfo *group = CGroupManager::Instance().FindGroup( req.groupid() );
		if( !group )
		{
			SERVER_LOG_ERROR( "CGroupClient,_GroupCreateGameRoomProc,FindGroup," << req.groupid() );
			return;
		}
		
		_NotifyGroupManagerCreateGameRoom( GateResId(), clientid, req.groupid(), req.game() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupClient,_GroupCreateGameRoomProc,ParseFromArray" );
	}

}

void CGroupClient::_NotifyManagerUserLogin(s32 gateResId, u64 clientid, const char *user)
{
    sglib::groupproto::GroupGroupmanagerUserLoginNtf ntf;
    ntf.set_gateresid( gateResId );
    ntf.set_clientid( clientid );
    ntf.set_user( user );
		
    CServerManager::Instance().SendRpcMsg( 
        CServerManager::Instance().GetGroupManagerServerId(),
        ntf, sglib::msgid::GPGM_USER_LOGIN_NTF );
}

void CGroupClient::_NotifyGroupgateLoginSuccess(u64 clientid)
{
	sglib::groupproto::GroupGateLoginSuccessNtf ntf;
	ntf.set_clientid( clientid );
	SendMsg( ntf, sglib::msgid::GPGG_LOGIN_SUCCESS_NTF );
}

void CGroupClient::_NotifyUserLoginResult(s32 result, u64 clientid)
{
    sglib::groupproto::SCGroupUserLoginRsp rsp;
    rsp.set_result( result );
	SendMsgToClient( clientid, rsp, sglib::msgid::SC_GROUP_LOGIN_RSP );
}

void CGroupClient::_NotifyManagerUserLogout(s32 gateResId, u64 clientid)
{
    sglib::groupproto::GroupGroupmanagerUserLogoutNtf ntf;
    ntf.set_gateresid( gateResId );
    ntf.set_clientid( clientid );
		
    CServerManager::Instance().SendRpcMsg( 
        CServerManager::Instance().GetGroupManagerServerId(),
        ntf, sglib::msgid::GPGM_USER_LOGOUT_NTF );
} 

void CGroupClient::_NotifyManagerCreateGroup(u64 gateid, u64 clientid, const char *name)
{
	sglib::groupproto::GroupGroupmanageCreateGroupReq req;
	req.set_gateid( gateid );
	req.set_gateresid( GateResId() );
	req.set_clientid( clientid );
	req.set_name( name );
	
	CServerManager::Instance().SendRpcMsg( 
        CServerManager::Instance().GetGroupManagerServerId(),
		req, sglib::msgid::GPGM_GROUP_CREATE_REQ );
}

s32 CGroupClient::_CheckLoginValid(const char *user)
{
	// TODO
	return sglib::errorcode::E_ErrorCode_Success;
}

void CGroupClient::_DoMemberLeaveGroup(s32 gateResId, u64 clientid, u64 groupid)
{
	s32 result = sglib::errorcode::E_ErrorCode_Success;
		
	vector<string> leaveUser;
	CGroupInfo *group = CGroupManager::Instance().FindGroup( groupid );
	if( group )
	{
		CGroupMember *member = group->FindMember(
			GateResId(), clientid );
		if( member )
		{
			leaveUser.push_back( member->GetUser() );
			member->SetOnline( false, member->GetGateResId(), member->GetClientId() );
			group->DelMember( member->GetUser().c_str() );
		}
		else
		{
			SERVER_LOG_ERROR( "CGroupClient,_DoMemberLeaveGroup,FindMember," << GateResId() << "," << clientid );
			result = sglib::errorcode::E_ErrorCode_NotInThisGroup;
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupClient,_DoMemberLeaveGroup,FindGroup," << groupid );
		result = sglib::errorcode::E_ErrorCode_NotFoundGroup;
	}

	_NotifyMemberLeaveGroupRsp( clientid, result, groupid );
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

void CGroupClient::_DoMemberDeleteGroup(s32 gateResId, u64 clientid, u64 groupid)
{
	s32 result = sglib::errorcode::E_ErrorCode_Success;
		
	map<s32, vector<u64> > alluser;
	vector<string> vecUser;
	CGroupInfo *group = CGroupManager::Instance().FindGroup( groupid );
	if( group )
	{
		group->DumpMemberInfo(  alluser, "", vecUser );
		CGroupMember *member = group->FindMember(
			GateResId(), clientid );
		if( !member )
		{
			SERVER_LOG_ERROR( "CGroupClient,_DoMemberDeleteGroup,FindMember," << GateResId() << "," << clientid );
			result = sglib::errorcode::E_ErrorCode_NotInThisGroup;
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupClient,_DoMemberDeleteGroup,FindGroup," << groupid );
		result = sglib::errorcode::E_ErrorCode_NotFoundGroup;
	}

	_NotifyMemberDeleteGroupRsp( clientid, result, groupid );
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

void CGroupClient::_NotifyManagerAddMemberToGroup(u64 clientid, u64 groupid, vector<string> vecUser)
{
	sglib::groupproto::GroupGroupmanagerAddMemberToGroupReq req;
	req.set_gateid( GetClientId() );
	req.set_gateresid( GateResId() );
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

void CGroupClient::_AskManagerInfoToAgree(s32 gateResId, u64 clientid, u64 groupid)
{
	sglib::groupproto::GroupGroupmanagerForAgreeJoinAskInfoReq req;
	req.set_gateresid( gateResId );
	req.set_clientid( clientid );
	req.set_groupid( groupid );
	
	CServerManager::Instance().SendRpcMsg( 
        CServerManager::Instance().GetGroupManagerServerId(),
		req, sglib::msgid::GPGM_AGREE_ASK_MEMBER_INFO_REQ );
}

void CGroupClient::_NotifyMemberLeaveGroupRsp(u64 clientid, s32 result, u64 groupid)
{
	sglib::groupproto::SCGroupLeaveRsp rsp;
	rsp.set_result( result );
	rsp.set_groupid( groupid );

	SendMsgToClient( clientid, rsp, sglib::msgid::SC_GROUP_LEAVE_RSP );
}

void CGroupClient::_NotifyGroupManagerMemberLeaveGroup(u64 groupid, const vector<string> &user)
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

void CGroupClient::_NotifyMemberDeleteGroupRsp(u64 clientid, s32 result, u64 groupid)
{
	sglib::groupproto::SCGroupDeleteRsp rsp;
	rsp.set_result( result );
	rsp.set_groupid( groupid );

	SendMsgToClient( clientid, rsp, sglib::msgid::SC_GROUP_DELETE_RSP );
}

void CGroupClient::_NotifyGroupGateDeleteGroup(u64 groupid)
{
	sglib::groupproto::GroupGateDelGroupNtf ntf;
	ntf.set_groupid( groupid );

	CGroupManager::Instance().BroadcastMsgToGates(
		ntf,
		sglib::msgid::GPGG_DEL_GROUPID_NTF );
}

void CGroupClient::_NotifyGroupManagerDeleteGroup(u64 groupid)
{
	sglib::groupproto::GroupGroupmanagerGroupDeleteNtf ntf;
	ntf.set_groupid( groupid );
	
	CServerManager::Instance().SendRpcMsg( 
		CServerManager::Instance().GetGroupManagerServerId(),
		ntf, sglib::msgid::GPGM_GROUP_DELETE_NTF );
}

void CGroupClient::_NotifyGroupManagerCreateGameRoom(s32 gateResId, u64 clientid, u64 groupid, s32 game)
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
