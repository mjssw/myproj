#include "GroupMember.h"
using namespace std;

CGroupMember::CGroupMember(const string &user) : 
	m_user( user )
{
}

CGroupMember::~CGroupMember()
{
}

const string& CGroupMember::GetUser()
{
	return m_user;
}

void CGroupMember::SetName(const string &name)
{
	m_name = name;
}

const string& CGroupMember::GetName()
{
	return m_name;
}

void CGroupMember::SetHead(const string &head)
{
	m_head = head;
}

const string& CGroupMember::GetHead()
{
	return m_head;
}

void CGroupMember::SetOnline(bool online)
{
	m_isonline = online;
}

bool CGroupMember::IsOnline()
{
	return m_isonline;
}
