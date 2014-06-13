#include "GroupCell.h"
#include "utils.h"

USING_NS_CC;

void CGroupTableViewCell::draw()
{
	CCTableViewCell::draw();

	// draw bounding box
 	CCPoint pos = getPosition();
	CCSize size = CCSizeMake(GROUP_ICON_WIDTH, GROUP_ICON_HEIGHT);
 	CCPoint vertices[4]={
 		ccp(pos.x+1, pos.y+1),
 		ccp(pos.x+size.width-1, pos.y+1),
 		ccp(pos.x+size.width-1, pos.y+size.height-1),
 		ccp(pos.x+1, pos.y+size.height-1),
 	};
 	ccDrawColor4B(0, 0, 255, 255);
 	ccDrawPoly(vertices, 4, true);
}