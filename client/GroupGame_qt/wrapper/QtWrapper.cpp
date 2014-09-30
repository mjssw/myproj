#include "QtWrapper.h"
#include "user/UserManager.h"
#include "net/NetManager.h"
#include <QDebug>
#include <QKeyEvent>
#include <QApplication>

CQtWrapper* CQtWrapper::m_pInstance = NULL;

void CQtWrapper::SetAppObject(QObject *app)
{
    m_appObj = app;
}

bool CQtWrapper::UserLogin(QString user, QString pwd)
{
    CUserManager::Instance().GetBasic().SetUser(
        user.toLatin1().data() );
    CUserManager::Instance().GetBasic().SetPwd(
        pwd.toLatin1().data() );

    qDebug("[CQtWrapper::UserLogin] %s:%s",
        CUserManager::Instance().GetBasic().GetUser().c_str(),
        CUserManager::Instance().GetBasic().GetPwd().c_str() );

    CUserManager::Instance().GetViewData().ResetLoginProgress();
    return CNetManager::Instance().StartLogin( LOGIN_IP, LOGIN_PORT );
}

bool CQtWrapper::UserRegister(QString user, QString pwd)
{
    CUserManager::Instance().GetBasic().SetRegUser(
        user.toLatin1().data() );
    CUserManager::Instance().GetBasic().SetRegPwd(
        pwd.toLatin1().data() );

    qDebug("[CQtWrapper::UserRegister] %s:%s",
        CUserManager::Instance().GetBasic().GetRegUser().c_str(),
        CUserManager::Instance().GetBasic().GetRegPwd().c_str() );

    return CNetManager::Instance().StartRegister( LOGIN_IP, LOGIN_PORT );
}

void CQtWrapper::SendHomeSignal()
{
    qDebug("CQtWrapper::SendHomeSignal");
    QKeyEvent keyEvent( QEvent::KeyRelease, Qt::Key_Home, Qt::NoModifier );
    QApplication::sendEvent( m_appObj, &keyEvent );
}
