#include "Client.h"
#include "ServerManager.h"

void CClient::OnAccept()
{
	s32 fid = CServerManager::Instance().GetDefaultForwardServerId();
	SetForwardId( fid );
 
	u64 clientId = CServerManager::Instance().NextClientId();
	_SetClientId( clientId );
    CServerManager::Instance().AddClient( clientId, this );
	
	SERVER_LOG_INFO( "CClient,OnAccept," << clientId );
}

void CClient::OnClose()
{
    CServerManager::Instance().DelClient( GetClientId() );
	
	SERVER_LOG_INFO( "CClient,OnClose," << GetClientId() );
}

void CClient::OnRecv(const byte *pPkg, s32 nPkgLen)
{
}

void CClient::SetForwardId(s32 fid)
{
	CGuardLock<CLock> g(m_Lock);
	m_ForwardId = fid;
}

s32 CClient::GetForwardId()
{
	CGuardLock<CLock> g(m_Lock);
	return m_ForwardId;
}

u64 CClient::GetClientId()
{
	return m_ClientId;
}

bool CClient::SendPkg(const byte *pPkg, s32 nPkgLen)
{
	CGuardLock<CLock> g(m_SendLock);
	return Send( pPkg, nPkgLen );
}

void CClient::_SetClientId(u64 id)
{
	m_ClientId = id;
}
