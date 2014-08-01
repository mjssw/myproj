#include "RegisterPopLayer.h"
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
	CCLog( "Text RegUser changed, old:%s new:%s", 
		CUserManager::Instance().GetBasic().GetRegUser().c_str(), 
		text.c_str() );
	CUserManager::Instance().GetBasic().SetRegUser( text );
}

static void TextPwdChanged(const std::string &text)
{
	CCLog( "Text RegPassword changed, old:%s new:%s", 
		CUserManager::Instance().GetBasic().GetRegPwd().c_str(), 
		text.c_str() );
	CUserManager::Instance().GetBasic().SetRegPwd( text );
}

void CRegisterPopLayer::onEnter()
{
	CPopLayerBase::onEnter();

	_AddEditBox();
	_AddRegisterBtn();
	_AddCloseBtn( false );
}

void CRegisterPopLayer::menuCloseCallback(Object *sender)
{
	removeFromParent();
	if( m_scene )
	{
		m_scene->UpdateView( CSceneManager::E_UpdateType_CloseRegisterDialog );
		CNetManager::Instance().CloseRegisterConn();
	}
}

void CRegisterPopLayer::menuRegisterCallback(cocos2d::Object *sender)
{
	if( CNetManager::Instance().StartRegister( LOGIN_IP, LOGIN_PORT ) )
	{
		Node *parent = getChildByTag( E_Tag_Bg );
		if( !parent )
		{
			CCLog( "[CRegisterPopLayer::menuRegisterCallback] get parent null" );
			return;
		}

		parent->removeChildByTag( E_Tag_Edit );
		parent->removeChildByTag( E_Tag_CloseBtn );
		parent->removeChildByTag( E_Tag_RegBtn );
	}
}

CRegisterPopLayer* CRegisterPopLayer::create(const char *bgimage)
{
	CRegisterPopLayer *layer = new CRegisterPopLayer();
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

void CRegisterPopLayer::_AddEditBox()
{
	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CRegisterPopLayer::onEnter] get parent null" );
		return;
	}

	CCSize bgSz = parent->getContentSize();

	CCSprite *editboxBg = CResManager::Instance().GetSpriteEditBox();
	CCAssert( editboxBg, "GetResEditBox Failed" );
	CCSize sz = editboxBg->getContentSize();
	editboxBg->setAnchorPoint( ccp(0, 0) );
	editboxBg->setPosition( 
		ccp((bgSz.width-sz.width)/2, (bgSz.height/2)) );
    parent->addChild( editboxBg, 1, E_Tag_Edit );

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

void CRegisterPopLayer::_AddRegisterBtn()
{
	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CRegisterPopLayer::onEnter] get parent null" );
		return;
	}

	CCMyMenuItemImage *regItem = CCMyMenuItemImage::create(
		"btn2_normal.png",
		"btn2_hover.png",
		this,
		menu_selector(CRegisterPopLayer::menuRegisterCallback) );
	if( !regItem )
	{
		CCLog( "[CRegisterPopLayer::onEnter] register item null" );
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
	parent->addChild( menu, 1, E_Tag_RegBtn );

	Label *text = Label::createWithSystemFont( a2u("×¢²á"), "arial", 25 );
	CCAssert( text, "text register failed" );
	text->setPosition( ccp(szItem.width/2, szItem.height/2) );
	regItem->addChild( text );
}

void CRegisterPopLayer::_AddCloseBtn(bool isCenter)
{
	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CRegisterPopLayer::onEnter] get parent null" );
		return;
	}

	CCMyMenuItemImage *closeItem = CCMyMenuItemImage::create(
		"btn2_normal.png",
		"btn2_hover.png",
		this,
		menu_selector(CRegisterPopLayer::menuCloseCallback) );
	if( !closeItem )
	{
		CCLog( "[CRegisterPopLayer::onEnter] close item null" );
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
	
	Label *text = Label::createWithSystemFont( a2u("ÍË³ö"), "arial", 25 );
	CCAssert( text, "text close failed" );
	text->setPosition( ccp(szItem.width/2, szItem.height/2) );
	closeItem->addChild( text );
}

void CRegisterPopLayer::_AddRegisterResult()
{
	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CRegisterPopLayer::_AddRegisterResult] get parent null" );
		return;
	}
	Size sz = parent->getContentSize();

	char strRet[128] = {0};
	sprintf( strRet, "×¢²á½á¹û:%d", CUserManager::Instance().GetBasic().GetRegResult() );
	Label *text = Label::createWithSystemFont( a2u(strRet), "arial", 25 );
	CCAssert( text, "text result failed" );
	text->setPosition( ccp(sz.width/2, sz.height/2) );
	parent->addChild( text );
}

void CRegisterPopLayer::SetView(CViewBase *view)
{
	m_scene = view;
}

CViewBase* CRegisterPopLayer::GetView()
{
	return m_scene;
}

void CRegisterPopLayer::Register()
{
	CRegisterClient *client = CNetManager::Instance().GetRegClientInstance();
	if( client )
	{
		client->Register(
			CUserManager::Instance().GetBasic().GetRegUser(),
			CUserManager::Instance().GetBasic().GetRegPwd() );
	}
}

void CRegisterPopLayer::RegisterResult()
{
	_AddRegisterResult();
	_AddCloseBtn( true );
}
