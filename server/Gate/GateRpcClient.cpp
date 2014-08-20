#include "GateRpcClient.h"
#include "ServerManager.h"
#include "gate.pb.h"
#include "msgid.pb.h"
using namespace std;

CGateRpcClient::CGateRpcClient(int nId) : 
	CRpcClient(nId), 
	m_reportTimerId(INVALID_VAL),
	m_bHasReportIpPort(false)
{
	_Register( sglib::msgid::SG_CLOSE_CLIENT_REQ,	&CGateRpcClient::CloseClientReqProc );
	_Register( sglib::msgid::SG_FORWARD_MSG_REQ,	&CGateRpcClient::ForwardMsgReqProc );
	_Register( sglib::msgid::SG_SET_FORWARD_SERVER,	&CGateRpcClient::SetForwardServerReqProc );
	_Register( sglib::msgid::SG_CONNECT_NEW_SERVER_NTF,	&CGateRpcClient::ConnectNewServerProc );
}

struct _callbackData
{
	void *pthis;
};

void CGateRpcClient::TimerCallBack(void *pData, s32 nDataLen)
{
	SG_ASSERT( nDataLen == sizeof(_callbackData) );
	CGateRpcClient *pClient = (CGateRpcClient*)(((_callbackData*)pData)->pthis);

	if( pClient )
	{
		pClient->_ReportStatus();
	}
}

void CGateRpcClient::OnConnect()
{
	CRpcClient::OnConnect();

	// 启动定时器定时汇报自己的状态
	_callbackData _data;
	_data.pthis = this;

	m_reportTimerId = CServerManager::Instance().AddTimer(
		E_ReportStatus_Timer,
		TimerCallBack,
		&_data, 
		sizeof(_data), 
		true );
}

void CGateRpcClient::OnClose()
{
	CRpcClient::OnClose();

	if( m_reportTimerId != INVALID_VAL )
	{
		CServerManager::Instance().DelTimer( m_reportTimerId );
		m_reportTimerId = INVALID_VAL;
	}
}

void CGateRpcClient::OnRecv(const byte *pPkg, s32 nPkgLen)
{
	s32 msgId = *((s32*)pPkg);

	//SERVER_LOG_DEBUG( "rpcclient:" << GetId() << " recv pkg, len=" << nPkgLen << " ,from server=" << m_ServerId << " ,msgId=" << msgId );

	map<int, ProtoProc>::iterator it = m_mapProtoProc.find( msgId );
	if( it != m_mapProtoProc.end() )
	{
		// 注意下面调用成员函数指针的方式 !!!
		(this->*it->second)( pPkg+MSG_ID_LEN, nPkgLen-MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CGateRpcClient,OnRecv," << GetId() << "," << msgId );
	}
}

void CGateRpcClient::_Register(int id, ProtoProc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}

void CGateRpcClient::CloseClientReqProc(const byte *pPkg, s32 nPkgLen)
{
	sglib::gateproto::ServerGateCloseClientReq req;
	if( req.ParseFromArray(pPkg, nPkgLen) )
	{
		CServerManager::Instance().CloseClient( req.clientid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGateRpcClient,CloseClientReqProc,ParseFromArray" );
	}
}

void CGateRpcClient::ForwardMsgReqProc(const byte *pPkg, s32 nPkgLen)
{
	sglib::gateproto::ServerGateForwardMsgReq req;
	if( req.ParseFromArray(pPkg, nPkgLen) )
	{
		int count = req.clientid_size();
		for( int i=0; i<count; ++i )
		{
			u64 id = req.clientid( i );
		
			//SERVER_LOG_DEBUG( "forward msg to clientid:" << id << " ,msgLen:" << req.msg().length() );
			CServerManager::Instance().SendClientMsg( 
				id, 
				(const byte*)req.msg().c_str(),
				(s32)req.msg().length() );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGateRpcClient,ForwardMsgReqProc,ParseFromArray" );
	}
}

void CGateRpcClient::SetForwardServerReqProc(const byte *pPkg, s32 nPkgLen)
{
	sglib::gateproto::ServerGateSetForwardServerReq req;
	if( req.ParseFromArray(pPkg, nPkgLen) )
	{
		CServerManager::Instance().SetClientForwardServerId( 
			req.clientid(),
			req.serverid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGateRpcClient,SetForwardServerReqProc,ParseFromArray" );
	}
}

void CGateRpcClient::ConnectNewServerProc(const byte *pPkg, s32 nPkgLen)
{
	sglib::gateproto::ServerGateConnectNewServerNtf ntf;
	if( ntf.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_INFO( "ConnectNewServer," << ntf.id() << "," \
			<< ntf.ip().c_str() << "," << ntf.port() );

		// 先将连接信息加入到自己的配置文件中
		if( !CServerManager::Instance().AddConnectItem(
				ntf.id(),
				ntf.ip().c_str(),
				ntf.port(),
				false ) )
		{
			SERVER_LOG_ERROR( "CGateRpcClient,ConnectNewServerProc,AddConnectItem" );
			return;
		}

		// 连接新服务器
		CServerManager::Instance().Connect( ntf.ip().c_str(), ntf.port() );
	}
	else
	{
		SERVER_LOG_ERROR( "CGateRpcClient,ConnectNewServerProc,ParseFromArray" );
	}
}

void CGateRpcClient::_ReportStatus()
{
	sglib::gateproto::GateServerReportStatusReq req;
	req.set_gateid( CServerManager::Instance().ServerId() );
	req.set_cur_conn( CServerManager::Instance().GetClientCount() );
	req.set_max_conn( CServerManager::Instance().MaxConnectionCount() );
	//if( !m_bHasReportIpPort )
	{
		const CServerConfig *pCfg = CServerManager::Instance().GetServerConfig();
		if( pCfg != NULL )
		{
			m_bHasReportIpPort = true;
			req.set_ip( pCfg->GetServerConfig().m_Ip );
			req.set_port( pCfg->GetServerConfig().m_Port );
		}
	}
	
	int total = req.ByteSize();
	byte buf[MAX_MSG_LEN] = {0};
	*((s32*)buf) = sglib::msgid::GS_REPORT_STATUS_REQ;
	if( req.SerializeToArray(buf+MSG_ID_LEN, MAX_MSG_LEN) )
	{
		bool ret = Send( buf, total + MSG_ID_LEN );
		if( !ret )
		{
			SERVER_LOG_ERROR( "CGateRpcClient,_ReportStatus,Send" );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CGateRpcClient,_ReportStatus,SerializeToArray" );
	}
}

