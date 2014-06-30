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

public:
	void NotifyAddGroupMemberRsp(u64 clientid, u64 groupid, s32 result);

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

    void _NotifyManagerUserLogin(s32 gateResId, u64 clientid, const char *user);
    void _NotifyUserLoginResult(s32 result, u64 clientid);
    void _NotifyManagerUserLogout(s32 gateResId, u64 clientid);
    void _NotifyManagerCreateGroup(u64 gateid, u64 clientid, const char *name);
	void _NotifyManagerAddMemberToGroup(u64 clientid, u64 groupid, std::vector<std::string> vecUser);
	void _AskManagerInfoToAgree(s32 gateResId, u64 clientid, u64 groupid);
	void _NotifyMemberLeaveGroupRsp(u64 clientid, s32 result, u64 groupid);
	void _NotifyGroupManagerMemberLeaveGroup(u64 groupid, const std::vector<std::string> &user);
	void _NotifyMemberDeleteGroupRsp(u64 clientid, s32 result, u64 groupid);
	void _NotifyGroupGateDeleteGroup(u64 groupid);
	void _NotifyGroupManagerDeleteGroup(u64 groupid);
	void _NotifyGroupManagerCreateGameRoom(s32 gateResId, u64 clientid, u64 groupid, s32 game);
	void _NotifyGroupgateLoginSuccess(u64 clientid);

	s32 _CheckLoginValid(const char *user);
	void _DoMemberLeaveGroup(s32 gateResId, u64 clientid, u64 groupid);
	void _DoMemberDeleteGroup(s32 gateResId, u64 clientid, u64 groupid);

private:
	std::map<s32, ClientProtoProcFunc> m_mapProtoProc;
};

#endif
