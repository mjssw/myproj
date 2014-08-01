#ifndef _REGISTER_POP_LAYER_H_
#define _REGISTER_POP_LAYER_H_

#include "PopLayerBase.h"
#include <string>

class CViewBase;
class CRegisterPopLayer : public CPopLayerBase
{
public:
	enum
	{
		E_Tag_Bg = 111,
		E_Tag_Edit = 123,
		E_Tag_CloseBtn = 120,
		E_Tag_RegBtn = 121,
	};

    virtual void onEnter();
	void menuCloseCallback(cocos2d::Object *sender);
	void menuRegisterCallback(cocos2d::Object *sender);
	static CRegisterPopLayer* create(const char *bgimage);

	void SetView(CViewBase *view);
	CViewBase* GetView();
	void Register();
	void RegisterResult();

private:
	void _AddEditBox();
	void _AddRegisterBtn();
	void _AddCloseBtn(bool isCenter);
	void _AddRegisterResult();

private:
	CViewBase *m_scene;
};

#endif
