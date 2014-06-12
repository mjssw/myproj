#ifndef _STATE_MANAGER_H_
#define _STATE_MANAGER_H_

#include "Define.h"
#include "StateBase.h"
#include <map>

template<class StateOwner>
class CStateManager
{
public:
	CStateManager()
	{}

	~CStateManager()
	{
		typename std::map<s32, CStateBase<StateOwner>* >::iterator it = m_States.begin();
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
		typename std::map<s32, CStateBase<StateOwner>* >::iterator it = m_States.find( stateid );
		if( it != m_States.end() )
		{
			return it->second;
		}
		return NULL;
	}

	void AddStateProcMessage(s32 state, s32 msgid)
	{
		m_StateMsgMap[ state ][ msgid ] = msgid;
	}

	bool StateCanProcMessage(s32 state, s32 msgid)
	{
		std::map<s32, std::map<s32, s32> >::iterator it =  m_StateMsgMap.find( state );
		if( it != m_StateMsgMap.end() )
		{
			return (it->second.find(msgid) != it->second.end() );
		}
		return false;
	}

private:
	std::map<s32, CStateBase<StateOwner>* > m_States;
	std::map<s32, std::map<s32, s32> > m_StateMsgMap;
};

#endif
