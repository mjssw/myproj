#ifndef _USER_VIEW_DATA_H_
#define _USER_VIEW_DATA_H_

#include <string>
#include "Define.h"

// 玩家一些界面操作的暂存信息
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

private:
	bool m_isSavePwd;
	bool m_isAutoLogin;
	CGroup *m_selectGroup;

	std::string m_newGroupName;
	bool m_isNewGroupDone;

	std::string m_inviteUser;

	u64 m_joinGroupId;
	std::string m_joinGroupName;
};

#endif
