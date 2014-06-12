#include "DynamicServerManager.h"
using namespace std;

INIT_SIGNLETON_CLASS(CDynamicServerManager);

void CDynamicServerManager::AddServer(CClient *client)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	m_mapDynamicServers[ client ] = 1;
}

void CDynamicServerManager::DelServer(CClient *client)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	map<CClient*, s32>::iterator it = m_mapDynamicServers.find( client );
	if( it != m_mapDynamicServers.end() )
	{
		m_mapDynamicServers.erase( it );
	}
}

bool CDynamicServerManager::HasServer(CClient *client)
{
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	map<CClient*, s32>::iterator it = m_mapDynamicServers.find( client );
	if( it != m_mapDynamicServers.end() )
	{
		return true;
	}

	return false;
}
