#ifndef _CHAT_TABLE_VIEW_H_
#define _CHAT_TABLE_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
#include <string>

struct ChatTableViewData
{
	enum EChatDataType
	{
		E_DataType_User,
		E_DataType_Text,
	} type;				// 0-成员;  1-聊天内容
	std::string user;
	std::string content;
	int width(){ return 100; };
	int height(){ return 20; };
};

class CChatTableView : public cocos2d::CCNode , public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
	enum
	{
		E_Tag_SelectBg = 999,
	};

	CChatTableView();
	~CChatTableView();

	void InitData(std::vector<ChatTableViewData> &data); 
	void SetAnchorPoint(cocos2d::CCPoint &pt);
	void SetPosition(cocos2d::CCPoint &pt);
	cocos2d::CCSize GetSize();
	bool InsertUpdate(int elemH);
	void RemoveUpdate(int elemH);

	//virtual void draw();

	static CChatTableView* create(cocos2d::CCSize &sz);

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
	std::vector<ChatTableViewData> *m_viewData;
	cocos2d::CCPoint m_scrollOffset;
};

#endif
