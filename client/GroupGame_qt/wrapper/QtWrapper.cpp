#include "QtWrapper.h"
#include "user/UserManager.h"
#include "net/NetManager.h"
#include <QDebug>

CQtWrapper* CQtWrapper::m_pInstance = NULL;

bool CQtWrapper::UserLogin(QString user, QString pwd)
{
    CUserManager::Instance().GetBasic().SetUser(
        user.toLatin1().data() );
    CUserManager::Instance().GetBasic().SetPwd(
        pwd.toLatin1().data() );

    qDebug("[CQtWrapper::mUserLogin] %s:%s",
        CUserManager::Instance().GetBasic().GetUser().c_str(),
        CUserManager::Instance().GetBasic().GetPwd().c_str() );


    return CNetManager::Instance().StartLogin( LOGIN_IP, LOGIN_PORT );
}

