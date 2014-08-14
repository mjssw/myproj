#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"
#include "view/MyTableView.h"
#include "ViewBase.h"
#include "Define.h"

class CMyEditBox;
class CChatTableView;
class CMainScene : public cocos2d::CCLayer, public CViewBase
{
public:
	enum
	{
		E_Tag_ListView = 201,
		E_Tag_MainView,
	};
	virtual void UpdateView(int type);

    virtual bool init();  
    static cocos2d::CCScene* scene();
    void menuCloseCallback(cocos2d::Object *pSender);
	void menuTestCallback(cocos2d::Object *pSender);
	void menuRadioButtonGroupCallback(cocos2d::Object *pSender);
	void menuRadioButtonGameCallback(cocos2d::Object *pSender);
	void menuRadioButtonHomeCallback(cocos2d::Object *pSender);
	void menuFaceCallback(cocos2d::Object *pSender);
	void menuSendCallback(cocos2d::Object *pSender);
	void menuCreateGroupCallback(cocos2d::Object *pSender);
	void menuCreateGameCallback(cocos2d::Object *pSender);
	void menuInviteMemberCallback(cocos2d::Object *pSender);
	void menuMemberListCallback(cocos2d::Object *pSender);
	void menuLeaveGroupCallback(cocos2d::Object *pSender);
	void menuChatHistoryCallback(cocos2d::Object *pSender);

	void GroupListTouchedCallback(Node *pSender, void *data);

    CREATE_FUNC(CMainScene);

	static int GobackHome(lua_State *ls);

private:
	void _AddSceneBg();
	void _AddTitle();
	void _AddTabButtons();
	void _AddListView();
	void _AddMainView();

	void _AddContentToMainView();
	void _AddChatViewToMain();

	void _AddContentToListView();
	void _AddGroupsToListView();

	void _ClearListView();
	void _ClearMainView();
	void _ClearContentOnListView();
	void _ClearContentOnMainView();

	cocos2d::Sprite* _GetHead();
	int _AddHeadOnTitle(cocos2d::Node &parent);
	int _AddNameOnTitle(cocos2d::Node &parent, int off);
	int _AddLevelOnTitle(cocos2d::Node &parent, int off);
	void _AddGoldOnTitle(cocos2d::Node &parent);
	void _AddDiamondOnTitle(cocos2d::Node &parent);

	void _DumpGroupList(std::vector<TableViewData> &vecData);
	void _UpdateGroupList();

	void _AddCreateGroupBtn(cocos2d::Node &parent, cocos2d::Size &sz);
	void _AddGroupFuncBtns(cocos2d::Node &parent);

	void _LeaveGroup();
	void _AskJoinGroup();
	void _GroupMessageNotify();

	void _AddChatContent(u64 groupid, const std::string &user, const std::string &text, bool isSelf);

private:
	int m_mainViewOffX;
	cocos2d::CCPoint m_ptTableView;
	cocos2d::CCSize m_szTableView;
	int m_listHigh;
	int m_titleHigh;
	enum ETabButton
	{
		E_Tab_Group,
		E_Tab_Game,
		E_Tab_Home,
	} m_tabSelect;
	
	CMyTableView *m_pGroupList;
	CMyEditBox *m_chatHistory;
	CChatTableView *m_chatTableView;
};

#endif
