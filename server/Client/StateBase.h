#ifndef _STATE_BASE_H_
#define _STATE_BASE_H_

#include <map>
#include "Define.h"

template<class StateOwner>
class CStateMachineBase;

template<class StateOwner>
class CStateBase
{
public:
	CStateBase()
	{}
	virtual ~CStateBase(){}

	virtual void Enter(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Leave(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
	virtual void Update(StateOwner *owner, CStateMachineBase<StateOwner> *machine){}
};

template<class StateOwner>
class CStateMachineBase
{
public:
	CStateMachineBase() : 
		m_pCurState(NULL)
	{}
	virtual ~CStateMachineBase(){}

	void Update(StateOwner *owner)
	{
		if( m_pCurState )
		{
			m_pCurState->Update( owner, this );
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

protected:
	CStateBase<StateOwner> *m_pCurState;
};

template<class StateOwner>
class CStateManager
{
public:
	static CStateManager<StateOwner>& Instance()
	{
		if( m_pInstance == NULL )
		{
			m_pInstance = new CStateManager<StateOwner>();
		}
		return *m_pInstance;
	}
	
	~CStateManager()
	{
		SAFE_DELETE( m_pInstance );
		std::map<s32, CStateBase<StateOwner>* >::iterator it = m_States.begin();
		for( ; it != m_States.end(); ++it )
		{
			SAFE_DELETE( it->second );
		}
		m_States.clear();
	}

	void Register(s32 stateid, CStateBase<StateOwner> *state)
	{
		if( state )
		{
			m_States[ stateid ] = state;
		}
	}

	CStateBase<StateOwner>* FindState(s32 stateid)
	{
		std::map<s32, CStateBase<StateOwner>* >::iterator it = m_States.find( stateid );
		if( it != m_States.end() )
		{
			return it->second;
		}
		return NULL;
	}

private:
	CStateManager()
	{}

private:
	static CStateManager<StateOwner> *m_pInstance;
	std::map<s32, CStateBase<StateOwner>* > m_States;
};

#endif
