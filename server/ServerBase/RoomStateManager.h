#ifndef _ROOM_STATE_MANAGER_H_
#define _ROOM_STATE_MANAGER_H_

#include "Utils.h"
#include "StateManager.h"

class CRoomBase;
class CRoomStateManager
{
	DEFINE_SINGLETON_CLASS(CRoomStateManager);

public:
	void InitRoomStateManager();
	void AddStateProcMessage(s32 state, s32 msgid);
	CStateBase<CRoomBase>* FindState(s32 stateid);

private:
	CStateManager<CRoomBase> m_StateManager;
};

#endif
