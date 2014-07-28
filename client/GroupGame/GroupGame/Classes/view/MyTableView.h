#ifndef _MY_TABLE_VIEW_H_
#define _MY_TABLE_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
#include <string>

struct TableViewData
{
	std::string icon;
	std::string text;
};

class CMyTableView : public cocos2d::CCNode , public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
	enum
	{
		E_Tag_SelectBg = 999,
	};

	CMyTableView();
	~CMyTableView();

	void SetAnchorPoint(cocos2d::CCPoint &pt);
	void SetPosition(cocos2d::CCPoint &pt);
	cocos2d::CCSize GetSize();
	bool InsertElement(TableViewData &data);
	void UpdateElements(std::vector<TableViewData> &data);
	void RemoveElementAtIndex(int idx);

	//virtual void draw();

	static CMyTableView* create(cocos2d::CCSize &sz, cocos2d::CCSize &cellSz, std::vector<TableViewData> &viewData, const char *selectBgImage);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView *view);
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView *view);
    virtual void tableCellTouched(cocos2d::extension::TableView *table, cocos2d::extension::TableViewCell *cell);
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);

private:
	void _AddSelectBg(cocos2d::extension::TableViewCell *cell);

private:
	cocos2d::extension::TableView *m_tableView;
	cocos2d::extension::TableViewCell *m_lastTouchCell;
	unsigned int m_lastTouchIdx;
	cocos2d::CCSize m_cellSz;
	int m_cellCount;
	std::vector<TableViewData> m_viewData;
	std::string m_selectBg;
	cocos2d::CCPoint m_scrollOffset;
};

#endif
