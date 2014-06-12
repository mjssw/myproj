#ifndef _GAME_MGR_CLIENT_H_
#define _GAME_MGR_CLIENT_H_

#include "Client.h"
#include "google/protobuf/message.h"
#include <map>

class CGameMgrClient;
typedef void (CGameMgrClient::*ProtoProc)(const byte *pPkg, s32 nPkgLen);

class CGameMgrClient : public CClient
{
public:
	CGameMgrClient(int nId);
	virtual void OnClose();
	virtual void OnRecv(const byte *pPkg, s32 nPkgLen);

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CGameMgrClient);
#endif

public:
	void SendMessageToGameServer(const ::google::protobuf::Message &msg, s32 msgid);

private:
	void _RegisterProc(s32 id, ProtoProc proc);
	void _SendMsgToCenter(const ::google::protobuf::Message &Msg, s32 nMsgId);

	void _AskEnterGame(const byte *pPkg, s32 nPkgLen);
	void _GameServerReportInfo(const byte *pPkg, s32 nPkgLen);
	void _GameGateClose(const byte *pPkg, s32 nPkgLen);
	void _GameServerDynamicStart(const byte *pPkg, s32 nPkgLen);
	void _GameServerCreateGameRoomResult(const byte *pPkg, s32 nPkgLen);
	
private:
	void _NotifyCenterGateClose(s32 gameid, s32 gateid);
	CGameMgrClient* _FindUnDynamicServerClient(s32 gameid, s32 serverid);
	void _NotifyNewServerDynamicStart(CGameMgrClient *client, s32 id, const char *ip, s32 port);
	void _NotifyGameServerClose(s32 gameid, s32 serverid);

private:
	std::map<int, ProtoProc> m_mapProtoProc;
};

#endif
