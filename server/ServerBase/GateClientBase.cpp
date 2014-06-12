#include "GateClientBase.h"
#include "ServerManager.h"
#include "GateClientManager.h"
#include "msgid.pb.h"
#include "common.pb.h"
#include "errno.pb.h"
#include "Hall.h"
using namespace std;

CGateClientBase::CGateClientBase(s32 nId) : CClient(nId), m_GateId(INVALID_VAL)
{
	CGateClientManager::Instance();

	// Gate msg
	_RegisterGateProc( sglib::msgid::GS_FORWARD_MSG_REQ, &CGateClientBase::_GSForwardMsgReqProc );
	_RegisterGateProc( sglib::msgid::GS_REPORT_STATUS_REQ, &CGateClientBase::_GSReportStatusReqProc );
	_RegisterGateProc( sglib::msgid::GS_CLIENT_CLOSE_NOTIFY, &CGateClientBase::_GSClientCloseProc );
}

void CGateClientBase::OnRecv(const byte *pPkg, s32 nPkgLen)
{
	s32 msgId = *((s32*)pPkg);

	//SERVER_LOG_DEBUG( "recv gate:" << GetClientId() << " rpc pkg, len=" << nPkgLen << " ,msgId=" << msgId );

	map<int, GateProtoProc>::iterator it = m_mapGateProtoProc.find( msgId );
	if( it != m_mapGateProtoProc.end() )
	{
		(this->*it->second)( pPkg+MSG_ID_LEN, nPkgLen-MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CGateClientBase,OnRecv," << GetClientId() << "," << msgId );
	}
}

void CGateClientBase::OnAccept()
{
	CClient::OnAccept();
}

void CGateClientBase::OnClose()
{
	CClient::OnClose();
	CGateClientManager::Instance().DelGateInfo( m_GateId );
}

void CGateClientBase::OnClientClose(u64 clientid)
{
}

void CGateClientBase::_RegisterGateProc(int id, GateProtoProc proc)
{
	if( proc != NULL )
	{
		m_mapGateProtoProc[ id ] = proc;
	}
}

void CGateClientBase::_DoSendMsgToClient(vector<u64> &clientId, const byte *pPkg, s32 nPkgLen)
{
	sglib::gateproto::ServerGateForwardMsgReq ntf;
	vector<u64>::iterator it = clientId.begin();
	for( ; it != clientId.end(); ++it )
	{
		ntf.add_clientid( *it );
	}
	ntf.set_msg( pPkg, nPkgLen );
				
	SendMsg( ntf, sglib::msgid::SG_FORWARD_MSG_REQ );
}

void CGateClientBase::SendMsgToClient(u64 clientId, const ::google::protobuf::Message &Msg, s32 nMsgId)
{
	int count = Msg.ByteSize();
	byte buf[MAX_MSG_LEN] = {0};
	*((s32*)buf) = nMsgId;
	if( Msg.SerializeToArray(buf+MSG_ID_LEN, MAX_MSG_LEN) )
	{
		vector<u64> vecId;
		vecId.push_back( clientId );
		_DoSendMsgToClient( vecId, buf, count + MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CGateClientBase,SendMsgToClient," << nMsgId );
	}
}

void CGateClientBase::SendMsgToClients(std::vector<u64> &clientIds, const ::google::protobuf::Message &Msg, s32 nMsgId)
{
	int count = Msg.ByteSize();
	byte buf[MAX_MSG_LEN] = {0};
	*((s32*)buf) = nMsgId;
	if( Msg.SerializeToArray(buf+MSG_ID_LEN, MAX_MSG_LEN) )
	{
		_DoSendMsgToClient( clientIds, buf, count + MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CGateClientBase,SendMsgToClients," << nMsgId );
	}
}

void CGateClientBase::SendMsg(const ::google::protobuf::Message &Msg, s32 nMsgId)
{
	int count = Msg.ByteSize();
	byte buf[MAX_MSG_LEN] = {0};
	*((s32*)buf) = nMsgId;
	if( Msg.SerializeToArray(buf+MSG_ID_LEN, MAX_MSG_LEN) )
	{
		if( !SendPkg( buf, count+MSG_ID_LEN ) )
		{
			SERVER_LOG_ERROR( "CGateClientBase,SendMsg,SendPkg" );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGateClientBase,SendMsg,SerializeToArray" );
	}
}

s32 CGateClientBase::GateResId()
{
    return m_GateId;
}

void CGateClientBase::_GSForwardMsgReqProc(const byte *pPkg, s32 nPkgLen)
{
	sglib::gateproto::GateServerForwardMsgReq req;
	if( req.ParseFromArray(pPkg, nPkgLen) )
	{
		const char *clientMsg = req.msg().c_str();
		s32 len = (s32)req.msg().length();
		s32 msgId = *((s32*)clientMsg);

		//SERVER_LOG_DEBUG( "gate:" << GetClientId() << " forward client pkg, len=" << len << " , client=" << req.clientid() << " ,msgId=" << msgId );

		ProcPlayerMessage(
			GetClientId(),
			req.clientid(),
			msgId,
			(const byte*)(clientMsg+MSG_ID_LEN), 
			len-MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CGateClientBase,_GSForwardMsgReqProc,ParseFromArray" );
	}
}

void CGateClientBase::_GSReportStatusReqProc(const byte *pPkg, s32 nPkgLen)
{
	sglib::gateproto::GateServerReportStatusReq req;
	if( req.ParseFromArray(pPkg, nPkgLen) )
	{
		//SERVER_LOG_DEBUG( "gate:" << req.gateid() << " report status, cur=" << req.cur_conn() << " max=" << req.max_conn() );

		m_GateId = req.gateid();
		CGateClientManager::Instance().SetGateInfo(
			m_GateId,
			req.cur_conn(),
			req.max_conn(),
			req.ip().c_str(),
			req.port(),
            GetClientId() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGateClientBase,_GSReportStatusReqProc,ParseFromArray" );
	}
}
void CGateClientBase::_GSClientCloseProc(const byte *pPkg, s32 nPkgLen)
{
	sglib::gateproto::GateServerClientCloseNtf ntf;
	if( ntf.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_INFO( "ClientCloseProc," << ntf.clientid() << "," << GetClientId() );

		OnClientClose( ntf.clientid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGateClientBase,_GSClientCloseProc,ParseFromArray" );
	}
}

