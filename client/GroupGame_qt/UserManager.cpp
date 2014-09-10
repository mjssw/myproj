#include "UserManager.h"
using namespace std;

SIGNLETON_CLASS_INIT(CUserManger);

const QString& CUserManger::GetUser()
{
    return m_user;
}

void CUserManger::SetUser(const QString &user)
{
    m_user = user;
}
