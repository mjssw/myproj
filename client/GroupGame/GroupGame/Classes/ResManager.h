#ifndef _RES_MANAGER_H_
#define _RES_MANAGER_H_

#include "cocos2d.h"
#include "Define.h"

class CResManager
{
	SINGLETON_CLASS_DEFINE(CResManager);

public:
	bool Init(const char *resRoot);

	cocos2d::CCSprite* GetSpriteBg(); 
	cocos2d::CCSprite* GetSpriteLoginBg(); 
	cocos2d::CCSprite* GetSpriteEditBox(); 
	cocos2d::CCSprite* GetSpriteWordUser();
	cocos2d::CCSprite* GetSpriteWordPwd();
	cocos2d::CCSprite* GetSpriteCheckBoxNormal();
	cocos2d::CCSprite* GetSpriteCheckBoxSelect();
	cocos2d::CCSprite* GetSpriteWordSavePwd();
	cocos2d::CCSprite* GetSpriteWordAutoLogin();
	cocos2d::CCSprite* GetSpriteBtnNormal();
	cocos2d::CCSprite* GetSpriteBtnHover();
	cocos2d::CCSprite* GetSpriteWordLogin();
	cocos2d::CCSprite* GetSpriteWordExit();
	cocos2d::CCSprite* GetSpriteLoadingText();

private:
    cocos2d::CCSprite *m_spriteBg;
    cocos2d::CCSprite *m_spriteLoginBg;
	cocos2d::CCSprite *m_spriteEditBox;
	cocos2d::CCSprite *m_spriteWordUser;
	cocos2d::CCSprite *m_spriteWordPwd;
	cocos2d::CCSprite *m_spriteCheckBoxNormal;
	cocos2d::CCSprite *m_spriteCheckBoxSelect;
	cocos2d::CCSprite *m_spriteWordSavePwd;
	cocos2d::CCSprite *m_spriteWordAutoLogin;
	cocos2d::CCSprite *m_spriteBtnNormal;
	cocos2d::CCSprite *m_spriteBtnHover;
	cocos2d::CCSprite *m_spriteWordLogin;
	cocos2d::CCSprite *m_spriteWordExit;
	cocos2d::CCSprite *m_spriteLoadingText;
};

#endif
