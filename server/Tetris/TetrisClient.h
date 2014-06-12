#ifndef _TETRIS_CLIENT__H_
#define _TETRIS_CLIENT__H_

#include "GateClientBase.h"
#include <map>

class CTetrisClient;
typedef void (CTetrisClient::*ClientProtoProcFunc)(u64 id, const byte *pkg, s32 len);

class CTetrisClient : public CGateClientBase
{
public:
	CTetrisClient(int nId);
	virtual void ProcPlayerMessage(u64 gateid, u64 clientid, s32 msgid, const byte *pkg, s32 len);
	virtual void OnClientClose(u64 clientid);
	virtual void OnClose();
	virtual void OnAccept();

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CTetrisClient);
#endif
private:
	void _LoginGameProc(u64 clientid, const byte *pkg, s32 len);
	void _LoginGame(u64 clientid, const std::string &userId, const std::string &token);
	void _NotifyLoginGameResult(u64 clientid, s32 result);
	void _NotifyGateClose();

	void _PlayerClose(u64 gateid, u64 clientid);
	void _RegisterClientProc(s32 id, ClientProtoProcFunc proc);

private:
	std::map<int, ClientProtoProcFunc> m_mapProtoProc;
};

#endif
