#ifndef _Group_Client_h_
#define _Group_Client_h_

#include "GateClientBase.h"
#include <map>
#include <vector>

class CGroupClient;
typedef void (CGroupClient::*ClientProtoProcFunc)(u64 clientid, const byte *pkg, s32 len);

class CGroupClient : public CGateClientBase
{
public:
	CGroupClient(s32 nId);
	virtual void ProcPlayerMessage(u64 gateid, u64 clientid, s32 msgid, const byte *pkg, s32 len);
	virtual void OnClientClose(u64 clientid);
	virtual void OnClose();
	virtual void OnAccept(); 

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CGroupClient);
#endif

private:
	void _RegisterClientProc(s32 id, ClientProtoProcFunc proc);
	void _NotifyGateClose();

	void _GroupUserLoginProc(u64 clientid, const byte *pkg, s32 len);
	void _GroupCreateProc(u64 clientid, const byte *pkg, s32 len);
	void _GroupSearchProc(u64 clientid, const byte *pkg, s32 len);
	void _GroupJoinProc(u64 clientid, const byte *pkg, s32 len);
	void _GroupAddMemberProc(u64 clientid, const byte *pkg, s32 len);
	void _GroupAgreeJoinProc(u64 clientid, const byte *pkg, s32 len);
	void _GroupLeaveProc(u64 clientid, const byte *pkg, s32 len);
	void _GroupDeleteProc(u64 clientid, const byte *pkg, s32 len);
	void _GroupMessageProc(u64 clientid, const byte *pkg, s32 len);
	void _GroupCreateGameRoomProc(u64 clientid, const byte *pkg, s32 len);
	void _GroupHistoryMessageProc(u64 clientid, const byte *pkg, s32 len);

private:
	std::map<s32, ClientProtoProcFunc> m_mapProtoProc;
};

#endif
