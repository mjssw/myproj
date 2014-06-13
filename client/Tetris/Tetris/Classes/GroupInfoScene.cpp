#include "GroupInfoScene.h"
#include "utils.h"
#include "NetManager.h"
#include "GameManager.h"
#include "GroupMainScene.h"
using namespace cocos2d;
using namespace std;

CCScene* CGroupInfoScene::scene()
{
    CCScene *scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        CGroupInfoScene *layer = CGroupInfoScene::create();
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}

bool CGroupInfoScene::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		// 背景
		cocos2d::CCSprite *pSpriteBg = CCSprite::create( PATCH_RES_DIR("bg8.png") );
		pSpriteBg->setScale(BG_SCALE);
		pSpriteBg->setAnchorPoint( ccp(0, 0) );
		pSpriteBg->setPosition( ccp(0, 0) );
		addChild( pSpriteBg, 0 );

        // 关闭按钮
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            PATCH_RES_DIR("close2.png"),
            PATCH_RES_DIR("close1.png"),
            this,
            menu_selector(CGroupInfoScene::menuCloseCallback) );
        CC_BREAK_IF( !pCloseItem );
		pCloseItem->setAnchorPoint( ccp(0, 0) );
		float w = visibleSize.width - pCloseItem->getContentSize().width - 5;
		float h = 5;
		pCloseItem->setPosition( ccp(w, h) );

        CCMenu *pMenu = CCMenu::create( pCloseItem, NULL );
        pMenu->setPosition( CCPointZero );
        CC_BREAK_IF( !pMenu );
        addChild( pMenu, 1 );

		// 后退
		{
			CCMenuItemImage *pBackItem = CCMenuItemImage::create(
				PATCH_RES_DIR("back.png"),
				PATCH_RES_DIR("back.png"),
				this,
				menu_selector(CGroupInfoScene::menuBackCallback) );
			CC_BREAK_IF( !pBackItem );
			pBackItem->setAnchorPoint( ccp(0, 0) );
			float w = visibleSize.width - pBackItem->getContentSize().width - 5 - pCloseItem->getContentSize().width;
			float h = 5;
			pBackItem->setPosition( ccp(w, h) );
			CCMenu *pMenu = CCMenu::create( pBackItem, NULL );
			pMenu->setPosition( CCPointZero );
			CC_BREAK_IF( !pMenu );
			addChild( pMenu, 1 );
		}

        bRet = true;

		// 群名字
		CGroupInfo *group = CGameManager::Instance().GetCurGroup();
		cocos2d::CCSprite *pGroupSprite = CCSprite::create( PATCH_RES_DIR("group.png") );
		pGroupSprite->setScale(BG_SCALE);
		pGroupSprite->setAnchorPoint( ccp(0, 0) );
		pGroupSprite->setPosition( ccp(10, visibleSize.height-pGroupSprite->getContentSize().height-5 ) );
		addChild( pGroupSprite, 0 );
		CCLabelTTF *labelGroup = CCLabelTTF::create(group->m_Name.c_str(), "Helvetica", 20.0);
		labelGroup->setAnchorPoint(CCPointZero);
		labelGroup->setPosition( ccp(pGroupSprite->getContentSize().width+20, visibleSize.height-pGroupSprite->getContentSize().height-5 ) );
		addChild( labelGroup, 0 );

		// 自己
		cocos2d::CCSprite *pSelf = CCSprite::create( PATCH_RES_DIR("group_self.png") );
		pSelf->setScale(BG_SCALE);
		pSelf->setAnchorPoint( ccp(0, 0) );
		pSelf->setPosition( ccp(visibleSize.width/2, visibleSize.height-pGroupSprite->getContentSize().height-5 ) );
		addChild( pSelf, 0 );
		CCLabelTTF *labelSelf = CCLabelTTF::create(CGameManager::Instance().GetUser().c_str(), "Helvetica", 20.0);
		labelSelf->setAnchorPoint(CCPointZero);
		labelSelf->setPosition( ccp(visibleSize.width/2+pSelf->getContentSize().width+20, visibleSize.height-pGroupSprite->getContentSize().height-5 ) );
		addChild( labelSelf, 0 );

		// 加载成员列表
		_InitMemberList();
		_InitAddGame();
			
		CNetManager::Instance().ResumeProcMsg();

		CGameManager::Instance().SetGameWorld( this );
    } while (0);

    return bRet;
}

void CGroupInfoScene::onExit()
{
	CCLayer::onExit();
}

void CGroupInfoScene::menuCloseCallback(CCObject* pSender)
{
	CNetManager::Instance().Stop();
    CCDirector::sharedDirector()->end();
}

void CGroupInfoScene::menuBackCallback(CCObject* pSender)
{
	CNetManager::Instance().PauseProcMsg();
	cocos2d::CCDirector::sharedDirector()->replaceScene( CGroupMainScene::scene() );
}

void CGroupInfoScene::menuAddGameCallback(CCObject* pSender)
{
	CGroupInfo *group = CGameManager::Instance().GetCurGroup();
	CNetManager::Instance().CreateGameRoom( group->m_Id, GAME_ID_TETRIS );
}

void CGroupInfoScene::menuEnterGroupGameRoomCallback(CCObject* pSender)
{
	CCMenuItemImage *pItem = (CCMenuItemImage*)pSender; 
	if( !pItem )
	{
		CCLog("ERROR, item is null");
		return ;
	}

	CGameRoomInfo *room = (CGameRoomInfo*)pItem->getUserData();
	CCLog( "room: %s:%d %d", room->ip.c_str(), room->port, room->roomid );

	CGameManager::Instance().SetCurGameRoom( room );
	if( CNetManager::Instance().Start( room->ip.c_str(), room->port ) )
	{
		CNetManager::Instance().SetState( CNetManager::E_State_Connecting_GameServer_ByGroup );
		CCLog( "++++++ connect to game server" );

		// 播放loading动画
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		cocos2d::CCSprite *pSpriteLoading = CCSprite::create( PATCH_RES_DIR("loading.png") );
		float w = (visibleSize.width) / 2;
		float h = (visibleSize.height) / 2;
		pSpriteLoading->setPosition( ccp(w, h) );

		CCActionInterval *actionBy = CCRotateBy::create( 0.8, 360 );
		CCActionInterval *seq = (CCActionInterval*)( CCSequence::create(actionBy, NULL) );
		CCAction *act = CCRepeatForever::create( seq );
    
        addChild( pSpriteLoading, 1 );
		pSpriteLoading->runAction( act );
	}
	else
	{
		CGameManager::Instance().SetCurGameRoom( NULL );
	}
}

void CGroupInfoScene::CreateGroupGameRoom()
{
	// 最后一个为新加的，draw it
	vector<CGameRoomInfo> &rooms = CGameManager::Instance().GetGameRoomInfo();

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	float w = visibleSize.width/ 2;
	float h = visibleSize.height - 120;

	CCMenuItemImage *pAddItem = CCMenuItemImage::create(
		PATCH_RES_DIR("group_room.png"),
		PATCH_RES_DIR("group_room.png"),
		this,
		menu_selector(CGroupInfoScene::menuEnterGroupGameRoomCallback) );

	h -= (rooms.size()-1) * (pAddItem->getContentSize().height + 5);

	pAddItem->setAnchorPoint( ccp(0, 0) );
	pAddItem->setPosition( ccp(w, h) );
	pAddItem->setUserData( &rooms[rooms.size()-1] );

	CCMenu *pMenu = CCMenu::create( pAddItem, NULL );
	pMenu->setPosition( CCPointZero );
	addChild( pMenu, 1 );
}

void CGroupInfoScene::_InitMemberList()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CGroupInfo *group = CGameManager::Instance().GetCurGroup();
	map<string, int>::iterator it = group->m_Members.begin();
	int w = 10;
	int h = visibleSize.height - 100;
	for( ; it != group->m_Members.end(); ++it )
	{
		cocos2d::CCSprite *member = CCSprite::create( PATCH_RES_DIR("man.png") );
		member->setScale(BG_SCALE);
		member->setAnchorPoint( ccp(0, 0) );
		member->setPosition( ccp(w, h) );
		addChild( member, 0 );

		CCLabelTTF *labelMem = CCLabelTTF::create(it->first.c_str(), "Helvetica", 20.0);
		labelMem->setAnchorPoint(CCPointZero);
		labelMem->setPosition( ccp(w+20, h) );
		addChild( labelMem, 0 );

		h -= (member->getContentSize().height + 5); 
	}
}

void CGroupInfoScene::_InitAddGame()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCMenuItemImage *pAddItem = CCMenuItemImage::create(
		PATCH_RES_DIR("add.png"),
		PATCH_RES_DIR("add.png"),
		this,
		menu_selector(CGroupInfoScene::menuAddGameCallback) );
	pAddItem->setAnchorPoint( ccp(0, 0) );
	float w = visibleSize.width - pAddItem->getContentSize().width - 5;
	float h = visibleSize.height - 100;
	pAddItem->setPosition( ccp(w, h) );

	CCMenu *pMenu = CCMenu::create( pAddItem, NULL );
	pMenu->setPosition( CCPointZero );
	addChild( pMenu, 1 );
}
