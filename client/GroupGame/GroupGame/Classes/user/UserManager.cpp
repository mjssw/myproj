#include "UserManager.h"
using namespace std;

SIGNLETON_CLASS_INIT(CUserManager);


void CUserManager::SetToken(const string &token)
{
	m_token = token;
}

const string& CUserManager::GetToken()
{
	return m_token;
}

CUserBasic& CUserManager::GetBasic()
{
	return m_basic;
}

CUserViewData& CUserManager::GetViewData()
{
	return m_viewData;
}

CGameList& CUserManager::GetGameList()
{
	return m_games;
}

CGroupManager& CUserManager::GetGroupManager()
{
	return m_groupManager;
}

void CUserManager::SetGroupConnInfo(const string &ip, s32 port)
{
	m_groupGateIp = ip;
	m_groupGatePort = port;
}

const string& CUserManager::GetGroupGateIp()
{
	return m_groupGateIp;
}

s32 CUserManager::GetGroupGatePort()
{
	return m_groupGatePort;
}
