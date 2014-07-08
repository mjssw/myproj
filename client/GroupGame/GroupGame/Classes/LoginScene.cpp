#include "LoginScene.h"
#include "ResManager.h"
#include "CommDef.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;
#include "MyMenuItemImage.h"

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
		_AddLoginView( false, false );
        
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
    } while( 0 );

    return ret;
}

void CLoginScene::menuTestCallback(CCObject *pSender)
{
	static int a = 1;
	if( a % 2 == 1 )
	{
		_RemoveLoginView();
	}
	else
	{
		_AddLoginView( false, false );
	}
	++a;
}

void CLoginScene::menuLoginCallback(CCObject *pSender)
{
}

void CLoginScene::menuCloseCallback(CCObject *pSender)
{
    CCDirector::sharedDirector()->end();
}

void CLoginScene::menuSavePwdCheckBoxCallback(CCObject *pSender)
{
}

void CLoginScene::menuAutoLoginCheckBoxCallback(CCObject *pSender)
{
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
	_AddCheckBtnLogin( *loginBg );
	_AddCheckBtnExit( *loginBg );
}

void CLoginScene::_AddEditBox(CCSprite &loginBg)
{
	CCSize bgSz = loginBg.getContentSize();

	CCSprite *editbox = CResManager::Instance().GetSpriteEditBox();
	CCAssert( editbox, "GetResEditBox Failed" );
	CCSize sz = editbox->getContentSize();
	editbox->setAnchorPoint( ccp(0, 0) );
	editbox->setPosition( 
		ccp((bgSz.width-sz.width)/2, ((bgSz.height/2)-sz.height)/2+(bgSz.height/2)) );
    loginBg.addChild( editbox, 0 );

	int x = 10;
	CCSprite *wordUser = CResManager::Instance().GetSpriteWordUser();
	CCAssert( wordUser, "GetResWordUser Failed" );
	CCSize wsz = wordUser->getContentSize();
	wordUser->setAnchorPoint( ccp(0, 0) );
	wordUser->setPosition( ccp(x, (sz.height/2-wsz.height)/2 + (sz.height/2)) );
	editbox->addChild( wordUser, 0 );

	CCSprite *wordPwd = CResManager::Instance().GetSpriteWordPwd();
	CCAssert( wordPwd, "GetResWordPwd Failed" );
	wsz = wordPwd->getContentSize();
	wordPwd->setAnchorPoint( ccp(0, 0) );
	wordPwd->setPosition( ccp(x, (sz.height/2-wsz.height)/2) );
	editbox->addChild( wordPwd, 0 );
	
	m_inputFrom = x + wsz.width + 10;
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

void CLoginScene::_AddCheckBtnLogin(CCSprite &loginBg)
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

void CLoginScene::_AddCheckBtnExit(CCSprite &loginBg)
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

void CLoginScene::_RemoveLoginView()
{
	removeChildByTag( E_Tag_LoginBg );
}
