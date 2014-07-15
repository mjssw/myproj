#include "MainScene.h"
using namespace cocos2d;
#include "CommDef.h"
#include "MyTableView.h"
#include "MyRadioButton.h"
#include "MyEditBox.h"
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

void CMainScene::menuCloseCallback(Object *pSender)
{
    CCDirector::sharedDirector()->end();
}

void CMainScene::menuTestCallback(Object *pSender)
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
	//*/


	/*
    CCMenuItemImage* pItem1 = CCMenuItemImage::create(
		"friend_normal.png", "friend_hover.png", "friend_normal.png",  
        this, menu_selector(CMainScene::menuRadioButtonCallback));  
    CCMenuItemImage* pItem2 = CCMenuItemImage::create(
		"game_normal.png", "game_hover.png", "game_normal.png", 
		this, menu_selector(CMainScene::menuRadioButtonCallback));
    CCMenuItemImage* pItem3 = CCMenuItemImage::create(
		"home_normal.png", "home_hover.png", "home_normal.png", 
		this, menu_selector(CMainScene::menuRadioButtonCallback));
	CMyRadioMenu* pMenu = CMyRadioMenu::create(1, pItem1, pItem2, pItem3, NULL);  
    pMenu->SetPosition(ccp(250, 260));
	pMenu->SetDefaultSelectItem( pItem2 );
	pMenu->SetItemsHorizontally(0);
    this->addChild(pMenu,2);
	//*/

	if( m_chatHistory )
	{
		//m_chatHistory->
	}
}

void CMainScene::menuRadioButtonCallback(Object *pSender)
{
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
	
	m_titleHigh = sz.height;
}

void CMainScene::_AddTabButtons()
{
	CCMenuItemImage *pItem1 = CCMenuItemImage::create(
		"friend_normal.png", "friend_hover.png", "friend_normal.png",  
        this, menu_selector(CMainScene::menuRadioButtonCallback) );  
    CCAssert( pItem1, "GetTabButton image friend Failed" );
	float scale = (WIN_SIZE_H - m_listHigh - m_titleHigh) / pItem1->getContentSize().height;

    CCMenuItemImage *pItem2 = CCMenuItemImage::create(
		"game_normal.png", "game_hover.png", "game_normal.png", 
		this, menu_selector(CMainScene::menuRadioButtonCallback) );
    CCAssert( pItem2, "GetTabButton image game Failed" );
    CCMenuItemImage *pItem3 = CCMenuItemImage::create(
		"home_normal.png", "home_hover.png", "home_normal.png", 
		this, menu_selector(CMainScene::menuRadioButtonCallback) );
    CCAssert( pItem3, "GetTabButton image home Failed" );
	
	CMyRadioMenu *pMenu = CMyRadioMenu::create(scale, pItem1, pItem2, pItem3, NULL );  
    CCAssert( pMenu, "GetRadioMenu Failed" );

	// TODO default select is from user data
	pMenu->SetDefaultSelectItem( pItem1 );
	pMenu->SetItemsHorizontally( 0 );

	CCPoint pt = ccp( pMenu->GetMenuWidth()/2, m_ptTableView.y+m_listHigh/2 + pMenu->getContentSize().height/2 ); 
	int y = m_ptTableView.y;
	y += m_listHigh / 2;
	y += (pMenu->GetMenuHeight()) / 2 - 2;
	pt.y = y;
    pMenu->SetPosition( pt );
	
    addChild( pMenu, 1 );
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
	m_listHigh = sz.height;

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


static void TextChatChanged(const std::string &text)
{
}

void CMainScene::_AddMainView()
{
	// TODO just test editbox
	CCSprite *pSprite = CCSprite::create( "mainbg.png" );
    CCAssert( pSprite, "GetMainbg Failed" );
	CCSize sz = pSprite->getContentSize();
	CCPoint pt = ccp(sz.width/2 + m_mainViewOffX + 2*g_off, sz.height/2 + g_off);
	pSprite->setPosition( pt ); 
    addChild( pSprite, 0 );
	
	CCSize editSz = CCSize( sz.width-2*g_off, sz.height-2*g_off);
	m_chatHistory = CMyEditBox::create(
		editSz, ccp(m_mainViewOffX+2*g_off, 2*g_off),
		"white_edit.png", "Paint Boy.ttf", 12, ccBLACK, 8 );
	CCAssert( m_chatHistory, "GetchatEditBox Failed" );
	m_chatHistory->SetEditChangedCallback( TextChatChanged );
    addChild( m_chatHistory, 1 );
}
