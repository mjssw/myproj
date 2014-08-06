#ifndef _CHAT_TABLE_VIEW_DATA_H_
#define _CHAT_TABLE_VIEW_DATA_H_

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

#endif
