#ifndef _LOGIN_RPC_CLIENT_H_
#define _LOGIN_RPC_CLIENT_H_

#include "RpcClient.h"
#include "Message.h"
#include <map>

class CLoginRpcClient;
typedef void (CLoginRpcClient::*ProtoProc)(const byte *pPkg, s32 nPkgLen);

class CLoginRpcClient : public CRpcClient
{
public:
	CLoginRpcClient(int nId);
	virtual void OnConnect();
	virtual void OnRecv(const byte *pPkg, s32 nPkgLen);

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CLoginRpcClient);
#endif
private:
	void CenterLoginUserLoginRsp(const byte *pPkg, s32 nPkgLen);
	void CenterLoginGameInfoNotify(const byte *pPkg, s32 nPkgLen);
	void CenterServerEnterGameRsp(const byte *pPkg, s32 nPkgLen);

private:
	void _SendMsgToClient(u64 gateid, u64 clientid, const ::google::protobuf::Message &Msg, s32 nMsgId);
	void _RegisterProc(int id, ProtoProc proc);

private:
	std::map<int, ProtoProc> m_mapProtoProc;
};

#endif
