#ifndef _ROOM_STATE_H_
#define _ROOM_STATE_H_

template<class StateOwner>
class CRoomStateMachine : public CStateMachineBase<StateOwner>
{
public:
	CRoomStateMachine() : CStateMachineBase<StateOwner>()
	{}
};

#endif
