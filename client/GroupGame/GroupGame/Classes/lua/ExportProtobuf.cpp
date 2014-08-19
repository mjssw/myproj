#include "ExportProtobuf.h"
#include "lua/DynamicProto.h"
using namespace google;
using namespace google::protobuf;
using namespace std;

int lua_dynamicproto_Init(lua_State *ls)
{
    int argc = lua_gettop( ls );
    if( argc == 1 )
    {
		string protodir = lua_tostring( ls, 1 );
		CDynamicProto::Instance().Init( protodir );
        return 1;
    }
    return 0;
}

int lua_dynamicproto_CreateMesage(lua_State *ls)
{
    int argc = lua_gettop( ls );
    if( argc == 2 )
    {
        string protofile = lua_tostring( ls, 1 );
        string name = lua_tostring( ls, 2 );
		Message *ret = CDynamicProto::Instance().CreateMessage( protofile, name );
        *(void**)lua_newuserdata(ls, sizeof(void*)) = ret;
		return 1;
    }
    return 0;
}

int lua_dynamicproto_MutableMessage(lua_State* ls)
{
	int argc = lua_gettop( ls );
    if( argc == 2 )
    {
		void **data = (void**)lua_touserdata( ls, 1 );
		Message *parent = ((Message*)(*data));
        string field = lua_tostring( ls, 2 );
		Message *ret = CDynamicProto::Instance().MutableMessage( parent, field );
        *(void**)lua_newuserdata(ls, sizeof(void*)) = ret;
		return 1;
	}
    
	return 0;
}

int lua_dynamicproto_AddMessage(lua_State* ls)
{
	int argc = lua_gettop( ls );
    if( argc == 2 )
    {
		void **data = (void**)lua_touserdata( ls, 1 );
		Message *parent = ((Message*)(*data));
        string field = lua_tostring( ls, 2 );
		Message *ret = CDynamicProto::Instance().AddMessage( parent, field );
        *(void**)lua_newuserdata(ls, sizeof(void*)) = ret;
		return 1;
	}
	return 0;
}

int lua_dynamicproto_DeleteMessage(lua_State *ls)
{
	int argc = lua_gettop( ls );
    if( argc == 1 )
    {
		void **data = (void**)lua_touserdata( ls, 1 );
		Message *msg = ((Message*)(*data));
		CDynamicProto::Instance().DeleteMessage( msg );
		return 1;
	}
	return 0;
}

#define _LUA_SET_MESSAGE_VALUE(type, lua_func, setfunc) \
	int argc = lua_gettop( ls );\
    if( argc == 3 )\
    {\
		void **data = (void**)lua_touserdata( ls, 1 );\
		Message *msg = ((Message*)(*data));\
        string field = lua_tostring( ls, 2 );\
		type value = (type)lua_func( ls, 3 );\
		bool ret = CDynamicProto::Instance().setfunc( msg, field, value );\
		lua_pushboolean( ls, ret );\
		return 1;\
	}\
	return 0;

int lua_dynamicproto_SetInt32(lua_State* ls)
{
	_LUA_SET_MESSAGE_VALUE( int32, lua_tonumber, SetInt32 );
}

int lua_dynamicproto_SetInt64(lua_State* ls)
{
	_LUA_SET_MESSAGE_VALUE( int64, lua_tonumber, SetInt64 );
}

int lua_dynamicproto_SetUInt32(lua_State* ls)
{
	_LUA_SET_MESSAGE_VALUE( uint32, lua_tonumber, SetUInt32 );
}

int lua_dynamicproto_SetUInt64(lua_State* ls)
{
	_LUA_SET_MESSAGE_VALUE( uint64, lua_tonumber, SetUInt64 );
}

int lua_dynamicproto_SetFloat(lua_State* ls)
{
	_LUA_SET_MESSAGE_VALUE( float, lua_tonumber, SetFloat );
}

int lua_dynamicproto_SetDouble(lua_State* ls)
{
	_LUA_SET_MESSAGE_VALUE( double, lua_tonumber, SetDouble );
}

int lua_dynamicproto_SetBool(lua_State* ls)
{
	_LUA_SET_MESSAGE_VALUE( bool, lua_toboolean, SetBool );
}

int lua_dynamicproto_SetString(lua_State* ls)
{
	_LUA_SET_MESSAGE_VALUE( string, lua_tostring, SetString );
}

#undef _LUA_SET_MESSAGE_VALUE
	
#define _LUA_ADD_MESSAGE_VALUE(type, lua_func, addfunc) \
	int argc = lua_gettop( ls );\
    if( argc == 3 )\
    {\
		void **data = (void**)lua_touserdata( ls, 1 );\
		Message *msg = ((Message*)(*data));\
        string field = lua_tostring( ls, 2 );\
		type value = (type)lua_func( ls, 3 );\
		bool ret = CDynamicProto::Instance().addfunc( msg, field, value );\
		lua_pushboolean( ls, ret );\
		return 1;\
	}\
	return 0;

int lua_dynamicproto_AddInt32(lua_State* ls)
{
	_LUA_ADD_MESSAGE_VALUE( int32, lua_tonumber, AddInt32 );
}

int lua_dynamicproto_AddInt64(lua_State* ls)
{
	_LUA_ADD_MESSAGE_VALUE( int64, lua_tonumber, AddInt64 );
}

int lua_dynamicproto_AddUInt32(lua_State* ls)
{
	_LUA_ADD_MESSAGE_VALUE( uint32, lua_tonumber, AddUInt32 );
}

int lua_dynamicproto_AddUInt64(lua_State* ls)
{
	_LUA_ADD_MESSAGE_VALUE( uint64, lua_tonumber, AddUInt64 );
}

int lua_dynamicproto_AddFloat(lua_State* ls)
{
	_LUA_ADD_MESSAGE_VALUE( float, lua_tonumber, AddFloat );
}

int lua_dynamicproto_AddDouble(lua_State* ls)
{
	_LUA_ADD_MESSAGE_VALUE( double, lua_tonumber, AddDouble );
}

int lua_dynamicproto_AddBool(lua_State* ls)
{
	_LUA_ADD_MESSAGE_VALUE( bool, lua_toboolean, AddBool );
}

int lua_dynamicproto_AddString(lua_State* ls)
{
	_LUA_ADD_MESSAGE_VALUE( string, lua_tostring, AddString );
}

#undef _LUA_ADD_MESSAGE_VALUE

int lua_dynamicproto_ParseFromArray(lua_State* ls)
{
	int argc = lua_gettop( ls );
    if( argc == 3 )
    {
		void **data = (void**)lua_touserdata( ls, 1 );
		Message *msg = ((Message*)(*data));

		data = (void**)lua_touserdata( ls, 2 );
		void *d = (void*)(*data);

		int size = lua_tonumber( ls, 3 );

		bool ret = CDynamicProto::Instance().ParseFromArray( msg, d, size );
		lua_pushboolean( ls, ret );
		return 1;
	}
	return 0;
}

#define _LUA_GET_MESSAGE_VALUE(type, getfunc, luafunc) \
	int argc = lua_gettop( ls );\
    if( argc == 2 )\
    {\
		void **data = (void**)lua_touserdata( ls, 1 );\
		Message *msg = ((Message*)(*data));\
        string field = lua_tostring( ls, 2 );\
		type ret = CDynamicProto::Instance().getfunc( *msg, field );\
		luafunc( ls, ret );\
		return 1;\
	}\
	return 0;

int lua_dynamicproto_GetInt32(lua_State* ls)
{
	_LUA_GET_MESSAGE_VALUE( int32, GetInt32, lua_pushnumber );
}

int lua_dynamicproto_GetInt64(lua_State* ls)
{
	_LUA_GET_MESSAGE_VALUE( int64, GetInt64, lua_pushnumber );
}

int lua_dynamicproto_GetUInt32(lua_State* ls)
{
	_LUA_GET_MESSAGE_VALUE( uint32, GetUInt32, lua_pushnumber );
}

int lua_dynamicproto_GetUInt64(lua_State* ls)
{
	_LUA_GET_MESSAGE_VALUE( uint64, GetUInt64, lua_pushnumber );
}

int lua_dynamicproto_GetFloat(lua_State* ls)
{
	_LUA_GET_MESSAGE_VALUE( float, GetFloat, lua_pushnumber );
}

int lua_dynamicproto_GetDouble(lua_State* ls)
{
	_LUA_GET_MESSAGE_VALUE( double, GetDouble, lua_pushnumber );
}

int lua_dynamicproto_GetBool(lua_State* ls)
{
	_LUA_GET_MESSAGE_VALUE( bool, GetBool, lua_pushboolean );
}

#undef _LUA_GET_MESSAGE_VALUE

int lua_dynamicproto_GetString(lua_State* ls)
{
	int argc = lua_gettop( ls );
    if( argc == 2 )
    {
		void **data = (void**)lua_touserdata( ls, 1 );
		Message *msg = ((Message*)(*data));
        string field = lua_tostring( ls, 2 );
		string ret = CDynamicProto::Instance().GetString( *msg, field );
		lua_pushstring( ls, ret.c_str() );
		return 1;
	}
	return 0;
}

int lua_dynamicproto_GetMessage(lua_State* ls)
{
	int argc = lua_gettop( ls );
    if( argc == 2 )
    {
		void **data = (void**)lua_touserdata( ls, 1 );
		Message *msg = ((Message*)(*data));
        string field = lua_tostring( ls, 2 );
		const Message *ret = CDynamicProto::Instance().GetMessage( *msg, field );
        *(const void**)lua_newuserdata(ls, sizeof(void*)) = ret;
		return 1;
	}
	return 0;
}


#define _LUA_GET_REPEATED_MESSAGE_VALUE(type, getfunc, luafunc) \
	int argc = lua_gettop( ls );\
    if( argc == 3 )\
    {\
		void **data = (void**)lua_touserdata( ls, 1 );\
		Message *msg = ((Message*)(*data));\
        string field = lua_tostring( ls, 2 );\
		int index = (int)lua_tonumber( ls, 3 );\
		type ret = CDynamicProto::Instance().getfunc( *msg, field, index );\
		luafunc( ls, ret );\
		return 1;\
	}\
	return 0;

int lua_dynamicproto_GetRepeatedInt32(lua_State* ls)
{
	_LUA_GET_REPEATED_MESSAGE_VALUE( int32, GetRepeatedInt32, lua_pushnumber );
}

int lua_dynamicproto_GetRepeatedInt64(lua_State* ls)
{
	_LUA_GET_REPEATED_MESSAGE_VALUE( int64, GetRepeatedInt64, lua_pushnumber );
}

int lua_dynamicproto_GetRepeatedUInt32(lua_State* ls)
{
	_LUA_GET_REPEATED_MESSAGE_VALUE( uint32, GetRepeatedUInt32, lua_pushnumber );
}

int lua_dynamicproto_GetRepeatedUInt64(lua_State* ls)
{
	_LUA_GET_REPEATED_MESSAGE_VALUE( uint64, GetRepeatedUInt64, lua_pushnumber );
}

int lua_dynamicproto_GetRepeatedFloat(lua_State* ls)
{
	_LUA_GET_REPEATED_MESSAGE_VALUE( float, GetRepeatedFloat, lua_pushnumber );
}

int lua_dynamicproto_GetRepeatedDouble(lua_State* ls)
{
	_LUA_GET_REPEATED_MESSAGE_VALUE( double, GetRepeatedDouble, lua_pushnumber );
}

int lua_dynamicproto_GetRepeatedBool(lua_State* ls)
{
	_LUA_GET_REPEATED_MESSAGE_VALUE( bool, GetRepeatedBool, lua_pushboolean );
}

#undef _LUA_GET_REPEATED_MESSAGE_VALUE

int lua_dynamicproto_GetRepeatedString(lua_State* ls)
{
	int argc = lua_gettop( ls );
    if( argc == 3 )
    {
		void **data = (void**)lua_touserdata( ls, 1 );
		Message *msg = ((Message*)(*data));
        string field = lua_tostring( ls, 2 );
		int index = (int)lua_tonumber( ls, 3 );
		string ret = CDynamicProto::Instance().GetRepeatedString( *msg, field, index );
		lua_pushstring( ls, ret.c_str() );
		return 1;
	}
	return 0;
}

int lua_dynamicproto_GetRepeatedMessage(lua_State* ls)
{
	int argc = lua_gettop( ls );
    if( argc == 3 )
    {
		void **data = (void**)lua_touserdata( ls, 1 );
		Message *msg = ((Message*)(*data));
        string field = lua_tostring( ls, 2 );
		int index = (int)lua_tonumber( ls, 3 );
		const Message *ret = CDynamicProto::Instance().GetRepeatedMessage( *msg, field, index );
        *(const void**)lua_newuserdata(ls, sizeof(void*)) = ret;
		return 1;
	}
	return 0;
}

int lua_dynamicproto_GetFieldSize(lua_State* ls)
{
	int argc = lua_gettop( ls );
    if( argc == 2 )
    {
		void **data = (void**)lua_touserdata( ls, 1 );
		Message *msg = ((Message*)(*data));
        string field = lua_tostring( ls, 2 );
		int ret = CDynamicProto::Instance().GetFieldSize( *msg, field );
		lua_pushnumber( ls, ret );
		return 1;
	}
	return 0;
}

void register_dynamicproto(lua_State *ls)
{
	lua_register( ls, "DPB_Init", lua_dynamicproto_Init );
	lua_register( ls, "DPB_CreateMessage", lua_dynamicproto_CreateMesage );
	lua_register( ls, "DPB_MutableMessage", lua_dynamicproto_MutableMessage );
	lua_register( ls, "DPB_AddMessage", lua_dynamicproto_AddMessage );
	lua_register( ls, "DPB_DeleteMessage", lua_dynamicproto_DeleteMessage );
	
	lua_register( ls, "DPB_SetInt32", lua_dynamicproto_SetInt32 );
	lua_register( ls, "DPB_SetInt64", lua_dynamicproto_SetInt64 ); 
	lua_register( ls, "DPB_SetUInt32", lua_dynamicproto_SetUInt32 );
	lua_register( ls, "DPB_SetUInt64", lua_dynamicproto_SetUInt64 );
	lua_register( ls, "DPB_SetFloat", lua_dynamicproto_SetFloat );
	lua_register( ls, "DPB_SetDouble", lua_dynamicproto_SetDouble );
	lua_register( ls, "DPB_SetBool", lua_dynamicproto_SetBool );
	lua_register( ls, "DPB_SetString", lua_dynamicproto_SetString );
	
	lua_register( ls, "DPB_AddInt32", lua_dynamicproto_AddInt32 );
	lua_register( ls, "DPB_AddInt64", lua_dynamicproto_AddInt64 );
	lua_register( ls, "DPB_AddUInt32", lua_dynamicproto_AddUInt32 );
	lua_register( ls, "DPB_AddUInt64", lua_dynamicproto_AddUInt64 );
	lua_register( ls, "DPB_AddFloat", lua_dynamicproto_AddFloat );
	lua_register( ls, "DPB_AddDouble", lua_dynamicproto_AddDouble );
	lua_register( ls, "DPB_AddBool", lua_dynamicproto_AddBool );  
	lua_register( ls, "DPB_AddString", lua_dynamicproto_AddString );
	
	lua_register( ls, "DPB_ParseFromArray", lua_dynamicproto_ParseFromArray );

	lua_register( ls, "DPB_GetInt32", lua_dynamicproto_GetInt32 );
	lua_register( ls, "DPB_GetInt64", lua_dynamicproto_GetInt64 );
	lua_register( ls, "DPB_GetUInt32", lua_dynamicproto_GetUInt32 );
	lua_register( ls, "DPB_GetUInt64", lua_dynamicproto_GetUInt64 );
	lua_register( ls, "DPB_GetFloat", lua_dynamicproto_GetFloat );
	lua_register( ls, "DPB_GetDouble", lua_dynamicproto_GetDouble );
	lua_register( ls, "DPB_GetBool", lua_dynamicproto_GetBool );
	lua_register( ls, "DPB_GetString", lua_dynamicproto_GetString );
	lua_register( ls, "DPB_GetMessage", lua_dynamicproto_GetMessage );

	lua_register( ls, "DPB_GetRepeatedInt32", lua_dynamicproto_GetRepeatedInt32 );
	lua_register( ls, "DPB_GetRepeatedInt64", lua_dynamicproto_GetRepeatedInt64 );
	lua_register( ls, "DPB_GetRepeatedUInt32", lua_dynamicproto_GetRepeatedUInt32 );
	lua_register( ls, "DPB_GetRepeatedUInt64", lua_dynamicproto_GetRepeatedUInt64 );
	lua_register( ls, "DPB_GetRepeatedFloat", lua_dynamicproto_GetRepeatedFloat );
	lua_register( ls, "DPB_GetRepeatedDouble", lua_dynamicproto_GetRepeatedDouble );
	lua_register( ls, "DPB_GetRepeatedBool", lua_dynamicproto_GetRepeatedBool );
	lua_register( ls, "DPB_GetRepeatedString", lua_dynamicproto_GetRepeatedString );
	lua_register( ls, "DPB_GetRepeatedMessage", lua_dynamicproto_GetRepeatedMessage );
	
	lua_register( ls, "DPB_GetFieldSize", lua_dynamicproto_GetFieldSize );
}
