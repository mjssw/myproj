#include "Hall.h"
#include "PlayerStateBase.h"
#include "RoomStateBase.h"
#include "ServerManager.h"
#include "GateClientManager.h"
#include "PlayerBase.h"
#include "RoomBase.h"
#include "GameId.h"
#include <vector>
using namespace std;

#include "msgid.pb.h"
#include "errno.pb.h"
#include "common.pb.h"
#include "gamemanager.pb.h"

CHall* CHall::m_pInstance = NULL;

CHall::~CHall()
{
	std::map<s32, CRoomBase*>::iterator it = m_mapRoom.begin();
	for( ; it != m_mapRoom.end(); ++it )
	{
		SAFE_DELETE( it->second );
	}
	m_mapRoom.clear();

	std::map<u64, std::map<u64, CPlayerBase*> >::iterator fit = m_Players.begin();
	for( ; fit != m_Players.end(); ++fit )
	{
		std::map<u64, CPlayerBase*>::iterator sid = fit->second.begin();
		for( ; sid != fit->second.end(); ++sid )
		{
			SAFE_DELETE( sid->second );
		}
	}
	m_Players.clear();

	SAFE_DELETE( m_PlayerTemplate );
}

void CHall::Init(s32 gameid, CPlayerBase *templatePlayer, const CHallConfig &hallCfg)
{
	m_pHallConfig = &hallCfg;

	m_nReportGateTimerId = CServerManager::Instance().AddTimer(
		E_ReportGameInfo_Timer,
		ReportGameInforCallBack,
		NULL,
		0,
		true );
	
	m_roomTimerId = CServerManager::Instance().AddTimer(
		E_Room_Timer,
		RoomTimerCallBack,
		NULL,
		0,
		true );

	m_PlayerTemplate = templatePlayer;
	SG_ASSERT( m_PlayerTemplate != NULL );
	m_bInit = true;

	m_nGameId = gameid;

	// 设置动态启动
	m_bIsDynamicStart = hallCfg.IsDynamicStart();
	m_bIsReady = (!m_bIsDynamicStart);
	if( m_bIsDynamicStart )
	{
		m_bHasReported = false;
	}
}

void CHall::Fini()
{
	if( m_nReportGateTimerId != INVALID_VAL )
	{
		CServerManager::Instance().DelTimer( m_nReportGateTimerId );
		m_nReportGateTimerId = INVALID_VAL;
	}
}

void CHall::InitRoomList(CRoomBase *roomTemplate)
{
	m_RoomTemplate = roomTemplate;
	SG_ASSERT( m_RoomTemplate != NULL );
	_InitRooms();
}


void CHall::ReportGameInforCallBack(void *pData, s32 nDataLen)
{
	if( !CHall::Instance().m_bIsReady )
	{
		// 动态启动的游戏服如果其gate还没连接上来，则不要汇报自身的状态

		if( !CHall::Instance().m_bHasReported )
		{
			// 如果还没有回报过，在汇报给GameManager
			CHall::Instance().m_bHasReported = CHall::Instance()._ReportDynamicStarted();
		}

		return;
	}

	vector<s32> vecGateId;
	CGateClientManager::Instance().GetGateIdList( vecGateId );

	if( vecGateId.size() > 0 )
	{
		sglib::gamemanagerproto::ServerGamemanagerReportGameInfo reportInfo;
		reportInfo.set_gameid( CHall::Instance().GameId() );	

		vector<s32>::iterator it = vecGateId.begin();
		for( ; it != vecGateId.end(); ++it )
		{
			const CGateClientInfo *info = CGateClientManager::Instance().GetGateInfo( *it );
			if( info == NULL )
			{
				continue;
			}

			sglib::publicproto::GateServerInfo *pInfo = reportInfo.add_gateservers();
			pInfo->set_id( info->m_GateId );
			pInfo->set_ip( info->m_GateIp );
			pInfo->set_port( info->m_GatePort );
			pInfo->set_cur_count( info->m_GateCurConnCount );
			pInfo->set_max_count( info->m_GateMaxConnCount );
		}

		reportInfo.mutable_gameserver()->set_serverid( CServerManager::Instance().ServerId() );
		reportInfo.mutable_gameserver()->set_cur_count( CHall::Instance().PlayerCount() );

		CServerManager::Instance().SendRpcMsg( 
			CServerManager::Instance().GetGameManagerServerId(),
			reportInfo, sglib::msgid::SGM_REPORT_GAME_INFO );
	}
}

void CHall::RoomTimerCallBack(void *pData, s32 nDataLen)
{
	map<s32, CRoomBase*>::iterator it = CHall::Instance().m_mapRoom.begin();
	for( ; it != CHall::Instance().m_mapRoom.end(); ++it )
	{
		it->second->TimerCallback();
	}
}

s32 CHall::RoomPlayerCount()
{
	if( m_pHallConfig )
	{
		return m_pHallConfig->RoomPlayerCount();
	}

	return 0;
}

s32 CHall::HallId()
{
	if( m_pHallConfig )
	{
		return m_pHallConfig->HallId();
	}

	return INVALID_VAL;
}

CRoomBase* CHall::FindRoom(s32 roomid)
{
	map<s32, CRoomBase*>::iterator it = m_mapRoom.find( roomid );
	if( it != m_mapRoom.end() )
	{
		return it->second;
	}

	return NULL;
}

CPlayerBase* CHall::FindPlayer(u64 gateid, u64 clientid)
{
	map<u64, map<u64, CPlayerBase*> >::iterator git = m_Players.find( gateid );
	if( git != m_Players.end() )
	{
		map<u64, CPlayerBase*>::iterator cit = git->second.find( clientid );
		if( cit != git->second.end() )
		{
			return cit->second;
		}
	}

	return NULL;
}

void CHall::ProcPlayerMessage(u64 gateid, u64 clientid, s32 msgid, const byte *pkg, s32 len)
{
	CPlayerBase *player = FindPlayer( gateid, clientid );
	if( player )
	{
		player->Update( msgid, pkg, len );
	}
	else
	{
		SERVER_LOG_ERROR( "CHall,ProcPlayerMessage," << gateid << "," << clientid );
	}
}

void CHall::EnterHall(u64 gateid, u64 clientid, const std::string &userid)
{
	if( m_bInit )
	{
		SG_ASSERT( m_PlayerTemplate != NULL );
		CPlayerBase *player = m_PlayerTemplate->Clone( gateid, clientid, userid );
		SG_ASSERT( player != NULL );

		m_Players[ player->GateId() ][ player->ClientId() ] = player;
		player->ChangeState( E_PlayerState_Free );

		player->OnEnterHall();
	}
}

void CHall::EnterRoom(CPlayerBase *player, const byte *pkg, s32 len)
{
	SG_ASSERT( player != NULL );
	sglib::commonproto::CSEnterRoomReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_DEBUG( "player:"<< player->UserId().c_str() << " CSEnterRoomReq: room=" << req.roomid() );
		_EnterRoom( player, req.roomid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CHall,EnterRoom,ParseFromArray," << player->UserId().c_str() );
	}
}


void CHall::LeaveRoom(CPlayerBase *player, const byte *pkg, s32 len)
{
	SG_ASSERT( player != NULL );
	sglib::commonproto::CSLeaveRoomReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_DEBUG( "player:"<< player->UserId().c_str()<< " CSLeaveRoomReq" );
		_LeaveRoom( player );
	}
	else
	{
		SERVER_LOG_ERROR( "CHall,LeaveRoom,ParseFromArray" );
	}
}

void CHall::LeaveHall(u64 gateid, u64 clientid)
{
	CPlayerBase *player = FindPlayer( gateid, clientid );
	if( player != NULL )
	{
		map<u64, map<u64, CPlayerBase*> >::iterator git = m_Players.find( player->GateId() );
		if( git != m_Players.end() )
		{
			map<u64, CPlayerBase*>::iterator cit = git->second.find( player->ClientId() );
			if( cit != git->second.end() )
			{
				SAFE_DELETE( cit->second );
				git->second.erase( cit );
			}
		}
	}
}

void CHall::AskEnterGame(CPlayerBase *player, const byte *pkg, s32 len)
{
	SG_ASSERT( player != NULL );
	sglib::commonproto::CSEnterGameReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		SERVER_LOG_DEBUG( "player:" << player->UserId().c_str() << " CSEnterGameReq: game=" << req.gameid() );
		_AskCenterEnterGame( player, req.gameid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CHall,AskEnterGame,ParseFromArray" );
	}
}

s32 CHall::PlayerCount()
{
	s32 count = 0;
	map<u64, map<u64, CPlayerBase*> >::iterator git = m_Players.begin();
	for( ; git != m_Players.end(); ++git )
	{
		count += (s32)git->second.size();
	}
	return count;
}

void CHall::SetGameServerReady()
{
	m_bIsReady = true;
}

s32 CHall::FindFreeRoom(u64 groupid)
{
	map<s32, CRoomBase*>::iterator it = m_mapRoom.begin();
	for( ; it != m_mapRoom.end(); ++it )
	{
		if( it->second->CurState() == E_RoomState_Wait &&
			!it->second->IsGroupRoom() )
		{
			it->second->SetGroupRoom( groupid );
			return it->second->Id();
		}
	}

	return INVALID_VAL;
}

void CHall::_InitRooms()
{
	s32 end = m_pHallConfig->RoomIdFrom() + m_pHallConfig->RoomCount();
	for( s32 id=m_pHallConfig->RoomIdFrom(); id<end; ++id )
	{
		CRoomBase *room = m_RoomTemplate->Clone( id );
		SG_ASSERT( room != NULL );
		room->_Init();
		m_mapRoom[ id ] = room;
		room->ChangeState( E_RoomState_Wait );
		room->OnInit();
	}
}

void CHall::_EnterRoom(CPlayerBase *player, s32 roomid)
{
	SG_ASSERT( player );
	CRoomBase *room = FindRoom( roomid );
	s32 ret = sglib::errorcode::E_ErrorCode_Success;
	do
	{
		if( room == NULL )
		{
			// TODO room maybe on other hall
			ret = sglib::errorcode::E_ErrorCode_NoThisRoom;
			break;
		}

		if( player->GetRoom() )
		{
			ret = sglib::errorcode::E_ErrorCode_AleardyInRoom;
			break;
		}

		ret = room->PlayerEnter( player );
	} while(0);

	if( ret != sglib::errorcode::E_ErrorCode_Success )
	{
		_NotifyEnterRoomFailed( player, roomid, ret );
	}
}

void CHall::_NotifyEnterRoomFailed(CPlayerBase *player, s32 roomid, s32 result)
{
	SG_ASSERT( player );
	sglib::commonproto::SCEnterRoomRsp rsp;
	rsp.set_result( result );
	rsp.set_roomid( roomid );
	player->SendMsg( rsp, sglib::msgid::SC_ENTER_ROOM_RSP );	
}

void CHall::_LeaveRoom(CPlayerBase *player)
{
	SG_ASSERT( player );
	s32 ret = sglib::errorcode::E_ErrorCode_Success;
	CRoomBase *room = player->GetRoom(); 
	if( room == NULL )
	{
		ret = sglib::errorcode::E_ErrorCode_Success;
	}
	else
	{
		ret = room->PlayerLeave( player );
	}

	_NotifyLeaveRoomResult( player, ret );
}

void CHall::_NotifyLeaveRoomResult(CPlayerBase *player, s32 result)
{
	SG_ASSERT( player );
	sglib::commonproto::SCLeaveRoomRsp rsp;
	rsp.set_result( result );
	player->SendMsg( rsp, sglib::msgid::SC_LEAVE_ROOM_RSP );	
}

void CHall::_AskCenterEnterGame(CPlayerBase *player, s32 gameid)
{
	sglib::gamemanagerproto::ServerGamemanagerEnterGameReq req;
	req.set_clientid( player->ClientId() );
	req.set_gateid( player->GateId() );
	req.set_gameid( gameid );
	CServerManager::Instance().SendRpcMsg(  
		CServerManager::Instance().GetGameManagerServerId(),
		req, sglib::msgid::SGM_ENTER_GAME_REQ );
}

bool CHall::_ReportDynamicStarted()
{
	sglib::gamemanagerproto::ServerGamemanagerDynamicStartNtf ntf;
	
	const CServerConfig *cfg = CServerManager::Instance().GetServerConfig();
	if( !cfg )
	{
		SERVER_LOG_ERROR( "CHall,_ReportDynamicStarted,GetServerConfig" );
		return false;
	}

	const ServerConfig &_cfg = cfg->GetServerConfig();
	ntf.set_id( _cfg.m_Id );
	ntf.set_ip( _cfg.m_Ip );
	ntf.set_port( _cfg.m_Port );
	ntf.set_gameid( m_nGameId );
	
	CServerManager::Instance().SendRpcMsg(  
		CServerManager::Instance().GetGameManagerServerId(),
		ntf, sglib::msgid::SGM_DYNAMIC_START_NTF );

	return true;
}
