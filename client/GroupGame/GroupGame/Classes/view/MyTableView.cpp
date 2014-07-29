#include "MyTableView.h"
#include "utils.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

void _DrawBox(const CCPoint &pos, const CCSize &size, bool isCenter)
{
	if( !isCenter )
	{
		CCPoint vertices[4]={
			ccp(pos.x, pos.y),
			ccp(pos.x+size.width, pos.y),
			ccp(pos.x+size.width, pos.y+size.height),
			ccp(pos.x, pos.y+size.height),
		};
		ccDrawColor4B(255, 0, 0, 255);
		ccDrawPoly(vertices, 4, true);
	}
	else
	{
		CCPoint vertices[4]={
			ccp(pos.x-size.width/2, pos.y-size.height/2),
			ccp(pos.x+size.width/2, pos.y-size.height/2),
			ccp(pos.x+size.width/2, pos.y+size.height/2),
			ccp(pos.x-size.width/2, pos.y+size.height/2),
		};
		ccDrawColor4B(255, 0, 0, 255);
		ccDrawPoly(vertices, 4, true);
	}
}
class CTableView1 : public TableView
{
public:

	static CTableView1* create(TableViewDataSource* dataSource, CCSize size, CCNode *container)
	{
		CTableView1 *table = new CTableView1();
		table->initWithViewSize(size, container);
		table->autorelease();
		table->setDataSource(dataSource);
		table->_updateCellPositions();
		table->_updateContentSize();

		return table;
	}

	/*
	virtual void draw()
	{
		TableView::draw();
		CCSize sz = getViewSize();
		//_DrawBox(CCPoint(0,0), sz, false);
	}
	//*/
};

CMyTableView::CMyTableView() : 
	m_tableView(NULL), 
	m_lastTouchCell(NULL),
	m_lastTouchIdx(-1),
	m_cellSz(0, 0), 
	m_cellCount(0),
	m_selectBg("")
{
	bool ret = init();
	CCAssert( ret, "CMyTableView CCNode::init Error" );
	autorelease();
	setContentSize( CCSize(10, 10) );
	setAnchorPoint( ccp(0.5, 0.5) );
}

CMyTableView::~CMyTableView()
{
}

void CMyTableView::SetAnchorPoint(CCPoint &pt)
{
	setAnchorPoint( pt );
}

void CMyTableView::SetPosition(CCPoint &pt)
{
	setPosition( pt );
}

CCSize CMyTableView::GetSize()
{
	if( m_tableView )
	{
		return m_tableView->getViewSize();
	}
	return CCSize(0, 0);
}

bool CMyTableView::InsertElement(TableViewData &data)
{
	if( !m_tableView )
	{
		return false;
	}

	m_viewData.push_back( data );
	++m_cellCount;

	CCPoint cur = ccp(m_scrollOffset.x,  m_scrollOffset.y - m_cellSz.height); 
	m_tableView->reloadData();
	m_tableView->setContentOffset( cur );
	return true;
}

void CMyTableView::UpdateElements(vector<TableViewData> &data)
{
	if( !m_tableView )
	{
		return;
	}

	m_viewData = data;
	m_cellCount = (int)data.size();

	CCPoint cur = ccp(m_scrollOffset.x,  m_scrollOffset.y - m_cellSz.height); 
	m_tableView->reloadData();
	m_tableView->setContentOffset( cur );
}

void CMyTableView::RemoveElementAtIndex(int idx)
{
	if( !m_tableView || m_viewData.size()==0 || idx<0 )
	{
		return;
	}

	vector<TableViewData>::iterator it = m_viewData.begin() + idx;
	m_viewData.erase( it );
	--m_cellCount;

	m_tableView->reloadData();
}

/*
void CMyTableView::draw()
{
	CCNode::draw();
	
	// draw bounding box
 	//CCPoint pos = getPosition();
	//CCSize size = CCSizeMake(100, 100);
	//CCSize size = getContentSize();
	//_DrawBox(CCPoint(0,0), size, false);
}
//*/

CMyTableView* CMyTableView::create(CCSize &sz, CCSize &cellSz, vector<TableViewData> &viewData, const char *selectBgImage)
{
	if( selectBgImage == NULL )
	{
		return NULL;
	}

	CMyTableView *view = new CMyTableView();
	if( !view )
	{
		return NULL;
	}

	view->m_cellSz = cellSz;
	view->m_cellCount = viewData.size();
	view->m_viewData = viewData;

	view->m_selectBg = selectBgImage;
	
	CCSize nodeSz = view->getContentSize();
	//view->m_tableView = CCTableView::create( view, sz );
	view->m_tableView = CTableView1::create( view, sz, NULL );
	view->m_tableView->setDirection( ScrollView::Direction::VERTICAL );
	view->m_tableView->setPosition( ccp((nodeSz.width - sz.width)/2, (nodeSz.height - sz.height)/2) );
	view->m_tableView->setDelegate( view );
	view->m_tableView->setVerticalFillOrder( TableView::VerticalFillOrder::TOP_DOWN );
	view->addChild( view->m_tableView, 1 );
	view->m_tableView->reloadData();

	return view;
}

void CMyTableView::scrollViewDidScroll(ScrollView *view)
{
	m_scrollOffset = view->getContentOffset();
	//CCLog( "CMyTableView::scrollViewDidScroll off(%d, %d)", (int)m_scrollOffset.x, (int)m_scrollOffset.y  );
}

void CMyTableView::scrollViewDidZoom(ScrollView *view)
{
	CCLog( "CMyTableView::scrollViewDidZoom" );
}

void CMyTableView::tableCellTouched(TableView *table, TableViewCell *cell)
{
	if( cell->getIdx() == m_lastTouchIdx )
	{
		return;
	}

	if( m_lastTouchCell )
	{
		m_lastTouchCell->removeChildByTag( E_Tag_SelectBg );
	}

	_AddSelectBg( cell );

	m_lastTouchIdx = cell->getIdx();
	m_lastTouchCell = cell;

	CCLog( "tableCellTouched %d", cell->getIdx() );
}

Size CMyTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return m_cellSz;
}

TableViewCell* CMyTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	const int tagLabel = 666;
	const int tagSprite = 777;
	bool hasData = true;
    TableViewCell *pCell = table->dequeueCell();
    if( !pCell )
	{
        pCell = new TableViewCell();
        pCell->autorelease();
		hasData = false;
	}
	else
	{
		pCell->removeChildByTag( tagSprite );
		if( idx != m_lastTouchIdx && pCell->getChildByTag(E_Tag_SelectBg) )
		{
			pCell->removeChildByTag( E_Tag_SelectBg );
		}
	}

	if( idx >= m_viewData.size() )
	{
		CCLog( "[CMyTableView::tableCellAtIndex]Error. idx(%d) greater than data.Size(%d)", idx, m_viewData.size() );
		return NULL;
	}

	int off = 5;
	int x = off;
	CCSprite *pSprite = CCSprite::create( m_viewData[idx].icon.c_str() );
	if( pSprite )
	{
		CCSize sz = pSprite->getContentSize();
		float scale = m_cellSz.height / sz.height;
		sz = sz * scale;
		pSprite->setScale( scale );
		pSprite->setPosition(
			ccp( sz.width/2 + off, m_cellSz.height/2) );
		pCell->addChild( pSprite, 1, tagSprite );
		x += (pSprite->getContentSize().width * scale) + off;
	}
	else
	{
		CCLog( "[CMyTableView::tableCellAtIndex]Error. can not load sprite:%s", m_viewData[idx].icon.c_str() );
	}

	CCLabelTTF *pLabel = NULL;
	if( hasData )
	{
		pLabel = (CCLabelTTF*)pCell->getChildByTag( tagLabel );
		CCAssert( pLabel, "get label child error" ); 
		pLabel->setString( m_viewData[idx].text.c_str() );
	}
	else
	{
		pLabel = CCLabelTTF::create( m_viewData[idx].text.c_str(), "Arial", 20.0 );
		pCell->addChild( pLabel, 1 );
	}

	if( pLabel )
	{
		pLabel->setAnchorPoint( ccp(0.5, 0.5) );
		pLabel->setPosition( ccp(x + pLabel->getContentSize().width/2, m_cellSz.height/2) );
		pLabel->setTag( tagLabel );
		pLabel->setColor( ccBLACK );
	}

	// 判断是否要重新加上选中的背景
	if( idx == m_lastTouchIdx && !pCell->getChildByTag(E_Tag_SelectBg) )
	{
		_AddSelectBg( pCell );
		m_lastTouchCell = pCell;
	}

    return pCell;
}

ssize_t CMyTableView::numberOfCellsInTableView(TableView *table)
{
	return m_cellCount;
}

void CMyTableView::_AddSelectBg(TableViewCell *cell)
{
	CCAssert( cell, "_AddSelectBg param is NULL" );
	CCSprite *selectBg = CCSprite::create( m_selectBg.c_str() );
	if( !selectBg )
	{
		CCLog( "[CMyTableView::_AddSelectBg] create select bg Error " );
		return;
	}
	selectBg->setAnchorPoint( ccp(0.5, 0.5) );
	selectBg->setPosition( ccp(m_cellSz.width/2, m_cellSz.height/2) );
	cell->addChild( selectBg, 0, E_Tag_SelectBg );
}
