#include "UserManager2.h"
using namespace std;

CUserManger2* CUserManger2::m_pInstance = NULL;

const QString& CUserManger2::GetUser()
{
    return m_user;
}

void CUserManger2::SetUser(const QString &user)
{
    m_user = user;
}
