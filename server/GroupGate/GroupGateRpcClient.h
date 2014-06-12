#ifndef _GROUP_GATE_RPC_CLIENT_H_
#define _GROUP_GATE_RPC_CLIENT_H_

#include "RpcClient.h"
#include <map>

class CGateRpcClient;
typedef void (CGateRpcClient::*ProtoProc)(const byte *pkg, s32 len);

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
	virtual void OnRecv(const byte *pkg, s32 len);

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CGateRpcClient);
#endif

private:
	// msg proc
	void CloseClientReqProc(const byte *pkg, s32 len);
	void ForwardMsgReqProc(const byte *pkg, s32 len);
	void SetForwardServerReqProc(const byte *pkg, s32 len);
	void ConnectNewServerProc(const byte *pkg, s32 len);
	void AddGroupIdMapProc(const byte *pkg, s32 len);
	void DelGroupIdMapProc(const byte *pkg, s32 len);

	static void TimerCallBack(void *pData, s32 nDataLen);
	void _ReportStatus();
	void _Register(int id, ProtoProc proc);
private:
	std::map<int, ProtoProc> m_mapProtoProc;
	s32 m_reportTimerId;
	bool m_bHasReportIpPort;
};

#endif
