#include "GroupManager.h"

INIT_SIGNLETON_CLASS(CGroupManager);

CGroupManager::~CGroupManager()
{
}

void CGroupManager::AddGroup(u64 groupid, s32 serverid)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	m_GroupMap[ groupid ] = serverid;
}

void CGroupManager::DelGroup(u64 groupid)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	std::map<u64, s32>::iterator it = m_GroupMap.find( groupid );
	if( it != m_GroupMap.end() )
	{
		m_GroupMap.erase( it );
	}
}

s32 CGroupManager::FindServerId(u64 groupid)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	std::map<u64, s32>::iterator it = m_GroupMap.find( groupid );
	if( it != m_GroupMap.end() )
	{
		return it->second;
	}

	return INVALID_VAL;
}

void CGroupManager::DisplayInfo()
{
#if _DEBUG
	printf( "========= Group Id Map ===========\n" );
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	std::map<u64, s32>::iterator it = m_GroupMap.begin();
	for( ; it != m_GroupMap.end(); ++it )
	{
		printf( "\tGroupId:%llu\tGroupServerId:%d\n", it->first, it->second );
	}
#endif
}
