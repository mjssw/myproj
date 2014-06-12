#ifndef _TETRIS_RPC_CLIENT_H_
#define _TETRIS_RPC_CLIENT_H_

#include "RpcClient.h"
#include <map>
#include "google/protobuf/message.h"

class CTetrisRpcClient;
typedef void (CTetrisRpcClient::*ProtoProc)(const byte *pPkg, s32 nPkgLen);

class CTetrisRpcClient : public CRpcClient
{
public:
	CTetrisRpcClient(int nId);
	~CTetrisRpcClient();

	virtual void OnConnect();
	virtual void OnRecv(const byte *pPkg, s32 nPkgLen);

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CTetrisRpcClient);
#endif
private:
	void _GamemanagerServerEnterGameRsp(const byte *pPkg, s32 nPkgLen);
	void _GamemanagerServerDynamicServerStart(const byte *pPkg, s32 nPkgLen);
	void _GamemanagerServerCreateGameRoom(const byte *pPkg, s32 nPkgLen);

private:
	void _SendMsgToClient(u64 gateid, u64 clientid, const ::google::protobuf::Message &Msg, s32 nMsgId);
	void _RegisterProc(int id, ProtoProc proc);

private:
	std::map<int, ProtoProc> m_mapProtoProc;
};

#endif
