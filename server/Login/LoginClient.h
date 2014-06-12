#ifndef _LOGIN_CLIENT__H_
#define _LOGIN_CLIENT__H_

#include "GateClientBase.h"
#include "login.pb.h"
#include <map>

class CLoginClient;
typedef void (CLoginClient::*ClientProtoProcFunc)(u64 id, const byte *pPkg, s32 nPkgLen);

class CLoginClient : public CGateClientBase
{
public:
	CLoginClient(int nId);
	virtual void ProcPlayerMessage(u64 gateid, u64 clientid, s32 msgid, const byte *pkg, s32 len);

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CLoginClient);
#endif

private:
	// client msg
	void _UserLoginProc(u64 clientId, const byte *pPkg, s32 nPkgLen);
	void _UserEnterGameProc(u64 clientId, const byte *pPkg, s32 nPkgLen);
	void _UserRegisterProc(u64 clientId, const byte *pPkg, s32 nPkgLen);

private:
	void _RegisterClientProc(s32 id, ClientProtoProcFunc proc);

private:
	std::map<int, ClientProtoProcFunc> m_mapLoginProtoProc;
};

#endif
