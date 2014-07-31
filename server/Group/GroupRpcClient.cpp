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
		CGroupManager::Instance().TryCreateGroup(
			ntf.gateid(), ntf.gateresid(), ntf.clientid(), ntf.user(), 
			ntf.name(), ntf.groupid(), ntf.head(), ntf.groupserverid() );
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
		CGroupManager::Instance().CreateGroupResult(
			rsp.result(), rsp.gateid(), rsp.clientid(), rsp.name(), 
			rsp.groupid(), rsp.head(), rsp.serverid() );
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
		CGroupManager::Instance().AddGroupMemberRsp( rsp );
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
		CGroupManager::Instance().AgreeJoinGroupAskInfoRsp( rsp );
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
		CGroupManager::Instance().LoadGroup( ntf );
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
		CGroupManager::Instance().GroupMemberOnline( ntf );
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
		CGroupManager::Instance().GroupMemberOffline( ntf );
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
		CGroupManager::Instance().CreateGroupGameResult( rsp );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupRpcClient,_GroupmanagerGroupMemberOfflineProc,ParseFromArray" );
	}
}
