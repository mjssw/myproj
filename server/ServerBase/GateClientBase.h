#ifndef _GATE_BASE_CLIENT_H_
#define _GATE_BASE_CLIENT_H_

#include "Client.h"
using namespace SGLib;
#include "gate.pb.h"
#include <map>
#include <vector>

class CGateClientBase;
typedef void (CGateClientBase::*GateProtoProc)(const byte *pPkg, s32 nPkgLen);

class CGateClientBase : public CClient 
{
public:
	CGateClientBase(s32 nId);

	virtual ~CGateClientBase(){}
	virtual void OnAccept();
	virtual void OnClose();
	virtual void OnRecv(const byte *pPkg, s32 nPkgLen);
	virtual void OnClientClose(u64 clientid);
	virtual void ProcPlayerMessage(u64 gateid, u64 clientid, s32 msgid, const byte *pkg, s32 len){}

	void SendMsgToClient(u64 clientId, const ::google::protobuf::Message &Msg, s32 nMsgId);
	void SendMsgToClients(std::vector<u64> &clientIds, const ::google::protobuf::Message &Msg, s32 nMsgId);
	void SendMsg(const ::google::protobuf::Message &Msg, s32 nMsgId);
    s32 GateResId();

protected:
	// gate proto
	void _GSForwardMsgReqProc(const byte *pPkg, s32 nPkgLen);
	void _GSReportStatusReqProc(const byte *pPkg, s32 nPkgLen);
	void _GSClientCloseProc(const byte *pPkg, s32 nPkgLen);

protected:
	void _DoSendMsgToClient(std::vector<u64> &clientId, const byte *pPkg, s32 nPkgLen);
	void _RegisterGateProc(int id, GateProtoProc proc);

protected:
	std::map<int, GateProtoProc> m_mapGateProtoProc;
	s32 m_GateId;		// 配置文件中配置的Id
};

#endif

