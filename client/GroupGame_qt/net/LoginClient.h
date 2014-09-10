#ifndef _LOGIN_CLIENT_H_
#define _LOGIN_CLIENT_H_

#include "ProtoClientBase.h"
#include <string>
#include "login.pb.h"

class CLoginClient : public CProtoClientBase
{
public:
	CLoginClient(s32 id);
	virtual int OnRecv(char *buf, int len);
	virtual void OnConnect();
	virtual void OnClose();
	virtual void OnConnectError();

	void Login(const std::string &user, const std::string &pwd);

private:
	void _UserLoginRsp(sglib::loginproto::SCUserLoginRsp &msg);
	void _GameInfoNotify(sglib::loginproto::SCGameInfoNotify &msg);
	void _GroupGateNotify(sglib::loginproto::SCGroupGateNotify &msg);
	void _UserBasicInfoNotify(sglib::loginproto::SCUserBasicInfoNotify &msg);
};

#endif
