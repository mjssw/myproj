#include "MyEditBox.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

CMyEditBox::CMyEditBox() : m_pEdit( NULL ), m_textChangedCallback(NULL)
{
}

CMyEditBox::~CMyEditBox()
{
}

void CMyEditBox::editBoxEditingDidBegin(EditBox *editBox)
{
    //CCLog("editBox %p DidBegin !", editBox);
}

void CMyEditBox::editBoxEditingDidEnd(EditBox *editBox)
{
    //CCLog("editBox %p DidEnd !", editBox);
}

void CMyEditBox::editBoxTextChanged(EditBox *editBox, const string &text)
{
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
	if( m_textChangedCallback )
	{
		(*m_textChangedCallback)( text );
	}
}

void CMyEditBox::editBoxReturn(EditBox *editBox)
{
    //CCLog("editBox %p was returned !");
}

EditBox* CMyEditBox::GetEditBox()
{
	return m_pEdit;
}

void CMyEditBox::SetAnchorPoint(CCPoint &pt)
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
		m_pEdit->setInputFlag( EditBox::InputFlag::PASSWORD );
		m_pEdit->setInputMode( EditBox::InputMode::SINGLE_LINE );
	}
}

void CMyEditBox::SetEditChangedCallback(TextChangedHandler callback)
{
	m_textChangedCallback = callback;
}

CMyEditBox* CMyEditBox::create(CCSize &sz, CCPoint &pos, const char *bgImage, const char *font, int fontSize, const cocos2d::ccColor3B &color, int maxLen)
{
	CMyEditBox *editBox = new CMyEditBox();
	if( !editBox )
	{
		return NULL;
	}
	editBox->setAnchorPoint( ccp(0, 0) );
	editBox->m_pEdit = EditBox::create( sz, Scale9Sprite::create(bgImage) );
	if( !editBox->m_pEdit )
	{
		return NULL;
	}
	editBox->m_pEdit->setAnchorPoint( ccp(0, 0) );
	editBox->m_pEdit->setPosition( pos );
	if( font )
	{
		editBox->m_pEdit->setFont( font, fontSize );
	}
	editBox->m_pEdit->setFontSize( fontSize );
    editBox->m_pEdit->setFontColor( color );
    editBox->m_pEdit->setPlaceHolder( "" );
    editBox->m_pEdit->setPlaceholderFontColor( ccWHITE );
    editBox->m_pEdit->setMaxLength( maxLen );
	editBox->m_pEdit->setReturnType( EditBox::KeyboardReturnType::DONE );
	editBox->m_pEdit->setInputMode( EditBox::InputMode::ANY );
    editBox->m_pEdit->setDelegate( editBox );


	editBox->addChild( editBox->m_pEdit, 0 );
	return editBox;
}
