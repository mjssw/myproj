#ifndef _CREATE_GAME_POP_LAYER_H_
#define _CREATE_GAME_POP_LAYER_H_

#include "PopLayerBase.h"
#include <string>

class CMyTableView; 
class CCreateGamePopLayer : public CPopLayerBase
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
	void menuCreateCallback(cocos2d::Object *sender);
	static CCreateGamePopLayer* create(const char *bgimage);

private:
	void _AddCreateBtn();
	void _AddCloseBtn(bool isCenter);
	void _AddGameList();

private:
	CMyTableView *m_gameList;
};

#endif
