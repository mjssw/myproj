#ifndef _USER_MANAGER_H_
#define _USER_MANAGER_H_

#include "Define.h"
#include <map>
#include <string>

class CUser;
class CUserManager
{
public:
	~CUserManager(){}
	static CUserManager& Instance()
	{
		if( m_pInstance == NULL )
		{
			m_pInstance = new CUserManager();
		}
		return *m_pInstance;
	}

protected:
	CUserManager(){}
	
	static CUserManager *m_pInstance;
	std::map<std::string, CUser*> m_mapUsers;
};

#endif
