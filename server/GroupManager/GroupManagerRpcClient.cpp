#include "GroupManagerRpcClient.h"
#include "ServerManager.h"
#include "GroupManagerClient.h"
#include "msgid.pb.h"

CGroupManagerRpcClient::CGroupManagerRpcClient(s32 nId) : 
	CRpcClient(nId)
{
}

CGroupManagerRpcClient::~CGroupManagerRpcClient()
{
}

void CGroupManagerRpcClient::OnRecv(const byte *pkg, s32 len)
{
	s32 msgId = *((s32*)pkg);

	map<s32, RpcProtoProc>::iterator it = m_mapProtoProc.find( msgId );
	if( it != m_mapProtoProc.end() )
	{
		(this->*it->second)( pkg+MSG_ID_LEN, len-MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CGroupManagerRpcClient,OnRecv," << GetId() << "," << msgId );
	}
}

void CGroupManagerRpcClient::_RegisterProc(s32 id, RpcProtoProc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}
