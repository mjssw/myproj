#include "PlayerStateManager.h"
#include "PlayerBase.h"
#include "PlayerStateBase.h"
#include "Hall.h"
#include "msgid.pb.h"

INIT_SIGNLETON_CLASS(CPlayerStateManager);

// Íæ¼Ò»ù±¾×´Ì¬
template<class StateOwner>
class CPlayerStateFree : public CStateBase<StateOwner>
{
public:
	CPlayerStateFree(s32 state) : CStateBase<StateOwner>(state){}

	virtual void Enter(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Leave(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Update(StateOwner *owner, CStateMachineBase<StateOwner> *machine, s32 msgid, const byte *pkg, s32 len)
	{
		switch( msgid )
		{
		case sglib::msgid::CS_ENTER_ROOM_REQ:
			CHall::Instance().EnterRoom( owner, pkg, len );
			break;
		case sglib::msgid::CS_USER_ENTER_GAME_REQ:
			CHall::Instance().AskEnterGame( owner, pkg, len );
			break;
		default:
			if( CPlayerStateManager::Instance().StateCanProcMessage(this->m_State, msgid) )
			{
				owner->ProcMessage( msgid, pkg, len );
			}
			break;
		};
	}
};

template<class StateOwner>
class CPlayerStateInRoomFree : public CStateBase<StateOwner>
{
public:
	CPlayerStateInRoomFree(s32 state) : CStateBase<StateOwner>(state){}

	virtual void Enter(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Leave(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Update(StateOwner *owner, CStateMachineBase<StateOwner> *machine, s32 msgid, const byte *pkg, s32 len)
	{
		switch( msgid )
		{
		case sglib::msgid::CS_LEAVE_ROOM_REQ:
			CHall::Instance().LeaveRoom( owner, pkg, len );
			break;
		case sglib::msgid::CS_TETRIS_READY_REQ:
			owner->ProcMessage( msgid, pkg, len );
			break;
		case sglib::msgid::CS_USER_ENTER_GAME_REQ:
			CHall::Instance().AskEnterGame( owner, pkg, len );
			break;
		default:
			if( CPlayerStateManager::Instance().StateCanProcMessage(this->m_State, msgid) )
			{
				owner->ProcMessage( msgid, pkg, len );
			}
			break;
		};
	}
};

template<class StateOwner>
class CPlayerStateInRoomReady : public CStateBase<StateOwner>
{
public:
	CPlayerStateInRoomReady(s32 state) : CStateBase<StateOwner>(state){}

	virtual void Enter(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Leave(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Update(StateOwner *owner, CStateMachineBase<StateOwner> *machine, s32 msgid, const byte *pkg, s32 len)
	{
		switch( msgid )
		{
		case sglib::msgid::CS_LEAVE_ROOM_REQ:
			CHall::Instance().LeaveRoom( owner, pkg, len );
			break;
		case sglib::msgid::CS_USER_ENTER_GAME_REQ:
			CHall::Instance().AskEnterGame( owner, pkg, len );
			break;
		default:
			if( CPlayerStateManager::Instance().StateCanProcMessage(this->m_State, msgid) )
			{
				owner->ProcMessage( msgid, pkg, len );
			}
			break;
		};
	}
};

template<class StateOwner>
class CPlayerStateInGame : public CStateBase<StateOwner>
{
public:
	CPlayerStateInGame(s32 state) : CStateBase<StateOwner>(state){}

	virtual void Enter(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Leave(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Update(StateOwner *owner, CStateMachineBase<StateOwner> *machine, s32 msgid, const byte *pkg, s32 len)
	{
		if( CPlayerStateManager::Instance().StateCanProcMessage(this->m_State, msgid) )
		{
			owner->ProcMessage( msgid, pkg, len );
		}
	}
};

void CPlayerStateManager::InitPlayerStateManager()
{
	m_StateManager.Register( 
		E_PlayerState_Free, new CPlayerStateFree<CPlayerBase>(E_PlayerState_Free) );
	m_StateManager.Register( 
		E_PlayerState_InRoom_Free, new CPlayerStateInRoomFree<CPlayerBase>(E_PlayerState_InRoom_Free) );
	m_StateManager.Register( 
		E_PlayerState_InRoom_Ready, new CPlayerStateInRoomReady<CPlayerBase>(E_PlayerState_InRoom_Ready) );
	m_StateManager.Register( 
		E_PlayerState_InGame, new CPlayerStateInGame<CPlayerBase>(E_PlayerState_InGame) );
}

void CPlayerStateManager::AddStateProcMessage(s32 state, s32 msgid)
{
	m_StateManager.AddStateProcMessage( state, msgid );
}

bool CPlayerStateManager::StateCanProcMessage(s32 state, s32 msgid)
{
	return m_StateManager.StateCanProcMessage( state, msgid );
}

CStateBase<CPlayerBase>* CPlayerStateManager::FindState(s32 stateid)
{
	return m_StateManager.FindState( stateid );
}
