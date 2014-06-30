#ifndef _GROUP_GATE_CLIENT_H_
#define _GROUP_GATE_CLIENT_H_

#include "Client.h"

class CGateClient : public CClient
{
public:
	CGateClient(int nId) : 
		CClient( nId ),
		m_isLogin( false )
	{
		//EnableSendDump();
	}

	virtual void OnRecv(const byte *pkg, s32 len);
	virtual void OnClose();

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CGateClient);
#endif

	bool IsLogin();
	void SetLogin(bool isLogin=true);

private:
	s32	_ParseForwardId(const byte *pkg, s32 len);
	bool _NeedCheckLogin(s32 msgid);

private:
	bool m_isLogin;
};

#endif
