#include "UserManager.h"

SIGNLETON_CLASS_INIT(CUserManager);


CUserBasic& CUserManager::GetBasic()
{
	return m_basic;
}

CUserViewData& CUserManager::GetViewData()
{
	return m_viewData;
}
