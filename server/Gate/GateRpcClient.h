#ifndef _GATE_RPC_CLIENT_H_
#define _GATE_RPC_CLIENT_H_

#include "RpcClient.h"
#include <map>

class CGateRpcClient;
typedef void (CGateRpcClient::*ProtoProc)(const byte *pPkg, s32 nPkgLen);

class CGateRpcClient : public CRpcClient
{
public:
	enum
	{
		E_ReportStatus_Timer = 5000,
	};

	CGateRpcClient(int nId);
	virtual void OnConnect();
	virtual void OnClose();
	virtual void OnRecv(const byte *pPkg, s32 nPkgLen);

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CGateRpcClient);
#endif
private:
	// msg proc
	void CloseClientReqProc(const byte *pPkg, s32 nPkgLen);
	void ForwardMsgReqProc(const byte *pPkg, s32 nPkgLen);
	void SetForwardServerReqProc(const byte *pPkg, s32 nPkgLen);
	void ConnectNewServerProc(const byte *pPkg, s32 nPkgLen);

	static void TimerCallBack(void *pData, s32 nDataLen);
	void _ReportStatus();
	void _Register(int id, ProtoProc proc);
private:
	std::map<int, ProtoProc> m_mapProtoProc;
	s32 m_reportTimerId;
	bool m_bHasReportIpPort;
};

#endif

