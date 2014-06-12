#ifndef _GAME_MGR_RPC_CLIENT_H_
#define _GAME_MGR_RPC_CLIENT_H_

#include "RpcClient.h"
#include "google/protobuf/message.h"
#include <map>

class CGameMgrRpcClient;
typedef void (CGameMgrRpcClient::*RpcProtoProc)(const byte *pPkg, s32 nPkgLen);

class CGameMgrRpcClient : public CRpcClient
{
public:
	CGameMgrRpcClient(int nId);
	virtual void OnConnect();
	virtual void OnRecv(const byte *pPkg, s32 nPkgLen);

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CGameMgrRpcClient);
#endif
private:
	void _RegisterProc(int id, RpcProtoProc proc);
	void _CenterServerEnterGameRsp(const byte *pkg, s32 len);
	void _GroupManagerAskCreateGameRoomProc(const byte *pkg, s32 len);

private:
	std::map<int, RpcProtoProc> m_mapProtoProc;
};

#endif
