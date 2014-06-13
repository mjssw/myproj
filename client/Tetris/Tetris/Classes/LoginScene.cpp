#include "LoginScene.h"
#include "utils.h"
#include "GameList.h"
#include "NetManager.h"
#include "GameManager.h"
#include "TetrisBattleScene.h"
#include "CursorTextControl.h"
#include "cocos-ext.h"
#include "DropDownList.h"
using namespace cocos2d;
using namespace cocos2d::extension;
	

CCScene* CLoginScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        CLoginScene *layer = CLoginScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CLoginScene::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		// 背景
		cocos2d::CCSprite *pSpriteBg = CCSprite::create( PATCH_RES_DIR("bg.png") );//
		pSpriteBg->setScale(BG_SCALE);
		pSpriteBg->setAnchorPoint( ccp(0, 0) );
		pSpriteBg->setPosition( ccp(0, 0) );
		addChild( pSpriteBg, 0 );

        // 关闭按钮
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            PATCH_RES_DIR("close2.png"),
            PATCH_RES_DIR("close1.png"),
            this,
            menu_selector(CLoginScene::menuCloseCallback) );
        CC_BREAK_IF( !pCloseItem );
		pCloseItem->setAnchorPoint( ccp(0, 0) );
		float w = visibleSize.width - pCloseItem->getContentSize().width - 5;
		float h = 5;
		pCloseItem->setPosition( ccp(w, h) );

        CCMenu *pMenu = CCMenu::create( pCloseItem, NULL );
        pMenu->setPosition( CCPointZero );
        CC_BREAK_IF( !pMenu );
        addChild( pMenu, 1 );

		// 标题
        CCLabelTTF *pLabel = CCLabelTTF::create( a2u("欢迎进入游戏世界").c_str(), "Arial", 24 );
		pLabel->setColor( ccc3(255,0,0) );
        CC_BREAK_IF( !pLabel );
        pLabel->setPosition(ccp(visibleSize.width / 2, visibleSize.height - 100));
        addChild( pLabel, 1 );

		// 登录开始游戏按钮
		bool badd = _AddStartButton();
        CC_BREAK_IF( !badd );

		_AddOfflineStartButton();
		_AddGroupStartButton();
		_AddGroupUserList();

		m_textField = CursorTextField::textFieldWithPlaceHolder(
			"Input Ip", "Thonburi", 32 );
		m_textField->setAnchorPoint(ccp(0, 0) );
		m_textField->setPosition(ccp( (visibleSize.width - m_textField->getContentSize().width)/2, 20));
		addChild(m_textField);

		/*
		// 注册网络消息
		CCNotificationCenter::sharedNotificationCenter()->addObserver(
			this,
			callfuncO_selector(CLoginScene::netMessageCallback),
			NET_MSG,
			NULL );
			//*/

        bRet = true;

		CGameManager::Instance().SetGameWorld( this );

		CGameManager::Instance().Test();
    } while (0);

    return bRet;
}

void CLoginScene::onExit()
{
	CCLayer::onExit();
	//CCNotificationCenter::sharedNotificationCenter()->removeObserver( this, NET_MSG );
}

void CLoginScene::menuCloseCallback(CCObject* pSender)
{
	CNetManager::Instance().Stop();
    CCDirector::sharedDirector()->end();
}

void CLoginScene::menuOffGameStartCallback(CCObject* pSender)
{
	CGameManager::Instance().SetOfflineGame();
	menuStartCallback( pSender );
}

void CLoginScene::menuGroupGameStartCallback(CCObject* pSender)
{
	string defaultIp = "172.18.5.55";// "127.0.0.1"
	string ip = m_textField->getString();
	if( ip == "" )
	{
		ip = defaultIp;  
	}
	CCLog( "try connect %s:10011", ip.c_str() );

	string user = m_dropList->getString();
	if( user == "" )
	{
		user = "sjj1";
	}
	CCLog( "%s try login group", user.c_str() );
	CGameManager::Instance().SetUser( user.c_str() );
	//return;
	
	if( CNetManager::Instance().StartGroup( ip.c_str(), 10011 ) )
	{
		CNetManager::Instance().SetState( CNetManager::E_State_Connecting_GroupServer );

		// 隐藏开始按钮
		removeChildByTag( E_Start_Btn_Tag );
		removeChildByTag( E_Start_Btn_Tag2 );
		removeChildByTag( E_Group_Btn_Tag2 );

		// 播放loading动画
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		cocos2d::CCSprite *pSpriteLoading = CCSprite::create( PATCH_RES_DIR("loading.png") );
		float w = (visibleSize.width) / 2;
		float h = (visibleSize.height) / 2;
		pSpriteLoading->setPosition( ccp(w, h) );

		CCActionInterval *actionBy = CCRotateBy::create( 0.8, 360 );
		CCActionInterval *seq = (CCActionInterval*)( CCSequence::create(actionBy, NULL) );
		CCAction *act = CCRepeatForever::create( seq );

		addChild( pSpriteLoading, 1 , E_Loading_Tag );
		pSpriteLoading->runAction( act );
	}
}

void CLoginScene::menuStartCallback(CCObject* pSender)
{
	string defaultIp = "172.18.5.55";// "127.0.0.1"
	string ip = m_textField->getString();
	if( ip == "" )
	{
		ip = defaultIp;
	}
	CCLog( "try connect %s:10003", ip.c_str() );

	string user = m_dropList->getString();
	if( user == "" )
	{
		user = "sjj1";
	}
	CGameManager::Instance().SetUser( user.c_str() );

	if( CGameManager::Instance().IsOfflineGame() )
	{
		cocos2d::CCDirector::sharedDirector()->replaceScene( CTetrisBattleScene::scene() );
	}
	else
	{
		if( CNetManager::Instance().Start( ip.c_str(), 10003 ) )
		{
			CNetManager::Instance().SetState( CNetManager::E_State_Connecting_LoginServer );

			// 隐藏开始按钮
			removeChildByTag( E_Start_Btn_Tag );
			removeChildByTag( E_Start_Btn_Tag2 );
			removeChildByTag( E_Group_Btn_Tag2 );

			// 播放loading动画
			CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
			cocos2d::CCSprite *pSpriteLoading = CCSprite::create( PATCH_RES_DIR("loading.png") );
			float w = (visibleSize.width) / 2;
			float h = (visibleSize.height) / 2;
			pSpriteLoading->setPosition( ccp(w, h) );

			CCActionInterval *actionBy = CCRotateBy::create( 0.8, 360 );
			CCActionInterval *seq = (CCActionInterval*)( CCSequence::create(actionBy, NULL) );
			CCAction *act = CCRepeatForever::create( seq );

			addChild( pSpriteLoading, 1 , E_Loading_Tag );
			pSpriteLoading->runAction( act );
		}
	}
}

void CLoginScene::netMessageCallback(CCObject* pSender)
{
	CNetManager::Instance().ProcMessage();
}

void CLoginScene::ConnectError()
{
	removeChildByTag( E_Loading_Tag );
	_AddStartButton();
	_AddOfflineStartButton();
	_AddGroupStartButton();
	CNetManager::Instance().Stop();
	CNetManager::Instance().StopGroup();
}

bool CLoginScene::_AddStartButton()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCMenuItemImage *pStartItem = CCMenuItemImage::create(
		PATCH_RES_DIR("login1.png"),
		PATCH_RES_DIR("login2.png"),
		this,
		menu_selector(CLoginScene::menuStartCallback) );
	if( !pStartItem ) 
	{
		return false;
	}
	pStartItem->setScale(BG_SCALE);
	pStartItem->setAnchorPoint( ccp(0, 0) );
	CCSize sz = pStartItem->getContentSize() * BG_SCALE;
	float w = (visibleSize.width - sz.width) / 2;
	float h = (visibleSize.height- sz.height) / 2 + 20;
	pStartItem->setPosition( ccp(w, h) );

	CCMenu *pStartMenu = CCMenu::create( pStartItem, NULL );
	pStartMenu->setPosition( CCPointZero );
	if( !pStartMenu )
	{
		return false;
	}
	addChild( pStartMenu, 1, E_Start_Btn_Tag );

	return true;
}

void CLoginScene::_AddOfflineStartButton()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCMenuItemImage *pStartItem = CCMenuItemImage::create(
		PATCH_RES_DIR("offgame2.png"),
		PATCH_RES_DIR("offgame3.png"),
		this,
		menu_selector(CLoginScene::menuOffGameStartCallback) );
	if( !pStartItem ) 
	{
		return ;
	}
	pStartItem->setScale(BG_SCALE);
	pStartItem->setAnchorPoint( ccp(0, 0) );
	CCSize sz = pStartItem->getContentSize() * BG_SCALE;
	float w = (visibleSize.width - sz.width) / 2;
	float h = (visibleSize.height- sz.height) / 2 - sz.height + 15;
	pStartItem->setPosition( ccp(w, h) );

	CCMenu *pStartMenu = CCMenu::create( pStartItem, NULL );
	pStartMenu->setPosition( CCPointZero );
	if( !pStartMenu )
	{
		return ;
	}
	addChild( pStartMenu, 1, E_Start_Btn_Tag2 );
}

void CLoginScene::_AddGroupStartButton()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCMenuItemImage *pStartItem = CCMenuItemImage::create(
		PATCH_RES_DIR("btn_group_normal.png"),
		PATCH_RES_DIR("btn_group_over.png"),
		this,
		menu_selector(CLoginScene::menuGroupGameStartCallback) );
	if( !pStartItem ) 
	{
		return ;
	}
	pStartItem->setScale(BG_SCALE);
	pStartItem->setAnchorPoint( ccp(0, 0) );
	CCSize sz = pStartItem->getContentSize() * BG_SCALE;
	float w = (visibleSize.width - sz.width) / 2;
	float h = sz.height;
	pStartItem->setPosition( ccp(w, h) );

	CCMenu *pStartMenu = CCMenu::create( pStartItem, NULL );
	pStartMenu->setPosition( CCPointZero );
	if( !pStartMenu )
	{
		return ;
	}
	addChild( pStartMenu, 1, E_Group_Btn_Tag2 );
}

void CLoginScene::_AddGroupUserList()
{
	CCLabelTTF* labeldropdown = new CCLabelTTF();
	labeldropdown->initWithString( a2u("sjj1").c_str(), "Arial", 22);
	CCSize itemSize = CCSizeMake(80,30);
	//azl::DropDownList* pdd = azl::DropDownList::create(labeldropdown,itemSize);
	DropDownList *pdd = DropDownList::create(labeldropdown,itemSize);

	CCLabelTTF* dd1 = new CCLabelTTF();
	dd1->initWithString( a2u("sjj1").c_str(), "Arial", 22);
	pdd->addLabel(dd1);

	CCLabelTTF* dd2 = new CCLabelTTF();
	dd2->initWithString( a2u("sjj2").c_str(), "Arial", 22);
	pdd->addLabel(dd2);
	
	CCLabelTTF* dd3 = new CCLabelTTF();
	dd3->initWithString( a2u("sjj3").c_str(), "Arial", 22);
	pdd->addLabel(dd3);

	pdd->setSelectedIndex(0);

	pdd->setPosition(10,80);
	addChild(pdd,2);

	m_dropList = pdd;
	//*/
}
