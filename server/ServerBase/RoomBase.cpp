#include "RoomBase.h"
#include "PlayerBase.h"
#include "Hall.h"
#include "ServerManager.h"
#include "RoomStateManager.h"
#include "StackTrace.h"
using namespace SGLib;

#include "errno.pb.h"
#include "common.pb.h"
#include "msgid.pb.h"

CRoomBase::CRoomBase(s32 id) : 
	m_Id(id),
	m_PlayerCount(0),
	m_Players(NULL),
	m_pStateMachine(NULL)
{}

CRoomBase::~CRoomBase()
{
	SAFE_DELETE( m_pStateMachine );
	SAFE_DELETE_ARRAY( m_Players );
}

s32 CRoomBase::Id()
{
	return m_Id;
}

s32 CRoomBase::PlayerEnter(CPlayerBase *player)
{
	SG_ASSERT( player != NULL );
	SG_ASSERT( player->GetRoom() == NULL );

	if( m_PlayerCount >= CHall::Instance().RoomPlayerCount() )
	{
		return sglib::errorcode::E_ErrorCode_RoomIsFull;
	}

	_PlayerEnter( player );
	player->EnterRoom( this );

	return sglib::errorcode::E_ErrorCode_Success;
}

s32 CRoomBase::PlayerLeave(CPlayerBase *player)
{
	SG_ASSERT( player != NULL );
	SG_ASSERT( player->GetRoom() != NULL );

	_PlayerLeave( player );
	player->LeaveRoom();

	return sglib::errorcode::E_ErrorCode_Success;
}

void CRoomBase::ChangeState(s32 stateid)
{
	CStateBase<CRoomBase> *state = CRoomStateManager::Instance().FindState( stateid );
	if( state )
	{
		SG_ASSERT( m_pStateMachine );
		m_pStateMachine->ChangeState( state, this );
	}
	else
	{
		SERVER_LOG_ERROR( "CRoomBase,ChangeState," << stateid << "," << Id() );
	}
}

s32 CRoomBase::CurState()
{
	SELF_ASSERT( m_pStateMachine, return INVALIDE_STATE; );
	return m_pStateMachine->CurState();
}

void CRoomBase::Broadcast(const ::google::protobuf::Message &msg, s32 msgId, CPlayerBase *exclude)
{
	for( s32 i=0; i<m_PlayerCount; ++i )
	{
		SG_ASSERT( m_Players[i] != NULL );
		if( m_Players[i] != exclude )
		{
			m_Players[i]->SendMsg( msg, msgId );
		}
	}
}

bool CRoomBase::PlayerInRoom(CPlayerBase *player)
{
	bool ret = false;
	for( s32 i=0; i<m_PlayerCount; ++i )
	{
		SG_ASSERT( m_Players[i] != NULL );
		if( m_Players[i] == player )
		{
			ret = true;
			break;
		}
	}

	return ret;
}

void CRoomBase::_Init()
{
	s32 count = CHall::Instance().RoomPlayerCount();
	SG_ASSERT( count > 0 );
	m_Players = new CPlayerBase*[ count ];
	SG_ASSERT( m_Players > 0 );
	for( s32 i=0; i<count; ++i )
	{
		m_Players[i] = NULL;
	}
}

void CRoomBase::_PlayerEnter(CPlayerBase *player)
{
	SG_ASSERT( player != NULL );
	SG_ASSERT( m_PlayerCount < CHall::Instance().RoomPlayerCount() );
	m_Players[ m_PlayerCount ] = player;
	++m_PlayerCount;

	_NotifyEnterRoomSuccess( player, Id() );
	_NotifyPlayerEnter( player->UserId() );

	SERVER_LOG_INFO( "CRoomBase,_PlayerEnter," << Id() << "," << player->ClientId() << "," << player->UserId().c_str() );
}

void CRoomBase::_NotifyPlayerEnter(const std::string &userId)
{
	sglib::commonproto::SCEnterRoomNtf ntf;
	ntf.set_user( userId );
	Broadcast( ntf, sglib::msgid::SC_ENTER_ROOM_NTF );
}

void CRoomBase::_PlayerLeave(CPlayerBase *player)
{
	SG_ASSERT( player != NULL );
	SG_ASSERT( m_PlayerCount > 0 );

	s32 idx = INVALID_VAL;
	for( s32 i=0; i<m_PlayerCount; ++i )
	{
		if( m_Players[i] == player )
		{
			idx = i;
			break;
		}
	}

	SG_ASSERT( idx != INVALID_VAL );
	m_Players[idx] = NULL;
	m_Players[idx] = m_Players[m_PlayerCount-1];
	--m_PlayerCount;

	if( m_PlayerCount > 0 )
	{
		_NotifyPlayerLeave( player->UserId() );
	}
	
	SERVER_LOG_INFO( "CRoomBase,_PlayerLeave," << Id() << "," << player->ClientId() << "," << player->UserId().c_str() );
}

void CRoomBase::_NotifyPlayerLeave(const std::string &userId)
{
	sglib::commonproto::SCLeaveRoomNtf ntf;
	ntf.set_user( userId );
	Broadcast( ntf, sglib::msgid::SC_LEAVE_ROOM_NTF );
}

void CRoomBase::_NotifyEnterRoomSuccess(CPlayerBase *player, s32 roomid)
{
	SG_ASSERT( player != NULL );
	if( player )
	{
		sglib::commonproto::SCEnterRoomRsp rsp;
		rsp.set_result( sglib::errorcode::E_ErrorCode_Success );
		rsp.set_roomid( roomid );
		player->SendMsg( rsp, sglib::msgid::SC_ENTER_ROOM_RSP );	
	}
}
