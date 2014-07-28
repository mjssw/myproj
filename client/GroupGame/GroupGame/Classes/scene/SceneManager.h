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
	};

	void SetCurView(CViewBase *curView);
	CViewBase* GetCurView();

private:
	CViewBase *m_curView;
};

#endif
