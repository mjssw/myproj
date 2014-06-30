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
	std::map<s32, ProtoProc> m_mapProtoProc;
};

#endif
