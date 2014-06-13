#ifndef _TETRIS_ROOM_SCENE_H_
#define _TETRIS_ROOM_SCENE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "GameWorldBase.h"

class CTetrisRoomScene : public cocos2d::CCLayer, public CGameWorldBase
{
public:
	enum
	{
		E_TetrisRoomScene_EnemyTag,
		E_TetrisRoomScene_SelfReadyTag,
		E_TetrisRoomScene_EnemyReadyTag,
		E_TetrisRoomScene_ReadyBtnTag,
	};

    virtual bool init();  
    virtual void onExit();

    static cocos2d::CCScene* scene();

	virtual void ConnectError(){}
	virtual void EnterRoom(const std::string& user);
	virtual void LeaveRoom(const std::string& user);
	virtual void DisplayGameWorld(){ cocos2d::CCLog("CTetrisRoomScene"); }
	virtual void PlayerReady();
	virtual void EnemyReady();
	virtual void RoomInfo(const std::string &enemy, bool isReady);
	
	void menuCloseCallback(CCObject* pSender);
	void menuBackCallback(CCObject* pSender);
	void menuReadyCallback(CCObject* pSender);

    CREATE_FUNC(CTetrisRoomScene);

private:
	void _InitRoom();
	void _EnemyEnterRoom();

	int m_EnemyX;
	int m_EnemyY;
	int m_SelfX;
	int m_SelfY;
	cocos2d::CCSize m_bgSize;
};

#endif
