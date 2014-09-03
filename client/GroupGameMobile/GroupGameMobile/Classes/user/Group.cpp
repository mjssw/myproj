#include "Group.h"
#include "GroupMember.h"
using namespace std;

CGroup::CGroup(u64 id, const string &name, const string &head) : 
	m_id( id ),
	m_name( name ),
	m_head( head )
{
}

CGroup::~CGroup()
{
	_Destory();
}

u64 CGroup::GetId()
{
	return m_id;
}

const string& CGroup::GetName()
{
	return m_name;
}

const string& CGroup::GetHead()
{
	return m_head;
}

bool CGroup::AddMember(CGroupMember *member)
{
	if( !member )
	{
		return false;
	}

	map<string, CGroupMember*>::iterator it = m_members.find( member->GetUser() );
	if( it != m_members.end() )
	{
		return false;
	}

	m_members[ member->GetUser() ] = member;
}

void CGroup::RemoveMember(const std::string &user)
{
	map<string, CGroupMember*>::iterator it = m_members.find( user );
	if( it == m_members.end() )
	{
		return;
	}

	m_members.erase( it );
}

CGroupMember* CGroup::FindMember(const std::string &user)
{
	map<string, CGroupMember*>::iterator it = m_members.find( user );
	if( it == m_members.end() )
	{
		return NULL;
	}

	return it->second;
}

void CGroup::Dump(vector<string> &vec)
{
	vec.clear();
	map<string, CGroupMember*>::iterator it = m_members.begin();
	for( ; it != m_members.end(); ++it )
	{
		vec.push_back( it->first );
	}
}

vector<ChatTableViewData>& CGroup::GetChatHistory()
{
	return m_chatHistory; 
}

void CGroup::CheckChatHistoryLimit()
{
	if( m_chatHistory.size() >= 30 )
	{
		m_chatHistory.erase( m_chatHistory.begin(), m_chatHistory.begin()+2 );
	}
}

void CGroup::_Destory()
{
	map<string, CGroupMember*>::iterator it = m_members.begin();
	for( ; it != m_members.end(); ++it )
	{
		SAFE_DELETE( it->second );
	}
	m_members.clear();
}