#include "LoginScene.h"
#include "ResManager.h"
#include "CommDef.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

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

        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(CLoginScene::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);
        this->addChild(pMenu, 1);

		// 长宽分别6等分, 登录背景分别占4格
		int x = ( WIN_SIZE_W / 6 );
		int y = ( WIN_SIZE_H / 6 );
		int h = y * 4;
		int w = x * 4;

		ret = _AddSceneBg();
        CC_BREAK_IF( !ret );

		ret = _AddLoginBg( x, y, h );
        CC_BREAK_IF( !ret );

		ret = _AddLoginBtn( x, y, w, h );
        CC_BREAK_IF( !ret );
		
		ret = _AddExitBtn( x, y, w, h );
        CC_BREAK_IF( !ret );

    } while( 0 );

    return ret;
}

void CLoginScene::menuCloseCallback(CCObject *pSender)
{
    CCDirector::sharedDirector()->end();
}

bool CLoginScene::_AddSceneBg()
{
	CCSprite *pSprite = CResManager::Instance().GetSpriteBg();
    if( !pSprite )
	{
		return false;
	}
	pSprite->setAnchorPoint( ccp(0, 0) );
	pSprite->setScaleX( WIN_SIZE_W / pSprite->getContentSize().width );
	pSprite->setScaleY( WIN_SIZE_H / pSprite->getContentSize().height );
    pSprite->setPosition( ccp(0, 0) );
    addChild( pSprite, 0 );

	return true;
}

bool CLoginScene::_AddLoginBg(int x, int y, int h)
{
	int lx = x;
	int ly = y;
	CCSprite *pSprite = CResManager::Instance().GetSpriteLoginBgLeft();
    if( !pSprite )
	{
		return false;
	}
	pSprite->setAnchorPoint( ccp(0, 0) );
	pSprite->setScaleY( h / pSprite->getContentSize().height );
    pSprite->setPosition( ccp(lx, ly) );
    addChild( pSprite, 0 );

	int mx = lx + pSprite->getContentSize().width;
	int my = y;

	int rx = WIN_SIZE_W - x - pSprite->getContentSize().width;
	int ry = y;
	pSprite = CResManager::Instance().GetSpriteLoginBgRight();
    if( !pSprite )
	{
		return false;
	}
	pSprite->setAnchorPoint( ccp(0, 0) );
	pSprite->setScaleY( h / pSprite->getContentSize().height );
	pSprite->setPosition( ccp(rx, ry) );
    addChild( pSprite, 0 );

	pSprite = CResManager::Instance().GetSpriteLoginBgMidd();
    if( !pSprite )
	{
		return false;
	}
	pSprite->setAnchorPoint( ccp(0, 0) );
	pSprite->setScaleY( h / pSprite->getContentSize().height );
	pSprite->setScaleX( (rx-mx) / pSprite->getContentSize().width );
	pSprite->setPosition( ccp(mx, my) );
    addChild( pSprite, 0 );
	
	return true;
}

bool CLoginScene::_AddLoginBtn(int loginBgX, int loginBgY, int loginBgW, int loginBgH)
{
	CCSprite *sprite = CResManager::Instance().GetSpriteBtnNormal();
	float scale = (loginBgW / 5 ) / ( sprite->getContentSize().width * 2 ) ;
	sprite->setScale( scale );
	sprite->setAnchorPoint( ccp(0, 0) );
	CCSize sz = sprite->getContentSize() * scale;

	int btnW = sz.width * 2;
	int btnH = sz.height;

	int x = loginBgX + (loginBgW - btnW) / 2;
	int y = loginBgY + (btnH / 2);
#if _ENABLE_EXIT_BTN
	x -= btnW;
#endif

	CCSprite *word = CResManager::Instance().GetSpriteBtnWordLogin();
	if( !word )
	{
		return false;
	}
	word->setAnchorPoint( ccp(0, 0) );
	CCSize szWord = word->getContentSize() * scale;
	word->setPosition( ccp((btnW-szWord.width)/2, (btnH-szWord.height)/2) );
	sprite->addChild( word );

	sprite->setPosition( ccp(x, y) );
    addChild( sprite, 0 );

	return true;
}

bool CLoginScene::_AddExitBtn(int loginBgX, int loginBgY, int loginBgW, int loginBgH)
{
#if _ENABLE_EXIT_BTN
	CCSprite *sprite = CResManager::Instance().GetSpriteBtnNormal();
	float scale = (loginBgW / 5 ) / ( sprite->getContentSize().width * 2 ) ;
	sprite->setScale( scale );
	sprite->setAnchorPoint( ccp(0, 0) );
	CCSize sz = sprite->getContentSize() * scale;

	int btnW = sz.width * 2;
	int btnH = sz.height;

	int x = loginBgX + (loginBgW - btnW) / 2 + btnW;
	int y = loginBgY + (btnH / 2);

	CCSprite *word = CResManager::Instance().GetSpriteBtnWordExit();
	if( !word )
	{
		return false;
	}
	word->setAnchorPoint( ccp(0, 0) );
	CCSize szWord = word->getContentSize() * scale;
	word->setPosition( ccp((btnW-szWord.width)/2, (btnH-szWord.height)/2) );
	sprite->addChild( word );

	sprite->setPosition( ccp(x, y) );
    addChild( sprite, 0 );
	
#endif
	return true;
}
