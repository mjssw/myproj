#ifndef _TABLE_VIEW_DATA_H_
#define _TABLE_VIEW_DATA_H_

#include "cocos2d.h"
#include <vector>
#include <string>

struct TableViewData
{
	TableViewData() : 
		iconRect(0,0,0,0), 
		useRect(false),
		data(NULL)
	{}

	std::string icon;
	std::string text;
	cocos2d::Rect iconRect;
	bool useRect;
	void *data;
};

#endif
