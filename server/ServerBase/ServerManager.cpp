#include "ServerManager.h"

CServerManager* CServerManager::m_pInstance = NULL;

void MysqlPingTimerCall(void *pData, s32 nDataLen)
{
	CServerManager::Instance().PingAllMysql();
}
