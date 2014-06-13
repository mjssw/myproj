#include "TetrisScene.h"
#include "utils.h"
#include "GameList.h"
#include "NetManager.h"
#include "GameManager.h"
#include "Room.h"
using namespace cocos2d;

CCScene* CTetrisScene::scene()
{
    CCScene *scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        CTetrisScene *layer = CTetrisScene::create();
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}

bool CTetrisScene::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		// ±³¾°
		cocos2d::CCSprite *pSpriteBg = CCSprite::create( PATCH_RES_DIR("bg2.png") );
		pSpriteBg->setScale(BG_SCALE);
		pSpriteBg->setAnchorPoint( ccp(0, 0) );
		pSpriteBg->setPosition( ccp(0, 0) );
		addChild( pSpriteBg, 0 );

        // ¹Ø±Õ°´Å¥
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            PATCH_RES_DIR("close2.png"),
            PATCH_RES_DIR("close1.png"),
            this,
            menu_selector(CTetrisScene::menuCloseCallback) );
        CC_BREAK_IF( !pCloseItem );
		pCloseItem->setAnchorPoint( ccp(0, 0) );
		float w = visibleSize.width - pCloseItem->getContentSize().width - 5;
		float h = 5;
		pCloseItem->setPosition( ccp(w, h) );

        CCMenu *pMenu = CCMenu::create( pCloseItem, NULL );
        pMenu->setPosition( CCPointZero );
        CC_BREAK_IF( !pMenu );
        addChild( pMenu, 1 );

		CreateRoom( 20 );
        bRet = true;
			
		CNetManager::Instance().ResumeProcMsg();

		CGameManager::Instance().SetGameWorld( this );
    } while (0);

    return bRet;
}

void CTetrisScene::onExit()
{
	CCLayer::onExit();
}

void CTetrisScene::menuCloseCallback(CCObject* pSender)
{
	CNetManager::Instance().Stop();
    CCDirector::sharedDirector()->end();
}

void CTetrisScene::menuEnterRoomCallback(CCObject* pSender)
{
	CCMenuItemImage *pRoomItem = (CCMenuItemImage*)pSender;
	if( pRoomItem )
	{
		void *data = pRoomItem->getUserData();
		if( data )
		{
			CRoom *room = (CRoom*)data;
			if( room )
			{
				room->Enter();

				// ÏÔÊ¾loading
				CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
				cocos2d::CCSprite *pSpriteLoading = CCSprite::create( PATCH_RES_DIR("loading.png") );
				float w = (visibleSize.width) / 2;
				float h = (visibleSize.height) / 2;
				pSpriteLoading->setPosition( ccp(w, h) );

				CCActionInterval *actionBy = CCRotateBy::create( 0.8, 360 );
				CCActionInterval *seq = (CCActionInterval*)( CCSequence::create(actionBy, NULL) );
				CCAction *act = CCRepeatForever::create( seq );

				addChild( pSpriteLoading, 1 , E_TetrisScene_Loading );
				pSpriteLoading->runAction( act );
			}
		}
	}
}

void CTetrisScene::CreateRoom(int count)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	int rowCount = count / ROOM_ONE_ROW;
	int realCount = rowCount * ROOM_ONE_ROW;
	CGameManager::Instance().InitRoomList( realCount );
	int id = ROOM_ID_FROM;
	for( int i=0; i<rowCount; ++i )
	{
		for( int j=0; j<ROOM_ONE_ROW; ++j )
		{
			CCMenuItemImage *pRoomItem = CCMenuItemImage::create(
				PATCH_RES_DIR("room.png"),
				PATCH_RES_DIR("room.png"),
				this,
				menu_selector(CTetrisScene::menuEnterRoomCallback) );
			if( !pRoomItem )
			{
				return;
			}
			pRoomItem->setAnchorPoint( ccp(0, 0) );
			float w = ROOM_LEFR_FROM + j*ROOM_W + j*ROOM_GRAP;
			float h = visibleSize.height - (i+1)*ROOM_H - (i+1)*ROOM_GRAP;
			pRoomItem->setPosition( ccp(w, h) );
			CRoomBase *room = CGameManager::Instance().GetRoom(id++);
			pRoomItem->setUserData( room );

			CCMenu *pMenu = CCMenu::create( pRoomItem, NULL );
			pMenu->setPosition( CCPointZero );
			if( !pMenu )
			{
				return;
			}
			addChild( pMenu, 1 );
		}
	}
}

void CTetrisScene::EnterRoomFailed()
{
	removeChildByTag( E_TetrisScene_Loading );
}

void CTetrisScene::EnterRoom(const std::string& user)
{
	CCLog( "CTetrisScene::EnterRoom, %s", user.c_str() );
}
