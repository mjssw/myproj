#ifndef _TEST_CLIENT_H_
#define _TEST_CLIENT_H_

#include "GateClientBase.h"
#include <map>

class CTestClient;
typedef void (CTestClient::*ClientProtoProcFunc)(u64 id, const byte *pkg, s32 len);

class CTestClient: public CGateClientBase
{
public:
	CTestClient(int nId);
#ifndef _FACE_TO_GATE
	virtual void OnRecv(const byte *pPkg, s32 nPkgLen);
#endif
	virtual void ProcPlayerMessage(u64 gateid, u64 clientid, s32 msgid, const byte *pkg, s32 len);
	virtual void OnClientClose(u64 clientid);

#ifdef _USE_IOSERVICE_EX_
	virtual CIoSocketEx* Clone(s32 id)
	{
		return new CTestClient( id );
	}
#endif

private:
	void _RegisterClientProc(s32 id, ClientProtoProcFunc proc);
	void _TestMsg(u64 id, const byte *pkg, s32 len);

private:
	std::map<int, ClientProtoProcFunc> m_mapProtoProc;
};

#endif
