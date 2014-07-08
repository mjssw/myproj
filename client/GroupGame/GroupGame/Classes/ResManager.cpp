#include "ResManager.h"
#include <string>
using namespace cocos2d;
using namespace std;

SIGNLETON_CLASS_INIT(CResManager);

bool CResManager::Init(const char *resRoot)
{
	string root( resRoot );
	string res = "";

#define _LOAD_RES(var, res_name) \
	res = root + res_name;\
	var = CCSprite::create( res.c_str() );\
	if( !var )\
	{\
		return false;\
	}\
	var->retain();

	_LOAD_RES( m_spriteBg,				"bg.png" ); 
	_LOAD_RES( m_spriteLoginBg,			"loginbg.png" ); 
	_LOAD_RES( m_spriteEditBox,			"editbox.png" ); 
	_LOAD_RES( m_spriteWordUser,		"user.png" );
	_LOAD_RES( m_spriteWordPwd,			"pwd.png" );
	_LOAD_RES( m_spriteCheckBoxNormal,	"checkbox_normal.png" );
	_LOAD_RES( m_spriteCheckBoxSelect,	"checkbox_select.png" );
	_LOAD_RES( m_spriteWordSavePwd,		"savepwd.png" );
	_LOAD_RES( m_spriteWordAutoLogin,	"autologin.png" );
	_LOAD_RES( m_spriteBtnNormal,		"btn_normal.png" );
	_LOAD_RES( m_spriteBtnHover,		"btn_hover.png" );
	_LOAD_RES( m_spriteWordLogin,		"login.png" );
	_LOAD_RES( m_spriteWordExit,		"exit.png" );

#undef _LOAD_RES

	return true;
}

#define _CLONE_SPRITE(obj) CCSprite::createWithTexture(obj->getTexture());

CCSprite* CResManager::GetSpriteBg()
{
	return _CLONE_SPRITE( m_spriteBg );
}

CCSprite* CResManager::GetSpriteLoginBg()
{
	return _CLONE_SPRITE( m_spriteLoginBg );
}

CCSprite* CResManager::GetSpriteEditBox()
{
	return _CLONE_SPRITE( m_spriteEditBox );
}

CCSprite* CResManager::GetSpriteWordUser()
{
	return _CLONE_SPRITE( m_spriteWordUser );
}

CCSprite* CResManager::GetSpriteWordPwd()
{
	return _CLONE_SPRITE( m_spriteWordPwd );
}

CCSprite* CResManager::GetSpriteCheckBoxNormal()
{
	return _CLONE_SPRITE( m_spriteCheckBoxNormal );
}

CCSprite* CResManager::GetSpriteCheckBoxSelect()
{
	return _CLONE_SPRITE( m_spriteCheckBoxSelect );
}

CCSprite* CResManager::GetSpriteWordSavePwd()
{
	return _CLONE_SPRITE( m_spriteWordSavePwd );
}

CCSprite* CResManager::GetSpriteWordAutoLogin()
{
	return _CLONE_SPRITE( m_spriteWordAutoLogin );
}

CCSprite* CResManager::GetSpriteBtnNormal()
{
	return _CLONE_SPRITE( m_spriteBtnNormal );
}

CCSprite* CResManager::GetSpriteBtnHover()
{
	return _CLONE_SPRITE( m_spriteBtnHover );
}

CCSprite* CResManager::GetSpriteWordLogin()
{
	return _CLONE_SPRITE( m_spriteWordLogin );
}

CCSprite* CResManager::GetSpriteWordExit()
{
	return _CLONE_SPRITE( m_spriteWordExit );
}
