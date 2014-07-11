#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class CMyTableView;
class CMainScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    void menuCloseCallback(CCObject *pSender);
	void menuTestCallback(CCObject *pSender);

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
	
	CMyTableView *m_pGroupList;
};

#endif
