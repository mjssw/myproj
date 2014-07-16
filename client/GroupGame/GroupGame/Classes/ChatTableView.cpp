#include "ChatTableView.h"
#include "utils.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

CChatTableView::CChatTableView() : 
	m_tableView(NULL),
	m_viewData(NULL),
	m_scrollOffset(0, 0)
{
	bool ret = init();
	CCAssert( ret, "CChatTableView CCNode::init Error" );
	autorelease();
	setContentSize( CCSize(10, 10) );
	setAnchorPoint( ccp(0.5, 0.5) );
}

CChatTableView::~CChatTableView()
{
}

void CChatTableView::InitData(std::vector<ChatTableViewData> &data)
{
	if( !m_tableView )
	{
		CCLog( "[CChatTableView::Init] tableview NULL" );
		return;
	}
	m_viewData = &data;
	m_tableView->reloadData();
}

void CChatTableView::SetAnchorPoint(CCPoint &pt)
{
	setAnchorPoint( pt );
}

void CChatTableView::SetPosition(CCPoint &pt)
{
	setPosition( pt );
}

CCSize CChatTableView::GetSize()
{
	if( m_tableView )
	{
		return m_tableView->getViewSize();
	}
	return CCSize(0, 0);
}

bool CChatTableView::InsertUpdate(int elemH)
{
	if( !m_tableView )
	{
		return false;
	}

	CCPoint cur = ccp(m_scrollOffset.x,  m_scrollOffset.y - elemH); 
	m_tableView->reloadData();
	m_tableView->setContentOffset( cur );
	return true;
}

void CChatTableView::RemoveUpdate(int elemH)
{
	if( !m_tableView )
	{
		return;
	}

	CCPoint cur = ccp(m_scrollOffset.x,  m_scrollOffset.y - elemH); 

	m_tableView->reloadData();
	m_tableView->setContentOffset( cur );
}

CChatTableView* CChatTableView::create(CCSize &sz)
{
	CChatTableView *view = new CChatTableView();
	if( !view )
	{
		return NULL;
	}

	view->m_viewData = NULL;
	
	CCSize nodeSz = view->getContentSize();
	view->m_tableView = TableView::create( view, sz, NULL );
	view->m_tableView->setDirection( ScrollView::Direction::VERTICAL );
	view->m_tableView->setPosition( ccp((nodeSz.width - sz.width)/2, (nodeSz.height - sz.height)/2) );
	view->m_tableView->setDelegate( view );
	view->m_tableView->setVerticalFillOrder( TableView::VerticalFillOrder::TOP_DOWN );
	view->addChild( view->m_tableView, 1 );
	view->m_tableView->reloadData();

	return view;
}

void CChatTableView::scrollViewDidScroll(ScrollView *view)
{
	m_scrollOffset = view->getContentOffset();
	//CCLog( "CChatTableView::scrollViewDidScroll off(%d, %d)", (int)m_scrollOffset.x, (int)m_scrollOffset.y  );
}

void CChatTableView::scrollViewDidZoom(ScrollView *view)
{
	CCLog( "CChatTableView::scrollViewDidZoom" );
}

void CChatTableView::tableCellTouched(TableView *table, TableViewCell *cell)
{
	CCLog( "CChatTableView::tableCellTouched %d", cell->getIdx() );
}

Size CChatTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	if( !m_viewData || m_viewData->size() <= idx )
	{
		CCLog( "[CChatTableView::tableCellSizeForIndex] Error" );
		return Size(0,0);
	}
	return Size( m_viewData->at(idx).width(), m_viewData->at(idx).height() );
}

TableViewCell* CChatTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	const int tagLabel = 666;
    TableViewCell *pCell = table->dequeueCell();
    if( !pCell )
	{
        pCell = new TableViewCell();
        pCell->autorelease();
	}
	else
	{
		pCell->removeChildByTag( tagLabel );
	}

	if( idx >= m_viewData->size() )
	{
		CCLog( "[CChatTableView::tableCellAtIndex]Error. idx(%d) greater than data.Size(%d)", idx, m_viewData->size() );
		return NULL;
	}

	int off = 5;
	int x = off;

	ChatTableViewData &data = m_viewData->at(idx);
	string text = (data.type == ChatTableViewData::E_DataType_User) ?
		(data.user + "  " + data.content) : ( "      " + data.content );
	CCLabelTTF *pLabel = CCLabelTTF::create( a2u(text.c_str()), "Arial", 12.0 );
	if( !pLabel )
	{
		return NULL;
	}
	Size labelSz = pLabel->getContentSize();

	pLabel->setAnchorPoint( ccp(0.5, 0.5) );
	pLabel->setPosition( ccp(labelSz.width/2 + 10, labelSz.height/2) );
	pLabel->setTag( tagLabel );
	pLabel->setColor( ccBLACK );
	pCell->addChild( pLabel );

    return pCell;
}

ssize_t CChatTableView::numberOfCellsInTableView(TableView *table)
{
	return m_viewData ? m_viewData->size() : 0;
}
