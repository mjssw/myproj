#ifndef _PLAYER_STATE_H_
#define _PLAYER_STATE_H_

#include "StateBase.h"

template<class StateOwner>
class CPlayerStateMachine : public CStateMachineBase<StateOwner>
{
public:
	CPlayerStateMachine() : CStateMachineBase<StateOwner>()
	{}
};

#endif
