#ifndef _CREATE_GROUP_POP_LAYER_H_
#define _CREATE_GROUP_POP_LAYER_H_

#include "PopLayerBase.h"
#include <string>

class CViewBase;
class CCreateGroupPopLayer : public CPopLayerBase
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
	void menuCreateGroupCallback(cocos2d::Object *sender);
	static CCreateGroupPopLayer* create(const char *bgimage);

	void SetView(CViewBase *view);
	CViewBase* GetView();

private:
	void _AddEditBox();
	void _AddCreateBtn();
	void _AddCloseBtn(bool isCenter);

private:
	CViewBase *m_scene;
};

#endif
