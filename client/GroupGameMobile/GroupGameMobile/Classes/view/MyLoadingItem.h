#ifndef _MY_LOADING_ITEM_H_
#define _MY_LOADING_ITEM_H_

#include "cocos2d.h"
class CMyLoadingItem : public cocos2d::CCNode
{
public:
	enum
	{
		E_Tag_Loadtail = 112,
	};

	CMyLoadingItem();
	~CMyLoadingItem();

	void SetAnchorPoint(cocos2d::CCPoint &pt);
	void SetPosition(cocos2d::Vec2 pt);
	bool UpdateProgress();
	bool IsLoadingEnd();
	static CMyLoadingItem* create(int steps); 

private:
	int m_curStep;
	int m_maxStep;
	int m_stepWidth;
	int m_stepHight;
};

#endif
