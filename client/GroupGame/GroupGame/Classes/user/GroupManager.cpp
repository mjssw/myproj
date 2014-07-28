#include "GroupManager.h"
#include "Group.h"
using namespace std;

CGroupManager::CGroupManager()
{
}

CGroupManager::~CGroupManager()
{
}

bool CGroupManager::AddGroup(CGroup *group)
{
	if( !group )
	{
		return false;
	}

	map<u64, CGroup*>::iterator it = m_groups.find( group->GetId() );
	if( it != m_groups.end() )
	{
		return false;
	}

	m_groups[ group->GetId() ] = group;
}

void CGroupManager::RemoveGroup(u64 id)
{
	map<u64, CGroup*>::iterator it = m_groups.find( id );
	if( it == m_groups.end() )
	{
		return;
	}

	CGroup *group = it->second;
	m_groups.erase( it );
	SAFE_DELETE( group );
}

CGroup* CGroupManager::FindGroup(u64 id)
{
	map<u64, CGroup*>::iterator it = m_groups.find( id );
	if( it == m_groups.end() )
	{
		return NULL;
	}

	return it->second;
}

void CGroupManager::Dump(vector<u64> &vec)
{
	vec.clear();
	map<u64, CGroup*>::iterator it = m_groups.begin();
	for( ; it != m_groups.end(); ++it )
	{
		vec.push_back( it->first );
	}
}
