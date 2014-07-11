#include "MainScene.h"
using namespace cocos2d;
#include "CommDef.h"
#include "MyTableView.h"
using namespace std;

CCScene* CMainScene::scene()
{
    CCScene *scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF( !scene );

        CMainScene *layer = CMainScene::create();
        CC_BREAK_IF( !layer );

        scene->addChild( layer );
    } while( 0 );

    return scene;
}

bool CMainScene::init()
{
    bool ret = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////
        CC_BREAK_IF( !CCLayer::init() );

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

		_AddSceneBg();
		_AddTitle();
		_AddListView();
		_AddMainView();
		_AddTabButtons();

#ifdef _DEBUG
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(CMainScene::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);
        this->addChild(pMenu, 1);
		
		CCMenuItemImage *paddItem = CCMenuItemImage::create(
            "add31.png",
            "add32.png",
            this,
            menu_selector(CMainScene::menuTestCallback));
        CC_BREAK_IF(! paddItem);
        paddItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 60, 20));
        CCMenu* pMenu2 = CCMenu::create(paddItem, NULL);
        pMenu2->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu2);
        this->addChild(pMenu2, 1);
#endif

        ret = true;
    } while( 0 );

    return ret;
}

void CMainScene::menuCloseCallback(CCObject *pSender)
{
    CCDirector::sharedDirector()->end();
}

void CMainScene::menuTestCallback(CCObject *pSender)
{
	// add
	char icon[64] = {0};
	char text[64] = {0};
	sprintf( icon, "group1.png");
	sprintf( text, "高中斗地主群" );
	TableViewData data;
	data.icon = icon;
	data.text = text;

	if( m_pGroupList )
	{
		m_pGroupList->InsertElement( data );
	}
}

void CMainScene::_AddSceneBg()
{
	CCSize winSz = CCDirector::sharedDirector()->getWinSize();

	CCSprite *pSprite = CCSprite::create( "bg.png" );
    CCAssert( pSprite, "GetBg Failed" );
	pSprite->setScaleX( WIN_SIZE_W / pSprite->getContentSize().width );
	pSprite->setScaleY( WIN_SIZE_H / pSprite->getContentSize().height );
	pSprite->setPosition( ccp(winSz.width/2, winSz.height/2) );
    addChild( pSprite, 0 );
}

void CMainScene::_AddTitle()
{
	CCSize winSz = CCDirector::sharedDirector()->getWinSize();

	CCSprite *pSprite = CCSprite::create( "title.png" );
    CCAssert( pSprite, "GetTitle Failed" );
	CCSize sz = pSprite->getContentSize();
	pSprite->setPosition( ccp(winSz.width/2, winSz.height-sz.height/2) );
    addChild( pSprite, 0 );
}

void CMainScene::_AddTabButtons()
{
}

static int g_off = 3;
void CMainScene::_AddListView()
{
	CCSprite *pSprite = CCSprite::create( "listbg.png" );
    CCAssert( pSprite, "GetListbg Failed" );
	CCSize sz = pSprite->getContentSize();
	CCPoint pt = ccp(sz.width/2 + g_off, sz.height/2 + g_off);
	pSprite->setPosition( pt );
    addChild( pSprite, 0 );
	
	m_mainViewOffX = sz.width;
	
	m_ptTableView = pt;
	m_szTableView = CCSize(sz.width-g_off, sz.height-32);

	// test
	vector<TableViewData> vecData;
	for( int i=0; i<12; ++i )
	{
		char icon[64] = {0};
		char text[64] = {0};
		sprintf( icon, "test\\g1 (%d).png", i+1 );
		sprintf( text, "abc游戏群 %d", i+1 );
		TableViewData data;
		data.icon = icon;
		data.text = text;
		vecData.push_back( data ); 
	}
	m_pGroupList = CMyTableView::create(m_szTableView, CCSize(264,58), vecData, "selectbg.png" );
    CCAssert( m_pGroupList, "GetTableView Failed" );
	m_pGroupList->SetPosition( m_ptTableView );
	addChild( m_pGroupList, 1 );
}

void CMainScene::_AddMainView()
{
	CCSprite *pSprite = CCSprite::create( "mainbg.png" );
    CCAssert( pSprite, "GetMainbg Failed" );
	CCSize sz = pSprite->getContentSize();
	pSprite->setPosition( ccp(sz.width/2 + m_mainViewOffX + 2*g_off, sz.height/2 + g_off) );
    addChild( pSprite, 0 );
}
