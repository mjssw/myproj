#include "SceneManager.h"

SIGNLETON_CLASS_INIT(CSceneManager);

void CSceneManager::SetCurView(CViewBase *curView)
{
	m_curView = curView;
}

CViewBase* CSceneManager::GetCurView()
{
	return m_curView;
}
