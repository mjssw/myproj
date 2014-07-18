#include "MemberListPopLayer.h"
#include "MyMenuItemImage.h"
using namespace cocos2d;
using namespace std;

void CMemberListPopLayer::onEnter()
{
	CPopLayerBase::onEnter();

	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CMemberListPopLayer::onEnter] get parent null" );
		return;
	}
	CCMyMenuItemImage *closeItem = CCMyMenuItemImage::create(
		"btncancel_normal.png",
		"btncancel_hover.png",
		this,
		menu_selector(CMemberListPopLayer::menuCloseCallback) );
	if( !closeItem )
	{
		CCLog( "[CMemberListPopLayer::onEnter] close item null" );
		return;
	}
	Size szItem = closeItem->getContentSize();
	Size szParent = parent->getContentSize();
	closeItem->setAnchorPoint( ccp(0.5, 0.5) );
	closeItem->setPosition( ccp( szParent.width/2, szItem.height/2 ) );

	CCMenu *menu = CCMenu::create( closeItem, NULL );
	if( !menu )
	{
		CCLog( "[CMemberListPopLayer::onEnter] close menu null" );
		return;
	}
	menu->setPosition( CCPointZero );
	parent->addChild( menu, 1 );
}

void CMemberListPopLayer::menuCloseCallback(Object *sender)
{
	removeFromParent();
}

CMemberListPopLayer* CMemberListPopLayer::create(const char *bgimage)
{
	CMemberListPopLayer* layer = new CMemberListPopLayer();
	if( layer && layer->init() )
	{
		layer->autorelease();
	
		CCSize szWin = CCDirector::sharedDirector()->getWinSize();
		Sprite *bg = Sprite::create( bgimage );
		if( !bg )
		{
			return NULL;
		}
		bg->setAnchorPoint( ccp(0.5, 0.5) );
		Size szBg = bg->getContentSize();
		bg->setPosition( ccp( szWin.width / 2, szWin.height / 2 ) );
		layer->addChild( bg, 0, E_Tag_Bg );
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}
