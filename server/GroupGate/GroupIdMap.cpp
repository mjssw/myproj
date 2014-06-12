#include "GroupIdMap.h"
using namespace std;
using namespace SGLib;

INIT_SIGNLETON_CLASS(CGroupIdMap);

void CGroupIdMap::AddGroupId(u64 groupid, s32 serverid)
{
	CGuardLock<CLock> g(m_Lock);
	m_GroupIdMap[ groupid ] = serverid;
}

void CGroupIdMap::DelGroupId(u64 groupid)
{
	CGuardLock<CLock> g(m_Lock);
	map<u64, s32>::iterator it = m_GroupIdMap.find( groupid );
	if( it != m_GroupIdMap.end() )
	{
		m_GroupIdMap.erase( it );
	}
}

s32 CGroupIdMap::GetServerIdByGroupId(u64 groupid)
{
	CGuardLock<CLock> g(m_Lock);
	map<u64, s32>::iterator it = m_GroupIdMap.find( groupid );
	if( it != m_GroupIdMap.end() )
	{
		return it->second;
	}

	return INVALID_VAL;
}

void CGroupIdMap::DisplayInfo()
{
#if _DEBUG
	printf( "========= Group Id Map ===========\n" );
	CGuardLock<CLock> g(m_Lock);
	map<u64, s32>::iterator it = m_GroupIdMap.begin();
	for( ; it != m_GroupIdMap.end(); ++it )
	{
		printf( "\tGroupId:%llu\tGroupServerId:%d\n", it->first, it->second );
	}
#endif
}
