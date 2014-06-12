#include "GroupInfoManager.h"
#include "GateClientManager.h"
#include "StackTrace.h"
using namespace std;
using namespace SGLib;

INIT_SIGNLETON_CLASS(CGroupInfoManager);

CGroupInfoManager::~CGroupInfoManager()
{
	m_mapGroupInfo.clear();
	map<s32, CGateClientInfo*>::iterator it = m_mapGroupGateInfo.begin();
	for( ; it != m_mapGroupGateInfo.end(); ++it )
	{
		SAFE_DELETE( it->second );
	}
	m_mapGroupGateInfo.clear();
}

void CGroupInfoManager::SetGroupInfo(s32 id, s32 totalGroup, s32 totalMember, u64 serverid)
{
	CGuardLock<CLock> g(m_Lock);
	SGroupInfo info = { totalGroup, totalMember, serverid };
	m_mapGroupInfo[ id ] = info;
}

void CGroupInfoManager::DelGroupInfo(s32 id)
{
	CGuardLock<CLock> g(m_Lock);
	map<s32, SGroupInfo>::iterator it = m_mapGroupInfo.find( id );
	if( it != m_mapGroupInfo.end() )
	{
		m_mapGroupInfo.erase( it );
	}
}

const SGroupInfo* CGroupInfoManager::GetGroupInfo(s32 id)
{
	CGuardLock<CLock> g(m_Lock);
	map<s32, SGroupInfo>::iterator it = m_mapGroupInfo.find( id );
	if( it != m_mapGroupInfo.end() )
	{
		return &(it->second);
	}

	return NULL;
}

void CGroupInfoManager::SetGateInfo(s32 gateId, const char *ip, s32 port, s32 curCount, s32 maxCount)
{
	CGuardLock<CLock> g(m_Lock);
	CGateClientInfo *pInfo = NULL;
	std::map<s32, CGateClientInfo*>::iterator it = m_mapGroupGateInfo.find( gateId );
	if( it != m_mapGroupGateInfo.end() )
	{
		pInfo = it->second;
	}
	else
	{
		pInfo = new CGateClientInfo( gateId );
		if( pInfo == NULL )
		{
			SG_ASSERT( false );
			return;
		}
		m_mapGroupGateInfo[ gateId ] = pInfo;
	}

	pInfo->m_GateCurConnCount = curCount;	
	pInfo->m_GateMaxConnCount = maxCount;
	strncpy( pInfo->m_GateIp, ip, MAX_IP_LEN );
	pInfo->m_GatePort = port;
}

void CGroupInfoManager::DelGateInfo(s32 gateId)
{
	CGuardLock<CLock> g(m_Lock);
	std::map<s32, CGateClientInfo*>::iterator it = m_mapGroupGateInfo.find( gateId );
	if( it != m_mapGroupGateInfo.end() )
	{
		SAFE_DELETE( it->second );
		m_mapGroupGateInfo.erase( it );
	}
}

void CGroupInfoManager::GetGateList(std::vector<s32> &vecId)
{
	vecId.clear();
	CGuardLock<CLock> g(m_Lock);
	map<s32, CGateClientInfo*>::iterator it = m_mapGroupGateInfo.begin();
	for( ; it != m_mapGroupGateInfo.end(); ++it )
	{
		vecId.push_back( it->first );
	}
}

const CGateClientInfo* CGroupInfoManager::GetGroupGateInfo(s32 gateId)
{
	CGuardLock<CLock> g(m_Lock);
	std::map<s32, CGateClientInfo*>::iterator it = m_mapGroupGateInfo.find( gateId );
	if( it != m_mapGroupGateInfo.end() )
	{
		return it->second;
	}

	return NULL;
}

u64 CGroupInfoManager::FindProperGroup()
{
	// TODO
	s32 minCount = -1;
	u64 serverid = 0;
	CGuardLock<CLock> g(m_Lock);
	map<s32, SGroupInfo>::iterator it = m_mapGroupInfo.begin();
	for( ; it != m_mapGroupInfo.end(); ++it )
	{
		if( minCount == -1 )
		{
			minCount = it->second.m_TotalMemberCount;
			serverid = it->second.m_InstanceId;
		}
		else
		{
			if( minCount > it->second.m_TotalMemberCount )
			{
				minCount = it->second.m_TotalMemberCount;
				serverid = it->second.m_InstanceId;
			}
		}
	}

	return serverid;
}
