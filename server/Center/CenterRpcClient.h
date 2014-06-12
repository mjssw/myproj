#ifndef _CENTER_RPC_CLIENT_H_
#define _CENTER_RPC_CLIENT_H_

#include "RpcClient.h"

class CCenterRpcClient : public CRpcClient
{
public:
	CCenterRpcClient(int nId) : CRpcClient(nId)
	{}

	virtual void OnRecv(const byte *pPkg, s32 nPkgLen){};
#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CCenterRpcClient);
#endif
};

#endif
