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

	ChatTableViewData(EChatDataType type_, const std::string &name_, const std::string &content_) : 
		type(type_),
		name(name_),
		content(content_),
		line(1),
		hasConvert(false)
	{}

	void SetUser(const std::string &user_)
	{
		user = user_;
	}

private:
	EChatDataType type;
	std::string user;
	std::string name;
	std::string content;
	int line;		// ÐÐÊý
	bool hasConvert;
};

#endif
