#ifndef _LOGIN_SCENE_H_
#define _LOGIN_SCENE_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class CMyEditBox;
class CLoginScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    void menuTestCallback(CCObject *pSender);
    void menuLoginCallback(CCObject *pSender);
    void menuCloseCallback(CCObject *pSender);
    void menuCancelCallback(CCObject *pSender);
    void menuSavePwdCheckBoxCallback(CCObject *pSender);
    void menuAutoLoginCheckBoxCallback(CCObject *pSender);

    CREATE_FUNC(CLoginScene);

	enum
	{
		E_Tag_LoginBg = 10,
		E_Tag_Loading,
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
	void _AddLoadingCancelBtn(cocos2d::CCNode &loadingNode);

	void _RemoveLoginView();
	void _UpdateLoadingStep();
	void _RemoveLoadingView();
};

#endif
