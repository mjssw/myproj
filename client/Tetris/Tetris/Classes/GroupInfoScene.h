#ifndef _GROUP_INFO_SCENE_H_
#define _GROUP_INFO_SCENE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "GameWorldBase.h"

class CGroupInfoScene : public cocos2d::CCLayer, public CGameWorldBase
{
public:

    virtual bool init();  
    virtual void onExit();

    static cocos2d::CCScene* scene();

	virtual void DisplayGameWorld(){ cocos2d::CCLog("CGroupInfoScene"); }
	virtual void CreateGroupGameRoom();

	void menuCloseCallback(CCObject* pSender);
	void menuBackCallback(CCObject* pSender);
	void menuAddGameCallback(CCObject* pSender);
	void menuEnterGroupGameRoomCallback(CCObject* pSender);

    CREATE_FUNC(CGroupInfoScene);

private:
	void _InitMemberList();
	void _InitAddGame();
};

#endif