#include "RpcClient.h"
#include "ServerManager.h"

void CRpcClient::OnConnect()
{
#ifdef _USE_IOSERVICE_EX_
	strncpy( m_ip, ConvertIpToStr(m_remoteIp), MAX_IP_LEN );
	m_port = (s32)m_remotePort; 
#else
	strncpy( m_ip, ConvertIpToStr(m_uiRemoteIp), MAX_IP_LEN );
	m_port = (s32)m_uiRemotePort; 
#endif
    m_ServerId = CServerManager::Instance().GetServerId( m_ip, m_port );
    SG_ASSERT( m_ServerId != INVALID_VAL );

	SERVER_LOG_INFO( "CRpcClient,OnConnect," << GetId() << "," << m_ip << "," << m_port << "," << m_ServerId );

    CServerManager::Instance().AddRpcClient( m_ServerId, this );
}

void CRpcClient::OnClose()
{
	SERVER_LOG_INFO( "CRpcClient,OnClose" << GetId() << "," << m_ServerId );
    
    CServerManager::Instance().DelRpcClient( m_ServerId );

	SERVER_LOG_INFO( "CRpcClient,OnClose,TryConnect," << m_ip << "," << m_port << "," << m_ServerId );
	CServerManager::Instance().Reconnect( m_ip, m_port );
}

void CRpcClient::OnRecv(const byte *pPkg, s32 nPkgLen)
{
}

void CRpcClient::OnConnectError()
{
#ifdef _USE_IOSERVICE_EX_
	char *ip = ConvertIpToStr( m_remoteIp );
	s32 port = (s32)m_remotePort; 
#else
	char *ip = ConvertIpToStr( m_uiRemoteIp );
	s32 port = (s32)m_uiRemotePort; 
#endif
	SERVER_LOG_ERROR( "CRpcClient,OnConnectError" << ip << "," << port );

	CServerManager::Instance().Reconnect( ip, port );
}
    
char* CRpcClient::ConvertIpToStr(u32 ip)
{
    struct in_addr in;
#if _USE_WIN32
	in.S_un.S_addr = ip;
#else
	in.s_addr = ip;
#endif
	return inet_ntoa( in );
}
