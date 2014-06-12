#ifndef _GAME_SERVER_INFO_H_
#define _GAME_SERVER_INFO_H_

class CGameServerInfo
{
public:
	CGameServerInfo(s32 id) : m_Id(id), m_CurCount(0)
	{}

	void SetCount(s32 count){ m_CurCount = count; }
	s32 Id() const { return m_Id; }
	s32 CurCount() const { return m_CurCount; }
	void SetServerInstanceId(u64 id){ m_InstanceId = id; }
	u64 GetServerInstanceId() const { return m_InstanceId; }

private:
	s32 m_Id;		  // xml中配置的serverid
	s32 m_CurCount;
	u64 m_InstanceId; // 内存的serverid
};

#endif
