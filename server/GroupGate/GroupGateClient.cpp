#include "GroupGateClient.h"
#include "ServerManager.h"
#include "GroupIdMap.h"
#include "gate.pb.h"
#include "group.pb.h"
#include "msgid.pb.h"

void CGateClient::OnRecv(const byte *pkg, s32 len)
{
	s32 fid = _ParseForwardId( pkg, len );

	SERVER_LOG_DEBUG( "ParseForwardId = " << fid );
	if( fid == INVALID_VAL )
	{
		SERVER_LOG_ERROR( "CGateClient,OnRecv,_ParseForwardId,FowwardId," << fid );
		return;
	}

	sglib::gateproto::GateServerForwardMsgReq req;
	req.set_clientid( GetClientId() );
	req.set_msg( (const char*)pkg, (size_t)len );

	CServerManager::Instance().SendRpcMsg( fid, req, sglib::msgid::GS_FORWARD_MSG_REQ );
}

void CGateClient::OnClose()
{
	CClient::OnClose();

	sglib::gateproto::GateServerClientCloseNtf ntf;
	ntf.set_clientid( GetClientId() );
    
	s32 fid = GetForwardId();
	CServerManager::Instance().SendRpcMsg( fid, ntf, sglib::msgid::GS_CLIENT_CLOSE_NOTIFY );
}

static s32 _FromMsgGroupAddMember(const byte *pkg, s32 len)
{
	sglib::groupproto::CSGroupAddMemberReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		return CGroupIdMap::Instance().GetServerIdByGroupId( req.groupid() );
	}
	return INVALID_VAL;
}
static s32 _FromMsgGroupAgreeJoin(const byte *pkg, s32 len)
{
	sglib::groupproto::CSGroupAgreeJoinReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		return CGroupIdMap::Instance().GetServerIdByGroupId( req.groupid() );
	}
	return INVALID_VAL;
}
static s32 _FromMsgGroupDelete(const byte *pkg, s32 len)
{
	sglib::groupproto::CSGroupDeleteReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		return CGroupIdMap::Instance().GetServerIdByGroupId( req.groupid() );
	}
	return INVALID_VAL;
}
static s32 _FromMsgGroupJoin(const byte *pkg, s32 len)
{
	sglib::groupproto::CSGroupJoinReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		return CGroupIdMap::Instance().GetServerIdByGroupId( req.groupid() );
	}
	return INVALID_VAL;
}
static s32 _FromMsgGroupLeave(const byte *pkg, s32 len)
{
	sglib::groupproto::CSGroupLeaveReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		return CGroupIdMap::Instance().GetServerIdByGroupId( req.groupid() );
	}
	return INVALID_VAL;
}
static s32 _FromMsgGroupSearch(const byte *pkg, s32 len)
{
	sglib::groupproto::CSGroupSearchReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		return CGroupIdMap::Instance().GetServerIdByGroupId( req.groupid() );
	}
	return INVALID_VAL;
}
static s32 _FromMsgGroupMessage(const byte *pkg, s32 len)
{
	sglib::groupproto::CSGroupMessageReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		return CGroupIdMap::Instance().GetServerIdByGroupId( req.groupid() );
	}
	return INVALID_VAL;
}

s32	CGateClient::_ParseForwardId(const byte *pkg, s32 len)
{
    s32 fid = GetForwardId();
	s32 msgid = *((s32*)pkg);
	switch(msgid)
	{
	case sglib::msgid::CS_GROUP_ADD_MEMBER_REQ:
		fid = _FromMsgGroupAddMember( pkg+MSG_ID_LEN, len-MSG_ID_LEN );
		break;
	case sglib::msgid::CS_GROUP_AGREE_JOIN_REQ:
		fid = _FromMsgGroupAgreeJoin( pkg+MSG_ID_LEN, len-MSG_ID_LEN );
		break;
	case sglib::msgid::CS_GROUP_DELETE_REQ:
		fid = _FromMsgGroupDelete( pkg+MSG_ID_LEN, len-MSG_ID_LEN );
		break;
	case sglib::msgid::CS_GROUP_JOIN_REQ:
		fid = _FromMsgGroupJoin( pkg+MSG_ID_LEN, len-MSG_ID_LEN );
		break;
	case sglib::msgid::CS_GROUP_LEAVE_REQ:
		fid = _FromMsgGroupLeave( pkg+MSG_ID_LEN, len-MSG_ID_LEN );
		break;
	case sglib::msgid::CS_GROUP_SEARCH_REQ:
		fid = _FromMsgGroupSearch( pkg+MSG_ID_LEN, len-MSG_ID_LEN );
		break;
	case sglib::msgid::CS_GROUP_MESSAGE_REQ:
		fid = _FromMsgGroupMessage( pkg+MSG_ID_LEN, len-MSG_ID_LEN );
		break;
	default:
		break;
	};

	return fid;
}
