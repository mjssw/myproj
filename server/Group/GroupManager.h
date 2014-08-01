#ifndef _GROUP_MANAGER_H_
#define _GROUP_MANAGER_H_

#include "GroupInfo.h"
#include "Utils.h"
#include "google/protobuf/message.h"
#include "group.pb.h"
#include "DBQuery.h"

enum EGroupAskInfoAction
{
	E_AskInfoAction_AgreeJoin = 1,
};

class CGroupClient;
class CGroupManager
{
DEFINE_SINGLETON_CLASS(CGroupManager);

public:
	enum
	{
		E_ReportStatusInfo_Timer = 10000,
	};

	~CGroupManager();
	CGroupInfo* AddGroup(u64 id, const char *name, const char *icon);
	void DelGroup(u64 id);
	CGroupInfo* FindGroup(u64 id);
	s32 TotalGroupCount();
	s32 TotalMemberCount();
	s32 GroupMemberCount(u64 id);
	void SetGroupReady();
	
    void SendMsgToClient(u64 gateInstanceId, u64 clientid, const ::google::protobuf::Message &msg, s32 msgid);
    void SendMsgToClient(s32 gateResId, u64 clientid, const ::google::protobuf::Message &msg, s32 msgid);
	void BroadcastMsgToGates(const ::google::protobuf::Message &msg, s32 msgid);

	void StartReportStatusTimer(bool isDynamicStart);
	static void ReportStatusInforCallBack(void *pData, s32 nDataLen);

	void NotifyMemberJoinGroup(u64 groupid, const std::string &newMember);
	void BroadcastGroupMessage(u64 groupid, const std::string &sender, const std::string &content);
	void NotifyGroupInfoToMember(u64 groupid, const std::string &member);
	void NotifyAllGroupGateAddGroupInfo(u64 groupid, s32 serverid);
	void BroadcastMsgInGroup(u64 groupid, const ::google::protobuf::Message &msg, s32 msgid);

	// group client interface
	void UserLogin(CGroupClient &client, s32 gateresid, u64 clientid, const std::string &user, const std::string &token);
	void CreateGroup(CGroupClient &client, s32 gateresid, u64 gateid, u64 clientid, const std::string &groupName, const std::string &groupHead);
	void AddGroupMembers(CGroupClient &client, s32 gateresid, u64 gateid, u64 clientid, sglib::groupproto::CSGroupAddMemberReq &req);
	void AgreeJoinGroup(CGroupClient &client, s32 gateresid, u64 gateid, u64 clientid, u64 groupid);
	void LeaveGroup(CGroupClient &client, s32 gateresid, u64 gateid, u64 clientid, u64 groupid);
	void DeleteGroup(CGroupClient &client, s32 gateresid, u64 gateid, u64 clientid, u64 groupid);
	void GroupMessage(s32 gateresid, u64 gateid, u64 clientid, u64 groupid, const std::string &content);
	void UserLogout(s32 gateresid, u64 clientid);
	void CreateGroupGame(CGroupClient &client, s32 gateresid, u64 gateid, u64 clientid, u64 groupid, s32 game);

	// group rpc client interface
	void TryCreateGroup(u64 gateid, s32 gateresid, u64 clientid, const std::string &user, const std::string &username, const std::string &userhead, const std::string &name, u64 groupid, const std::string &head, u64 groupserverid);
	void CreateGroupResult(s32 result, u64 gateid, u64 clientid, const std::string &name, u64 groupid, const std::string &head, u64 groupserverid);
	void AddGroupMemberRsp(sglib::groupproto::GroupmanagerGroupAddMemberToGroupRsp &rsp);
	void AgreeJoinGroupAskInfoRsp(sglib::groupproto::GroupmanagerGroupForAgreeJoinAskInfoRsp &rsp);
	void LoadGroup(sglib::groupproto::GroupmanagerGroupLoadGroupNtf &ntf);
	void GroupMemberOnline(sglib::groupproto::GroupmanagerGroupMemberOnlineNtf &ntf);
	void GroupMemberOffline(sglib::groupproto::GroupmanagerGroupMemberOfflineNtf &ntf);
	void CreateGroupGameResult(sglib::groupproto::GroupmanagerGroupCreateGameRoomRsp &rsp);

	void DisplayInfo();
private:
	bool _ReportDynamicStarted();
	void _NotifyNewMemberJoin(u64 groupid, std::map<s32, std::vector<u64> > &alluser, CGroupMember &newMember);
	void _NotifyGroupInfoToMember(CGroupInfo &group, std::vector<std::string> &vecUser, CGroupMember &member);

	// group client
	s32 _CheckLoginValid(const std::string &user, const std::string &token);
	void _NotifyManagerUserLogin(s32 gateResId, u64 clientid, const std::string &user);
	void _NotifyGroupgateLoginSuccess(CGroupClient &client, u64 clientid);
	void _NotifyUserLoginResult(CGroupClient &client, s32 result, u64 clientid);
	void _NotifyManagerCreateGroup(u64 gateid, s32 gateresid, u64 clientid, const std::string &name, const std::string &head);
	void _NotifyAddGroupMemberRsp(CGroupClient &client, u64 clientid, u64 groupid, s32 result);
	void _NotifyManagerAddMemberToGroup(s32 gateresid, u64 gateid, u64 clientid, u64 groupid, std::vector<std::string> &vecUser);
	void _AskManagerInfoToAgree(s32 gateResId, u64 clientid, u64 groupid);
	void _DoMemberLeaveGroup(CGroupClient &client, s32 gateResId, u64 clientid, u64 groupid);
	void _NotifyMemberLeaveGroupRsp(CGroupClient &client, u64 clientid, s32 result, u64 groupid);
	void _NotifyGroupManagerMemberLeaveGroup(u64 groupid, const std::vector<std::string> &user);
	void _DoMemberDeleteGroup(CGroupClient &client, s32 gateResId, u64 clientid, u64 groupid);
	void _NotifyMemberDeleteGroupRsp(CGroupClient &client, u64 clientid, s32 result, u64 groupid);
	void _NotifyGroupGateDeleteGroup(u64 groupid);
	void _NotifyGroupManagerDeleteGroup(u64 groupid);
	void _NotifyGroupManagerCreateGameRoom(s32 gateResId, u64 clientid, u64 groupid, s32 game);

	// group rpc client
	void _NotifyGroupmanagerCreateGroupResult(s32 result, u64 gateid, u64 clientid, const char *user, const char *name, u64 groupid, const char *head, u64 serverid);
	void _NotifyClientCreateGroupRsp(s32 result, u64 gateid, u64 clientid, const char *name, u64 groupid, const char *head);
	s32  _DoLoadGroup(u64 groupid, s32 gateresid, u64 clientid, const char *user);
	void _NotifyLoadGroupResult(s32 result, u64 groupid, s32 serverid, const char *user);
	void _MemberOnline(u64 groupid, s32 gateresid, u64 clientid, const std::string &user);
	void _MemberOffline(u64 groupid, s32 gateresid, u64 clientid, const std::string &user);
	void _NotifyCreateGameRoomResult(s32 gateresid, u64 clientid, s32 result, u64 groupid, s32 game, const std::string &ip, s32 port, s32 roomid, const std::string &roompwd);

	void _LoadGroupDone(u64 groupid, const std::string &user);
	void _TryGetUserBasicInfo(const std::string &user, CGroupInfo &group);
	void _TryCreateGroup(CGroupInfo &group, CGroupMember &member);

private:
	void _GetGroupInfoCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len);
	void _GetGroupMemberCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len);
	void _GetUserBasicInfoCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len);
	void _CreateGroupCallback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len);

private:
	SGLib::CLock m_Lock;
	std::map<u64, CGroupInfo*> m_Groups;

	s32 m_nReportTimerId;

	// 动态启动服务器相关属性
	bool m_bIsDynamicStart;		// 此服务器是否是动态启动的
	bool m_bIsReady;			// 动态启动的服务器是否已经连接好gate
	bool m_bHasReported;		// 动态启动后是否已经向GroupManager汇报过
};

#endif
