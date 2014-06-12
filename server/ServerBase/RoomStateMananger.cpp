#include "RoomStateManager.h"
#include "RoomBase.h"
#include "RoomStateBase.h"

INIT_SIGNLETON_CLASS(CRoomStateManager);

// 房间基本状态
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

void CRoomStateManager::InitRoomStateManager()
{
	m_StateManager.Register( 
		E_RoomState_Wait, new CRoomStateWait<CRoomBase>(E_RoomState_Wait) );
	m_StateManager.Register( 
		E_RoomState_InGame, new CRoomStateInGame<CRoomBase>(E_RoomState_InGame) );
}

void CRoomStateManager::AddStateProcMessage(s32 state, s32 msgid)
{
	m_StateManager.AddStateProcMessage( state, msgid );
}

CStateBase<CRoomBase>* CRoomStateManager::FindState(s32 stateid)
{
	return m_StateManager.FindState( stateid );
}
