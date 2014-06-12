#include "LoginMemberManager.h"
using namespace SGLib;
using namespace std;

INIT_SIGNLETON_CLASS(CLoginMemberManager);

CLoginMemberManager::~CLoginMemberManager()
{
}

CGroupMemberPositionManager& CLoginMemberManager::MemberManager()
{
	return m_Members;
}

void CLoginMemberManager::AddGroup(const std::string &user, u64 groupid)
{
	CGuardLock<CLock> g(m_Lock);
	_AddGroup( user, groupid );
}

void CLoginMemberManager::AddGroup(const std::string &user, const std::vector<u64> &vecGroupid)
{
	CGuardLock<CLock> g(m_Lock);
	vector<u64>::const_iterator cit = vecGroupid.begin();
	for( ; cit != vecGroupid.end(); ++cit )
	{
		_AddGroup( user, *cit );
	}
}

void CLoginMemberManager::DelGroup(const std::string &user, u64 groupid)
{
	CGuardLock<CLock> g(m_Lock);
	map< string, map<u64, s32> >::iterator it = m_MembersGroup.find( user );
	if( it != m_MembersGroup.end() )
	{
		map<u64, s32>::iterator _tmp = it->second.find( groupid ); 
		if( _tmp != it->second.end() )
		{
			it->second.erase( _tmp );
		}
	}
}

void CLoginMemberManager::DelAllGroup(const std::string &user)
{
	CGuardLock<CLock> g(m_Lock);
	map< string, map<u64, s32> >::iterator it = m_MembersGroup.find( user );
	if( it != m_MembersGroup.end() )
	{
		m_MembersGroup.erase( it );
	}
}

void CLoginMemberManager::GetGroups(const std::string &user, std::vector<u64> &vecGroups)
{
	vecGroups.clear();
	CGuardLock<CLock> g(m_Lock);
	map< string, map<u64, s32> >::iterator it = m_MembersGroup.find( user );
	if( it != m_MembersGroup.end() )
	{
		map<u64, s32>::iterator _tmp = it->second.begin();
		for( ; _tmp != it->second.end(); ++_tmp )
		{
			vecGroups.push_back( _tmp->first );
		}
	}
}

void CLoginMemberManager::DisplayInfo()
{
#if _DEBUG
	printf( "========= Member's Groups===========\n" );
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	map< string, map<u64, s32> >::iterator it = m_MembersGroup.begin();
	for( ; it != m_MembersGroup.end(); ++it )
	{
		printf( "\tUser:%s Groups:\n\t", it->first.c_str() );
		map<u64, s32>::iterator git = it->second.begin();
		for( ; git != it->second.end(); ++git )
		{
			printf( "%llu\t", git->first );
		}
		printf( "\n" );
	}
#endif
}

void CLoginMemberManager::_AddGroup(const std::string &user, u64 groupid)
{
	m_MembersGroup[ user ][ groupid ] = 1;
}
