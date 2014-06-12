#ifndef _TEST_RPC_CLIENT_H_
#define _TEST_RPC_CLIENT_H_

#include "RpcClient.h"
#include <map>
#include "google/protobuf/message.h"

class CTestRpcClient;
typedef void (CTestRpcClient::*ProtoProc)(const byte *pPkg, s32 nPkgLen);

class CTestRpcClient: public CRpcClient
{
public:
	CTestRpcClient(int nId);
	~CTestRpcClient();

#ifdef _USE_IOSERVICE_EX_
	virtual CIoSocketEx* Clone(s32 id)
	{
		return new CTestRpcClient( id );
	}
#endif

	virtual void OnRecv(const byte *pPkg, s32 nPkgLen);
private:
	void _SendMsgToClient(u64 cid, u64 gate_cid, const ::google::protobuf::Message &Msg, s32 nMsgId);
	void _RegisterProc(int id, ProtoProc proc);

private:
	std::map<int, ProtoProc> m_mapProtoProc;
};

#endif
