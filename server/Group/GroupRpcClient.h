#ifndef _Group_Rpc_Client_h_
#define _Group_Rpc_Client_h_

#include "RpcClient.h"
#include <map>
#include "google/protobuf/message.h"

class CGroupRpcClient;
typedef void (CGroupRpcClient::*ProtoProc)(const byte *pkg, s32 len);

class CGroupRpcClient: public CRpcClient
{
public:
	CGroupRpcClient(s32 nId);
	~CGroupRpcClient();

	virtual void OnRecv(const byte *pkg, s32 len);

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CGroupRpcClient);
#endif

private:
	void _SendMsgToClient(u64 gateid, u64 userid, const ::google::protobuf::Message &msg, s32 msgid);
	void _RegisterProc(s32 id, ProtoProc proc);
	void _GroupmanagerServerDynamicServerStart(const byte *pkg, s32 len);
	void _GroupmanagerGroupCreateProc(const byte *pkg, s32 len);
	void _GroupmanagerGroupCreateRspProc(const byte *pkg, s32 len);
	void _GroupmanagerGroupAddMemberRspProc(const byte *pkg, s32 len);
	void _GroupmanagerGroupAgreeJoinAskInfoRspProc(const byte *pkg, s32 len);
	void _GroupmanagerGroupLoadGroupProc(const byte *pkg, s32 len);
	void _GroupmanagerGroupMemberOnlineProc(const byte *pkg, s32 len);
	void _GroupmanagerGroupMemberOfflineProc(const byte *pkg, s32 len); 
	void _GroupmanagerGroupCreateGameResultProc(const byte *pkg, s32 len);

private:
	void _NotifyGroupmanagerCreateGroupResult(s32 result, u64 gateid, u64 clientid, const char *user, const char *name, u64 groupid, u64 serverid);
	void _NotifyClientCreateGroupRsp(s32 result, u64 gateid, u64 clientid, const char *name, u64 groupid);
	s32  _DoLoadGroup(u64 groupid, s32 gateresid, u64 clientid, const char *user);
	void _NotifyLoadGroupResult(s32 result, u64 groupid, s32 serverid, const char *user);
	void _MemberOnline(u64 groupid, s32 gateresid, u64 clientid, const std::string &user);
	void _MemberOffline(u64 groupid, s32 gateresid, u64 clientid, const std::string &user);
	void _NotifyCreateGameRoomResult(s32 gateresid, u64 clientid, s32 result, u64 groupid, s32 game, const std::string &ip, s32 port, s32 roomid, const std::string &roompwd);

private:
	std::map<s32, ProtoProc> m_mapProtoProc;
};

#endif
