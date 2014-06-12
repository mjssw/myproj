#include "GateClient.h"
#include "ServerManager.h"
#include "gate.pb.h"
#include "msgid.pb.h"

void CGateClient::OnRecv(const byte *pPkg, s32 nPkgLen)
{
	//SERVER_LOG_DEBUG( "client:" << GetClientId() << " recv pkg, len=" << nPkgLen << ", ForwardToServer" );

    s32 fid = GetForwardId();
    
	sglib::gateproto::GateServerForwardMsgReq req;
	req.set_clientid( GetClientId() );
	req.set_msg( (const char*)pPkg, (size_t)nPkgLen );

	CServerManager::Instance().SendRpcMsg( fid, req, sglib::msgid::GS_FORWARD_MSG_REQ );
}

void CGateClient::OnClose()
{
	CClient::OnClose();

	sglib::gateproto::GateServerClientCloseNtf ntf;
	ntf.set_clientid( GetClientId() );
    
	s32 fid = GetForwardId();
	CServerManager::Instance().SendRpcMsg( fid, ntf, sglib::msgid::GS_CLIENT_CLOSE_NOTIFY );
}
