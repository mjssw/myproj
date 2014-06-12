#ifndef _Template_Rpc_Client_h_
#define _Template_Rpc_Client_h_

#include "RpcClient.h"
#include <map>
#include "google/protobuf/message.h"

class CTemplateRpcClient;
typedef void (CTemplateRpcClient::*ProtoProc)(const byte *pkg, s32 len);

class CTemplateRpcClient: public CRpcClient
{
public:
	CTemplateRpcClient(s32 nId);
	~CTemplateRpcClient();

	virtual void OnRecv(const byte *pkg, s32 len);

private:
	void _SendMsgToClient(u64 gateid, u64 userid, const ::google::protobuf::Message &msg, s32 msgid);
	void _RegisterProc(s32 id, ProtoProc proc);

private:
	std::map<s32, ProtoProc> m_mapProtoProc;
};

#endif
