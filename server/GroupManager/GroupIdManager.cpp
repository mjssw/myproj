#include "GroupIdManager.h"
#include "ServerManager.h"

INIT_SIGNLETON_CLASS(CGroupIdManager);

CGroupIdManager::~CGroupIdManager()
{
}

void CGroupIdManager::Init(u64 from)
{
	// TODO
	m_NextId = from;
}

u64 CGroupIdManager::GetNextGroupId()
{
	u64 id = 0;
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	id = m_NextId;
	++m_NextId;

	CServerManager::Instance().IncrementNextGroupId();

	return id;
}
