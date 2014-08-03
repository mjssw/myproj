#include "ViewData.h"
using namespace std;

CUserViewData::CUserViewData() : 
	m_isSavePwd( false ),
	m_isAutoLogin( false ),
	m_selectGroup( 0 )
{
}

CUserViewData::~CUserViewData()
{
}

void CUserViewData::LoadFromConfig()
{
	// TODO 
	SetSavePwd( false );
	SetAutoLogin( false );
}

void CUserViewData::SetSavePwd(bool isSavePwd)
{
	m_isSavePwd = isSavePwd;
}

bool CUserViewData::GetSavePwd()
{
	return m_isSavePwd;
}

void CUserViewData::SetAutoLogin(bool isAutoLogin)
{
	m_isAutoLogin = isAutoLogin;
}

bool CUserViewData::GetAutoLogin()
{
	return m_isAutoLogin;
}

void CUserViewData::SetSelectGroup(CGroup *group)
{
	m_selectGroup = group;
}

CGroup* CUserViewData::GetSelectGroup()
{
	return m_selectGroup;
}

void CUserViewData::SetNewGroupName(const string &name)
{
	m_newGroupName = name;	
}

const string& CUserViewData::GetNewGroupName()
{
	return m_newGroupName;
}

void CUserViewData::SetNewGroupDone(bool done)
{
	m_isNewGroupDone = done;
}

bool CUserViewData::IsNewGroupDone()
{
	return m_isNewGroupDone;
}
