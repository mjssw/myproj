#include "MyEditBox.h"
USING_NS_CC;
USING_NS_CC_EXT;

CMyEditBox::CMyEditBox() : m_pEdit( NULL ), m_textChangedCallback(NULL)
{
}

CMyEditBox::~CMyEditBox()
{
}

void CMyEditBox::editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox)
{
    //CCLog("editBox %p DidBegin !", editBox);
}

void CMyEditBox::editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox)
{
    //CCLog("editBox %p DidEnd !", editBox);
}

void CMyEditBox::editBoxTextChanged(cocos2d::extension::CCEditBox *editBox, const std::string &text)
{
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
	if( m_textChangedCallback )
	{
		(*m_textChangedCallback)( text );
	}
}

void CMyEditBox::editBoxReturn(CCEditBox *editBox)
{
    //CCLog("editBox %p was returned !");
}

CCEditBox* CMyEditBox::GetEditBox()
{
	return m_pEdit;
}

void CMyEditBox::SetAnchorPoint(cocos2d::CCPoint &pt)
{
	setAnchorPoint( pt );
	if( m_pEdit )
	{
		m_pEdit->setAnchorPoint( pt );
	}
}

void CMyEditBox::SetPasswordMode()
{
	if( m_pEdit )
	{
		m_pEdit->setInputFlag( kEditBoxInputFlagPassword );
		m_pEdit->setInputMode( kEditBoxInputModeSingleLine );
	}
}

void CMyEditBox::SetEditChangedCallback(TextChangedHandler callback)
{
	m_textChangedCallback = callback;
}

CMyEditBox* CMyEditBox::create(cocos2d::CCSize &sz, cocos2d::CCPoint &pos, const char *bgImage, const char *font, int fontSize, const cocos2d::ccColor3B &color, int maxLen)
{
	CMyEditBox *editBox = new CMyEditBox();
	if( !editBox )
	{
		return NULL;
	}
	editBox->setAnchorPoint( ccp(0, 0) );
	editBox->m_pEdit = CCEditBox::create( sz, CCScale9Sprite::create(bgImage) );
	if( !editBox->m_pEdit )
	{
		return NULL;
	}
	editBox->m_pEdit->setAnchorPoint( ccp(0, 0) );
	editBox->m_pEdit->setPosition( pos );
	editBox->m_pEdit->setFont( font, fontSize );
    editBox->m_pEdit->setFontColor( color );
    editBox->m_pEdit->setPlaceHolder( "" );
    editBox->m_pEdit->setPlaceholderFontColor( ccWHITE );
    editBox->m_pEdit->setMaxLength( maxLen );
    editBox->m_pEdit->setReturnType( kKeyboardReturnTypeDone );
    editBox->m_pEdit->setDelegate( editBox );

	editBox->addChild( editBox->m_pEdit, 0 );
	return editBox;
}
