#ifndef _ASK_JOIN_GROUP_POP_LAYER_H_
#define _ASK_JOIN_GROUP_POP_LAYER_H_

#include "PopLayerBase.h"
#include <string>

class CViewBase;
class CAskJoinGroupPopLayer : public CPopLayerBase
{
public:
	enum
	{
		E_Tag_Bg = 111,
		E_Tag_Edit = 123,
		E_Tag_CloseBtn = 120,
		E_Tag_OkBtn = 121,
	};

    virtual void onEnter();
	void menuCloseCallback(cocos2d::Object *sender);
	void menuJoinCallback(cocos2d::Object *sender);
	static CAskJoinGroupPopLayer* create(const char *bgimage);

	void SetView(CViewBase *view);
	CViewBase* GetView();

private:
	void _AddJoinBtn();
	void _AddCloseBtn(bool isCenter);
	void _AddTitle();

private:
	CViewBase *m_scene;
};

#endif
