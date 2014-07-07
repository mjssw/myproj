#include "ResManager.h"
#include <string>
using namespace cocos2d;
using namespace std;

SIGNLETON_CLASS_INIT(CResManager);

bool CResManager::Init(const char *resRoot)
{
	string root( resRoot );
	string res = root + "bg.png";
	m_spriteBg = CCSprite::create( res.c_str() );
	if( !m_spriteBg )
	{
		return false;
	}
	m_spriteBg->retain();

	res = root + "layout.png";
	m_spriteLayout = CCSprite::create( res.c_str() );
	if( !m_spriteLayout )
	{
		return false;
	}
	m_spriteLayout->retain();

	return true;
}

CCSprite* CResManager::GetSpriteBg()
{
	return m_spriteBg;
}

CCSprite* CResManager::GetSpriteLoginBgLeft()
{
	if( m_spriteLayout )
	{
		CCRect rc( 155, 0, 88, 476 );	
		return CCSprite::createWithTexture( m_spriteLayout->getTexture(), rc ); 
	}
	return NULL;
}

CCSprite* CResManager::GetSpriteLoginBgMidd()
{
	if( m_spriteLayout )
	{
		CCRect rc( 200, 0, 40, 476 );	
		return CCSprite::createWithTexture( m_spriteLayout->getTexture(), rc ); 
	}
	return NULL;
}

CCSprite* CResManager::GetSpriteLoginBgRight()
{
	/*
	if( m_spriteLayout )
	{
		CCRect rc( 155, 0, 88, 476 );	
		CCSprite *sprite = CCSprite::createWithTexture( m_spriteLayout->getTexture(), rc );
		sprite->setFlipX( true );
		return sprite;
	}
	return NULL;
	//*/
	CCSprite *sprite = GetSpriteLoginBgLeft();
	if( sprite )
	{
		sprite->setFlipX( true );
	}
	return sprite;
}

CCSprite* CResManager::GetSpriteBtnNormalLeft()
{
	if( m_spriteLayout )
	{
		CCRect rc( 40, 10, 95, 85 );
		return CCSprite::createWithTexture( m_spriteLayout->getTexture(), rc );
	}
	return NULL;
}

CCSprite* CResManager::GetSpriteBtnNormalRight()
{
	CCSprite *sprite = GetSpriteBtnNormalLeft();
	if( sprite )
	{
		sprite->setFlipX( true );
	}
	return sprite;
}

CCSprite* CResManager::GetSpriteBtnNormal()
{
	CCSprite *pSprite = CResManager::Instance().GetSpriteBtnNormalLeft();
    if( !pSprite )
	{
		return NULL;
	}
	pSprite->setAnchorPoint( ccp(0, 0) );

	CCSprite *rpSprite = CResManager::Instance().GetSpriteBtnNormalRight();
    if( !rpSprite )
	{
		return NULL;
	}
	rpSprite->setAnchorPoint( ccp(0, 0) );
	rpSprite->setPosition( ccp(pSprite->getContentSize().width, 0) );
    pSprite->addChild( rpSprite, 0 );
	return pSprite;
}

CCSprite* CResManager::GetSpriteBtnHoverLeft()
{
	if( m_spriteLayout )
	{
		CCRect rc( 40, 110, 100, 95 );	
		CCSprite *sprite = CCSprite::createWithTexture( m_spriteLayout->getTexture(), rc );
		sprite->setFlipX( true );
		return sprite;
	}
	return NULL;
}

CCSprite* CResManager::GetSpriteBtnHoverRight()
{
	CCSprite *sprite = GetSpriteBtnHoverLeft();
	if( sprite )
	{
		sprite->setFlipX( true );
	}

	return sprite;
}

CCSprite* CResManager::GetSpriteBtnHover()
{
	CCSprite *pSprite = CResManager::Instance().GetSpriteBtnHoverLeft();
    if( !pSprite )
	{
		return NULL;
	}
	pSprite->setAnchorPoint( ccp(0, 0) );

	CCSprite *rpSprite = CResManager::Instance().GetSpriteBtnHoverRight();
    if( !rpSprite )
	{
		return NULL;
	}
	rpSprite->setAnchorPoint( ccp(0, 0) );
	rpSprite->setPosition( ccp(pSprite->getContentSize().width, 0) );
    pSprite->addChild( rpSprite, 0 );
	return pSprite;
}

CCSprite* CResManager::GetSpriteBtnWordLogin()
{
	if( m_spriteLayout )
	{
		CCRect rc( 655, 160, 85, 35 );	
		return CCSprite::createWithTexture( m_spriteLayout->getTexture(), rc );
	}
	return NULL;
}

CCSprite* CResManager::GetSpriteBtnWordExit()
{
	if( m_spriteLayout )
	{
		CCRect rc( 482, 765, 85, 35 );	
		return CCSprite::createWithTexture( m_spriteLayout->getTexture(), rc );
	}
	return NULL;
}
