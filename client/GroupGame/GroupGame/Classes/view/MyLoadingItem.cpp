#include "MyLoadingItem.h"
USING_NS_CC;

CMyLoadingItem::CMyLoadingItem() : 
	m_curStep( 0 ),
	m_maxStep( 0 ),
	m_stepHight( 0 ),
	m_stepWidth( 0 )
{
	bool ret = init();
	CCAssert( ret, "CMyLoadingItem CCNode::init Error" );
	autorelease();
	setContentSize( CCSize(10, 10) );
}

CMyLoadingItem::~CMyLoadingItem()
{
}

void CMyLoadingItem::SetAnchorPoint(cocos2d::CCPoint &pt)
{
	setAnchorPoint( pt );
}

void CMyLoadingItem::SetPosition(cocos2d::Vec2 pt)
{
	setPosition( pt );
}

bool CMyLoadingItem::UpdateProgress()
{
	if( m_curStep >= m_maxStep )
	{
		return false;
	}

	// 获取尾部进度节点
	CCNode *loadtail = getChildByTag( E_Tag_Loadtail );
	if( !loadtail )
	{
		CCLog( "[CMyLoadingIte] mgetchild E_Tag_Loadtail Error" );
		return false;
	}
	CCPoint pos = loadtail->getPosition();
	
	// 添加中间进度
	CCSprite *loadmid = CCSprite::create( 
		"loadmid.png", 
		CCRect(0, 0, m_stepWidth, m_stepHight) );
	if( !loadmid )
	{
		CCLog( "[CMyLoadingIte] create loadmid png Error" );
		return false;
	}
	CCSize midSz = loadmid->getContentSize();
	int x = pos.x - loadtail->getContentSize().width / 2 + m_stepWidth / 2;
	int y = pos.y;
	loadmid->setPosition( ccp(x, y) );
	addChild( loadmid, 0 );
	
	// 调整尾部进步位置
	pos.x += m_stepWidth;
	loadtail->setPosition( pos );

	++m_curStep;
	return true;
}

bool CMyLoadingItem::IsLoadingEnd()
{
	return ( m_curStep >= m_maxStep );
}

CMyLoadingItem* CMyLoadingItem::create(int steps)
{
	CMyLoadingItem *item = new CMyLoadingItem();
	if( !item )
	{
		return NULL;
	}
	
	CCSize nodeSz = item->getContentSize();
	
	CCSprite *loadbg = CCSprite::create( "loadbg.png" );
	if( !loadbg )
	{
		return NULL;
	}

	CCSize bgSz = loadbg->getContentSize();
	int x = nodeSz.width / 2;
	int y = nodeSz.height / 2;
	loadbg->setPosition( ccp(x, y) );
	item->addChild( loadbg, 0 );

	CCSprite *loadhead = CCSprite::create( "loadhead.png" );
	if( !loadhead )
	{
		return NULL;
	}
	CCSize headSz = loadhead->getContentSize();
	
	CCSprite *loadtail = CCSprite::create( "loadtail.png" );
	if( !loadtail )
	{
		return NULL;
	}
	CCSize tailSz = loadtail->getContentSize();
	
	int off = 10;
	int remainW = bgSz.width - headSz.width - tailSz.width - 2*off;
	int stepW = remainW / steps;
	item->m_stepWidth = (stepW % 2) ? (stepW+1) : stepW; // 保证是偶数
	item->m_stepHight = headSz.height;
	
	x = nodeSz.width / 2 - bgSz.width / 2 + off + headSz.width / 2;
	y = nodeSz.height / 2;
	loadhead->setPosition( ccp(x, y) );
	item->addChild( loadhead, 0 );

	x += headSz.width / 2 + tailSz.width / 2;
	loadtail->setPosition( ccp(x, y) );
	item->addChild( loadtail, 0, E_Tag_Loadtail );

	item->m_maxStep = steps;

	return item;
}
