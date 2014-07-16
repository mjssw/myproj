#include "MainScene.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#include "CommDef.h"
#include "MyTableView.h"
#include "MyRadioButton.h"
#include "MyEditBox.h"
#include "MyMenuItemImage.h"
#include "ChatTableView.h"
using namespace std;

// for test
vector<ChatTableViewData> g_chatData;

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

		// TODO
		m_tabSelect = E_Tab_Group;

		_AddSceneBg();
		_AddTitle();
		_AddListView();
		_AddTabButtons();
		_AddMainView();

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
	/*
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

	// test show chat
	/*
	Node *parent = getChildByTag( E_Tag_MainView );
	if( !parent ) return;
	Size sz = parent->getContentSize();
	Scale9Sprite *sp = Scale9Sprite::create("friend_talk.png");
	if( !sp ) return;
	sp->setPosition(	ccp( sz.width/3, sz.height/3*2 ) );
	parent->addChild( sp );
	//*/

	
}

void CMainScene::menuRadioButtonGroupCallback(Object *pSender)
{
	if( m_tabSelect == E_Tab_Group )
	{
		return;
	}
	CCLog( "menuRadioButtonGroupCallback" );
	m_tabSelect = E_Tab_Group;

	_ClearContentOnListView();
	_ClearContentOnMainView();

	_AddContentToListView();
	_AddContentToMainView();
}

void CMainScene::menuRadioButtonGameCallback(Object *pSender)
{
	if( m_tabSelect == E_Tab_Game )
	{
		return;
	}
	CCLog( "menuRadioButtonGameCallback" );
	m_tabSelect = E_Tab_Game;
	
	_ClearContentOnListView();
	_ClearContentOnMainView();
}

void CMainScene::menuRadioButtonHomeCallback(Object *pSender)
{
	if( m_tabSelect == E_Tab_Home )
	{
		return;
	}
	CCLog( "menuRadioButtonHomeCallback" );
	m_tabSelect = E_Tab_Home;
	
	_ClearContentOnListView();
	_ClearContentOnMainView();
}

void CMainScene::menuFaceCallback(Object *pSender)
{
	CCLog( "menuFaceCallback" );
}

void CMainScene::menuSendCallback(Object *pSender)
{
	CCLog( "menuSendCallback" );

	// for test
	static bool user = true;
	ChatTableViewData data;
	if( user )
	{
		data.type = ChatTableViewData::E_DataType_User;
		data.user = "我";
		data.content = "2014-07-16 17:33:23";
		g_chatData.push_back( data );
		user = false;
	}
	else
	{
		data.type = ChatTableViewData::E_DataType_Text;
		data.user = "我";
		data.content = "来了来了来了来了来了来了来了来了来了来了来了来了来了来了来了来了";
		g_chatData.push_back( data );
	}
	m_chatTableView->InsertUpdate( data.height() );
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

	// TODO 根据userdata 显示内容
}

void CMainScene::_AddTabButtons()
{
	CCMenuItemImage *pItem1 = CCMenuItemImage::create(
		"friend_normal.png", "friend_hover.png", "friend_normal.png",  
        this, menu_selector(CMainScene::menuRadioButtonGroupCallback) );  
    CCAssert( pItem1, "GetTabButton image friend Failed" );
	float scale = (WIN_SIZE_H - m_listHigh - m_titleHigh) / pItem1->getContentSize().height;

    CCMenuItemImage *pItem2 = CCMenuItemImage::create(
		"game_normal.png", "game_hover.png", "game_normal.png", 
		this, menu_selector(CMainScene::menuRadioButtonGameCallback) );
    CCAssert( pItem2, "GetTabButton image game Failed" );
    CCMenuItemImage *pItem3 = CCMenuItemImage::create(
		"home_normal.png", "home_hover.png", "home_normal.png", 
		this, menu_selector(CMainScene::menuRadioButtonHomeCallback) );
    CCAssert( pItem3, "GetTabButton image home Failed" );
	
	CMyRadioMenu *pMenu = CMyRadioMenu::create(scale, pItem1, pItem2, pItem3, NULL );  
    CCAssert( pMenu, "GetRadioMenu Failed" );

	switch( m_tabSelect )
	{
	case E_Tab_Group:
		pMenu->SetDefaultSelectItem( pItem1 );
		break;
	case E_Tab_Game:
		pMenu->SetDefaultSelectItem( pItem2 );
		break;
	case E_Tab_Home:
	default:
		pMenu->SetDefaultSelectItem( pItem3 );
		break;
	};

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
	addChild( pSprite, 0, E_Tag_ListView );
	
	m_mainViewOffX = sz.width;
	
	m_ptTableView = pt;
	m_szTableView = CCSize(sz.width-g_off, sz.height-32);
	m_listHigh = sz.height;

	_AddContentToListView();

}


static void TextChatChanged(const std::string &text)
{
}

void CMainScene::_AddMainView()
{
	CCSprite *pSprite = CCSprite::create( "mainbg.png" );
	CCAssert( pSprite, "GetMainbg Failed" );
	CCSize sz = pSprite->getContentSize();
	CCPoint pt = ccp(sz.width/2 + m_mainViewOffX + 2*g_off, sz.height/2 + g_off);
	pSprite->setPosition( pt ); 
	addChild( pSprite, 0, E_Tag_MainView );

	// TODO 根据当前tab btn的选择和userdata数据 创建后续内容
	_AddContentToMainView();
}

void CMainScene::_AddContentToMainView()
{
	switch( m_tabSelect )
	{
	case E_Tab_Group:
		_AddChatViewToMain();
		break;
	case E_Tab_Game:
		break;
	case E_Tab_Home:
		break;
	}
}

void CMainScene::_AddChatViewToMain()
{
	Node *parent = getChildByTag( E_Tag_MainView );
	if( !parent )
	{
		CCLog( "[_AddChatViewToMain] get main view failed" );
		return;
	}
	CCSize parentSz = parent->getContentSize();

	CCSprite *chatTitle = CCSprite::create( "chattitle.png" );
    CCAssert( chatTitle, "GetChatTitle Failed" );
	CCSize szTitle = chatTitle->getContentSize();
	CCPoint ptTitle = ccp(parentSz.width - szTitle.width/2 - 2, parentSz.height/3);
	chatTitle->setPosition( ptTitle );
	parent->addChild( chatTitle );

	{
		CCMyMenuItemImage *face = CCMyMenuItemImage::create(
			"face_normal.png",
			"face_hover.png",
			this,
			menu_selector(CMainScene::menuFaceCallback));
		CCAssert( face, "CreateFaceItem Failed" );
		CCSize faceSz = face->getContentSize();
		face->setAnchorPoint( ccp(0.5, 0.5) );
		face->setPosition(ccp(faceSz.width/2, szTitle.height/2) );
		CCMenu *menu = CCMenu::create( face, NULL );
		menu->setPosition( CCPointZero );
		CCAssert( menu, "CreateFaceMenu Failed" );
		chatTitle->addChild( menu, 0 );
	}

	{
		CCMyMenuItemImage *sendItem = CCMyMenuItemImage::create(
			"btn_send_normal.png",
			"btn_send_down.png",
			this,
			menu_selector(CMainScene::menuSendCallback));
		CCAssert( sendItem, "CreateSendItem Failed" );
		CCSize sendSz = sendItem->getContentSize();
		float scale = szTitle.height / sendSz.height;
		CCSize realSz = sendSz * scale;
		sendItem->setScale( scale );
		sendItem->setAnchorPoint( ccp(0.5, 0.5) );
		sendItem->setPosition(ccp(szTitle.width - realSz.width/2, szTitle.height/2-1) );
		CCMenu *menu = CCMenu::create( sendItem, NULL );
		menu->setPosition( CCPointZero );
		CCAssert( menu, "CreateSendMenu Failed" );
		chatTitle->addChild( menu, 0 );
	}

	CCSprite *splite = CCSprite::create( "splitline.png" );
    CCAssert( splite, "GetSplitline Failed" );
	CCSize szSplite = splite->getContentSize();
	CCPoint ptSplite = ccp(ptTitle.x - szTitle.width/2, parentSz.height/2);
	splite->setPosition( ptSplite );
	parent->addChild( splite );

	// add chat table view
	Size chatSz = Size(szTitle.width,  parentSz.height/3*2-szTitle.height/2);
	m_chatTableView = CChatTableView::create( chatSz );
    CCAssert( m_chatTableView, "GetChatTableView Failed" );
	m_chatTableView->SetPosition( ccp(ptTitle.x, ptTitle.y+chatSz.height/2+szTitle.height/2) );
	parent->addChild( m_chatTableView );
	// test add data
	ChatTableViewData data;
	{
		data.type = ChatTableViewData::E_DataType_User;
		data.user = "小流";
		data.content = "2014-07-16 17:32:23";
		g_chatData.push_back( data );
	}
	{
		data.type = ChatTableViewData::E_DataType_Text;
		data.user = "小流";
		data.content = "斗地主房间101，速度";
		g_chatData.push_back( data );
	}
	{
		data.type = ChatTableViewData::E_DataType_User;
		data.user = "大瞎";
		data.content = "2014-07-16 17:32:56";
		g_chatData.push_back( data );
	}
	{
		data.type = ChatTableViewData::E_DataType_Text;
		data.user = "大瞎";
		data.content = "三缺1，快";
		g_chatData.push_back( data );
	}
	{
		data.type = ChatTableViewData::E_DataType_Text;
		data.user = "大瞎";
		data.content = "五马，快来";
		g_chatData.push_back( data );
	}
	m_chatTableView->InitData( g_chatData );
}

void CMainScene::_AddContentToListView()
{
	switch( m_tabSelect )
	{
	case E_Tab_Group:
		_AddGroupsToListView();
		break;
	case E_Tab_Game:
		break;
	case E_Tab_Home:
		break;
	default:
		break;
	};
}

void CMainScene::_AddGroupsToListView()
{
	Node *parent = getChildByTag( E_Tag_ListView );
	if( !parent )
	{
		CCLog( "[_AddGroupsToListView] get list view failed" );
		return;
	}
	Size sz = parent->getContentSize();

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
	m_pGroupList->SetPosition( ccp(sz.width/2, sz.height/2) );
	parent->addChild( m_pGroupList );
}

void CMainScene::_ClearListView()
{
	removeChildByTag( E_Tag_ListView );
}

void CMainScene::_ClearMainView()
{
	removeChildByTag( E_Tag_MainView );
}

void CMainScene::_ClearContentOnListView()
{
	Node *node = getChildByTag( E_Tag_ListView );
	if( !node )
	{
		CCLog( "[_ClearContentOnListView] not found child" );
		return;
	}

	node->removeAllChildrenWithCleanup( true );
}

void CMainScene::_ClearContentOnMainView()
{
	Node *node = getChildByTag( E_Tag_MainView );
	if( !node )
	{
		CCLog( "[_ClearContentOnMainView] not found child" );
		return;
	}

	node->removeAllChildrenWithCleanup( true );
}
