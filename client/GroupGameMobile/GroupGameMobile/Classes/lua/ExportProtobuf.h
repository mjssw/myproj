#ifndef _EXPORT_PROTOBUF_H_
#define _EXPORT_PROTOBUF_H_

extern "C" {
#include "lua.h"
}

void register_dynamicproto(lua_State *ls);

#endif
