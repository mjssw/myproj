#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"

class CMyTableView;
class CMyEditBox;
class CMainScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    void menuCloseCallback(cocos2d::Object *pSender);
	void menuTestCallback(cocos2d::Object *pSender);
	void menuRadioButtonCallback(cocos2d::Object *pSender);

    CREATE_FUNC(CMainScene);

private:
	void _AddSceneBg();
	void _AddTitle();
	void _AddTabButtons();
	void _AddListView();
	void _AddMainView();

private:
	int m_mainViewOffX;
	cocos2d::CCPoint m_ptTableView;
	cocos2d::CCSize m_szTableView;
	int m_listHigh;
	int m_titleHigh;
	
	CMyTableView *m_pGroupList;
	CMyEditBox *m_chatHistory;
};

#endif
