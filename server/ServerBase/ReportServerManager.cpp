#include "ReportServerManager.h"
using namespace std;

INIT_SIGNLETON_CLASS(CReportServerManager);

void CReportServerManager::AddServer(CClient *client)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	m_mapReportServers[ client ] = 1;
}

void CReportServerManager::DelServer(CClient *client)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	map<CClient*, s32>::iterator it = m_mapReportServers.find( client );
	if( it != m_mapReportServers.end() )
	{
		m_mapReportServers.erase( it );
	}
}

bool CReportServerManager::HasServer(CClient *client)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	map<CClient*, s32>::iterator it = m_mapReportServers.find( client );
	if( it != m_mapReportServers.end() )
	{
		return true;
	}

	return false;
}
