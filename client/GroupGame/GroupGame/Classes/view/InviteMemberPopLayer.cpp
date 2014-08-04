#include "InviteMemberPopLayer.h"
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

static void TextUserChanged(const std::string &text)
{
	CCLog( "Text invite user changed, old:%s new:%s", 
		CUserManager::Instance().GetViewData().GetInviteUser().c_str(),
		text.c_str() );
	CUserManager::Instance().GetViewData().SetInviteUser( text );
}

void CInviteMemberPopLayer::onEnter()
{
	CPopLayerBase::onEnter();

	_AddEditBox();
	_AddCreateBtn();
	_AddCloseBtn( false );
	_AddTitle();
}

void CInviteMemberPopLayer::menuCloseCallback(Object *sender)
{
	removeFromParent();
}

void CInviteMemberPopLayer::menuInviteCallback(cocos2d::Object *sender)
{
	string user = CUserManager::Instance().GetViewData().GetInviteUser();
	if( user == "" )
	{
		CCLog( "invite user, user is null" );
		return;
	}

	CGroupClient *client = CNetManager::Instance().GetGroupClientInstance();
	if( !client )
	{
		CCLog( "[ERROR] menuInviteCallback group client null" );
		return;
	}

	CGroup *group = CUserManager::Instance().GetViewData().GetSelectGroup();
	if( !group )
	{
		CCLog( "[ERROR] menuInviteCallback cur group is null" );
		return;
	}

	client->InviteMember( group->GetId(), user );
	menuCloseCallback( sender );
}

CInviteMemberPopLayer* CInviteMemberPopLayer::create(const char *bgimage)
{
	CInviteMemberPopLayer *layer = new CInviteMemberPopLayer();
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

void CInviteMemberPopLayer::_AddEditBox()
{
	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CInviteMemberPopLayer::onEnter] get parent null" );
		return;
	}

	CCSize bgSz = parent->getContentSize();

	CCSprite *editboxBg = CResManager::Instance().GetSpriteEditBox();
	CCAssert( editboxBg, "GetResEditBox Failed" );
	CCSize sz = editboxBg->getContentSize();
	editboxBg->setAnchorPoint( ccp(0, 0) );
	editboxBg->setPosition( 
		ccp((bgSz.width-sz.width)/2, (bgSz.height/2)-30) );
    parent->addChild( editboxBg, 1, E_Tag_Edit );

	int x = 10;
	CCSprite *wordUser = CResManager::Instance().GetSpriteWordUser();
	CCAssert( wordUser, "GetResWordUser Failed" );
	
	Label *text = Label::createWithSystemFont( a2u("帐号:"), "arial", 25 );
	CCAssert( text, "text group name failed" );
	CCSize wsz = text->getContentSize();
	int userY = (sz.height/2-wsz.height)/2 + (sz.height/2);
	text->setColor( Color3B::BLACK );
	text->setAnchorPoint( ccp(0, 0) );
	text->setPosition( ccp(x, userY) );
	editboxBg->addChild( text, 3 );

	CCSize editSz = CCSize( sz.width - 3*x - wsz.width, sz.height/3);
	CMyEditBox *editUser = CMyEditBox::create(
		editSz, ccp(2*x+wsz.width, (sz.height/2-editSz.height)/2 + (sz.height/2)), 
		"white_edit.png", "Paint Boy.ttf", 25, ccBLACK, 8 );
	CCAssert( editUser, "GetUserEditBox Failed" );
	editUser->SetEditChangedCallback( TextUserChanged );
    editboxBg->addChild( editUser, 1 );
}

void CInviteMemberPopLayer::_AddCreateBtn()
{
	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CInviteMemberPopLayer::onEnter] get parent null" );
		return;
	}

	CCMyMenuItemImage *regItem = CCMyMenuItemImage::create(
		"btn2_normal.png",
		"btn2_hover.png",
		this,
		menu_selector(CInviteMemberPopLayer::menuInviteCallback) );
	if( !regItem )
	{
		CCLog( "[CInviteMemberPopLayer::onEnter] register item null" );
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

void CInviteMemberPopLayer::_AddCloseBtn(bool isCenter)
{
	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CInviteMemberPopLayer::onEnter] get parent null" );
		return;
	}

	CCMyMenuItemImage *closeItem = CCMyMenuItemImage::create(
		"btn2_normal.png",
		"btn2_hover.png",
		this,
		menu_selector(CInviteMemberPopLayer::menuCloseCallback) );
	if( !closeItem )
	{
		CCLog( "[CInviteMemberPopLayer::onEnter] close item null" );
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

void CInviteMemberPopLayer::_AddTitle()
{
	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CInviteMemberPopLayer::_AddTitle] get parent null" );
		return;
	}
	Size sz = parent->getContentSize();

	CGroup *group = CUserManager::Instance().GetViewData().GetSelectGroup();
	CCAssert( group, "CInviteMemberPopLayer::_AddTitle cur group null" );

	char title[128] = {0};
	string gpname = u2a(group->GetName().c_str());
	sprintf( title, "邀请朋友加入群:%s", gpname.c_str() );
	Label *text = Label::createWithSystemFont( a2u(title), "arial", 25 );
	CCAssert( text, "text close failed" );
	Size textSz = text->getContentSize();
	text->setPosition( ccp(sz.width/2, sz.height-textSz.height/2-10) );
	parent->addChild( text );
}

void CInviteMemberPopLayer::SetView(CViewBase *view)
{
	m_scene = view;
}

CViewBase* CInviteMemberPopLayer::GetView()
{
	return m_scene;
}
