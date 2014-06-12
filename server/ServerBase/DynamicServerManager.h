#ifndef _DYNAMIC_SERVER_MANAGER_H_
#define _DYNAMIC_SERVER_MANAGER_H_

#include "Lock.h"
#include "Utils.h"
#include <map>

class CClient;
class CDynamicServerManager
{
DEFINE_SINGLETON_CLASS(CDynamicServerManager);

public:
	void AddServer(CClient *client);
	void DelServer(CClient *client);
	bool HasServer(CClient *client);

private:
	SGLib::CLock m_Lock;
	std::map<CClient*, s32> m_mapDynamicServers;
};

#endif
