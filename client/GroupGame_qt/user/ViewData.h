#ifndef _USER_VIEW_DATA_H_
#define _USER_VIEW_DATA_H_

#include <string>
#include <vector>
#include <deque>
#include "Define.h"

struct Chatmsg
{
	Chatmsg(u64 id, const std::string &sender_, const std::string &text) : 
		groupid(id),
		sender(sender_),
		context(text)
	{
	}
	u64 groupid;
	std::string sender;
	std::string context;	
};

class CGroup;
class CUserViewData
{
public:
	CUserViewData();
	~CUserViewData();

	void LoadFromConfig();

	void SetSavePwd(bool isSavePwd);
	bool GetSavePwd();
	void SetAutoLogin(bool isAutoLogin);
	bool GetAutoLogin();

	void SetSelectGroup(CGroup *group);
	CGroup* GetSelectGroup();

	void SetNewGroupName(const std::string &name);
	const std::string& GetNewGroupName();
	void SetNewGroupDone(bool done);
	bool IsNewGroupDone();

	void SetInviteUser(const std::string &user);
	const std::string& GetInviteUser();

	void SetJoinGroupInfo(u64 id, const std::string &name);
	u64 GetJoinGroupId();
	const std::string& GetJoinGroupName();

	void SetChatText(const std::string &text);
	const std::string& GetChatText();

	void PushChatMsg(u64 groupid, const std::string &sender, const std::string &content);
	Chatmsg* GetTopChatMsg();
	void PopChatMsg();

	void SetCurGameRoom(CGameRoomInfo *room);
    CGameRoomInfo* GetCurGameRoom();

    void ResetLoginProgress();
    void UpdateLoginProgress();

private:
	bool m_isSavePwd;
	bool m_isAutoLogin;
	CGroup *m_selectGroup;

	std::string m_newGroupName;
	bool m_isNewGroupDone;

	std::string m_inviteUser;

	u64 m_joinGroupId;
	std::string m_joinGroupName;

	std::string m_chatText;
	

	std::deque<Chatmsg> m_chatmsg;

    CGameRoomInfo *m_curGameRoom;

    enum { E_LoginProgress_Limit = 7, };
    s32 m_loginProgress;
};

#endif
