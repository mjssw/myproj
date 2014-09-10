#ifndef _USER_MANAGER_H_
#define _USER_MANAGER_H_

#include "GameRoomInfo.h"
#include "Define.h"
#include "Basic.h"
#include "ViewData.h"
#include "GameInfo.h"
#include "GroupManager.h"
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
	CGroupManager& GetGroupManager();
	CGameRoomList& GetGameRoomList();

	void SetGroupConnInfo(const std::string &ip, s32 port);
	const std::string& GetGroupGateIp();
	s32 GetGroupGatePort();

    /*
	bool LoadPublicScripts();
	void GetGameInfo(int gameid, std::string &dir, std::string &icon, std::string &name);
	void RegisterLuaInterface();
    //*/

public:
	// for lua interface
//	static int GetUser(lua_State *ls);
//	static int GetToken(lua_State *ls);
//	static int GetRoomId(lua_State *ls);

private:
	std::string m_token;
	CUserBasic m_basic;
	CUserViewData m_viewData;
	CGameList m_games;
	std::string m_groupGateIp;
	s32 m_groupGatePort;
	CGroupManager m_groupManager;

    //lua_State *m_pbState;

	CGameRoomList m_roomList;
};

#endif
