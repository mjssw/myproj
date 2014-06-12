#ifndef _ROOM_STATE_BASE_H_
#define _ROOM_STATE_BASE_H_

//#include "StateBase.h"

enum ERoomState
{
	E_RoomState_Wait = 100,
	E_RoomState_InGame,
};

/*
template<class StateOwner>
class CRoomStateWait : public CStateBase<StateOwner>
{
public:
	CRoomStateWait(s32 state) : CStateBase<StateOwner>(state){}

	virtual void Enter(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Leave(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Update(StateOwner *owner, CStateMachineBase<StateOwner> *machine, s32 msgid, const byte *pkg, s32 len)
	{
	}
};

template<class StateOwner>
class CRoomStateInGame : public CStateBase<StateOwner>
{
public:
	CRoomStateInGame(s32 state) : CStateBase<StateOwner>(state){}
	
	virtual void Enter(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Leave(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Update(StateOwner *owner, CStateMachineBase<StateOwner> *machine, s32 msgid, const byte *pkg, s32 len)
	{	
	}
};

//*/

#endif
