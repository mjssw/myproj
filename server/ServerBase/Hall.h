#ifndef _HALL_H_
#define _HALL_H_

#include "Define.h"
#include "HallConfig.h"
#include <map>

// TODO need lock

// 所有GameServer使用, 非GameServer不要使用
class CRoomBase;
class CPlayerBase;
class CHall
{
public:
	enum
	{
		E_ReportGameInfo_Timer = 10000,
	};

	~CHall();
	static CHall& Instance()
	{
		if( m_pInstance == NULL )
		{
			m_pInstance = new CHall();
		}
		return *m_pInstance;
	}

	s32 GameId(){ return m_nGameId; }

	void Init(s32 gameid, CPlayerBase *templatePlayer, const CHallConfig &hallCfg);
	void InitRoomList(CRoomBase *roomTemplate);
	void Fini();
	s32 RoomPlayerCount();
	s32 HallId();
	CRoomBase* FindRoom(s32 roomid);
	CPlayerBase* FindPlayer(u64 gateid, u64 clientid);
	void ProcPlayerMessage(u64 gateid, u64 clientid, s32 msgid, const byte *pkg, s32 len);

	void EnterHall(u64 gateid, u64 clientid, const std::string &userid);
	void EnterRoom(CPlayerBase *player, const byte *pkg, s32 len);
	void LeaveRoom(CPlayerBase *player, const byte *pkg, s32 len);
	void LeaveHall(u64 gateid, u64 clientid);
	void AskEnterGame(CPlayerBase *player, const byte *pkg, s32 len);

	s32 PlayerCount();
	void SetGameServerReady();
	s32 FindFreeRoom();

	static void ReportGameInforCallBack(void *pData, s32 nDataLen);

private:
	CHall() : 
		m_bInit(false),
		m_PlayerTemplate(NULL),
		m_nReportGateTimerId(INVALID_VAL),
		m_pHallConfig(NULL),
		m_mapRoom(),
		m_Players(),
		m_nGameId(0),
		m_bIsDynamicStart(false),
		m_bIsReady(true),
		m_bHasReported(true)
	{}
	
	void _InitRooms();
	void _EnterRoom(CPlayerBase *player, s32 roomid);
	void _NotifyEnterRoomFailed(CPlayerBase *player, s32 roomid, s32 result);
	void _LeaveRoom(CPlayerBase *player);
	void _NotifyLeaveRoomResult(CPlayerBase *player, s32 result);
	void _AskCenterEnterGame(CPlayerBase *player, s32 gameid);
	bool _ReportDynamicStarted();

private:
	static CHall *m_pInstance;
	bool m_bInit;
	CPlayerBase *m_PlayerTemplate;
	CRoomBase *m_RoomTemplate;

	s32 m_nReportGateTimerId;
	const CHallConfig *m_pHallConfig;
	std::map<s32, CRoomBase*> m_mapRoom;
	std::map<u64, std::map<u64, CPlayerBase*> > m_Players;

	s32 m_nGameId;

	// 动态启动服务器相关属性
	bool m_bIsDynamicStart;		// 此游戏服是否是动态启动的
	bool m_bIsReady;			// 动态启动的服务器是否已经连接好gate
	bool m_bHasReported;		// 动态启动后是否已经向GameManager汇报过
};

#endif
