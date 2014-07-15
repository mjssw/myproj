#include "MyRadioButton.h"
using namespace cocos2d;  

CMyRadioMenu* CMyRadioMenu::create(float scale, CCMenuItem *item, ...)  
{  
    CMyRadioMenu *menu = new CMyRadioMenu();
	if( !menu || !menu->init() )
	{
		return NULL;
	}
	menu->m_itemCount = 0;
	menu->setAnchorPoint( ccp(0.5, 0.5) );
    menu->autorelease();

	if( !item )
	{
		return menu;
	}

	menu->addChild( item );
	++menu->m_itemCount;

    va_list args;  
    va_start(args, item);  

	CCMenuItem *elem = va_arg(args, CCMenuItem*);  
	while( elem )  
	{  
		menu->addChild( elem );
		++menu->m_itemCount;
		elem = va_arg(args, CCMenuItem*);  
	}  
  
    va_end(args);  
 
	//menu->setScale( scale );
	menu->m_padding = 0;
	menu->SetItemsHorizontally( menu->m_padding );
	menu->m_isVertically = false;
	menu->m_itemSz = item->getContentSize();
    return menu;  
}  
  
void CMyRadioMenu::SetPosition(CCPoint &pos)
{
	setPosition( pos );
}

void CMyRadioMenu::SetDefaultSelectItem(CCMenuItem *item)
{
	if( item )
	{
		m_lastSelectedItem = item;
		m_lastSelectedItem->selected();
	}
}

void CMyRadioMenu::SetItemsVertically(int padding)
{
	m_padding = padding;
	alignItemsVerticallyWithPadding( padding );
}

void CMyRadioMenu::SetItemsHorizontally(int padding)
{
	m_padding = padding;
	alignItemsHorizontallyWithPadding( padding );
}

int CMyRadioMenu::GetMenuWidth()
{
	if( m_isVertically )
	{
		return m_itemSz.width;
	}
	else
	{
		return (m_itemSz.width * m_itemCount ) + ( (m_itemCount-1) * m_padding );
	}
}

int CMyRadioMenu::GetMenuHeight()
{
	if( m_isVertically )
	{
		return (m_itemSz.height * m_itemCount ) + ( (m_itemCount-1) * m_padding );
	}
	else
	{
		return m_itemSz.height;
	}
}

void CMyRadioMenu::onEnter()  
{  
	CCMenu::onEnter();  
	if( _children.size() > 0 )
	{  
		if( !m_lastSelectedItem )
		{  
			CCMenuItem *pFirstItem = (CCMenuItem*)( _children.at(0) );
			pFirstItem->selected();
			m_lastSelectedItem = pFirstItem;
		}
	}
}
  
bool CMyRadioMenu::onTouchBegan(CCTouch* touch, CCEvent* event)  
{  
    CC_UNUSED_PARAM(event);  
	if( _state != kCCMenuStateWaiting || !_enabled /*|| !m_bEnabled*/ )  
    {  
        return false;  
    }  
  
    for(CCNode *c = this->_parent; c != NULL; c = c->getParent() )  
    {  
        if( c->isVisible() == false )  
        {  
            return false;  
        }  
    }  
  
	_selectedItem = this->getItemForTouch( touch );  
    if( _selectedItem )  
    {  
        _state = kCCMenuStateTrackingTouch;  
        _selectedItem->selected();  
        // 上一个被选中的改为未被选中  
        if( m_lastSelectedItem && m_lastSelectedItem != _selectedItem )  
        {  
            m_lastSelectedItem->unselected();  
        }  
        return true;  
    }  

    return false;  
}  
  
void CMyRadioMenu::onTouchEnded(CCTouch* touch, CCEvent* event)  
{  
    CC_UNUSED_PARAM(touch);  
    CC_UNUSED_PARAM(event);  
    CCAssert(_state == kCCMenuStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");  
    if( _selectedItem )  
    {  
        // 这里注释掉触摸松开后还是处于被选中状态  
        //m_pSelectedItem->unselected();  
        _selectedItem->activate();  
        m_lastSelectedItem = _selectedItem;  
    }  
    _state = kCCMenuStateWaiting;  
}  

/*
void CMyRadioMenu::draw()
{
	CCNode::draw();

	//extern void _DrawBox(const CCPoint &pos, const CCSize &size, bool isCenter);
	//_DrawBox(CCPoint(0,0), getContentSize(), false);
}
//*/
