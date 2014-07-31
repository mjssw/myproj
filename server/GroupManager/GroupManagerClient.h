#ifndef _GroupManager_Client_h_
#define _GroupManager_Client_h_

#include "Client.h"
#include <map>
#include "google/protobuf/message.h"
#include "DBQuery.h"

class CGroupManagerClient;
typedef void (CGroupManagerClient::*ProtoProc)(const byte *pkg, s32 len);

class CGroupManagerClient : public CClient 
{
public:
	CGroupManagerClient(s32 nId);
	virtual void OnClose();
	virtual void OnRecv(const byte *pkg, s32 len);

#ifdef _USE_IOSERVICE_EX_
	DECLARE_IOSERVICEEX_CLONE_FUNC(CGroupManagerClient);
#endif
public:
	void SendMessageToClient(const ::google::protobuf::Message &msg, s32 msgid);
	void SendMessageToGroupServer(const ::google::protobuf::Message &msg, s32 msgid);

private:
	void _RegisterProc(s32 id, ProtoProc proc);
	void _SendMsgToCenter(const ::google::protobuf::Message &Msg, s32 nMsgId);

	void _GroupServerReportInfo(const byte *pkg, s32 len);
	void _GroupGateClose(const byte *pkg, s32 len);
	void _GroupServerDynamicStart(const byte *pkg, s32 len);
	void _UserLoginGroupProc(const byte *pkg, s32 len);
	void _UserLogoutGroupProc(const byte *pkg, s32 len);
	void _CreateGroupProc(const byte *pkg, s32 len);
	void _CreateGroupResultProc(const byte *pkg, s32 len);
	void _AddMemberToGroupProc(const byte *pkg, s32 len);
	void _AgreeJoinAskInfoProc(const byte *pkg, s32 len);
	void _LoadGroupResultProc(const byte *pkg, s32 len);
	void _GroupMemberLeaveProc(const byte *pkg, s32 len);
	void _GroupDeleteProc(const byte *pkg, s32 len);
	void _GameManagerReportInfo(const byte *pkg, s32 len);
	void _GroupCreateGameRoomProc(const byte *pkg, s32 len);
	void _GameManagerCreateRoomResultProc(const byte *pkg, s32 len);
	
private:
	void _NotifyCenterGateClose(s32 gateid);
	CGroupManagerClient* _FindUnDynamicServerClient(s32 serverid);
	void _NotifyNewServerDynamicStart(CGroupManagerClient *client, s32 id, const char *ip, s32 port);
	void _NotifyGroupServerClose(s32 serverid);
	void _NotifyGroupServerCreateGroup(u64 gateid, s32 gateresid, u64 clientid, const char *user, const char *name, u64 groupid, const char *head);
	void _NotifyGroupCreateResult(s32 result, u64 gateid, u64 clientid, const char *name, u64 groupid, const char *head, s32 serverid, u64 instanceId);
	void _DoLoadMemberGroups(const std::string &user, s32 gateresid, u64 clientid);
	void _NotifyLoadGroup(const std::string &user, s32 gateresid, u64 clientid, u64 groupid);
	void _NotifyMemberOnline(s32 groupServerId, const std::string &user, s32 gateresid, u64 clientid, u64 groupid);
	void _DoMemberLogout(const std::string &user, s32 gateresid, u64 clientid);
	void _NotifyMemberOffline(s32 groupServerId, const std::string &user, s32 gateresid, u64 clientid, u64 groupid);
	void _NotifyCreateGameRoomResult(u64 serverId, s32 result, u64 groupid, s32 gateresid, u64 clientid, s32 game, const char *ip, s32 port, s32 roomid, const char *roompwd);
	void _NotifyGameManagerCreateGameRoom(CGroupManagerClient *gameManager, u64 groupid, s32 gateresid, u64 clientid, s32 game, s32 gameid);

private:
	void _GetUserGroupsCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len);

private:
	std::map<s32, ProtoProc> m_mapProtoProc;
};

#endif
