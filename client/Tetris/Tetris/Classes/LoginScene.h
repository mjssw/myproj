#ifndef _LOGIN_SCENE_H_
#define _LOGIN_SCENE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "GameWorldBase.h"

class DropDownList; 
class CursorTextField; 
class CLoginScene : public cocos2d::CCLayer, public CGameWorldBase
{
public:
	enum
	{
		E_Loading_Tag = 1,
		E_Start_Btn_Tag,
		E_Start_Btn_Tag2,
		E_Group_Btn_Tag2,
	};

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    virtual void onExit();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    void menuCloseCallback(CCObject* pSender);
    void menuStartCallback(CCObject* pSender);
    void menuOffGameStartCallback(CCObject* pSender);
	void menuGroupGameStartCallback(CCObject* pSender);
	void netMessageCallback(CCObject* pSender);

	virtual void ConnectError();

    // implement the "static node()" method manually
    CREATE_FUNC(CLoginScene);

private:
	CursorTextField *m_textField;
	DropDownList *m_dropList;
	bool _AddStartButton();
	void _AddOfflineStartButton();
	void _AddGroupStartButton();
	void _AddGroupUserList();
};

#endif
