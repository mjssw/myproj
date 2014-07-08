#ifndef _LOGIN_SCENE_H_
#define _LOGIN_SCENE_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class CLoginScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    void menuTestCallback(CCObject *pSender);
    void menuLoginCallback(CCObject *pSender);
    void menuCloseCallback(CCObject *pSender);
    void menuSavePwdCheckBoxCallback(CCObject *pSender);
    void menuAutoLoginCheckBoxCallback(CCObject *pSender);

    CREATE_FUNC(CLoginScene);

	enum
	{
		E_Tag_LoginBg = 10,
	};

private:
	void _AddSceneBg();
	void _AddLoginView(bool isSavePwdSelect, bool isAutoLoginSelect);
	void _AddEditBox(cocos2d::CCSprite &loginBg);
	void _AddCheckBoxSavePwd(cocos2d::CCSprite &loginBg, bool isSelect);
	void _AddCheckBoxAuthLogin(cocos2d::CCSprite &loginBg, bool isSelect);
	void _AddCheckBtnLogin(cocos2d::CCSprite &loginBg);
	void _AddCheckBtnExit(cocos2d::CCSprite &loginBg);

	void _RemoveLoginView();

private:
	int m_inputFrom;
};

#endif
