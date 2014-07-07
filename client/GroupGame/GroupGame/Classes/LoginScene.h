#ifndef _LOGIN_SCENE_H_
#define _LOGIN_SCENE_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class CLoginScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    void menuCloseCallback(CCObject *pSender);

    CREATE_FUNC(CLoginScene);

private:
	bool _AddSceneBg();
	bool _AddLoginBg(int x, int y, int h);
	bool _AddLoginBtn(int loginBgX, int loginBgY, int loginBgW, int loginBgH);
	bool _AddExitBtn(int loginBgX, int loginBgY, int loginBgW, int loginBgH);
};

#endif
