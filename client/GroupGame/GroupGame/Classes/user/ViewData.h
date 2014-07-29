#ifndef _USER_VIEW_DATA_H_
#define _USER_VIEW_DATA_H_

// ���һЩ����������ݴ���Ϣ
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

private:
	bool m_isSavePwd;
	bool m_isAutoLogin;
	CGroup *m_selectGroup;
};

#endif
