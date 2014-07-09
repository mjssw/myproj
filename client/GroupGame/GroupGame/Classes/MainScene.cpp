#include "MainScene.h"
using namespace cocos2d;
#include "CommDef.h"

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
#endif

        ret = true;
    } while( 0 );

    return ret;
}

void CMainScene::menuCloseCallback(CCObject *pSender)
{
    CCDirector::sharedDirector()->end();
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
	pSprite->setPosition( ccp(sz.width/2 + g_off, sz.height/2 + g_off) );
    addChild( pSprite, 0 );
	
	m_mainViewOffX = sz.width;
}

void CMainScene::_AddMainView()
{
	CCSprite *pSprite = CCSprite::create( "mainbg.png" );
    CCAssert( pSprite, "GetMainbg Failed" );
	CCSize sz = pSprite->getContentSize();
	pSprite->setPosition( ccp(sz.width/2 + m_mainViewOffX + 2*g_off, sz.height/2 + g_off) );
    addChild( pSprite, 0 );
}
