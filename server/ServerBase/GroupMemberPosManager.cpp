#include "GroupMemberPosManager.h"
#include "StackTrace.h"
#include "stdio.h"
using namespace std;
using namespace SGLib;

CGroupMemberPositionManager::CGroupMemberPositionManager()
{
}

CGroupMemberPositionManager::~CGroupMemberPositionManager()
{
	map<s32, map<u64, CGroupMemberPosition*> >::iterator oit = m_Members.begin();
	for( ; oit != m_Members.end(); ++oit )
	{
		map<u64, CGroupMemberPosition*>::iterator it = oit->second.begin();
		for( ; it != oit->second.end(); ++it )
		{
			SAFE_DELETE( it->second );
		}
		oit->second.clear();
	}
	m_Members.clear();
}

void CGroupMemberPositionManager::SetMember(s32 gateResId, u64 clientid, const char *user)
{
	CGuardLock<CLock> g(m_Lock);
	CGroupMemberPosition *member = NULL;
	map<s32, map<u64, CGroupMemberPosition*> >::iterator oit = m_Members.find( gateResId );
	if( oit != m_Members.end() )
	{
		map<u64, CGroupMemberPosition*>::iterator it = oit->second.find( clientid );
		if( it != oit->second.end() )
		{
			member = it->second;
		}
	}

	if( member != NULL )
	{
		return;
	}

	member = new CGroupMemberPosition( user );
	if( member == NULL )
	{
		SG_ASSERT( false );
		return;
	}
	member->SetGateResId( gateResId );
	member->SetClientId( clientid );

	m_Members[ gateResId ][ clientid ] = member;
	m_MembersIndex[ user ] = member;
}

void CGroupMemberPositionManager::DelMember(s32 gateResId, u64 clientid)
{
	CGuardLock<CLock> g(m_Lock);
	map<s32, map<u64, CGroupMemberPosition*> >::iterator oit = m_Members.find( gateResId );
	if( oit != m_Members.end() )
	{
		map<u64, CGroupMemberPosition*>::iterator it = oit->second.find( clientid );
		if( it != oit->second.end() )
		{
			string user = it->second->User();
			SAFE_DELETE( it->second );
			oit->second.erase( it );

			map<string, CGroupMemberPosition*>::iterator _it = m_MembersIndex.find( user );
			SG_ASSERT( _it != m_MembersIndex.end() );
			m_MembersIndex.erase( _it );
		}
	}
}

CGroupMemberPosition* CGroupMemberPositionManager::FindMember(s32 gateResId, u64 clientid)
{
	CGuardLock<CLock> g(m_Lock);
	map<s32, map<u64, CGroupMemberPosition*> >::iterator oit = m_Members.find( gateResId );
	if( oit != m_Members.end() )
	{
		map<u64, CGroupMemberPosition*>::iterator it = oit->second.find( clientid );
		if( it != oit->second.end() )
		{
			return it->second;
		}
	}

	return NULL;
}

CGroupMemberPosition* CGroupMemberPositionManager::FindMember(const std::string &user)
{
	CGuardLock<CLock> g(m_Lock);
	map<string, CGroupMemberPosition*>::iterator it = m_MembersIndex.find( user );
	if( it != m_MembersIndex.end() )
	{
		return it->second;
	}

	return NULL;
}

void CGroupMemberPositionManager::DisplayInfo()
{
#if _DEBUG
	printf( "========= Group Member Position ===========\n" );
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	map<string, CGroupMemberPosition*>::iterator it = m_MembersIndex.begin();
	for( ; it != m_MembersIndex.end(); ++it )
	{
		printf( "\tUser:%s\tGroupGateId:%d\tClientId:%llu\tUSER:%s\n", 
			it->first.c_str(), 
			it->second->GetGateResId(), 
			it->second->GetClientId(),
			it->second->User().c_str() );
	}
#endif
}
