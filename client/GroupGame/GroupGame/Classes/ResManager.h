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
	cocos2d::CCSprite* GetSpriteLoginBgLeft(); 
	cocos2d::CCSprite* GetSpriteLoginBgMidd(); 
	cocos2d::CCSprite* GetSpriteLoginBgRight();
	cocos2d::CCSprite* GetSpriteBtnNormalLeft();
	cocos2d::CCSprite* GetSpriteBtnNormalRight();
	cocos2d::CCSprite* GetSpriteBtnNormal();
	cocos2d::CCSprite* GetSpriteBtnHoverLeft();
	cocos2d::CCSprite* GetSpriteBtnHoverRight();
	cocos2d::CCSprite* GetSpriteBtnHover();
	cocos2d::CCSprite* GetSpriteBtnWordLogin();
	cocos2d::CCSprite* GetSpriteBtnWordExit();

private:
    cocos2d::CCSprite *m_spriteBg;
    cocos2d::CCSprite *m_spriteLayout;
};

#endif
