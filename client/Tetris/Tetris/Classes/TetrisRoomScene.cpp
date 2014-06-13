#include "TetrisRoomScene.h"
#include "utils.h"
#include "NetManager.h"
#include "GameManager.h"
#include "Room.h"
#include "TetrisScene.h"
using namespace cocos2d;

CCScene* CTetrisRoomScene::scene()
{
    CCScene *scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        CTetrisRoomScene *layer = CTetrisRoomScene::create();
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}

bool CTetrisRoomScene::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		// 背景
		cocos2d::CCSprite *pSpriteBg = CCSprite::create( PATCH_RES_DIR("bg2.png") );
		pSpriteBg->setScale(BG_SCALE);
		pSpriteBg->setAnchorPoint( ccp(0, 0) );
		pSpriteBg->setPosition( ccp(0, 0) );
		addChild( pSpriteBg, 0 );

        // 关闭按钮
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            PATCH_RES_DIR("close2.png"),
            PATCH_RES_DIR("close1.png"),
            this,
            menu_selector(CTetrisRoomScene::menuCloseCallback) );
        CC_BREAK_IF( !pCloseItem );
		pCloseItem->setAnchorPoint( ccp(0, 0) );
		float w = visibleSize.width - pCloseItem->getContentSize().width - 5;
		float h = 5;
		pCloseItem->setPosition( ccp(w, h) );

        CCMenu *pMenu = CCMenu::create( pCloseItem, NULL );
        pMenu->setPosition( CCPointZero );
        CC_BREAK_IF( !pMenu );
        addChild( pMenu, 1 );

		// 返回按钮
		CCMenuItemImage *pBackItem = CCMenuItemImage::create(
            PATCH_RES_DIR("btn_back_normal.png"),
            PATCH_RES_DIR("btn_back_over.png"),
            this,
            menu_selector(CTetrisRoomScene::menuBackCallback) );
        CC_BREAK_IF( !pBackItem );
		pBackItem->setAnchorPoint( ccp(0, 0) );
		float sle = pCloseItem->getContentSize().height / pBackItem->getContentSize().height;
		pBackItem->setScale( sle );
		CCSize sz = pBackItem->getContentSize() * sle;
		w = visibleSize.width - pCloseItem->getContentSize().width - 10 - sz.width;
		h = 5;
		pBackItem->setPosition( ccp(w, h) );

        pMenu = CCMenu::create( pBackItem, NULL );
        pMenu->setPosition( CCPointZero );
        CC_BREAK_IF( !pMenu );
        addChild( pMenu, 1 );

		_InitRoom();
		if( CGameManager::Instance().GetEnemy() != "" )
		{
			EnterRoom( CGameManager::Instance().GetEnemy() );
		}
        bRet = true;

		CGameManager::Instance().SetGameWorld( this );
			
		cocos2d::CCLog( "tetris room scene init done" );
    } while (0);

    return bRet;
}

void CTetrisRoomScene::onExit()
{
	CCLayer::onExit();
}

void CTetrisRoomScene::menuCloseCallback(CCObject* pSender)
{
	CNetManager::Instance().Stop();
    CCDirector::sharedDirector()->end();
}

void CTetrisRoomScene::menuBackCallback(CCObject* pSender)
{
	CNetManager::Instance().LeaveRoom();
	//cocos2d::CCDirector::sharedDirector()->replaceScene( CTetrisScene::scene() );
}

void CTetrisRoomScene::menuReadyCallback(CCObject* pSender)
{
	CNetManager::Instance().Ready();
}

void CTetrisRoomScene::_InitRoom()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	cocos2d::CCSprite *pSpriteBg = CCSprite::create( PATCH_RES_DIR("playerbg.png") );
	cocos2d::CCSprite *pVSBg = CCSprite::create( PATCH_RES_DIR("VS.png") );
	pVSBg->setAnchorPoint( ccp(0, 0) );
	int grap = (visibleSize.width - 2*pSpriteBg->getContentSize().width -  pVSBg->getContentSize().width) / 4;

	// self bg
	pSpriteBg->setAnchorPoint( ccp(0, 0) );
	int x = grap;
	int y = (visibleSize.height * 2 ) / 3 - pSpriteBg->getContentSize().height;
	pSpriteBg->setPosition( ccp(x, y) );
	addChild( pSpriteBg, 0 );
	int sx = x;
	int sy = y;
	m_SelfX = sx;
	m_SelfY = sy;

	// VS bg
	x += (grap + pSpriteBg->getContentSize().width );
	y += (pSpriteBg->getContentSize().height - pVSBg->getContentSize().height) / 2;
	pVSBg->setPosition( ccp(x, y) );
	addChild( pVSBg, 0 );

	// enemy bg
	pSpriteBg = CCSprite::create( PATCH_RES_DIR("playerbg.png") );
	pSpriteBg->setAnchorPoint( ccp(0, 0) );
	x = (visibleSize.width - grap - pSpriteBg->getContentSize().width );
	y = (visibleSize.height * 2 ) / 3 - pSpriteBg->getContentSize().height;
	pSpriteBg->setPosition( ccp(x, y) );
	addChild( pSpriteBg, 0 );
	m_EnemyX = x;
	m_EnemyY = y;
	m_bgSize = pSpriteBg->getContentSize();

	// self enter
	float scale = 0.6;
	cocos2d::CCSprite *pSelf = CCSprite::create( PATCH_RES_DIR("self.png") );
	pSelf->setScale( scale );
	pSelf->setAnchorPoint( ccp(0, 0) );
	CCSize newSize = pSelf->getContentSize() * scale;
	x = sx + (pSpriteBg->getContentSize().width - newSize.width) / 2;
	y = sy + (pSpriteBg->getContentSize().height - newSize.height) / 4;
	pSelf->setPosition( ccp(x, y) );
	addChild( pSelf, 1 );

	// ready button
	CCMenuItemImage *pReadyItem = CCMenuItemImage::create(
		PATCH_RES_DIR("btn_common_normal.png"),
		PATCH_RES_DIR("btn_common_over.png"),
		this,
		menu_selector(CTetrisRoomScene::menuReadyCallback) );
	pReadyItem->setScale( scale );
	pReadyItem->setAnchorPoint( ccp(0, 0) );
	newSize = pReadyItem->getContentSize() * scale; 
	int w = (visibleSize.width - newSize.width) / 2;
	int h = (m_SelfY - newSize.height) * 2 / 3;
	pReadyItem->setPosition( ccp(w, h) );

	CCMenu *pMenu = CCMenu::create( pReadyItem, NULL );
	pMenu->setPosition( CCPointZero );
	addChild( pMenu, 1, E_TetrisRoomScene_ReadyBtnTag );

	CNetManager::Instance().ResumeProcMsg();
}

void CTetrisRoomScene::EnterRoom(const std::string& user)
{
	CCLOG( "user:%s enter", user.c_str() );
	if( user != CGameManager::Instance().GetUser() )
	{
		// 敌人进入房间
		_EnemyEnterRoom();
		CGameManager::Instance().SetEnemy( user );
	}
}

void CTetrisRoomScene::LeaveRoom(const std::string& user)
{
	CCLOG( "user:%s leave", user.c_str() );
	if( user != CGameManager::Instance().GetUser() )
	{
		// 敌人离开房间
		removeChildByTag( E_TetrisRoomScene_EnemyTag );
		removeChildByTag( E_TetrisRoomScene_EnemyReadyTag );
		
		CGameManager::Instance().DelEnemy();
	}
}

void CTetrisRoomScene::_EnemyEnterRoom()
{
	float scale = 0.6;
	cocos2d::CCSprite *pEnemy = CCSprite::create( PATCH_RES_DIR("enemy.png") );
	pEnemy->setScale( scale );
	pEnemy->setAnchorPoint( ccp(0, 0) );
	CCSize newSize = pEnemy->getContentSize() * scale;
	int x = m_EnemyX + (m_bgSize.width - newSize.width) / 2;
	int y = m_EnemyY + (m_bgSize.height - newSize.height) / 4;
	pEnemy->setPosition( ccp(x, y) );
	addChild( pEnemy, 1, E_TetrisRoomScene_EnemyTag );
	
	CCLOG( "enemy enter" );
}

void CTetrisRoomScene::PlayerReady()
{
	removeChildByTag( E_TetrisRoomScene_ReadyBtnTag );
	
	float scale = 0.5;
	CCSprite *pSpriteBg = CCSprite::create( PATCH_RES_DIR("ready3.png") );
	pSpriteBg->setScale( scale );
	pSpriteBg->setAnchorPoint( ccp(0, 0) );
	CCSize newSize = pSpriteBg->getContentSize() * scale;
	int x = m_SelfX + (m_bgSize.width - newSize.width) / 2;
	int y = m_SelfY + 10;
	pSpriteBg->setPosition( ccp(x, y) );
	addChild( pSpriteBg, 2, E_TetrisRoomScene_SelfReadyTag );
}

void CTetrisRoomScene::EnemyReady()
{
	float scale = 0.5;
	CCSprite *pSpriteBg = CCSprite::create( PATCH_RES_DIR("ready3.png") );
	pSpriteBg->setScale( scale );
	pSpriteBg->setAnchorPoint( ccp(0, 0) );
	CCSize newSize = pSpriteBg->getContentSize() * scale;
	int x = m_EnemyX + (m_bgSize.width - newSize.width) / 2;
	int y = m_EnemyY + 10;
	pSpriteBg->setPosition( ccp(x, y) );
	addChild( pSpriteBg, 2, E_TetrisRoomScene_EnemyReadyTag );
}

void CTetrisRoomScene::RoomInfo(const std::string &enemy, bool isReady)
{
	EnterRoom( enemy );
	if( isReady )
	{
		EnemyReady();
	}
}
