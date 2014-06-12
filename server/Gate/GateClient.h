#ifndef _GATE_CLIENT__H_
#define _GATE_CLIENT__H_

#include "Client.h"

class CGateClient : public CClient
{
public:
	CGateClient(int nId) : 
		CClient(nId)
	{
		//EnableSendDump();
	}

	virtual void OnRecv(const byte *pPkg, s32 nPkgLen);
	virtual void OnClose(); 

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CGateClient);
#endif
};

#endif
