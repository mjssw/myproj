#include "Room.h"
#include "Player.h"
#include "Hall.h"
#include "StackTrace.h"
#include "PlayerState.h"
#include "ServerManager.h"
#include "StateManager.h"
#include "errno.pb.h"
#include "tetris.pb.h"
#include "msgid.pb.h"
#include "RoomState.h"
#include "PlayerStateBase.h"
#include "RoomStateBase.h"

CRoom::CRoom(s32 id) : 
	CRoomBase(id)
{
	m_pStateMachine = new CRoomStateMachine<CRoomBase>();
}

CRoom::~CRoom()
{
}

void CRoom::OnInit()
{
}

CRoomBase* CRoom::Clone(s32 id)
{
	CRoomBase *ret = new CRoom( id );
	SG_ASSERT( ret != NULL );
	return ret;
}

void CRoom::PlayerReady(CPlayer *player)
{
	SG_ASSERT( player != NULL );
	SG_ASSERT( player->GetRoom() == this );
	SG_ASSERT( PlayerInRoom(player) );

	s32 ret = player->Ready();
	_PlayerReadyResponse( player, ret );

	if( ret == sglib::errorcode::E_ErrorCode_Success )
	{
		SERVER_LOG_DEBUG( "player:" << player->UserId().c_str() << " ready" );

		_NotifyPlayerReady( player );
		_CheckGameStart();
	}
}

void CRoom::PlayerLost(CPlayer *player)
{
	SG_ASSERT( player != NULL );
	SG_ASSERT( player->GetRoom() == this );
	SG_ASSERT( PlayerInRoom(player) );

	SERVER_LOG_DEBUG( "player:" << player->UserId().c_str() << " lost" );

	_GameEnd( player );
	_NotifyGameEnd( player->UserId() );
}

void CRoom::PlayerPutBlock(CPlayer *player, s32 blockid, s32 column)
{
	SG_ASSERT( player != NULL );
	SG_ASSERT( player->GetRoom() == this );
	SG_ASSERT( PlayerInRoom(player) );

	SERVER_LOG_DEBUG( "player:" << player->UserId().c_str() << " put block:" << blockid << " at:" << column );

	_PutBlock( player, blockid, column );
}

void CRoom::PlayerClearBlock(CPlayer *player, const s32 *rows, s32 count)
{
	SG_ASSERT( player != NULL );
	SG_ASSERT( player->GetRoom() == this );
	SG_ASSERT( PlayerInRoom(player) );

	SERVER_LOG_DEBUG( "player:" << player->UserId().c_str() << " clear " << count << " rows" );

	_ClearBlock( player, rows, count );
}

void CRoom::PlayerForceLeave(CPlayer *player)
{
	SG_ASSERT( player != NULL );
	SG_ASSERT( player->GetRoom() == this );
	SG_ASSERT( PlayerInRoom(player) );
	SG_ASSERT( m_pStateMachine != NULL );

	SERVER_LOG_DEBUG( "player:" << player->UserId().c_str() << " force leave" );

	if( m_pStateMachine->CurState() == E_RoomState_InGame )
	{
		SERVER_LOG_DEBUG( "player:" << player->UserId().c_str() << " force leave, game over" );

		SG_ASSERT( player->CurState() == E_PlayerState_InGame );
		PlayerLost( player );
	}

	CRoomBase::PlayerLeave( player );
}

void CRoom::NotifyRoomInfo(CPlayer *player)
{
	SG_ASSERT( player != NULL );
	SG_ASSERT( player->GetRoom() == this );
	SG_ASSERT( PlayerInRoom(player) );

	sglib::tetrisproto::SCMsgRoomInfoNtf ntf;
	for( s32 i=0; i<m_PlayerCount; ++i )
	{
		CPlayer *pPlayer = (CPlayer*)m_Players[i];
		if( player != pPlayer )
		{
			sglib::publicproto::RoomPlayerInfo *info = ntf.add_players();
			SG_ASSERT( info != NULL );
			info->set_user( pPlayer->UserId() );
			info->set_isready( pPlayer->IsReady()?1:0 );
		}
	}
	player->SendMsg( ntf, sglib::msgid::SC_TETRIS_ROOM_INFO_NOTIFY );
}

void CRoom::_CheckGameStart()
{
	if( m_PlayerCount == CHall::Instance().RoomPlayerCount() )
	{
		bool isAllReady = true;
		for( s32 i=0; i<m_PlayerCount; ++i )
		{
			CPlayer *player = (CPlayer*)m_Players[i];
			SG_ASSERT( player != NULL );
			if( !player->IsReady() )
			{
				isAllReady = false;
				break;
			}
		}

		if( isAllReady )
		{
			_GameStart();
			_NotifyGameStart();
		}
	}
}

void CRoom::_PlayerReadyResponse(CPlayer *player, s32 result)
{
	SG_ASSERT( player != NULL );

	sglib::tetrisproto::SCMsgReadyRsp rsp;
	rsp.set_result( result );
	player->SendMsg( rsp, sglib::msgid::SC_TETRIS_READY_RSP );
}
	
void CRoom::_GameStart()
{
	SERVER_LOG_INFO( "GameStart," << Id() );

	ChangeState( E_RoomState_InGame );

	SG_ASSERT( m_PlayerCount == CHall::Instance().RoomPlayerCount() );
	for( s32 i=0; i<m_PlayerCount; ++i )
	{
		CPlayer *player = (CPlayer*)m_Players[i];
		SG_ASSERT( player && player->IsReady() );
		player->GameStart();
	}
}

void CRoom::_NotifyGameStart()
{
	sglib::tetrisproto::SCMsgGameStartNtf ntf;
	// TODO
	ntf.set_blockseed( 100 );
	Broadcast( ntf, sglib::msgid::SC_TETRIS_GAMESTART_NOTIFY );
}

void CRoom::_GameEnd(CPlayer *loser)
{
	SG_ASSERT( loser != NULL );

	SERVER_LOG_INFO( "GameEnd," << Id() << "," << loser->UserId().c_str() );

	ChangeState( E_RoomState_Wait );

	SG_ASSERT( m_PlayerCount == CHall::Instance().RoomPlayerCount() );
	for( s32 i=0; i<m_PlayerCount; ++i )
	{
		CPlayer *player = (CPlayer*)m_Players[i];
		SG_ASSERT( player );
		player->GameEnd();
	}
}

void CRoom::_NotifyGameEnd(const std::string &loser)
{
	sglib::tetrisproto::SCMsgGameEndNtf ntf;
	ntf.set_loser( loser );
	Broadcast( ntf, sglib::msgid::SC_TETRIS_GAMEEND_NOTIFY );
}

void CRoom::_PutBlock(CPlayer *player, s32 blockid, s32 column)
{
	SG_ASSERT( player != NULL );

	if( player->PutBlock( blockid, column ) )
	{
		_NotifyPutBlock( player, blockid, column );
	}
	else
	{
		PlayerLost( player );
	}
}

void CRoom::_ClearBlock(CPlayer *player, const s32 *rows, s32 count)
{
	SG_ASSERT( player != NULL );

	player->ClearBlock( rows, count );
	_NotifyClearBlock( player, rows, count );
}

void CRoom::_NotifyPutBlock(CPlayer *player, s32 blockid, s32 column)
{
	SG_ASSERT( player != NULL );

	sglib::tetrisproto::SCMsgPutBlockNtf ntf;
	ntf.set_blockid( blockid );
	ntf.set_column( column );
	Broadcast( ntf, sglib::msgid::SC_TETRIS_PUT_BLOCK_NOTIFY, player );
}

void CRoom::_NotifyClearBlock(CPlayer *player, const s32 *rows, s32 count)
{
	sglib::tetrisproto::SCMsgClearBlockNtf ntf;
	for( s32 i=0; i<count; ++i )
	{
		ntf.add_rows( rows[i] );
	}
	Broadcast( ntf, sglib::msgid::SC_TETRIS_CLEAR_BLOCK_NOTIFY, player );
}

void CRoom::_NotifyPlayerReady(CPlayer *player)
{
	SG_ASSERT( player != NULL );
	
	sglib::tetrisproto::SCMsgReadyNtf ntf;
	ntf.set_player( player->UserId() );
	Broadcast( ntf, sglib::msgid::SC_TETRIS_READY_NOTIFY, player );
}
