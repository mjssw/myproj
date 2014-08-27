#include "CreateGamePopLayer.h"
#include "MyMenuItemImage.h"
#include "view/MyTableView.h"
#include "user/UserManager.h"
#include "user/ViewData.h"
#include "user/Group.h"
#include "user/GroupMember.h"
#include "net/NetManager.h"
#include "net/GroupClient.h"
#include "utils.h"
#include "CommDef.h"
using namespace cocos2d;
using namespace std;

void CCreateGamePopLayer::onEnter()
{
	CPopLayerBase::onEnter();

	_AddCreateBtn();
	_AddCloseBtn( false );
	_AddGameList();
}

void CCreateGamePopLayer::menuCloseCallback(Object *sender)
{
	removeFromParent();
}

void CCreateGamePopLayer::menuCreateCallback(cocos2d::Object *sender)
{
	// Create game
	CGameInfo *game = (CGameInfo*)m_gameList->GetTochCellData();
	CGroup *group = CUserManager::Instance().GetViewData().GetSelectGroup();
	CGroupClient *client = CNetManager::Instance().GetGroupClientInstance();
	if( game || group || client )
	{
		CCLog( "[NOTE] send msg to create game:%d in group:%llu", game->m_gameid, group->GetId() );
		client->CreateGameRoom( group->GetId(), game->m_gameid );
	}
	removeFromParent();
}

CCreateGamePopLayer* CCreateGamePopLayer::create(const char *bgimage)
{
	CCreateGamePopLayer* layer = new CCreateGamePopLayer();
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

void CCreateGamePopLayer::_AddCreateBtn()
{
	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CCreateGroupPopLayer::onEnter] get parent null" );
		return;
	}

	CCMyMenuItemImage *regItem = CCMyMenuItemImage::create(
		"btn2_normal.png",
		"btn2_hover.png",
		this,
		menu_selector(CCreateGamePopLayer::menuCreateCallback) );
	if( !regItem )
	{
		CCLog( "[CCreateGroupPopLayer::onEnter] register item null" );
		return;
	}
	Size szItem = regItem->getContentSize();
	Size szParent = parent->getContentSize();
	regItem->setAnchorPoint( ccp(0.5, 0.5) );
	regItem->setPosition( ccp( szParent.width/2, szItem.height/2) );

	CCMenu *menu = CCMenu::create( regItem, NULL );
	if( !menu )
	{
		CCLog( "[CMemberListPopLayer::onEnter] register menu null" );
		return;
	}
	menu->setPosition( CCPointZero );
	parent->addChild( menu, 1, E_Tag_OkBtn );

	Label *text = Label::createWithSystemFont( a2u("È·¶¨").c_str(), "arial", 25 );
	CCAssert( text, "text register failed" );
	text->setPosition( ccp(szItem.width/2, szItem.height/2) );
	regItem->addChild( text );
}

void CCreateGamePopLayer::_AddCloseBtn(bool isCenter)
{
	Node *parent = getChildByTag( E_Tag_Bg );
	if( !parent )
	{
		CCLog( "[CCreateGamePopLayer::onEnter] get parent null" );
		return;
	}
	CCMyMenuItemImage *closeItem = CCMyMenuItemImage::create(
		"close_normal.png",
		"close_hover.png",
		this,
		menu_selector(CCreateGamePopLayer::menuCloseCallback) );
	if( !closeItem )
	{
		CCLog( "[CCreateGamePopLayer::onEnter] close item null" );
		return;
	}
	Size szItem = closeItem->getContentSize();
	Size szParent = parent->getContentSize();
	closeItem->setAnchorPoint( ccp(0.5, 0.5) );
	closeItem->setPosition( ccp( szParent.width-szItem.width, szParent.height-szItem.height ) );

	CCMenu *menu = CCMenu::create( closeItem, NULL );
	if( !menu )
	{
		CCLog( "[CCreateGamePopLayer::onEnter] close menu null" );
		return;
	}
	menu->setPosition( CCPointZero );
	parent->addChild( menu, 1 );
}

void CCreateGamePopLayer::_AddGameList()
{
	int bgTitleH = 75;
	int bgBorderW = 16;
	int headH = 40;

	Node *bg = getChildByTag( E_Tag_Bg );
	CCAssert( bg, "bg is null" );
	Size szBg = bg->getContentSize();
	Size szCell( szBg.width-2*bgBorderW, headH );
	Size szList( szCell.width, szBg.height-bgTitleH-bgBorderW );

	vector<TableViewData> vecData;

	vector<s32> vecgameid;
	CUserManager::Instance().GetGameList().Dump( vecgameid );

	vector<s32>::iterator it = vecgameid.begin();
	for( ; it != vecgameid.end(); ++it )
	{
		int gameid = *it;
		string dir,name,icon;
		CUserManager::Instance().GetGameInfo(gameid, dir, icon, name);
		TableViewData data;
		data.icon = dir + "/" + icon;
		data.text = name;
		data.data = CUserManager::Instance().GetGameList().GetGameInfo( gameid );
		vecData.push_back( data ); 
	}
	m_gameList = CMyTableView::create(szList, szCell, vecData, "selectbg.png" );
    CCAssert( m_gameList, "CCreateGamePopLayer GetTableView Failed" );
	m_gameList->SetPosition( ccp(szBg.width/2, szList.height/2+bgBorderW) );
	bg->addChild( m_gameList );
}
