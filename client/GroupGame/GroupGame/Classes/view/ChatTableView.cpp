#include "ChatTableView.h"
#include "utils.h"
#include "CommDef.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define CHAT_CONTENT_CELL_OFF 20

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

bool CChatTableView::InsertUpdate()
{
	if( !m_tableView )
	{
		return false;
	}

	// 调整最后一行的文字, 正常情况下最新加入的必然是聊天的内容
	int idx = m_viewData->size() - 1;
	if( idx < 0 )
	{
		return false;
	}
	_AdjustChatContent( m_viewData->at(idx) );

	m_tableView->reloadData();
	if( m_scrollOffset.y < 0 )
	{
		m_tableView->setContentOffset( ccp(0, 0) );
	}
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

	int off = 10; // 右侧留边
	view->m_chineseSize = chineseSizeWithFont( CHAT_FONT_NAME, CHAT_FONT_SIZE );
	view->m_asciiSize = asciiSizeWithFont( CHAT_FONT_NAME, CHAT_FONT_SIZE );
	view->m_userCellWidth = sz.width - off;
	view->m_chatCellWidth = sz.width - CHAT_CONTENT_CELL_OFF - off;
	view->m_lineHeight= view->m_chineseSize.height > view->m_asciiSize.height ?  
		view->m_chineseSize.height : view->m_asciiSize.height; 

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

	ChatTableViewData &data = m_viewData->at( idx );
	int w = (data.type==ChatTableViewData::E_DataType_User) ?
		m_userCellWidth : m_chatCellWidth;
	return Size( w, data.line * m_lineHeight );
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

	ChatTableViewData &data = m_viewData->at(idx);
	string text;
	int off = 0;
	Color3B color;
	if( data.type == ChatTableViewData::E_DataType_User)
	{
		off = 0;
		color = Color3B::BLUE;
		text = (data.user + "  " + data.content);
	}
	else
	{
		off = CHAT_CONTENT_CELL_OFF;
		color = Color3B::BLACK;
		text = data.content;
	}
	Label *pLabel = Label::createWithSystemFont( text.c_str(), CHAT_FONT_NAME, CHAT_FONT_SIZE );
	if( !pLabel )
	{
		return NULL;
	}
	Size labelSz = pLabel->getContentSize();

	int h = data.line * m_lineHeight;
	pLabel->setAnchorPoint( ccp(0.5, 0.5) );
	pLabel->setPosition( ccp(labelSz.width/2 + 10 + off, h/2) );
	pLabel->setTag( tagLabel );
	pLabel->setColor( color );
	pCell->addChild( pLabel );

    return pCell;
}

ssize_t CChatTableView::numberOfCellsInTableView(TableView *table)
{
	return m_viewData ? m_viewData->size() : 0;
}

void CChatTableView::_AdjustChatContent(ChatTableViewData &data)
{
	if( data.type == ChatTableViewData::E_DataType_User )
	{
		// 发言者的当前行不需要调整(必然不超过1行)
		return;
	}

	int maxW = m_chatCellWidth;
	int curW = 0;
	int line = 1;
	string _u8str = data.content;
	int len = _u8str.length();
	string ret;
    int i = 0;
    while( i < len )
	{
		if( ~(_u8str.at(i) >> 8) == 0 )
		{
			if( curW + m_chineseSize.width > maxW )
			{
				ret.append( "\n" );
				curW = 0;
				++line;
			}

			ret.append( _u8str.substr(i, 3) );
			i = i + 3;
			curW += m_chineseSize.width; 
		}
		else
		{
			if( curW + m_asciiSize.width > maxW )
			{
				ret.append( "\n" );
				curW = 0;
				++line;
			}

			ret.append( _u8str.substr(i, 1) );
			i = i + 1;
			curW += m_asciiSize.width; 
		}
	}

	data.content = ret;
	data.line = line;
}
