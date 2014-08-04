#include "AskJoinGroupPopLayer.h"
#include "MyMenuItemImage.h"
#include "user/UserManager.h"
#include "user/ViewData.h"
#include "user/Group.h"
#include "user/GroupMember.h"
#include "utils.h"
#include "CommDef.h"
#include "scene/ViewBase.h"
#include "scene/LoginScene.h"
#include "scene/SceneManager.h"
#include "res/ResManager.h"
#include "view/MyEditBox.h"
#include "net/NetManager.h"
using namespace cocos2d;
using namespace std;

void CAskJoinGroupPopLayer::onEnter()
{
	CPopLayerBase::onEnter();

	_AddJoinBtn();
	_AddCloseBtn( false );
	_AddTitle();
}

void CAskJoinGroupPopLayer::menuCloseCallback(Object *sender)
{
	removeFromParent();
}

void CAskJoinGroupPopLayer::menuJoinCallback(cocos2d::Object *sender)
{
	CGroupClient *client = CNetManager::Instance().GetGroupClientInstance();
	if( !client )
	{
		CCLog( "[CAskJoinGroupPopLayer::menuJoinCallback] [ERROR] group client is null" );
		return;
	}

	client->AgreeJoinGroup(
		CUserManager::Instance().GetViewData().GetJoinGroupId() );

	menuCloseCallback( sender );
}

CAskJoinGroupPopLayer* CAskJoinGroupPopLayer::create(const char *bgimage)
{
	CAskJoinGroupPopLayer *layer = new CAskJoinGroupPopLayer();
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

void CAskJoinGroupPopLayer::_AddJoinBtn()
{
	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CAskJoinGroupPopLayer::onEnter] get parent null" );
		return;
	}

	CCMyMenuItemImage *regItem = CCMyMenuItemImage::create(
		"btn2_normal.png",
		"btn2_hover.png",
		this,
		menu_selector(CAskJoinGroupPopLayer::menuJoinCallback) );
	if( !regItem )
	{
		CCLog( "[CAskJoinGroupPopLayer::onEnter] register item null" );
		return;
	}
	Size szItem = regItem->getContentSize();
	Size szParent = parent->getContentSize();
	regItem->setAnchorPoint( ccp(0.5, 0.5) );
	regItem->setPosition( ccp( szParent.width/4, szParent.height/4) );

	CCMenu *menu = CCMenu::create( regItem, NULL );
	if( !menu )
	{
		CCLog( "[CMemberListPopLayer::onEnter] register menu null" );
		return;
	}
	menu->setPosition( CCPointZero );
	parent->addChild( menu, 1, E_Tag_OkBtn );

	Label *text = Label::createWithSystemFont( a2u("确定"), "arial", 25 );
	CCAssert( text, "text register failed" );
	text->setPosition( ccp(szItem.width/2, szItem.height/2) );
	regItem->addChild( text );
}

void CAskJoinGroupPopLayer::_AddCloseBtn(bool isCenter)
{
	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CAskJoinGroupPopLayer::onEnter] get parent null" );
		return;
	}

	CCMyMenuItemImage *closeItem = CCMyMenuItemImage::create(
		"btn2_normal.png",
		"btn2_hover.png",
		this,
		menu_selector(CAskJoinGroupPopLayer::menuCloseCallback) );
	if( !closeItem )
	{
		CCLog( "[CAskJoinGroupPopLayer::onEnter] close item null" );
		return;
	}
	Size szItem = closeItem->getContentSize();
	Size szParent = parent->getContentSize();
	closeItem->setAnchorPoint( ccp(0.5, 0.5) );
	if( isCenter )
	{
		closeItem->setPosition( ccp( szParent.width/2, szParent.height/4) );
	}
	else
	{
		closeItem->setPosition( ccp( szParent.width/4*3, szParent.height/4) );
	}

	CCMenu *menu = CCMenu::create( closeItem, NULL );
	if( !menu )
	{
		CCLog( "[CMemberListPopLayer::onEnter] close menu null" );
		return;
	}
	menu->setPosition( CCPointZero );
	parent->addChild( menu, 1, E_Tag_CloseBtn );
	
	Label *text = Label::createWithSystemFont( a2u("退出"), "arial", 25 );
	CCAssert( text, "text close failed" );
	text->setPosition( ccp(szItem.width/2, szItem.height/2) );
	closeItem->addChild( text );
}

void CAskJoinGroupPopLayer::_AddTitle()
{
	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CAskJoinGroupPopLayer::_AddTitle] get parent null" );
		return;
	}
	Size sz = parent->getContentSize();

	string groupname = CUserManager::Instance().GetViewData().GetJoinGroupName();
	char title[128] = {0};
	string gpname = u2a(groupname.c_str());
	sprintf( title, "邀您加入群:%s 是否同意?", gpname.c_str() );
	Label *text = Label::createWithSystemFont( a2u(title), "arial", 25 );
	CCAssert( text, "text close failed" );
	Size textSz = text->getContentSize();
	text->setPosition( ccp(sz.width/2, sz.height-textSz.height/2-10) );
	parent->addChild( text );
}

void CAskJoinGroupPopLayer::SetView(CViewBase *view)
{
	m_scene = view;
}

CViewBase* CAskJoinGroupPopLayer::GetView()
{
	return m_scene;
}
