#ifndef _PROTO_CLIENT_BASE_H_
#define _PROTO_CLIENT_BASE_H_

#include "ClientLib.h"
#include "google/protobuf/message.h"

class CProtoClientBase : public SGLib::CClientBase
{
public:
	CProtoClientBase(s32 id);
	void SendMsg(const ::google::protobuf::Message &msg, s32 msgid);
};

#endif
