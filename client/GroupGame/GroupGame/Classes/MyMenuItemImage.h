#ifndef _MY_MENU_ITEM_IMAGE_H_
#define _MY_MENU_ITEM_IMAGE_H_

#include "menu_nodes/CCMenuItem.h"
using namespace cocos2d;

class CCMyMenuItemImage : public CCMenuItemImage
{
public:
    CCMyMenuItemImage(){}
    virtual ~CCMyMenuItemImage(){}
    
    static CCMyMenuItemImage* create(const char *normalImage, const char *selectedImage);
    static CCMyMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage);
    static CCMyMenuItemImage* create(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector);
    static CCMyMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector);
    
    bool init();

	virtual void setSelectedImage(CCNode* pImage);
	
    static CCMyMenuItemImage* create();
};

#endif
