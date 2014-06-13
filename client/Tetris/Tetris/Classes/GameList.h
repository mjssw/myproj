#ifndef _GAME_LIST_H_
#define _GAME_LIST_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "GameWorldBase.h"
#include <vector>

#define GAME_ICON_TAG(id) ((id)*1000)

class CGameList : public cocos2d::CCLayer, public CGameWorldBase, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
public:
	enum
	{
		E_GameList_TableView_Tag = 100,
		E_GameList_Loading,
	};

    virtual bool init();  

	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

    static cocos2d::CCScene* scene();
    
    void menuCloseCallback(CCObject* pSender);
    CREATE_FUNC(CGameList);

	void DisplayGameList(float t);
	virtual void ConnectError();

private:
	void _DisplayGameList();

private:
	cocos2d::SEL_SCHEDULE m_showGameListSchedule;
	int m_GameListSize;
	std::vector<int> m_vecGameId;
	int m_nextIdx;
};
#endif
