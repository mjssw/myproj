#ifndef _CENTER_CLIENT_H_
#define _CENTER_CLIENT_H_

#include "Client.h"
#include "Message.h"
#include <map>

class CCenterClient;
typedef void (CCenterClient::*ProtoProc)(const byte *pPkg, s32 nPkgLen);

class CCenterClient : public CClient
{
public:
	CCenterClient(int nId);
	virtual void OnRecv(const byte *pPkg, s32 nPkgLen);

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CCenterClient);
#endif

private:
	void _UserLogin(const byte *pPkg, s32 nPkgLen);
	void _UserLogout(const byte *pPkg, s32 nPkgLen);
	void _GameInfoReport(const byte *pPkg, s32 nPkgLen);
	void _UserAskEnterGame(const byte *pPkg, s32 nPkgLen);
	void _GameGateClose(const byte *pPkg, s32 nPkgLen);
	void _GameServerClose(const byte *pPkg, s32 nPkgLen);
	void _GroupGateClose(const byte *pPkg, s32 nPkgLen);
	void _GroupServerClose(const byte *pPkg, s32 nPkgLen);
	void _GroupGateInfoReport(const byte *pPkg, s32 nPkgLen);

private:
	void _RegisterProc(int id, ProtoProc proc);
	void _SendMsg(const ::google::protobuf::Message &Msg, s32 nMsgId);

private:
	std::map<int, ProtoProc> m_mapProtoProc;
};

#endif
