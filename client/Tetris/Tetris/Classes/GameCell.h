#ifndef _GAME_CELL_H_
#define _GAME_CELL_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class CGameTableViewCell : public cocos2d::extension::CCTableViewCell
{
public:
	virtual void draw();
};

#endif
