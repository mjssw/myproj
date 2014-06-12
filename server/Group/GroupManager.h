#ifndef _GROUP_MANAGER_H_
#define _GROUP_MANAGER_H_

#include "GroupInfo.h"
#include "Utils.h"
#include "google/protobuf/message.h"

enum EGroupAskInfoAction
{
	E_AskInfoAction_AgreeJoin = 1,
};

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

	void DisplayInfo();
private:
	bool _ReportDynamicStarted();
	void _NotifyNewMemberJoin(u64 groupid, std::map<s32, std::vector<u64> > &alluser, CGroupMember &newMember);
	void _NotifyGroupInfoToMember(CGroupInfo &group, std::vector<std::string> &vecUser, CGroupMember &member);

private:
	SGLib::CLock m_Lock;
	std::map<u64, CGroupInfo*> m_Groups;

	s32 m_nReportTimerId;

	// ��̬�����������������
	bool m_bIsDynamicStart;		// �˷������Ƿ��Ƕ�̬������
	bool m_bIsReady;			// ��̬�����ķ������Ƿ��Ѿ����Ӻ�gate
	bool m_bHasReported;		// ��̬�������Ƿ��Ѿ���GroupManager�㱨��
};

#endif
