#include "GroupMainScene.h"
#include "utils.h"
#include "GameList.h"
#include "NetManager.h"
#include "GameManager.h"
#include "GroupCell.h"
#include "GroupInfoScene.h"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

CCScene* CGroupMainScene::scene()
{
    CCScene *scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        CGroupMainScene *layer = CGroupMainScene::create();
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}

bool CGroupMainScene::init()
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
            menu_selector(CGroupMainScene::menuCloseCallback) );
        CC_BREAK_IF( !pCloseItem );
		pCloseItem->setAnchorPoint( ccp(0, 0) );
		float w = visibleSize.width - pCloseItem->getContentSize().width - 5;
		float h = 5;
		pCloseItem->setPosition( ccp(w, h) );

        CCMenu *pMenu = CCMenu::create( pCloseItem, NULL );
        pMenu->setPosition( CCPointZero );
        CC_BREAK_IF( !pMenu );
        addChild( pMenu, 1 );

        bRet = true;
			
		// 增加时钟检查是否已经得到游戏列表了 
		m_showGroupListSchedule = schedule_selector(CGroupMainScene::DisplayGroupList);
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
			m_showGroupListSchedule, this, 1.0f, false);

        bRet = true;
		
		CNetManager::Instance().ResumeProcMsg();
		CGameManager::Instance().SetGameWorld( this );
    } while (0);

    return bRet;
}

void CGroupMainScene::onExit()
{
	CCLayer::onExit();
}

void CGroupMainScene::menuCloseCallback(CCObject* pSender)
{
	CNetManager::Instance().Stop();
    CCDirector::sharedDirector()->end();
}

void CGroupMainScene::DisplayGroupList(float t)
{
	if( CGameManager::Instance().GroupCount() > 0 )
	{
		CCDirector::sharedDirector()->getScheduler()->unscheduleSelector( m_showGroupListSchedule, this );
		_DisplayGroupList();
	}
}

void CGroupMainScene::_DisplayGroupList()
{
	removeChildByTag( E_GroupList_TableView_Tag );

	m_nextIdx = 0;
	m_vecGroupName.clear();

	CGameManager::Instance().DumpGroupNameList( m_vecGroupName );
	m_GroupListSize = (int)m_vecGroupName.size();
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCTableView *tableView = CCTableView::create( this, CCSizeMake(GROUP_ICON_WIDTH, (winSize.height*2/3)) );
	CCSize tabViewSize = tableView->getViewSize();
	tableView->setAnchorPoint( ccp(0, 0) );
	tableView->setDirection( kCCScrollViewDirectionVertical );
	tableView->setPosition( 
		ccp((winSize.width - tabViewSize.width)/2, (winSize.height- tabViewSize.height)/2) );
	tableView->setDelegate( this );
	tableView->setVerticalFillOrder( kCCTableViewFillTopDown );
	addChild( tableView, 1, E_GroupList_TableView_Tag );
	tableView->reloadData();
}

void CGroupMainScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
	CGroupInfo *info = (CGroupInfo*)cell->getUserData();
	if( info )
	{
		CCLOG( "cell touched group:%s", info->m_Name.c_str() );
		CGameManager::Instance().SetCurGroup( info );
		CNetManager::Instance().PauseProcMsg();

		cocos2d::CCDirector::sharedDirector()->replaceScene( CGroupInfoScene::scene() );
	}
}

CCSize CGroupMainScene::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake( GROUP_ICON_WIDTH, GROUP_ICON_HEIGHT );
}

CCTableViewCell* CGroupMainScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	if( m_nextIdx < m_vecGroupName.size() )
	{
		u64 id = m_vecGroupName[ m_nextIdx ];
		++m_nextIdx;
		if( m_nextIdx >= m_vecGroupName.size() )
		{
			m_nextIdx = 0;
		}

		string icon = PATCH_RES_DIR("group.png");
		cocos2d::CCSprite *pSprite = CCSprite::create( icon.c_str() );
		if( !pSprite )
		{
			return NULL;
		}
		pSprite->setAnchorPoint( CCPointZero );
		pSprite->setPosition( CCPointZero );
		pSprite->setScale( GROUP_ICON_HEIGHT / pSprite->getContentSize().height );

		CCTableViewCell *cell = table->dequeueCell();
		if( !cell )
		{
			cell = new CGroupTableViewCell();
			cell->autorelease();
		}
		cell->addChild( pSprite );

		CGroupInfo *group = CGameManager::Instance().GetGroupInfo(id);
		cell->setUserData( group );
        
		CCLabelTTF *label = CCLabelTTF::create(group->m_Name.c_str(), "Helvetica", 20.0);
		label->setAnchorPoint(CCPointZero);
		label->setPosition(
			ccp(pSprite->getContentSize().width + 20, pSprite->getContentSize().height/2));
        cell->addChild(label);

		return cell;
	}

	return NULL;
}

unsigned int CGroupMainScene::numberOfCellsInTableView(CCTableView *table)
{
    return m_GroupListSize;
}
