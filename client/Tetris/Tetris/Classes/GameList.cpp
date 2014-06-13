#include "GameList.h"
#include "utils.h"
#include "GameManager.h"
#include "NetManager.h"
#include "GameCell.h"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

CCScene* CGameList::scene()
{
    CCScene * scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF( !scene );

        CGameList *layer = CGameList::create();
        CC_BREAK_IF( !layer );

        scene->addChild(layer);
    } while (0);

    return scene;
}

bool CGameList::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF( !CCLayer::init() );

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		// 背景
		cocos2d::CCSprite *pSpriteBg = CCSprite::create( PATCH_RES_DIR("bg.png") );
		pSpriteBg->setScale(BG_SCALE);
		pSpriteBg->setAnchorPoint( ccp(0, 0) );
		pSpriteBg->setPosition( ccp(0, 0) );
		addChild( pSpriteBg, 0 );

        // 关闭按钮
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            PATCH_RES_DIR("close2.png"),
            PATCH_RES_DIR("close1.png"),
            this,
            menu_selector(CGameList::menuCloseCallback) );
        CC_BREAK_IF( !pCloseItem );
		pCloseItem->setAnchorPoint( ccp(0, 0) );
		float w = visibleSize.width - pCloseItem->getContentSize().width - 5;
		float h = 5;
		pCloseItem->setPosition( ccp(w, h) );

        CCMenu *pMenu = CCMenu::create( pCloseItem, NULL );
        pMenu->setPosition( CCPointZero );
        CC_BREAK_IF( !pMenu );
        addChild( pMenu, 1 );

		// 增加时钟检查是否已经得到游戏列表了 
		m_showGameListSchedule = schedule_selector(CGameList::DisplayGameList);
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
			m_showGameListSchedule, this, 1.0f, false);

        bRet = true;
		
		CGameManager::Instance().SetGameWorld( this );
    } while (0);

    return bRet;
}

void CGameList::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
	CGameInfo *info = (CGameInfo*)cell->getUserData();
	if( info )
	{
		CCLOG( "cell touched game:%d", info->m_Id );

		CNetManager::Instance().EnterGame( info->m_Id );
		
		// 播放loading动画
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		cocos2d::CCSprite *pSpriteLoading = CCSprite::create( PATCH_RES_DIR("loading.png") );
		float w = (visibleSize.width) / 2;
		float h = (visibleSize.height) / 2;
		pSpriteLoading->setPosition( ccp(w, h) );

		CCActionInterval *actionBy = CCRotateBy::create( 0.8, 360 );
		CCActionInterval *seq = (CCActionInterval*)( CCSequence::create(actionBy, NULL) );
		CCAction *act = CCRepeatForever::create( seq );
    
        addChild( pSpriteLoading, 1 , E_GameList_Loading );
		pSpriteLoading->runAction( act );
	}
}

CCSize CGameList::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake( GAME_ICON_WIDTH, GAME_ICON_HEIGHT );
}

CCTableViewCell* CGameList::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	if( m_nextIdx < m_vecGameId.size() )
	{
		int id = m_vecGameId[ m_nextIdx ];
		++m_nextIdx;
		if( m_nextIdx >= m_vecGameId.size() )
		{
			m_nextIdx = 0;
		}

		string icon = CGameManager::Instance().GetGameIcon( id );
		if( icon == "" )
		{
			return NULL;
		}
		int tag = GAME_ICON_TAG(id);
		cocos2d::CCSprite *pSprite = CCSprite::create( icon.c_str() );
		if( !pSprite )
		{
			return NULL;
		}
		pSprite->setAnchorPoint( CCPointZero );
		pSprite->setPosition( ccp(0, 0) );
		pSprite->setScale( GAME_ICON_HEIGHT / pSprite->getContentSize().height );

		CCTableViewCell *cell = table->dequeueCell();
		if( !cell )
		{
			cell = new CGameTableViewCell();
			cell->autorelease();
		}
		cell->addChild( pSprite );
		cell->setUserData( CGameManager::Instance().GetGameInfo(id) );

		return cell;
	}

	return NULL;
}

unsigned int CGameList::numberOfCellsInTableView(CCTableView *table)
{
    return m_GameListSize;
}

void CGameList::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
}

void CGameList::DisplayGameList(float t)
{
	if( CGameManager::Instance().GameCount() > 0 )
	{
		CCDirector::sharedDirector()->getScheduler()->unscheduleSelector( m_showGameListSchedule, this );
		_DisplayGameList();
	}
}

void CGameList::_DisplayGameList()
{
	removeChildByTag( E_GameList_TableView_Tag );

	m_nextIdx = 0;
	m_vecGameId.clear();

	CGameManager::Instance().DumpGameIdList( m_vecGameId );
	m_GameListSize = (int)m_vecGameId.size();
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCTableView *tableView = CCTableView::create( this, CCSizeMake(GAME_ICON_WIDTH, (winSize.height*2/3)) );
	CCSize tabViewSize = tableView->getViewSize();
	tableView->setAnchorPoint( ccp(0, 0) );
	tableView->setDirection( kCCScrollViewDirectionVertical );
	tableView->setPosition( 
		ccp((winSize.width - tabViewSize.width)/2, (winSize.height- tabViewSize.height)/2) );
	tableView->setDelegate( this );
	tableView->setVerticalFillOrder( kCCTableViewFillTopDown );
	addChild( tableView, 1, E_GameList_TableView_Tag );
	tableView->reloadData();
}

void CGameList::ConnectError()
{
	removeChildByTag( E_GameList_Loading );
}
