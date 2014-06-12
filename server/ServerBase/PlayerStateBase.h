#ifndef _PLAYER_STATE_BASE_H_
#define _PLAYER_STATE_BASE_H_

//#include "StateBase.h"
//#include "Hall.h"
//#include "msgid.pb.h"

enum EPlayerState
{
	E_PlayerState_Free = 1,
	E_PlayerState_InRoom_Free,
	E_PlayerState_InRoom_Ready,
	E_PlayerState_InGame,
};

/*
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
			if( CStateManager<CPlayerBase>::Instance().StateCanProcMessage(m_State, msgid) )
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
			if( CStateManager<CPlayerBase>::Instance().StateCanProcMessage(m_State, msgid) )
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
			if( CStateManager<CPlayerBase>::Instance().StateCanProcMessage(m_State, msgid) )
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
		if( CStateManager<CPlayerBase>::Instance().StateCanProcMessage(m_State, msgid) )
		{
			owner->ProcMessage( msgid, pkg, len );
		}
	}
};

//*/

#endif
