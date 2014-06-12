#ifndef _GATE_CLIENT_MANAGER_H_
#define _GATE_CLIENT_MANAGER_H_

#include "Lock.h"
#include "globalcfg.h"
#include <map>
#include <vector>

class CGateClientInfo
{
public:
	CGateClientInfo(s32 id) : 
		m_GateId(id),
		m_GatePort(0),
		m_GateCurConnCount(0),
		m_GateMaxConnCount(0)
	{}

	s32  m_GateId;
	char m_GateIp[SGLib::MAX_IP_LEN];
	s32  m_GatePort;
	s32  m_GateCurConnCount;
	s32  m_GateMaxConnCount;
    u64  m_GateInstanceId;      // The Id is the value when server accepted gate and alloced Id. Not the res Id in xml config file.
};

class CGateClientManager
{
public:
	~CGateClientManager();
	static CGateClientManager& Instance()
	{
		if( m_pInstance == NULL )
		{
			m_pInstance = new CGateClientManager();
		}
		return *m_pInstance;
	}

	void SetGateInfo(s32 id, s32 curCount, s32 maxCount, const char *ip, s32 port, u64 instancdId);
	void DelGateInfo(s32 id);
	const CGateClientInfo* GetGateInfo(s32 id);
	void GetGateIdList(std::vector<s32> &vecId);

private:
	CGateClientManager() : 
		m_GateClientLock(),
		m_GateClientMap()
	{
	}

	static CGateClientManager *m_pInstance;

	SGLib::CLock m_GateClientLock;
	std::map<s32, CGateClientInfo*> m_GateClientMap;
};

#endif
