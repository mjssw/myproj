#include "GateClientManager.h"
#include "ServerManager.h"

CGateClientManager* CGateClientManager::m_pInstance = NULL;

CGateClientManager::~CGateClientManager()
{
	std::map<s32, CGateClientInfo*>::iterator it = m_GateClientMap.begin();
	for( ; it != m_GateClientMap.end(); ++it )
	{
		if( it->second != NULL )
		{
			SAFE_DELETE( it->second );
		}
	}
}

void CGateClientManager::SetGateInfo(s32 id, s32 curCount, s32 maxCount, const char *ip, s32 port, u64 instancdId)
{
	CGateClientInfo *pInfo = NULL;
	SGLib::CGuardLock<SGLib::CLock> g(m_GateClientLock);
	std::map<s32, CGateClientInfo*>::iterator it = m_GateClientMap.find( id );
	if( it != m_GateClientMap.end() )
	{
		pInfo = it->second;
	}
	else
	{
		pInfo = new CGateClientInfo( id );
		if( pInfo == NULL )
		{
			return;
		}

		strncpy( pInfo->m_GateIp, ip, SGLib::MAX_IP_LEN );
		pInfo->m_GatePort = port;
        pInfo->m_GateInstanceId = instancdId;
		m_GateClientMap[ id ] = pInfo;
	}

	pInfo->m_GateCurConnCount = curCount;	
	pInfo->m_GateMaxConnCount = maxCount;
}

void CGateClientManager::DelGateInfo(s32 id)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_GateClientLock);
	std::map<s32, CGateClientInfo*>::iterator it = m_GateClientMap.find( id );
	if( it != m_GateClientMap.end() )
	{
		SAFE_DELETE( it->second );
		m_GateClientMap.erase( it );
	}
}

const CGateClientInfo* CGateClientManager::GetGateInfo(s32 id)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_GateClientLock);
	std::map<s32, CGateClientInfo*>::iterator it = m_GateClientMap.find( id );
	if( it != m_GateClientMap.end() )
	{
		return it->second;
	}

	return NULL;
}

void CGateClientManager::GetGateIdList(std::vector<s32> &vecId)
{
	vecId.clear();

	SGLib::CGuardLock<SGLib::CLock> g(m_GateClientLock);
	std::map<s32, CGateClientInfo*>::iterator it = m_GateClientMap.begin();
	for( ; it != m_GateClientMap.end(); ++it )
	{
		vecId.push_back( it->first );
	}
}
