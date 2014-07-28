#ifndef _GROUP_CLIENT_H_
#define _GROUP_CLIENT_H_

#include "ProtoClientBase.h"
#include "group.pb.h"
#include <string>

class CGroupClient : public CProtoClientBase
{
public:
	CGroupClient(s32 id);
	virtual int OnRecv(char *buf, int len);
	virtual void OnConnect();
	virtual void OnClose();
	virtual void OnConnectError();

	void Login(const std::string &user, const std::string &token);

private:
	void _LoginGroupResult(sglib::groupproto::SCGroupUserLoginRsp &msg);
	void _GroupListUpdate(sglib::groupproto::SCGroupListUpdate &msg);
	void _GroupInfoUpdate(sglib::groupproto::SCGroupInfoUpdate &msg);
};

#endif
