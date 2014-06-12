#include "GroupInfo.h"
#include "ServerManager.h"

using namespace std;
using namespace SGLib;

void CGroupMember::SetOnline(bool bOnline, s32 gateResId, u64 clientId)
{ 
	m_bOnline = bOnline;

	if( bOnline )
	{
		m_GateResId = gateResId;
		m_ClientId = clientId;
		m_Group._AddMemberIndex( this );
	}
	else
	{
		m_Group._DelMemberIndex( this );
		m_GateResId = 0;
		m_ClientId = 0;
	}
}

CGroupInfo::CGroupInfo(u64 id, const char *name, const char *icon) : 
	m_Id(id),
	m_Name(name),
	m_Icon(icon)
{
}

CGroupInfo::~CGroupInfo()
{
	map<string, CGroupMember*>::iterator it = m_Members.begin();
	for( ; it != m_Members.end(); ++it )
	{
		SAFE_DELETE( it->second );
	}
	m_Members.clear();
}

u64 CGroupInfo::GetId()
{
	return m_Id;
}

const string& CGroupInfo::GetName()
{
	return m_Name;
}

const string& CGroupInfo::GetIcon()
{
	return m_Icon;
}

void CGroupInfo::SetName(const char *name)
{
	m_Name = name;
}

void CGroupInfo::SetIcon(const char *icon)
{
	m_Icon = icon;
}

CGroupMember* CGroupInfo::AddMember(const char *user, const char *nickname, const char *head, bool online)
{
	CGuardLock<CLock> g(m_Lock);

	std::map<std::string, CGroupMember*>::iterator it = m_Members.find( user );
	if( it == m_Members.end() )
	{
		CGroupMember *member = new CGroupMember( user, nickname, head, online, *this );
		if( member )
		{
			m_Members[ member->GetUser() ] = member;
			return member;
		}
		else
		{
			SERVER_LOG_ERROR( "CGroupInfo,AddMember,new CGroupMember," << user );
			return NULL;
		}
	}
	else
	{
		it->second->SetNickName( nickname );
		it->second->SetHead( head );
		return it->second;
	}

	return NULL;
}

void CGroupInfo::DelMember(const char *user)
{
	CGuardLock<CLock> g(m_Lock);
	map<string, CGroupMember*>::iterator it = m_Members.find( user );
	if( it != m_Members.end() )
	{
		SAFE_DELETE( it->second );
		m_Members.erase( it );
	}
}

CGroupMember* CGroupInfo::FindMember(const char *user)
{
	CGuardLock<CLock> g(m_Lock);
	map<string, CGroupMember*>::iterator it = m_Members.find( user );
	if( it != m_Members.end() )
	{
		return it->second;
	}

	return NULL;
}

CGroupMember* CGroupInfo::FindMember(s32 gateresid, u64 clientid)
{
	CGuardLock<CLock> g(m_Lock);
	map<s32, map<u64, CGroupMember*> >::iterator it = m_MemberIndex.find( gateresid );
	if( it != m_MemberIndex.end() )
	{
		map<u64, CGroupMember*>::iterator _tmp = it->second.find( clientid );
		if( _tmp != it->second.end() )
		{
			return _tmp->second;
		}
	}

	return NULL;
}

s32 CGroupInfo::MemberCount()
{
	CGuardLock<CLock> g(m_Lock);
	return (s32)m_Members.size();
}

void CGroupInfo::DumpMemberInfo(std::map<s32, std::vector<u64> > &alluser, const std::string &excludeUser, std::vector<std::string> &vecUser)
{
	alluser.clear();
	vecUser.clear();

	CGuardLock<CLock> g(m_Lock);
	map<string, CGroupMember*>::iterator it = m_Members.begin();
	for( ; it != m_Members.end(); ++it )
	{
		vecUser.push_back( it->first );

		if( it->first!=excludeUser && it->second )
		{
			if( it->second->IsOnline() )
			{
				alluser[ it->second->GetGateResId() ].push_back( it->second->GetClientId() );
			}
		}
	}
}

void CGroupInfo::_AddMemberIndex(CGroupMember *member)
{
	SG_ASSERT( member != NULL );
	CGroupMember *tmp = FindMember( member->GetUser().c_str() );
	SG_ASSERT( tmp != NULL );
	
	CGuardLock<CLock> g(m_Lock);
	m_MemberIndex[ member->GetGateResId() ][ member->GetClientId() ] = member;
}

void CGroupInfo::_DelMemberIndex(CGroupMember *member)
{
	SG_ASSERT( member != NULL );
	CGroupMember *tmp = FindMember( member->GetUser().c_str() );
	SG_ASSERT( tmp != NULL );

	CGuardLock<CLock> g(m_Lock);
	map<s32, map<u64, CGroupMember*> >::iterator it = m_MemberIndex.find( member->GetGateResId() );
	if( it != m_MemberIndex.end() )
	{
		map<u64, CGroupMember*>::iterator _tmp = it->second.find( member->GetClientId() );
		if( _tmp != it->second.end() )
		{
			it->second.erase( _tmp );
		}

		if( it->second.size() == 0 )
		{
			m_MemberIndex.erase( it );
		}
	}
}

void CGroupInfo::DisplayInfo()
{
#if _DEBUG
	CGuardLock<CLock> g(m_Lock);
	printf( ">>> Group:%llu\t%s\n", m_Id, m_Name.c_str() );
	printf( "------ Group Member Info ------\n" );
	std::map<std::string, CGroupMember*>::iterator it = m_Members.begin();
	for( ; it != m_Members.end(); ++it )
	{
		printf( "\tUser:%s\tNickName:%s\tHead:%s\tIsOnline:%d\tGateResId:%d\tClientId:%llu\n",
			it->second->GetUser().c_str(),
			it->second->GetNickName().c_str(),
			it->second->GetHead().c_str(),
			(it->second->IsOnline()?1:0),
			it->second->GetGateResId(),
			it->second->GetClientId() );
	}
	printf( "\n------ Group Member Index Info ------\n" );
	map<s32, map<u64, CGroupMember*> >::iterator idxit = m_MemberIndex.begin();
	for( ; idxit != m_MemberIndex.end(); ++idxit )
	{
		map<u64, CGroupMember*>::iterator mit = idxit->second.begin();
		printf( "\tGateResId:%d\n", idxit->first );
		for( ; mit != idxit->second.end(); ++mit )
		{
			printf( "\t\tClientId:%llu\tUser:%s\tIsOnline:%d\tGateResId:%d\tClientId:%llu\n",
				mit->first, 
				mit->second->GetUser().c_str(),
				(mit->second->IsOnline()?1:0),
				mit->second->GetGateResId(),
				mit->second->GetClientId() );
		}
	}

#endif
}
