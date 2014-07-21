#include "ViewData.h"

CUserViewData::CUserViewData() : 
	m_isSavePwd( false ),
	m_isAutoLogin( false )
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
