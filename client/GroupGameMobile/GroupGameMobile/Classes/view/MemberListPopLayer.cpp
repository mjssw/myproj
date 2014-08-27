#include "MemberListPopLayer.h"
#include "MyMenuItemImage.h"
#include "view/MyTableView.h"
#include "user/UserManager.h"
#include "user/ViewData.h"
#include "user/Group.h"
#include "user/GroupMember.h"
#include "utils.h"
#include "CommDef.h"
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
		"close_normal.png",
		"close_hover.png",
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
	closeItem->setPosition( ccp( szParent.width-szItem.width, szParent.height-szItem.height ) );

	CCMenu *menu = CCMenu::create( closeItem, NULL );
	if( !menu )
	{
		CCLog( "[CMemberListPopLayer::onEnter] close menu null" );
		return;
	}
	menu->setPosition( CCPointZero );
	parent->addChild( menu, 1 );

	_AddMemberList();
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

void CMemberListPopLayer::_AddMemberList()
{
	int bgTitleH = 75;
	int bgBorderW = 16;
	int headH = 40;

	Node *bg = getChildByTag( E_Tag_Bg );
	CCAssert( bg, "bg is null" );
	Size szBg = bg->getContentSize();
	Size szCell( szBg.width-2*bgBorderW, headH );
	Size szList( szCell.width, szBg.height-bgTitleH-bgBorderW );

	CGroup *group = CUserManager::Instance().GetViewData().GetSelectGroup();
	if( !group )
	{
		CCLog( "not found select group" );
		return;
	}

	vector<string> vecMember;
	group->Dump( vecMember );

	vector<TableViewData> vecData;
	vector<string>::iterator it = vecMember.begin();
	for( ; it != vecMember.end(); ++it )
	{
		CGroupMember *member = group->FindMember( (*it) );
		CCAssert( member, "can not believe it, member not found" );
		char icon[64] = {0};
		sprintf( icon, "syshead.png");
		TableViewData data;
		data.text = member->GetName();
		CalcUserHead( member->GetHead(), E_Sex_Male, data.icon, data.iconRect, data.useRect );
		vecData.push_back( data ); 
	}
	m_memberList = CMyTableView::create(szList, szCell, vecData, "selectbg.png" );
    CCAssert( m_memberList, "CMemberListPopLayer GetTableView Failed" );
	m_memberList->SetPosition( ccp(szBg.width/2, szList.height/2+bgBorderW) );
	bg->addChild( m_memberList );
}
