#ifndef _PLAYER_STATE_MANAGER_H_
#define _PLAYER_STATE_MANAGER_H_

#include "Utils.h"
#include "StateManager.h"

class CPlayerBase;
class CPlayerStateManager
{
	DEFINE_SINGLETON_CLASS(CPlayerStateManager);

public:
	void InitPlayerStateManager();
	void AddStateProcMessage(s32 state, s32 msgid);
	bool StateCanProcMessage(s32 state, s32 msgid);
	CStateBase<CPlayerBase>* FindState(s32 stateid);

private:
	CStateManager<CPlayerBase> m_StateManager;
};

#endif
