#ifndef _GROUP_MAIN_SCENE_H_
#define _GROUP_MAIN_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "GameWorldBase.h"

class CGroupMainScene : public cocos2d::CCLayer, public CGameWorldBase, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
public:
	enum
	{
		E_GroupList_TableView_Tag = 200,
		E_GroupList_Loading,
	};

    virtual bool init();  
    virtual void onExit();
	
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

    static cocos2d::CCScene* scene();

	virtual void ConnectError(){}
	virtual void DisplayGameWorld(){ cocos2d::CCLog("CGroupMainScene"); }

	void menuCloseCallback(CCObject* pSender);

    CREATE_FUNC(CGroupMainScene);

	void DisplayGroupList(float t);

private:
	void _DisplayGroupList();

private:
	cocos2d::SEL_SCHEDULE m_showGroupListSchedule;
	int m_GroupListSize;
	std::vector<unsigned long long> m_vecGroupName;
	int m_nextIdx;
};

#endif