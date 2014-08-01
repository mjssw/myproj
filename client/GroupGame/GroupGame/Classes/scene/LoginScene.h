#ifndef _LOGIN_SCENE_H_
#define _LOGIN_SCENE_H_

#include "cocos2d.h"
#include "ViewBase.h"

class CMyEditBox;
class CMyLoadingItem;
class CLoginScene : public cocos2d::CCLayer, public CViewBase
{
public:
	virtual void UpdateView(int type);

    virtual bool init();  
    static cocos2d::CCScene* scene();
	void TestTimer(float t);
    void menuTestCallback(cocos2d::Object *pSender);
    void menuLoginCallback(cocos2d::Object *pSender);
    void menuCloseCallback(cocos2d::Object *pSender);
    void menuCancelCallback(cocos2d::Object *pSender);
    void menuSavePwdCheckBoxCallback(cocos2d::Object *pSender);
    void menuAutoLoginCheckBoxCallback(cocos2d::Object *pSender);
	void menuRegisterCallback(cocos2d::Object *pSender);

    CREATE_FUNC(CLoginScene);

	enum
	{
		E_Tag_LoginBg = 10,
		E_Tag_Loading,
		E_Tag_Loading_Tail,
	};

private:
	void _AddSceneBg();
	void _AddLoginView(bool isSavePwdSelect, bool isAutoLoginSelect);
	void _AddEditBox(cocos2d::CCSprite &loginBg);
	void _AddCheckBoxSavePwd(cocos2d::CCSprite &loginBg, bool isSelect);
	void _AddCheckBoxAuthLogin(cocos2d::CCSprite &loginBg, bool isSelect);
	void _AddBtnLogin(cocos2d::CCSprite &loginBg);
	void _AddBtnExit(cocos2d::CCSprite &loginBg);
	void _AddLoadingView(int steps);
	void _AddLoadingIcon(cocos2d::CCNode &loadingNode);
	void _AddLoadingText(cocos2d::CCNode &loadingNode);
	void _AddLoadingProgress(cocos2d::CCNode &loadingNode, int steps);
	void _AddLoadingCancelBtn(cocos2d::CCNode &loadingNode);
	void _AddRegisterBnt(cocos2d::CCSprite &loginBg);

	void _RemoveLoginView();
	void _UpdateLoadingStep();
	void _RemoveLoadingView();
	void _UserLogin();
	void _UserLoginGroup();
			
	void _DoRegister();
	void _RegisterResult();

	void JumpMainSceneCallback(float t);
private:
	struct
	{
		CMyLoadingItem *m_loadItem;
	} m_loadView;

	cocos2d::Node *m_regPopLayer;
};

#endif
