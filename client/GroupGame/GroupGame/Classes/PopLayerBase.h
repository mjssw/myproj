#ifndef _POP_LAYER_BASE_H_
#define _POP_LAYER_BASE_H_

#include "cocos2d.h"

class CPopLayerBase : public cocos2d::Layer
{
public:
	CPopLayerBase();
	virtual ~CPopLayerBase();

    virtual void onEnter();
    virtual void onExit();

protected:
	void _PauseParentEventListener();
	void _ResumeParentEventListener();

private:
	cocos2d::Node *m_parent;
};

#endif
