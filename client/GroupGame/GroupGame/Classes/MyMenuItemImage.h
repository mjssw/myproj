#ifndef _MY_MENU_ITEM_IMAGE_H_
#define _MY_MENU_ITEM_IMAGE_H_

#include "menu_nodes/CCMenuItem.h"
using namespace cocos2d;

class CCMyMenuItemImage : public CCMenuItemImage
{
public:
    CCMyMenuItemImage(){}
    virtual ~CCMyMenuItemImage(){}
    
    /** creates a menu item with a normal and selected image*/
    static CCMyMenuItemImage* create(const char *normalImage, const char *selectedImage);
    /** creates a menu item with a normal,selected  and disabled image*/
    static CCMyMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage);
    /** creates a menu item with a normal and selected image with target/selector */
    static CCMyMenuItemImage* create(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector);
    /** creates a menu item with a normal,selected  and disabled image with target/selector */
    static CCMyMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector);
    
    bool init();

	virtual void setSelectedImage(CCNode* pImage);
	
    /** Creates an CCMyMenuItemImage.
     */
    static CCMyMenuItemImage* create();
};

#endif
