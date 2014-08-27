#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "Define.h"

class CViewBase;
class CSceneManager
{
	SINGLETON_CLASS_DEFINE(CSceneManager);

public:
	enum
	{
		E_UpdateType_ConnectLogin,
		E_UpdateType_LoadingProgress,
		E_UpdateType_LoginFailed, 
		E_UpdateType_ConnectLoginError,
		E_UpdateType_ConnectGroup,
		E_UpdateType_LoginGroup,
		E_UpdateType_UpdateGroupList,
		E_UpdateType_CloseRegisterDialog,
		E_UpdateType_ConnectRegister,
		E_UpdateType_RegisterResult,
		E_UpdateType_AskJoinGroup,
		E_UpdateType_GroupMessage,
		E_UpdateType_GameRoomUpdate,
		E_UpdateType_ConnectGame,
	};

	void SetCurView(CViewBase *curView);
	CViewBase* GetCurView();

private:
	CViewBase *m_curView;
};

#endif
