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

CGameRoomList& CUserManager::GetGameRoomList()
{
	return m_roomList;
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

/*
bool CUserManager::LoadPublicScripts()
{
	m_pbState = lua_open();
	luaL_openlibs( m_pbState );
	return (luaL_dofile( m_pbState, "./public/GetGameInfo.lua" ) == 0);
}

void CUserManager::GetGameInfo(int gameid, std::string &dir, std::string &icon, std::string &name)
{
	const char *strFuncName = "GetGameInfo";
	lua_getglobal( m_pbState, strFuncName );
	lua_pushnumber( m_pbState, gameid );

	s32 game = lua_pcall( m_pbState, 1, 3, 0 );
	if( game != 0 || lua_gettop(m_pbState) != 3 )
	{
		lua_pop( m_pbState, lua_gettop(m_pbState) );
		return;
	}

	if( lua_isstring(m_pbState, -1) == 1 )
	{
		name = lua_tostring( m_pbState, -1 );
	}
	if( lua_isstring(m_pbState, -2) == 1 )
	{
		icon = lua_tostring( m_pbState, -2 );
	}
	if( lua_isstring(m_pbState, -3) == 1 )
	{
		dir = lua_tostring( m_pbState, -3 );
	}

	lua_pop( m_pbState, lua_gettop(m_pbState) );
}

void CUserManager::RegisterLuaInterface()
{
	lua_register(LuaEngine::getInstance()->getLuaStack()->getLuaState(), "GetUser", GetUser );
	lua_register(LuaEngine::getInstance()->getLuaStack()->getLuaState(), "GetToken", GetToken );
	lua_register(LuaEngine::getInstance()->getLuaStack()->getLuaState(), "GetRoomId", GetRoomId );
}

// ------------------------------------------------------------------

int CUserManager::GetUser(lua_State *ls)
{
	lua_pushstring( ls, Instance().GetBasic().GetUser().c_str() );
	return 1;
}

int CUserManager::GetToken(lua_State *ls)
{
	lua_pushstring( ls, Instance().GetToken().c_str() );
	return 1;
}

int CUserManager::GetRoomId(lua_State *ls)
{
	if( Instance().GetViewData().GetCurGameRoom() )
	{
		lua_pushnumber( ls, Instance().GetViewData().GetCurGameRoom()->m_roomid );
		return 1;
	}
	return 0;
}
//*/

