#ifndef _STATE_BASE_H_
#define _STATE_BASE_H_

#include "Define.h"

#define INVALIDE_STATE	(-1111)

template<class StateOwner>
class CStateMachineBase;

template<class StateOwner>
class CStateBase
{
public:
	CStateBase(s32 state) : m_State(state)
	{}
	virtual ~CStateBase(){}

	s32 State() const { return m_State; }

	virtual void Enter(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Leave(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Update(StateOwner *owner, CStateMachineBase<StateOwner> *machine, s32 msgid, const byte *pkg, s32 len){}

protected:
	s32 m_State;
};

template<class StateOwner>
class CStateMachineBase
{
public:
	CStateMachineBase() : 
		m_pCurState(NULL)
	{}
	virtual ~CStateMachineBase(){}

	void Update(StateOwner *owner, s32 msgid, const byte *pkg, s32 len)
	{
		if( m_pCurState )
		{
			m_pCurState->Update( owner, this, msgid, pkg, len );
		}
	}

	void ChangeState(CStateBase<StateOwner> *pState, StateOwner *owner)
	{
		if( !pState )
		{
			return;
		}

		if( m_pCurState )
		{
			m_pCurState->Leave( owner, this );
		}

		m_pCurState = pState;
		m_pCurState->Enter( owner, this );
	}

	s32 CurState()
	{
		if( m_pCurState )
		{
			return m_pCurState->State();
		}
		return INVALIDE_STATE;	
	}

protected:
	CStateBase<StateOwner> *m_pCurState;
};

#endif
