#ifndef _MY_RADIO_BUTTON_H_
#define _MY_RADIO_BUTTON_H_

#include "cocos2d.h"  
  
class CMyRadioMenu : public cocos2d::CCMenu  
{  
public:  
    CMyRadioMenu() : m_lastSelectedItem(NULL)
	{}
    virtual ~CMyRadioMenu(){}

    virtual void onEnter();
    virtual bool onTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void onTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	//virtual void draw();

	void SetPosition(cocos2d::CCPoint &pos);
	void SetDefaultSelectItem(cocos2d::CCMenuItem *item);
    void SetItemsVertically(int padding);
	void SetItemsHorizontally(int padding);
	int GetMenuWidth();
	int GetMenuHeight();
  
    static CMyRadioMenu* create(float scale, cocos2d::CCMenuItem *item, ...);  
    
protected:  
    cocos2d::CCMenuItem *m_lastSelectedItem;
	bool m_isVertically;
	int m_itemCount;
	int m_padding;
	cocos2d::CCSize m_itemSz;
};  

#endif
