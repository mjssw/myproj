#ifndef _USER_MANAGER_H_
#define _USER_MANAGER_H_

#include "Define.h"
#include "Basic.h"
#include "ViewData.h"
#include "GameInfo.h"
#include <string>

class CUserManager
{
	SINGLETON_CLASS_DEFINE(CUserManager);

public:
	void SetToken(const std::string &token);
	const std::string& GetToken();
	CUserBasic& GetBasic();
	CUserViewData& GetViewData(); 
	CGameList& GetGameList();

	void SetGroupConnInfo(const std::string &ip, s32 port);
	const std::string& GetGroupGateIp();
	s32 GetGroupGatePort();

private:
	std::string m_token;
	CUserBasic m_basic;
	CUserViewData m_viewData;
	CGameList m_games;
	std::string m_groupGateIp;
	s32 m_groupGatePort;
};

#endif
