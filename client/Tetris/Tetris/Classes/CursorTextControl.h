#ifndef _CURSOR_TEXT_FIELD_H_
#define _CURSOR_TEXT_FIELD_H_
#include "cocos2d.h"
#include <string>
USING_NS_CC;
class CursorTextField: public CCTextFieldTTF, public CCTextFieldDelegate, public CCTouchDelegate
{
private:
    // 点击开始位置
    CCPoint m_beginPos;
    // 光标精灵
    CCSprite *m_pCursorSprite;
    // 光标动画
    CCAction *m_pCursorAction;
    // 光标坐标
    CCPoint m_cursorPos;
    // 输入框内容
    std::string *m_pInputText;
public:
    CursorTextField();
    ~CursorTextField();
    
    // static
    static CursorTextField* textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);
    
    // CCLayer
    void onEnter();
    void onExit();
    
    // 初始化光标精灵
    void initCursorSprite(int nHeight);
    
    // CCTextFieldDelegate
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF *pSender);
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);
    
    // CCLayer Touch
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    // 判断是否点击在TextField处
    bool isInTextField(CCTouch *pTouch);
    // 得到TextField矩形
    CCRect getRect();
    // 打开输入法
    void openIME();
    // 关闭输入法
	void closeIME();
	void test123();
	//*/
};

#endif