#ifndef _REPORT_SERVER_H_
#define _REPORT_SERVER_H_

#include "Lock.h"
#include "Utils.h"
#include <map>

class CClient;
class CReportServerManager
{
DEFINE_SINGLETON_CLASS(CReportServerManager);

public:
	void AddServer(CClient *client);
	void DelServer(CClient *client);
	bool HasServer(CClient *client);

private:
	SGLib::CLock m_Lock;
	std::map<CClient*, s32> m_mapReportServers;
};

#endif
