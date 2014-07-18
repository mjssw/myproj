#ifndef _MEMBER_LIST_POP_LAYER_H_
#define _MEMBER_LIST_POP_LAYER_H_

#include "PopLayerBase.h"
#include <string>

class CMemberListPopLayer : public CPopLayerBase
{
public:
	enum
	{
		E_Tag_Bg = 111,
	};

    virtual void onEnter();
	void menuCloseCallback(cocos2d::Object *sender);
	static CMemberListPopLayer* create(const char *bgimage);
};

#endif
