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
    
	CGroupManager::Instance().UserLogout( GateResId(), clientid );
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
		CGroupManager::Instance().UserLogin( *this, GateResId(), clientid, req.user(), req.token() );
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
		CGroupManager::Instance().CreateGroup( *this, GateResId(), GetClientId(), clientid, req.name() );
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
		CGroupManager::Instance().AddGroupMembers( *this, GateResId(), GetClientId(), clientid, req );
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
		CGroupManager::Instance().AgreeJoinGroup( 
			*this, GateResId(), GetClientId(), clientid, req.groupid() );
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
		CGroupManager::Instance().LeaveGroup( 
			*this, GateResId(), GetClientId(), clientid, req.groupid() );
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
		CGroupManager::Instance().DeleteGroup( 
			*this, GateResId(), GetClientId(), clientid, req.groupid() );
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
		CGroupManager::Instance().GroupMessage(
			GateResId(), GetClientId(), clientid, req.groupid(), req.content() );
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
		CGroupManager::Instance().CreateGroupGame(
			*this, GateResId(), GetClientId(), clientid, req.groupid(), req.game() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupClient,_GroupCreateGameRoomProc,ParseFromArray" );
	}
}
