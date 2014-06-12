#ifndef _LUA_MODULE_H_
#define _LUA_MODULE_H_

#include "GameId.h"
#include "Define.h"
#include "Lock.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
};

class CLuaModule
{
public:
	static CLuaModule& Instance()
	{
		if( m_pInstance == NULL )
		{
			m_pInstance = new CLuaModule();
		}
		return *m_pInstance;
	}
private:
	CLuaModule() : m_pState(NULL)
	{}
	static CLuaModule *m_pInstance;

public:
	~CLuaModule()
	{
		Fini();
	}
	void Init()
	{
		if( m_pState == NULL )
		{
			m_pState = luaL_newstate();
			luaL_openlibs( m_pState );
        }
	}
	void Fini()
	{
		if( m_pState != NULL )
		{
			lua_close( m_pState );
			m_pState = NULL;
		}
	}
	bool LoadScripts(const char *script)
	{ 
		if( m_pState == NULL )
		{
			SGDEBUG( "[ERROR] Lua not init!\n");
			return false;
		}
		int ret = luaL_dofile( m_pState, script );
		if( ret != 0 )
		{
			SGDEBUG( "[ERROR] lua load file:%s errmsg:%s\n", script, lua_tostring(m_pState, -1) );
			return false;
		}
		
		return true;
	}

	s32 GetGameById(s32 id)
	{
		SGLib::CGuardLock<SGLib::CLock> g(m_Lock);

		if( m_pState == NULL )
        {
			SGDEBUG( "[ERROR] Lua not init!\n");
			return GAME_NOUSE;
        }

		const char *strFuncName = "GetGameById";
        lua_getglobal( m_pState, strFuncName );
        lua_pushnumber( m_pState, id );

        s32 game = lua_pcall( m_pState, 1, 1, 0 );
        if( game != 0 )
        {
			SGDEBUG( "[ERROR] lua call:%s errmsg:%s\n", strFuncName, lua_tostring(m_pState, -1) );
			return GAME_NOUSE;
        }

        if( lua_isnumber(m_pState, -1) == 1 )
        {
			s32 ret = lua_tonumber(m_pState, -1);
			lua_pop( m_pState, lua_gettop(m_pState) );
			return ret;
        }

		return GAME_NOUSE;
	}

private:
	SGLib::CLock m_Lock;
	lua_State *m_pState;
};

#endif
