#ifndef _MY_EDITBOX_H_
#define _MY_EDITBOX_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

typedef void (*TextChangedHandler)(const std::string &text);

class CMyEditBox : public cocos2d::CCLayer, public cocos2d::extension::CCEditBoxDelegate
{
public:
	CMyEditBox();
    virtual ~CMyEditBox();
    
    virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxTextChanged(cocos2d::extension::CCEditBox *editBox, const std::string &text);
    virtual void editBoxReturn(cocos2d::extension::CCEditBox *editBox);

    static CMyEditBox* create(cocos2d::CCSize &sz, cocos2d::CCPoint &pos, const char *bgImage, const char *font, int fontSize, const cocos2d::ccColor3B &color, int maxLen);

	cocos2d::extension::CCEditBox* GetEditBox();
	void SetAnchorPoint(cocos2d::CCPoint &pt);
	void SetPasswordMode();
	void SetEditChangedCallback(TextChangedHandler callback);

private:
    cocos2d::extension::CCEditBox *m_pEdit;
    TextChangedHandler m_textChangedCallback;
};

#endif
