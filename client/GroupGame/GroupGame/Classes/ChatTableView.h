#ifndef _CHAT_TABLE_VIEW_H_
#define _CHAT_TABLE_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
#include <string>

class CChatTableView; 
class ChatTableViewData
{
public:
	friend class CChatTableView; 
	enum EChatDataType
	{
		E_DataType_User,
		E_DataType_Text,
	};

	ChatTableViewData(EChatDataType type_, const std::string &user_, const std::string &content_) : 
		type(type_),
		user(user_),
		content(content_),
		line(1)
	{}

private:
	EChatDataType type;
	std::string user;
	std::string name;
	std::string content;
	int line;		// ÐÐÊý
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
	bool InsertUpdate();
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
	void _AdjustChatContent(ChatTableViewData &data);

private:
	cocos2d::extension::TableView *m_tableView;
	std::vector<ChatTableViewData> *m_viewData;
	cocos2d::CCPoint m_scrollOffset;

	cocos2d::Size m_chineseSize;
	cocos2d::Size m_asciiSize;
	int m_userCellWidth;
	int m_chatCellWidth;
	int m_lineHeight;
};

#endif
