#ifndef _MY_MENU_ITEM_IMAGE_H_
#define _MY_MENU_ITEM_IMAGE_H_

#include "cocos2d.h"
#include "2d/CCMenuItem.h"
using namespace cocos2d;

class CCMyMenuItemImage : public MenuItemImage
{
public:
    CCMyMenuItemImage(){}
    virtual ~CCMyMenuItemImage(){}
    
    static CCMyMenuItemImage* create(const char *normalImage, const char *selectedImage);
    static CCMyMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage);
    static CCMyMenuItemImage* create(const char *normalImage, const char *selectedImage, Object *target, SEL_MenuHandler selector);
    static CCMyMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject *target, SEL_MenuHandler selector);
    
    bool init();

	//virtual void setSelectedImage(Node *pImage);
    virtual void selected();
    virtual void unselected();
	
    static CCMyMenuItemImage* create();
};

#endif
