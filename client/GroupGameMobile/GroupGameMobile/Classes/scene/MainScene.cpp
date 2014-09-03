#include "MainScene.h"
#include "extensions/cocos-ext.h"
#include "base/CCScriptSupport.h"
#include "CCLuaEngine.h"
USING_NS_CC;
USING_NS_CC_EXT;

#include "CommDef.h"
#include "view/MyTableView.h"
#include "view/MyRadioButton.h"
#include "view/MyEditBox.h"
#include "view/MyMenuItemImage.h"
#include "view/ChatTableView.h"
#include "utils.h"
#include "view/MemberListPopLayer.h"
#include "user/UserManager.h"
#include "net/NetManager.h"
#include "scene/SceneManager.h"
#include "user/GroupManager.h"
#include "user/Group.h"
#include "user/GroupMember.h"
#include "view/CreateGroupPopLayer.h"
#include "view/InviteMemberPopLayer.h"
#include "view/AskJoinGroupPopLayer.h"
#include "view/CreateGamePopLayer.h"
using namespace std;

static void TextChatChanged(const std::string &text)
{
	CCLog( "Text chat changed" );
	CUserManager::Instance().GetViewData().SetChatText( text );
}

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

void CMainScene::UpdateView(int type)
{
	switch( type )
	{
	case CSceneManager::E_UpdateType_UpdateGroupList:
		{
			_UpdateGroupList();
		}
		break;
	case CSceneManager::E_UpdateType_AskJoinGroup:
		{
			_AskJoinGroup();
		}
		break;
	case CSceneManager::E_UpdateType_GroupMessage:
		{
			_GroupMessageNotify();
		}
		break;
	case CSceneManager::E_UpdateType_GameRoomUpdate:
		{
			_GameRoomUpdate();
		}
		break;
	case CSceneManager::E_UpdateType_ConnectGame:
		{
			_ConnectGameSuccess();
		}
		break;
	default:
		CCLog( "[CMainScene::UpdateView] unknown type:%d", type );
		break;
	};
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

		CSceneManager::Instance().SetCurView( this );
		//CNetManager::Instance().ResumeProcessMessage();
		
		lua_register(LuaEngine::getInstance()->getLuaStack()->getLuaState(), "GobackHome", GobackHome);

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
	sprintf( text, "���ж�����Ⱥ" );
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

	// test pop layer
	/*
	CMemberListPopLayer *pop = CMemberListPopLayer::create( "memberlistbg.png" ); 
	if( pop )
	{
		pop->setPosition(ccp(0,0));
		addChild( pop, 99999 );
	}//*/

	// test jump lua scene
	//LuaEngine::getInstance()->reload("tetris/lua/main.lua");

	// test room list
	//CUserManager::Instance().GetGameRoomList().AddGameRoomInfo( 10, 1, "sjj1", "127.0.0.1", 123, 111, "" );
	//_GameRoomUpdate();
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

	CGroupClient *client = CNetManager::Instance().GetGroupClientInstance();
	if( client )
	{
		CGroup *group = CUserManager::Instance().GetViewData().GetSelectGroup();
		if( !group )
		{
			CCLog( "[CMainScene::menuSendCallback] ERROR select group is null" );
			return;
		}

		/*
		_AddChatContent(
			group->GetId(),
			CUserManager::Instance().GetBasic().GetUser(),
			CUserManager::Instance().GetViewData().GetChatText(),
			true );
		//*/

		client->ChatMessage(
			group->GetId(),
			CUserManager::Instance().GetViewData().GetChatText() );
	}
}

void CMainScene::menuCreateGroupCallback(cocos2d::Object *pSender)
{
	CCreateGroupPopLayer *pop = CCreateGroupPopLayer::create( "commonbg.png" ); 
	if( pop )
	{
		pop->SetView( this );
		pop->setPosition( ccp(0, 0) );
		addChild( pop, 99999 );
	}
}

void CMainScene::menuCreateGameCallback(cocos2d::Object *pSender)
{
	if( CUserManager::Instance().GetViewData().GetSelectGroup() )
	{
		CCreateGamePopLayer *pop = CCreateGamePopLayer::create( "memberlistbg.png" ); 
		if( pop )
		{
			pop->setPosition( ccp(0, 0) );
			addChild( pop, 99999 );
		}
	}
	else
	{
		CCLog( "Not select group , cant not create game" );
	}
}

void CMainScene::menuInviteMemberCallback(cocos2d::Object *pSender)
{
	if( !CUserManager::Instance().GetViewData().GetSelectGroup() )
	{
		CCLog( "menuInviteMemberCallback select group null" );
		return;
	}

	CInviteMemberPopLayer *pop = CInviteMemberPopLayer::create( "commonbg.png" ); 
	if( pop )
	{
		pop->SetView( this );
		pop->setPosition( ccp(0, 0) );
		addChild( pop, 99999 );
	}
}

void CMainScene::menuMemberListCallback(cocos2d::Object *pSender)
{
	CMemberListPopLayer *pop = CMemberListPopLayer::create( "memberlistbg.png" ); 
	if( pop )
	{
		pop->setPosition( ccp(0, 0) );
		addChild( pop, 99999 );
	}
}

void CMainScene::menuLeaveGroupCallback(cocos2d::Object *pSender)
{
	CCLog( "menuLeaveGroupCallback" );
	_LeaveGroup();
}

void CMainScene::menuChatHistoryCallback(cocos2d::Object *pSender)
{
	CCLog( "menuChatHistoryCallback" );
	CGroupClient *client = CNetManager::Instance().GetGroupClientInstance();
	if( !client )
	{
		CCLog( "[menuChatHistoryCallback][ERROR] group client null" );
		return;
	}

	CGroup *group = CUserManager::Instance().GetViewData().GetSelectGroup();
	if( !group )
	{
		CCLog( "[menuChatHistoryCallback][ERROR] select group null" );
		return;
	}
	client->ChatHistory( group->GetId(), 0 );
}

void CMainScene::GroupListTouchedCallback(Node *pSender, void *data)
{
	if( data )
	{
		CGroup *group = (CGroup*)data;
		CUserManager::Instance().GetViewData().SetSelectGroup( group );
		CCLog( "group:%llu:%s is selected", group->GetId(), group->GetName().c_str() );
	
		if( m_chatTableView )
		{
			m_chatTableView->InitData( 
				group->GetChatHistory() );
			m_chatTableView->InsertUpdate();
		}
		if( m_pGameRoomList )
		{
			_GameRoomUpdate();
		}
	}
}

void CMainScene::GameRoomListTouchedCallback(Node *pSender, void *data)
{
	CCLog( "CMainScene::GameRoomListTouchedCallback" );
	if( data )
	{
		CGameRoomInfo *info = (CGameRoomInfo*)data;
		CCLog( "try enter game:%d ip:%s port:%d room:%d",
			info->m_gameid, info->m_ip.c_str(), info->m_port, info->m_roomid);

		bool ret = CNetManager::Instance().StartGame( info->m_ip.c_str(), info->m_port );
		if( !ret )
		{
			CCLog( "[CMainScene::GameRoomListTouchedCallback][ERROR] connect game(%d:%s:%d) failed",
				info->m_gameid, info->m_ip.c_str(), info->m_port );
		}
		else
		{
			CUserManager::Instance().GetViewData().SetCurGameRoom( info );
		}
	}
}

int CMainScene::GobackHome(lua_State *ls)
{
	Scene *home = CMainScene::scene();
    Director *director = Director::getInstance();
	if( director->getRunningScene() )
	{
		director->replaceScene( home );
	}
	else
	{
		director->runWithScene( home );
	}

	// �Ͽ���Ϸ���ӣ�����������Դ
	CNetManager::Instance().CloseGameConn();
	CNetManager::Instance().SetGameClientInstance( NULL );
	CUserManager::Instance().GetViewData().SetCurGameRoom( NULL );

	return 0;
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

	// TODO ����userdata ��ʾ����
	int off = _AddHeadOnTitle( *pSprite );
	off = _AddNameOnTitle( *pSprite, off );
	off = _AddLevelOnTitle( *pSprite, off );
	_AddDiamondOnTitle( *pSprite );
	_AddGoldOnTitle( *pSprite );

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

void CMainScene::_AddMainView()
{
	CCSprite *pSprite = CCSprite::create( "mainbg.png" );
	CCAssert( pSprite, "GetMainbg Failed" );
	CCSize sz = pSprite->getContentSize();
	CCPoint pt = ccp(sz.width/2 + m_mainViewOffX + 2*g_off, sz.height/2 + g_off);
	pSprite->setPosition( pt ); 
	addChild( pSprite, 0, E_Tag_MainView );

	// TODO ���ݵ�ǰtab btn��ѡ���userdata���� ������������
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

#define LongSentencesExample "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."
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

	Point sendItemPos;
	Size sendItemSz;
	{
		CCMyMenuItemImage *sendItem = CCMyMenuItemImage::create(
			"btn_send_normal.png",
			"btn_send_down.png",
			this,
			menu_selector(CMainScene::menuSendCallback));
		CCAssert( sendItem, "CreateSendItem Failed" );
		sendItemSz = sendItem->getContentSize();
		float scale = szTitle.height / sendItemSz.height;
		CCSize realSz = sendItemSz * scale;
		sendItem->setScale( scale );
		sendItem->setAnchorPoint( ccp(0.5, 0.5) );
		sendItemPos = ccp(szTitle.width - realSz.width/2, szTitle.height/2-1);
		sendItem->setPosition( sendItemPos );
		CCMenu *menu = CCMenu::create( sendItem, NULL );
		menu->setPosition( CCPointZero );
		CCAssert( menu, "CreateSendMenu Failed" );
		chatTitle->addChild( menu, 0 );
	}

	{
		CCMyMenuItemImage *sendItem = CCMyMenuItemImage::create(
			"chathistory_normal.png",
			"chathistory_down.png",
			this,
			menu_selector(CMainScene::menuChatHistoryCallback));
		CCAssert( sendItem, "CreateChatHistoryItem Failed" );
		CCSize sz = sendItem->getContentSize();
		float scale = szTitle.height / sz.height;
		CCSize realSz = sz * scale;
		sendItem->setScale( scale );
		sendItem->setAnchorPoint( ccp(0.5, 0.5) );
		sendItem->setPosition(ccp(sendItemPos.x-sendItemSz.width/2-sz.width/2, sendItemPos.y) );
		CCMenu *menu = CCMenu::create( sendItem, NULL );
		menu->setPosition( CCPointZero );
		CCAssert( menu, "CreateChatHistoryMenu Failed" );
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

	// add edit view
	{
		Size editSz = Size(szTitle.width-szSplite.width/2, ptTitle.y-szTitle.height/2);
		CMyEditBox *editChat = CMyEditBox::create(
			editSz, ccp(ptTitle.x-szTitle.width/2+szSplite.width/2, 0), 
			"white_edit.png", NULL, 10, ccBLACK, 200 );
		CCAssert( editChat, "GetChatEditBox Failed" );
		editChat->SetEditChangedCallback( TextChatChanged );
		editChat->GetEditBox()->setFontName("arial");
		editChat->GetEditBox()->setFontSize(20);
		parent->addChild( editChat, 1 );
	}

	_AddGroupFuncBtns( *parent );

	Size roomListSz = Size( ptSplite.x - szSplite.width/2 ,parentSz.height - 20);
	Size roomCellSz = Size(roomListSz.width, 40); 
	_AddGameRoomList( *parent, roomListSz, roomCellSz );
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

	vector<TableViewData> vecData;
	/*
	//_DumpGroupList( vecData );
	for( int i=0; i<12; ++i )
	{
		char icon[64] = {0};
		char text[64] = {0};
		sprintf( icon, "group5.png", i+1 );
		sprintf( text, "abc��ϷȺ %d", i+1 );
		TableViewData data;
		data.icon = icon;
		data.text = text;
		vecData.push_back( data ); 
	}
	//*/
	_DumpGroupList( vecData );
    Size _sz = CCSize(264,58);
	m_pGroupList = CMyTableView::create(m_szTableView, _sz, vecData, "selectbg.png" );
    CCAssert( m_pGroupList, "GetTableView Failed" );
	m_pGroupList->SetPosition( ccp(sz.width/2, sz.height/2) );
	m_pGroupList->SetTouchCallback( callfuncND_selector(CMainScene::GroupListTouchedCallback), this );
	parent->addChild( m_pGroupList );
	
	_AddCreateGroupBtn( *parent, sz );
}

void CMainScene::_ClearListView()
{
	removeChildByTag( E_Tag_ListView );
	m_pGroupList = NULL;
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
	m_pGroupList = NULL;
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

Sprite* CMainScene::_GetHead()
{
	Rect rc;
	bool useRc;
	string realhead = "";
	CalcUserHead( 
		CUserManager::Instance().GetBasic().GetHead(),
		CUserManager::Instance().GetBasic().GetSex(),
		realhead, rc, useRc );
	if( useRc )
	{
		return Sprite::create( realhead, rc );
	}
	else
	{
		return Sprite::create( realhead );
	}
	/*
	string head = CUserManager::Instance().GetBasic().GetHead();
	if( head == "" )
	{
		// û��ͷ��ʹ��Ĭ�ϵ�
		if( CUserManager::Instance().GetBasic().GetSex() == E_Sex_Male )
		{
			head = "boy.png";
		}
		else
		{
			head = "girl.png";
		}
		return Sprite::create( head );
	}
	else if( head.at(0) == '#' )
	{
		// ʹ�õ�ϵͳͷ��
		string real = head.substr( 1 );
		int offset = atoi( real.c_str() );
		int offx = (offset >> 16);
		int offy = (offset & 0xFFFF );
		Rect rt;
		rt.origin = ccp( offx * SYS_HEAD_SIZE_W, offy * SYS_HEAD_SIZE_H );
		rt.size = Size( SYS_HEAD_SIZE_W, SYS_HEAD_SIZE_H );
		return Sprite::create( "syshead.png", rt );
	}
	else
	{
		// ʹ���Զ���ͷ��
		CCAssert( head == CUserManager::Instance().GetBasic().GetUser(), "[_GetHead]selfdefine head is not same with user" );
		return Sprite::create( head );
	}
	//*/
}

int CMainScene::_AddHeadOnTitle(cocos2d::Node &parent)
{
	Size sz = parent.getContentSize();
	CCSprite *head = _GetHead();
    CCAssert( head, "GetHead Failed" );
	float scale = sz.height / head->getContentSize().height;
	Size headSz = head->getContentSize() * scale;
	head->setScale( scale );
	head->setPosition( ccp( headSz.width/2, sz.height/2 ) );
	parent.addChild( head );

	return headSz.width;
}

static int GAP = 5;
int CMainScene::_AddNameOnTitle(cocos2d::Node &parent, int off)
{
	//string u8name = a2u( CUserManager::Instance().GetBasic().GetName().c_str() );
	string u8name = CUserManager::Instance().GetBasic().GetName();
	Label *name = Label::createWithSystemFont( u8name.c_str(), "arial", 25 );
	CCAssert( name, "create label name failed" );
	name->setAnchorPoint( ccp( 0.5, 0.5 ) );
	Size sz = name->getContentSize();
	name->setPosition( ccp( off+GAP+sz.width/2, parent.getContentSize().height/2 ) );
	parent.addChild( name );

	return (off+GAP+sz.width);
}

int CMainScene::_AddLevelOnTitle(cocos2d::Node &parent, int off)
{
	char strLevel[32] = {0};
	sprintf( strLevel, "%llu", CUserManager::Instance().GetBasic().GetLevel() );
	string text = string("Lv.") + strLevel;
	
	int lgap = GAP * 5;
	Label *level = Label::createWithSystemFont( text.c_str(), "arial", 25 );
	CCAssert( level, "create label level failed" );
	level->setAnchorPoint( ccp( 0.5, 0.5 ) );
	Size sz = level->getContentSize();
	level->setPosition( ccp( off+lgap+sz.width/2, parent.getContentSize().height/2 ) );
	parent.addChild( level );

	return (off+lgap+sz.width);
}

void CMainScene::_AddGoldOnTitle(cocos2d::Node &parent)
{	
	Size sz = parent.getContentSize();
	
	Sprite *icon = Sprite::create( "gold.png" );
	CCAssert( icon, "create gold iocn failed" );
	Size iconSz = icon->getContentSize();
	float scale = sz.height / iconSz.height;
	icon->setScale( scale );
	iconSz = iconSz * scale;
	int x = sz.width/4*3 + iconSz.width/2;
	icon->setPosition( ccp( x, sz.height/2) );
	parent.addChild( icon );

	char strVal[32] = {0};
	sprintf( strVal, "%llu", CUserManager::Instance().GetBasic().GetMoney(E_Money_Gold) );
	Label *val = Label::createWithSystemFont( strVal, "arial", 30 );
	CCAssert( val, "create label gold failed" );
	val->setColor( Color3B::YELLOW );
	val->setAnchorPoint( ccp( 0.5, 0.5 ) );
	Size szVal = val->getContentSize();
	x += iconSz.width/2 + GAP + szVal.width/2;
	val->setPosition( ccp( x, sz.height/2 ) );
	parent.addChild( val );
}

void CMainScene::_AddDiamondOnTitle(cocos2d::Node &parent)
{
	Size sz = parent.getContentSize();
	
	Sprite *icon = Sprite::create( "diamond.png" );
	CCAssert( icon, "create diamond iocn failed" );
	Size iconSz = icon->getContentSize();
	float scale = sz.height / iconSz.height;
	icon->setScale( scale );
	iconSz = iconSz * scale;
	int x = sz.width/2 + iconSz.width/2;
	icon->setPosition( ccp( x, sz.height/2) );
	parent.addChild( icon );

	char strVal[32] = {0};
	sprintf( strVal, "%llu", CUserManager::Instance().GetBasic().GetMoney(E_Money_Diamond) );
	Label *val = Label::createWithSystemFont( strVal, "arial", 30 );
	CCAssert( val, "create label diamond failed" );
	val->setColor( Color3B::YELLOW );
	val->setAnchorPoint( ccp( 0.5, 0.5 ) );
	Size szVal = val->getContentSize();
	x += iconSz.width/2 + GAP + szVal.width/2;
	val->setPosition( ccp( x, sz.height/2 ) );
	parent.addChild( val );
}

void CMainScene::_DumpGroupList(std::vector<TableViewData> &vecData)
{
	vecData.clear();
	CGroupManager &mgr = CUserManager::Instance().GetGroupManager();
	vector<u64> vecid;
	mgr.Dump( vecid );
	vector<u64>::iterator it = vecid.begin();
	for( ; it != vecid.end(); ++it )
	{
		CGroup *group = mgr.FindGroup( *it );
		CCAssert( group, "[CMainScene::_DumpGroupList] group is null" );

		TableViewData data;
		data.icon = group->GetHead();
		data.text = group->GetName();

		data.data = group;
		vecData.push_back( data ); 
	}
}

void CMainScene::_UpdateGroupList()
{
	CCLog( "CMainScene::_UpdateGroupList" );
	if( m_pGroupList )
	{
		vector<TableViewData> vecData;
		_DumpGroupList( vecData );
		m_pGroupList->UpdateElements( vecData );
	}
}

void CMainScene::_AddCreateGroupBtn(Node &parent, Size &sz)
{
	int x = 5, y = 5;
	CCMyMenuItemImage *item = CCMyMenuItemImage::create(
		"creategroup_normal.png",
		"creategroup_hover.png",
		this,
		menu_selector(CMainScene::menuCreateGroupCallback));
	if( !item )
	{
		CCLog( "[ERROR] _AddCreateGroupBtn item NULL" );
		return;
	}
	Size itemSz = item->getContentSize();
	item->setPosition(
		ccp(sz.width - itemSz.width/2 - x, itemSz.height/2 + y));
	CCMenu *menu = CCMenu::create( item, NULL );
	if( !menu )
	{
		CCLog( "[ERROR] _AddCreateGroupBtn menu NULL" );
		return;
	}
	menu->setPosition( CCPointZero );
	parent.addChild( menu, 1 );
}

void CMainScene::_AddGroupFuncBtns(cocos2d::Node &parent)
{
	const char *png[] = {
		"creategame_normal.png",
		"creategame_hover.png",
		"addmember_normal.png",
		"addmember_hover.png",
		"memberlist_normal.png",
		"memberlist_hover.png",
		"leavegroup_normal.png",
		"leavegroup_hover.png",
	};

	SEL_MenuHandler handle[] = {
		menu_selector(CMainScene::menuCreateGameCallback),
		menu_selector(CMainScene::menuInviteMemberCallback),
		menu_selector(CMainScene::menuMemberListCallback),
		menu_selector(CMainScene::menuLeaveGroupCallback),
	};

	int len = sizeof(handle) / sizeof(SEL_MenuHandler);
	int x = 5, y = 5, off = 13;
	for( int i=0; i<len; ++i )
	{
		CCMyMenuItemImage *item = CCMyMenuItemImage::create(
			png[2*i],
			png[2*i+1],
			this,
			(handle[i]));
		if( !item )
		{
			CCLog( "[ERROR] _AddCreateGroupBtn item NULL" );
			return;
		}
		Size itemSz = item->getContentSize();
		item->setPosition(
			ccp(x + itemSz.width/2, y + itemSz.height/2));
		CCMenu *menu = CCMenu::create( item, NULL );
		if( !menu )
		{
			CCLog( "[ERROR] _AddCreateGroupBtn menu NULL" );
			return;
		}
		menu->setPosition( CCPointZero );
		parent.addChild( menu, 1 );

		x += itemSz.width + off;
	}
}

void CMainScene::_LeaveGroup()
{
	CGroup *curGroup = CUserManager::Instance().GetViewData().GetSelectGroup();
	if( !curGroup )
	{
		CCLog( "[CMainScene::_LeaveGroup] not select group" );
		return;
	}
	CCLog( "[CMainScene::_LeaveGroup] try leave group:%llu:%s", curGroup->GetId(), curGroup->GetName().c_str() );
	CGroupClient *client = CNetManager::Instance().GetGroupClientInstance();
	if( !client )
	{
		CCLog( "[CMainScene::_LeaveGroup] [ERROR] group client null" );
		return;
	}
	client->LeaveGroup( curGroup->GetId() );
}

void CMainScene::_AskJoinGroup()
{
	CAskJoinGroupPopLayer *pop = CAskJoinGroupPopLayer::create( "commonbg.png" ); 
	if( pop )
	{
		pop->SetView( this );
		pop->setPosition( ccp(0, 0) );
		addChild( pop, 99999 );
	}
}

void CMainScene::_GroupMessageNotify()
{
	Chatmsg *msg = CUserManager::Instance().GetViewData().GetTopChatMsg();
	if( msg )
	{
		bool self = ( msg->sender == CUserManager::Instance().GetBasic().GetUser() );
		_AddChatContent( msg->groupid, msg->sender, msg->context, self );
		CUserManager::Instance().GetViewData().PopChatMsg();
	}
}

void CMainScene::_AddChatContent(u64 groupid, const std::string &user, const std::string &text, bool isSelf)
{
	CGroup *group = CUserManager::Instance().GetGroupManager().FindGroup( groupid );
	if( !group )
	{
		CCLog( "[CMainScene::_AddChatContent][ERROR] group:%llu is null", groupid );
		return;
	}
	group->CheckChatHistoryLimit();

	CGroupMember *member = group->FindMember( user );
	CCAssert( member, "CMainScene::_AddChatContent member is null" );
	{
		ChatTableViewData data(
			ChatTableViewData::E_DataType_User,
			member->GetName(),
			GetCurTime() );
		if( isSelf )
		{
			data.SetUser( CUserManager::Instance().GetBasic().GetUser() );
		}
		group->GetChatHistory().push_back( data );
	}
	{
		ChatTableViewData data(
			ChatTableViewData::E_DataType_Text,
			member->GetName(),
			text );
		group->GetChatHistory().push_back( data );
	}

	if( CUserManager::Instance().GetViewData().GetSelectGroup() == group )
	{
		m_chatTableView->InsertUpdate();
	}
}

extern void DumpRoomList(u64 groupid, CGameRoomList &roomList, std::vector<TableViewData> &vecData);
void CMainScene::_GameRoomUpdate()
{
	if( m_pGameRoomList && CUserManager::Instance().GetViewData().GetSelectGroup() )
	{
		vector<TableViewData> vecData;
		DumpRoomList( 
			CUserManager::Instance().GetViewData().GetSelectGroup()->GetId(),
			CUserManager::Instance().GetGameRoomList(), vecData );
		m_pGameRoomList->UpdateElements( vecData );
	}
}

void CMainScene::_AddGameRoomList(cocos2d::Node &parent, Size &lstSz, Size &cellSz)
{
	vector<TableViewData> vecData;
	m_pGameRoomList = CMyTableView::create( lstSz, cellSz, vecData, "selectbg.png" );
    CCAssert( m_pGameRoomList, "_AddGameRoomList GetTableView Failed" );
	m_pGameRoomList->SetPosition( ccp(lstSz.width/2, lstSz.height/2 + 10) );
	m_pGameRoomList->SetTouchCallback( callfuncND_selector(CMainScene::GameRoomListTouchedCallback), this );
	parent.addChild( m_pGameRoomList );
}

void CMainScene::_ConnectGameSuccess()
{
	int gameid = CUserManager::Instance().GetViewData().GetCurGameRoom()->m_gameid; 
	CCLog( "CMainScene::_ConnectGameSuccess game:%d", gameid );

	string dir,name,icon;
	CUserManager::Instance().GetGameInfo(gameid, dir, icon, name);
	string gamelua = dir + "/lua/main.lua";
	LuaEngine::getInstance()->reload( gamelua.c_str() );
}