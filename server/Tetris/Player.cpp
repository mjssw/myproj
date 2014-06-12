#include "Player.h"
#include "PlayerState.h"
#include "StackTrace.h"
#include "Room.h"
#include "errno.pb.h"
#include "msgid.pb.h"
#include "tetris.pb.h"
#include "ServerManager.h"
#include "StateManager.h"
#include "PlayerStateBase.h"
#include <stdio.h>
#include <stdlib.h>

CPlayer::CPlayer() : 
	CPlayerBase()
{
}

CPlayer::CPlayer(u64 gateId, u64 clientId, const std::string &userId) : 
	CPlayerBase(gateId, clientId, userId)
{
	m_pStateMachine = new CPlayerStateMachine<CPlayerBase>();

	_Register( sglib::msgid::CS_TETRIS_READY_REQ, &CPlayer::_ReadyProc );
	_Register( sglib::msgid::CS_TETRIS_GAMELOST_REQ, &CPlayer::_GameLostProc );
	_Register( sglib::msgid::CS_TETRIS_PUT_BLOCK_REQ, &CPlayer::_PutBlockProc );
	_Register( sglib::msgid::CS_TETRIS_CLEAR_BLOCK_REQ, &CPlayer::_ClearBlockProc );
}

CPlayer::~CPlayer()
{
}

CPlayerBase* CPlayer::Clone(u64 gateId, u64 clientId, const std::string &userId)
{
	CPlayerBase *player = new CPlayer(
		gateId,
		clientId,
		userId );
	SG_ASSERT( player != NULL );
	return player;
}

void CPlayer::OnEnterHall()
{
}

void CPlayer::OnEnterRoom()
{
	SG_ASSERT( m_pRoom != NULL );
	CRoom *room = (CRoom*)m_pRoom;
	room->NotifyRoomInfo( this );
}

void CPlayer::OnLeaveRoom()
{
}

void CPlayer::ProcMessage(s32 msgid, const byte *pkg, s32 len)
{
	map<s32, PlayerMsgProcFunc>::iterator it = m_mapMsgProc.find( msgid );
	if( it != m_mapMsgProc.end() )
	{
		(this->*it->second)( msgid, pkg, len );
	}
	else
	{
		SERVER_LOG_ERROR( "CPlayer,ProcMessage," << msgid << "," << UserId().c_str() );
	}
}

s32 CPlayer::Ready()
{
	ChangeState( E_PlayerState_InRoom_Ready );
	return sglib::errorcode::E_ErrorCode_Success;
}

bool CPlayer::IsReady()
{
	return (m_pStateMachine->CurState() == E_PlayerState_InRoom_Ready);
}

void CPlayer::GameStart()
{
	ChangeState( E_PlayerState_InGame );
	m_GameScene.Init();
}

void CPlayer::GameEnd()
{
	ChangeState( E_PlayerState_InRoom_Free );
}

bool CPlayer::PutBlock(s32 blockid, s32 column)
{
	SERVER_LOG_DEBUG( "player:" << UserId().c_str() << " put block:" << blockid << " at column:" << column );
	bool ret = m_GameScene.PutBlock( blockid, column );
	m_GameScene.DebugDisplayScene();
	return ret;
}

void CPlayer::ClearBlock(const s32 *rows, s32 count)
{
	string str = "[";
	for( s32 i=0; i<count; ++i )
	{
		char num[32] = {0};
		sprintf( num, "%d,", rows[i] );
		str += num;
	}
	str += "]";
	SERVER_LOG_ERROR( "player:" << UserId().c_str() << " clear rows:" << str.c_str() );

	m_GameScene.ClearBlock( rows, count );
	m_GameScene.DebugDisplayScene();
}

s32 CPlayer::CurState()
{
	SG_ASSERT( m_pStateMachine != NULL );
	return m_pStateMachine->CurState();
}

void CPlayer::_Register(s32 msgid, PlayerMsgProcFunc func)
{
	if( func )
	{
		m_mapMsgProc[ msgid ] = func;
	}
}

void CPlayer::_ReadyProc(s32 msgid, const byte *pkg, s32 len)
{
	sglib::tetrisproto::CSMsgReadyReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		_Ready();
	}
	else
	{
		SERVER_LOG_ERROR( "CPlayer,_ReadyProc,ParseFromArray" );
	}
}

void CPlayer::_GameLostProc(s32 msgid, const byte *pkg, s32 len)
{
	sglib::tetrisproto::CSMsgGameLostReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		_Lost();
	}
	else
	{
		SERVER_LOG_ERROR( "CTetrisClient,_GameLostProc,ParseFromArray" );
	}
}

void CPlayer::_PutBlockProc(s32 msgid, const byte *pkg, s32 len)
{
	sglib::tetrisproto::CSMsgPutBlockReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		_PutBlock( req.blockid(), req.column() );
	}
	else
	{
		SERVER_LOG_ERROR( "CPlayer,_PutBlockProc,ParseFromArray" );
	}
}

void CPlayer::_ClearBlockProc(s32 msgid, const byte *pkg, s32 len)
{
	sglib::tetrisproto::CSMsgClearBlockReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		_ClearBlock( req.rows().data(), req.rows().size() );
	}
	else
	{
		SERVER_LOG_ERROR( "CPlayer,_ClearBlockProc,ParseFromArray" );
	}
}

void CPlayer::_Ready()
{
	SERVER_LOG_DEBUG( "player:"<< UserId().c_str() << " ready" );

	CRoomBase *pRoom = GetRoom();
	if( pRoom != NULL )
	{
		CRoom *room = (CRoom*)pRoom;
		room->PlayerReady( this );
	}
	else
	{
		SERVER_LOG_ERROR( "CPlayer,_Ready,GetRoom," << UserId().c_str() );
	}
}

void CPlayer::_Lost()
{
	SERVER_LOG_DEBUG( "player:"<< UserId().c_str() << " lost" );

	CRoomBase *pRoom = GetRoom();
	if( pRoom != NULL )
	{
		CRoom *room = (CRoom*)pRoom;
		room->PlayerLost( this );
	}
	else
	{
		SERVER_LOG_ERROR( "CPlayer,_Lost,GetRoom," << UserId().c_str() );
	}
}

void CPlayer::_PutBlock(s32 blockid, s32 column)
{
	SERVER_LOG_DEBUG( "player:"<< UserId().c_str() << " put block:" << blockid << "," << column );

	CRoomBase *pRoom = GetRoom();
	if( pRoom != NULL )
	{
		CRoom *room = (CRoom*)pRoom;
		room->PlayerPutBlock( this, blockid, column );
	}
	else
	{
		SERVER_LOG_ERROR( "CPlayer,_PutBlock,GetRoom," << UserId().c_str() );
	}
}

void CPlayer::_ClearBlock(const s32 *rows, s32 count)
{
	string clear = "(";
	for( s32 i=0; i<count; ++i )
	{
		char numstr[16] = {0};
#ifdef _USE_WIN32
		clear += itoa( rows[i], numstr, sizeof(numstr) );
#else
        snprintf( numstr, sizeof(numstr), "%d", rows[i] );
#endif
		clear += ",";
	}
	clear += ")";
	SERVER_LOG_DEBUG( "player:"<< UserId().c_str() << " clear block:" << clear.c_str() );

	CRoomBase *pRoom = GetRoom();
	if( pRoom != NULL )
	{
		CRoom *room = (CRoom*)pRoom;
		if( count > 0 )
		{
			room->PlayerClearBlock( this, rows, count );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CPlayer,_ClearBlock,GetRoom," << UserId().c_str() );
	}
}
