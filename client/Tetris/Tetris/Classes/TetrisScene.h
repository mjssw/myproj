#ifndef _TETRIS_SCENE_H_
#define _TETRIS_SCENE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "GameWorldBase.h"

class CTetrisScene : public cocos2d::CCLayer, public CGameWorldBase
{
public:
	enum
	{
		E_TetrisScene_Loading,
	};

    virtual bool init();  
    virtual void onExit();

    static cocos2d::CCScene* scene();

	virtual void ConnectError(){}
	virtual void EnterRoomFailed();
	virtual void EnterRoom(const std::string& user);
	virtual void DisplayGameWorld(){ cocos2d::CCLog("CTetrisScene"); }

	void menuCloseCallback(CCObject* pSender);
	void menuEnterRoomCallback(CCObject* pSender);
	void CreateRoom(int count);

    CREATE_FUNC(CTetrisScene);

private:

};

#endif