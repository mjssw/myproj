#ifndef _USER_MANAGER_H_
#define _USER_MANAGER_H_

#include "Define.h"
#include "Basic.h"
#include "ViewData.h"
#include <string>

class CUserManager
{
	SINGLETON_CLASS_DEFINE(CUserManager);

public:
	CUserBasic& GetBasic();
	CUserViewData& GetViewData(); 

private:
	CUserBasic m_basic;
	CUserViewData m_viewData;
};

#endif
