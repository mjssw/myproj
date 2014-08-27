#include "cocos2d.h"
#include "ProtoClientBase.h"
#include "CommDef.h"
using namespace cocos2d;

CProtoClientBase::CProtoClientBase(s32 id) : 
	CClientBase( id )
{
}

void CProtoClientBase::SendMsg(const ::google::protobuf::Message &msg, s32 msgid)
{
	int total = msg.ByteSize();
	char buf[MAX_MSG_LEN] = {0};
	*((s32*)(buf)) = total + MSG_ID_LEN + MSG_HEAD_LEN;
	*((s32*)(buf + MSG_HEAD_LEN)) = msgid;
	if( msg.SerializeToArray(buf+MSG_ID_LEN+MSG_HEAD_LEN, MAX_MSG_LEN) )
	{
		if( !Send(buf, total+MSG_ID_LEN+MSG_HEAD_LEN) )
		{
			Fini();
		}
		else
		{
			CCLog("[CProtoClientBase] send msg: [%d] %d", GetSocket(), msgid );
		}
	}
	else
	{
		CCLog( "[CProtoClientBase] Msg:%d SerializeToArray failed", msgid );
	}
}
