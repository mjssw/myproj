#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_

#include "cocos2d.h"
#include "Define.h"

class CNetManager
{
	SINGLETON_CLASS_DEFINE(CNetManager);

public:
	bool Init();
};

#endif
