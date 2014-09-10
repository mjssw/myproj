#ifndef _GROUP_H_
#define _GROUP_H_

#include "Define.h"
#include <map>
#include <vector>
#include <string>

class CGroupMember;
class CGroup
{
public:
	CGroup(u64 id, const std::string &name, const std::string &head);
	~CGroup();
	u64 GetId();
	const std::string& GetName();
	const std::string& GetHead();
	bool AddMember(CGroupMember *member);
	void RemoveMember(const std::string &user);
	CGroupMember* FindMember(const std::string &user);
	void Dump(std::vector<std::string> &vec);
	
    //std::vector<ChatTableViewData>& GetChatHistory();
    //void CheckChatHistoryLimit();

private:
	void _Destory();

private:
	u64 m_id;
	std::string m_name;
	std::string m_head;
	std::map<std::string, CGroupMember*> m_members;
	
    //std::vector<ChatTableViewData> m_chatHistory;
};

#endif
