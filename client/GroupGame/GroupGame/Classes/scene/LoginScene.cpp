#include "scene/LoginScene.h"
#include "res/ResManager.h"
#include "net/NetManager.h"
#include "CommDef.h"
#include "cocos-ext.h"
#include "view/MyMenuItemImage.h"
#include "view/MyEditBox.h"
#include "user/UserManager.h"
#include "view/MyLoadingItem.h"
#include "MainScene.h"
#include "SceneManager.h"
using namespace cocos2d;
using namespace cocos2d::extension;

static void TextUserChanged(const std::string &text)
{
	CCLog( "Text User changed, old:%s new:%s", 
		CUserManager::Instance().GetBasic().GetUser().c_str(), 
		text.c_str() );
	CUserManager::Instance().GetBasic().SetUser( text );
}

static void TextPwdChanged(const std::string &text)
{
	CCLog( "Text Password changed, old:%s new:%s", 
		CUserManager::Instance().GetBasic().GetPwd().c_str(), 
		text.c_str() );
	CUserManager::Instance().GetBasic().SetPwd( text );
}

CCScene* CLoginScene::scene()
{
    CCScene *scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF( !scene );

        CLoginScene *layer = CLoginScene::create();
        CC_BREAK_IF( !layer );

        scene->addChild( layer );
    } while( 0 );

    return scene;
}

void CLoginScene::UpdateView(int type)
{
	switch( type )
	{
	case CSceneManager::E_UpdateType_ConnectLogin:
		{
			_UpdateLoadingStep();
			_UserLogin();
		}
		break;
	case CSceneManager::E_UpdateType_LoadingProgress:
		{
			_UpdateLoadingStep();
		}
		break;
	case CSceneManager::E_UpdateType_ConnectLoginError:
	case CSceneManager::E_UpdateType_LoginFailed:
		{
			_RemoveLoadingView();
			_AddLoginView( 
				CUserManager::Instance().GetViewData().GetSavePwd(),
				CUserManager::Instance().GetViewData().GetAutoLogin() );
		}
		break;
	default:
		CCLog( "[CLoginScene::UpdateView] unknown type:%d", type );
		break;
	};
}

bool CLoginScene::init()
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
		_AddLoginView( 
			CUserManager::Instance().GetViewData().GetSavePwd(),
			CUserManager::Instance().GetViewData().GetAutoLogin() );
		m_loadView.m_loadItem = NULL;
        
#ifdef _DEBUG
		// for test some code	
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(CLoginScene::menuTestCallback));
        CC_BREAK_IF(! pCloseItem);
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);
        this->addChild(pMenu, 1);
#endif

		// test control button
		//CCScale9Sprite *backgroundButton = CCScale9Sprite::create("btn_normal_p9.9.png");
		//CCScale9Sprite *backgroundHighlightedButton = CCScale9Sprite::create("btn_hover_p9.9.png");

		/*
		//CCRect rc(60,15,80,45);
		CCScale9Sprite *backgroundButton = CCScale9Sprite::create("abc.png");
		CCScale9Sprite *backgroundHighlightedButton = CCScale9Sprite::create("abc.png");
		CCControlButton *button = CCControlButton::create(backgroundButton);
		button->setContentSize( CCSize(120,120) );
		//button->setBackgroundSpriteForState(backgroundHighlightedButton, CCControlStateHighlighted);
		//button->setAnchorPoint( ccp(0,0) );
		button->setPosition( ccp( 100, 100 ) );
		this->addChild( button, 1 );
		//CCScale9Sprite *abc = CCScale9Sprite::create("abc.png");
		//abc->setPosition( ccp( 200, 200 ) );
		//this->addChild( abc, 1 );
		//*/

		//backgroundButton->setPosition( ccp( 100, 100 ) );
		//this->addChild( backgroundButton, 1 );

		ret = true;

		CSceneManager::Instance().SetCurView( this );

    } while( 0 );

    return ret;
}

void CLoginScene::TestTimer(float t)
{
#ifdef _DEBUG
	_UpdateLoadingStep();
#endif
}

void CLoginScene::menuTestCallback(CCObject *pSender)
{
#ifdef _DEBUG
	CCDirector::sharedDirector()->replaceScene( CMainScene::scene() );
	/*
	static int a = 1;
	if( a % 2 == 1 )
	{
		_RemoveLoginView();
	}
	else
	{
		_AddLoginView( 
			CUserManager::Instance().GetSavePwd(),
			CUserManager::Instance().GetAutoLogin() );
	}
	++a;
	//*/
#endif
}

const int g_loadingStep = 6;
void CLoginScene::menuLoginCallback(CCObject *pSender)
{
	if( CNetManager::Instance().StartLogin( LOGIN_IP, LOGIN_PORT ) )
	{
		_RemoveLoginView();
		_AddLoadingView( g_loadingStep );
	}
	else
	{
		CCLog( "[CLoginScene::menuLoginCallback] start login (%s:%d) failed",
			LOGIN_IP, LOGIN_PORT );
	}
}

void CLoginScene::menuCloseCallback(CCObject *pSender)
{
    CCDirector::sharedDirector()->end();
}

void CLoginScene::menuCancelCallback(CCObject *pSender)
{
	_RemoveLoadingView();
	_AddLoginView( 
		CUserManager::Instance().GetViewData().GetSavePwd(),
		CUserManager::Instance().GetViewData().GetAutoLogin() );
}

void CLoginScene::menuSavePwdCheckBoxCallback(CCObject *pSender)
{
	bool curState = CUserManager::Instance().GetViewData().GetSavePwd();
	CCLog( "click save pwd checkbox, pre state:%d, new state:%d", curState?1:0, !curState?1:0 );
	CUserManager::Instance().GetViewData().SetSavePwd( !curState );
}

void CLoginScene::menuAutoLoginCheckBoxCallback(CCObject *pSender)
{
	bool curState = CUserManager::Instance().GetViewData().GetAutoLogin();
	CCLog( "click auto login checkbox, cur state:%d, new state:%d", curState?1:0, !curState?1:0 );
	CUserManager::Instance().GetViewData().SetAutoLogin( !curState );
}

void CLoginScene::_AddSceneBg()
{
	CCSprite *pSprite = CResManager::Instance().GetSpriteBg();
    CCAssert( pSprite, "GetResLoginBg Failed" );
	pSprite->setAnchorPoint( ccp(0, 0) );
	pSprite->setScaleX( WIN_SIZE_W / pSprite->getContentSize().width );
	pSprite->setScaleY( WIN_SIZE_H / pSprite->getContentSize().height );
    pSprite->setPosition( ccp(0, 0) );
    addChild( pSprite, 0 );
}

void CLoginScene::_AddLoginView(bool isSavePwdSelect, bool isAutoLoginSelect)
{
	CCSize winSz = CCDirector::sharedDirector()->getWinSize();

	CCSprite *loginBg = CResManager::Instance().GetSpriteLoginBg();
	CCAssert( loginBg, "GetResLoginBg Failed" ); 
	loginBg->setAnchorPoint( ccp(0, 0) );
	loginBg->setPosition( 
		ccp((winSz.width-loginBg->getContentSize().width)/2, (winSz.height-loginBg->getContentSize().height)/2) );
    addChild( loginBg, 1, E_Tag_LoginBg );

	_AddEditBox( *loginBg );
	_AddCheckBoxSavePwd( *loginBg, isSavePwdSelect );
	_AddCheckBoxAuthLogin( *loginBg, isAutoLoginSelect );
	_AddBtnLogin( *loginBg );
	_AddBtnExit( *loginBg );
}

void CLoginScene::_AddEditBox(CCSprite &loginBg)
{
	CCSize bgSz = loginBg.getContentSize();

	CCSprite *editboxBg = CResManager::Instance().GetSpriteEditBox();
	CCAssert( editboxBg, "GetResEditBox Failed" );
	CCSize sz = editboxBg->getContentSize();
	editboxBg->setAnchorPoint( ccp(0, 0) );
	editboxBg->setPosition( 
		ccp((bgSz.width-sz.width)/2, ((bgSz.height/2)-sz.height)/2+(bgSz.height/2)) );
    loginBg.addChild( editboxBg, 0 );

	int x = 10;
	CCSprite *wordUser = CResManager::Instance().GetSpriteWordUser();
	CCAssert( wordUser, "GetResWordUser Failed" );
	CCSize wsz = wordUser->getContentSize();
	int userY = (sz.height/2-wsz.height)/2 + (sz.height/2);
	wordUser->setAnchorPoint( ccp(0, 0) );
	wordUser->setPosition( ccp(x, userY) );
	editboxBg->addChild( wordUser, 0 );

	CCSprite *wordPwd = CResManager::Instance().GetSpriteWordPwd();
	CCAssert( wordPwd, "GetResWordPwd Failed" );
	wsz = wordPwd->getContentSize();
	wordPwd->setAnchorPoint( ccp(0, 0) );
	wordPwd->setPosition( ccp(x, (sz.height/2-wsz.height)/2) );
	editboxBg->addChild( wordPwd, 0 );

	CCSize editSz = CCSize( sz.width - 3*x - wsz.width, sz.height/3);
	CMyEditBox *editUser = CMyEditBox::create(
		editSz, ccp(2*x+wsz.width, (sz.height/2-editSz.height)/2 + (sz.height/2)), 
		"white_edit.png", "Paint Boy.ttf", 25, ccBLACK, 8 );
	CCAssert( editUser, "GetUserEditBox Failed" );
	editUser->SetEditChangedCallback( TextUserChanged );
    editboxBg->addChild( editUser, 1 );
	
	CMyEditBox *pwdEdit = CMyEditBox::create(
		editSz, ccp(2*x+wsz.width, (sz.height/2-editSz.height)/2), 
		"white_edit.png", "Paint Boy.ttf", 25, ccBLACK, 8 );
	CCAssert( pwdEdit, "GetPwdEditBox Failed" );
	pwdEdit->SetPasswordMode();
	pwdEdit->SetEditChangedCallback( TextPwdChanged );
    editboxBg->addChild( pwdEdit, 1 );
}

void CLoginScene::_AddCheckBoxSavePwd(CCSprite &loginBg, bool isSelect)
{
	CCSize bgSz = loginBg.getContentSize();
	CCMenu *menu = CCMenu::createWithItem(NULL);
	CCAssert( menu, "GetMenu Failed" );

	CCSprite *checkboxNormal = CResManager::Instance().GetSpriteCheckBoxNormal();
	CCAssert( checkboxNormal, "GetResCheckBoxNormal Failed" );
	CCSize sz = checkboxNormal->getContentSize();

	CCSprite *checkboxSelect = CResManager::Instance().GetSpriteCheckBoxSelect();
	CCAssert( checkboxSelect, "GetResCheckBoxSelect Failed" );
	CCSize selSz = checkboxSelect->getContentSize();

	CCSprite *wordSave = CResManager::Instance().GetSpriteWordSavePwd();
	CCAssert( wordSave, "GetResWordSavePwd Failed" );
	CCSize wsz = wordSave->getContentSize();

	CCMenuItemSprite *itemNormal = CCMenuItemSprite::create( checkboxNormal, NULL );
	CCMenuItemSprite *itemSelect = CCMenuItemSprite::create( checkboxSelect, NULL );
	if( isSelect )
	{
		CCMenuItemToggle *item = CCMenuItemToggle::createWithTarget(
			this,
			menu_selector(CLoginScene::menuSavePwdCheckBoxCallback),
			itemSelect, itemNormal, NULL );
		CCAssert( itemNormal && itemSelect && item, "GetCheckBoxItems Failed" );
		menu->addChild( item );
	}
	else
	{
		CCMenuItemToggle *item = CCMenuItemToggle::createWithTarget(
			this,
			menu_selector(CLoginScene::menuSavePwdCheckBoxCallback),
			itemNormal, itemSelect, NULL );
		CCAssert( itemNormal && itemSelect && item, "GetCheckBoxItems Failed" );
		menu->addChild( item );
	}

	int off = 10;
	int x = (bgSz.width/2-sz.width-wsz.width-off) / 2;
	int y = bgSz.height / 3;

	menu->setAnchorPoint( ccp(0, 0) );
	// ???
	// 如果 menu->setPosition( ccp(0,0) ); 从效果来看，上面的锚点设置没有成功, why ?
	// x,y是算好的坐标，下面只能在x,y基础上加上半个sz大小偏移
	menu->setPosition( ccp(x+sz.width/2, y+sz.height/2) );
	loginBg.addChild( menu, 0 );
	
	wordSave->setAnchorPoint( ccp(0, 0) );
	wordSave->setPosition( ccp(x+sz.width+off, y+((sz.height-wsz.height)/2)) );
	loginBg.addChild( wordSave, 0 );
}

void CLoginScene::_AddCheckBoxAuthLogin(CCSprite &loginBg, bool isSelect)
{
	CCSize bgSz = loginBg.getContentSize();
	CCMenu *menu = CCMenu::createWithItem(NULL);
	CCAssert( menu, "GetMenu Failed" );

	CCSprite *checkboxNormal = CResManager::Instance().GetSpriteCheckBoxNormal();
	CCAssert( checkboxNormal, "GetResCheckBoxNormal Failed" );
	CCSize sz = checkboxNormal->getContentSize();

	CCSprite *checkboxSelect = CResManager::Instance().GetSpriteCheckBoxSelect();
	CCAssert( checkboxSelect, "GetResCheckBoxSelect Failed" );
	CCSize selSz = checkboxSelect->getContentSize();

	CCSprite *wordAutoLogin = CResManager::Instance().GetSpriteWordAutoLogin();
	CCAssert( wordAutoLogin, "GetResWordAutoLogin Failed" );
	CCSize wsz = wordAutoLogin->getContentSize();
	wordAutoLogin->setAnchorPoint( ccp(0, 0) );

	CCMenuItemSprite *itemNormal = CCMenuItemSprite::create( checkboxNormal, NULL );
	CCMenuItemSprite *itemSelect = CCMenuItemSprite::create( checkboxSelect, NULL );
	if( isSelect )
	{
		CCMenuItemToggle *item = CCMenuItemToggle::createWithTarget(
			this,
			menu_selector(CLoginScene::menuAutoLoginCheckBoxCallback),
			itemSelect, itemNormal, NULL );
		CCAssert( itemNormal && itemSelect && item, "GetCheckBoxItems Failed" );
		menu->addChild( item );
	}
	else
	{
		CCMenuItemToggle *item = CCMenuItemToggle::createWithTarget(
			this,
			menu_selector(CLoginScene::menuAutoLoginCheckBoxCallback),
			itemNormal, itemSelect, NULL );
		CCAssert( itemNormal && itemSelect && item, "GetCheckBoxItems Failed" );
		menu->addChild( item );
	}

	int off = 10;
	int x = (bgSz.width/2-sz.width-wsz.width-off) / 2 + (bgSz.width/2);
	int y = bgSz.height / 3;

	menu->setAnchorPoint( ccp(0, 0) );
	// ???
	// 如果 menu->setPosition( ccp(0,0) ); 从效果来看，上面的锚点设置没有成功, why ?
	// x,y是算好的坐标，下面只能在x,y基础上加上半个sz大小偏移
	menu->setPosition( ccp(x+sz.width/2, y+sz.height/2) );
	loginBg.addChild( menu, 0 );
	
	wordAutoLogin->setPosition( ccp(x+sz.width+off, y+((sz.height-wsz.height)/2)) );
	loginBg.addChild( wordAutoLogin, 0 );
}

void CLoginScene::_AddBtnLogin(CCSprite &loginBg)
{
	CCSize bgSz = loginBg.getContentSize();

	CCMyMenuItemImage *loginItem = CCMyMenuItemImage::create(
		"btnlogin_normal.png",
		"btnlogin_hover.png",
		this,
		menu_selector(CLoginScene::menuLoginCallback));
	CCAssert( loginItem, "CreateLoginItem Failed" );
	CCSize btnSz = loginItem->getContentSize();
	loginItem->setAnchorPoint( ccp(0, 0) );

	int x = (bgSz.width - btnSz.width) / 2;
#if _ENABLE_EXIT_BTN
	x = (bgSz.width/2 - btnSz.width) / 2;
#endif
	int y = bgSz.height/6 - btnSz.height/2;

	loginItem->setPosition(ccp(x, y) );
	CCMenu *menu = CCMenu::create( loginItem, NULL );
	menu->setPosition(CCPointZero);
	CCAssert( menu, "CreateLoginMenu Failed" );
	loginBg.addChild( menu, 0 );
}

void CLoginScene::_AddBtnExit(CCSprite &loginBg)
{
#if _ENABLE_EXIT_BTN
	CCSize bgSz = loginBg.getContentSize();

	CCMyMenuItemImage *exitItem = CCMyMenuItemImage::create(
		"btnexit_normal.png",
		"btnexit_hover.png",
		this,
		menu_selector(CLoginScene::menuCloseCallback));
	CCAssert( exitItem, "CreateExitItem Failed" );
	CCSize btnSz = exitItem->getContentSize();
	exitItem->setAnchorPoint( ccp(0, 0) );

	int x = (bgSz.width/2 - btnSz.width) / 2 + bgSz.width/2;
	int y = bgSz.height/6 - btnSz.height/2;
	exitItem->setPosition( ccp(x, y) );

	CCMenu *menu = CCMenu::create( exitItem, NULL );
	menu->setPosition(CCPointZero);
	CCAssert( menu, "CreateExitMenu Failed" );
	loginBg.addChild( menu, 0 );
#endif
}

void CLoginScene::_AddLoadingView(int steps)
{
	CCSize winSz = CCDirector::sharedDirector()->getWinSize();

	CCNode *loadingNode = CCNode::create();
	CCAssert( loadingNode, "CreateLoadingNode Failed" );
	CCSize sz = CCSize(10, 10);
	loadingNode->setContentSize( sz );
	loadingNode->setAnchorPoint( ccp(0.5, 0.5) );
	loadingNode->setPosition( ccp(winSz.width/2, winSz.height/2) );
	addChild( loadingNode, 1, E_Tag_Loading );

	_AddLoadingIcon( *loadingNode );
	_AddLoadingText( *loadingNode );
	_AddLoadingProgress( *loadingNode, steps );
	_AddLoadingCancelBtn( *loadingNode );

#ifdef _DEBUG
	// test
	//CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
	//	schedule_selector(CLoginScene::TestTimer), this, 1.0f, false);
#endif
}

void CLoginScene::_AddLoadingIcon(cocos2d::CCNode &loadingNode)
{
}

void CLoginScene::_AddLoadingText(cocos2d::CCNode &loadingNode)
{
	CCSize winSz = CCDirector::sharedDirector()->getWinSize();
	CCSize nodeSz = loadingNode.getContentSize();

	CCSprite *text = CResManager::Instance().GetSpriteLoadingText();
	CCAssert( text, "CreateResLoadingText Failed" );
	text->setPosition( ccp(nodeSz.width/2, nodeSz.height/2) );
	loadingNode.addChild( text, 0 );
}

void CLoginScene::_AddLoadingProgress(cocos2d::CCNode &loadingNode, int steps)
{
	CCSize winSz = CCDirector::sharedDirector()->getWinSize();
	CCSize nodeSz = loadingNode.getContentSize();
	int x = nodeSz.width / 2;
	int y = nodeSz.height / 2 - (winSz.height / 6) ;
	m_loadView.m_loadItem = CMyLoadingItem::create( steps );
	m_loadView.m_loadItem->SetPosition( ccp(x, y) );
	loadingNode.addChild( m_loadView.m_loadItem, 0 );
}

void CLoginScene::_AddLoadingCancelBtn(cocos2d::CCNode &loadingNode)
{	
	CCSize winSz = CCDirector::sharedDirector()->getWinSize();
	CCSize nodeSz = loadingNode.getContentSize();

	CCMyMenuItemImage *cancelItem = CCMyMenuItemImage::create(
		"btncancel_normal.png",
		"btncancel_hover.png",
		this,
		menu_selector(CLoginScene::menuCancelCallback));
	CCAssert( cancelItem, "CreateExitItem Failed" );
	CCSize btnSz = cancelItem->getContentSize();

	int x = nodeSz.width / 2;
	int y = nodeSz.height / 2 - (winSz.height / 3) ;
	cancelItem->setPosition( ccp(x, y) );

	CCMenu *menu = CCMenu::create( cancelItem, NULL );
	menu->setPosition( CCPointZero );
	CCAssert( menu, "CreateCancelMenu Failed" );
	loadingNode.addChild( menu, 0 );
}

void CLoginScene::_RemoveLoginView()
{
	removeChildByTag( E_Tag_LoginBg );
}

void CLoginScene::_UpdateLoadingStep()
{
	if( m_loadView.m_loadItem )
	{
		m_loadView.m_loadItem->UpdateProgress();
	}
}

void CLoginScene::_UserLogin()
{
	CLoginClient *client = CNetManager::Instance().GetLoginClientInstance();
	if( !client )
	{
		CCLog( "[CLoginScene::_UserLogin] login client instance NULL" );
		return;
	}

	client->Login( 
		CUserManager::Instance().GetBasic().GetUser(),
		CUserManager::Instance().GetBasic().GetPwd() );
}

void CLoginScene::_RemoveLoadingView()
{
#ifdef _DEBUG
	// test
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
		schedule_selector(CLoginScene::TestTimer), this );
#endif

	removeChildByTag( E_Tag_Loading );
	m_loadView.m_loadItem = NULL;
}
