#include "MyMenuItemImage.h"

CCMyMenuItemImage* CCMyMenuItemImage::create()
{
    CCMyMenuItemImage *pRet = new CCMyMenuItemImage();
    if( pRet && pRet->init() )
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCMyMenuItemImage::init(void)
{
    return initWithNormalImage( NULL, NULL, NULL, NULL, NULL );
}

CCMyMenuItemImage* CCMyMenuItemImage::create(const char *normalImage, const char *selectedImage)
{
    return CCMyMenuItemImage::create( normalImage, selectedImage, NULL, NULL, NULL );
}

CCMyMenuItemImage* CCMyMenuItemImage::create(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector)
{
    return CCMyMenuItemImage::create( normalImage, selectedImage, NULL, target, selector );
}

CCMyMenuItemImage* CCMyMenuItemImage::create(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector)
{
    CCMyMenuItemImage *pRet = new CCMyMenuItemImage();
	if( !disabledImage )
	{
		disabledImage = normalImage;
	}
    if( pRet && pRet->initWithNormalImage(normalImage, selectedImage, disabledImage, target, selector) )
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CCMyMenuItemImage* CCMyMenuItemImage::create(const char *normalImage, const char *selectedImage, const char *disabledImage)
{
    CCMyMenuItemImage *pRet = new CCMyMenuItemImage();
    if( pRet && pRet->initWithNormalImage(normalImage, selectedImage, disabledImage, NULL, NULL) )
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

/*
void CCMyMenuItemImage::setSelectedImage(CCNode *pImage)
{
	const unsigned int kSelectedTag = 0x2;
	if( pImage != _normalImage )
    {
        if( pImage )
        {
            addChild( pImage, 0, kSelectedTag );

			// 计算selectedImage的anchorPoint比例，使之与normalImage “同心”，而不是“同左下角”
			CCSize sz = pImage->getContentSize();
			CCSize norSz = _normalImage->getContentSize();
			float xRate = 0.5 * (sz.width - norSz.width) / sz.width;
			float yRate = 0.5 * (sz.height - norSz.height) / sz.height;
			pImage->setAnchorPoint( ccp(xRate, yRate) );
        }

        if( _selectedImage )
        {
            removeChild( _selectedImage, true );
        }

        _selectedImage = pImage;
        this->updateImagesVisibility();
    }
}
//*/

void CCMyMenuItemImage::selected()
{
	MenuItemSprite::selected();
	
	CCSize sz = _selectedImage->getContentSize();
	CCSize norSz = _normalImage->getContentSize();
	float xRate = 0.5 * (sz.width - norSz.width) / sz.width;
	float yRate = 0.5 * (sz.height - norSz.height) / sz.height;
	_selectedImage->setAnchorPoint( ccp(xRate, yRate) );
}

void CCMyMenuItemImage::unselected()
{
	MenuItemSprite::unselected();
}


