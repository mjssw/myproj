#ifndef _GroupManager_Rpc_Client_h_
#define _GroupManager_Rpc_Client_h_

#include "RpcClient.h"
#include <map>
#include "google/protobuf/message.h"

class CGroupManagerRpcClient;
typedef void (CGroupManagerRpcClient::*RpcProtoProc)(const byte *pkg, s32 len);

class CGroupManagerRpcClient: public CRpcClient
{
public:
	CGroupManagerRpcClient(s32 nId);
	~CGroupManagerRpcClient();

	virtual void OnRecv(const byte *pkg, s32 len);

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CGroupManagerRpcClient);
#endif
private:
	void _RegisterProc(s32 id, RpcProtoProc proc);

private:
	std::map<s32, RpcProtoProc> m_mapProtoProc;
};

#endif
