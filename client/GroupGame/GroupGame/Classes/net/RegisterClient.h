#ifndef _REGISTER_CLIENT_H_
#define _REGISTER_CLIENT_H_

#include "ProtoClientBase.h"
#include "login.pb.h"

class CRegisterClient : public CProtoClientBase
{
public:
	CRegisterClient(s32 id);
	virtual int OnRecv(char *buf, int len);
	virtual void OnConnect();
	virtual void OnClose();
	virtual void OnConnectError();
	
	void Register(const std::string &user, const std::string &pwd);

private:
	void _UserRegisterRsp(sglib::loginproto::SCUserRegisterRsp &msg);
};

#endif
