#include "QtWrapper.h"
#include "user/UserManager.h"
#include "net/NetManager.h"
#include <QDebug>
#include <QKeyEvent>
#include <QApplication>
#include <QWidget>
#include <QDesktopWidget>
#include <QWindow>
#include <QWindowList>

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

    //QKeyEvent keyEvent( QEvent::KeyRelease, Qt::Key_Home, Qt::NoModifier );
    //QApplication::sendEvent( m_appObj, &keyEvent );

    QApplication *app = (QApplication*)m_appObj;
    //QWidget *w = app->activeWindow();
    //w->lower();

    //w->showMinimized();
    //w->showFullScreen();
    //w->lower();
    //qDebug("w=%d h=%d", w->width(), w->height());

    QWindowList list = app->allWindows();
    //QWidgetList  list = app->allWidgets();
    int count = list.count();
    QWindow *w = list.at(0);
    //w->showMinimized();
    w->lower();
    qDebug("w=%d h=%d", w->width(), w->height());
    qDebug("widget count=%d", count);
    //*/
}

qint8 CQtWrapper::GetMsgDataS8(QVariant msgdata, int idx)
{
    const char *data = (const char*)msgdata.value<void*>();
    return (qint8)ntohs( *((qint8*)(data+idx)) );
}

quint8 CQtWrapper::GetMsgDataU8(QVariant msgdata, int idx)
{
    const char *data = (const char*)msgdata.value<void*>();
    return (quint8)ntohs( *((quint8*)(data+idx)) );
}

qint16 CQtWrapper::GetMsgDataS16(QVariant msgdata, int idx)
{
    const char *data = (const char*)msgdata.value<void*>();
    return (qint16)ntohs( *((qint16*)(data+idx)) );
}

quint16 CQtWrapper::GetMsgDataU16(QVariant msgdata, int idx)
{
    const char *data = (const char*)msgdata.value<void*>();
    return (quint16)ntohs( *((quint16*)(data+idx)) );
}

qint32 CQtWrapper::GetMsgDataS32(QVariant msgdata, int idx)
{
    const char *data = (const char*)msgdata.value<void*>();
    return (qint32)ntohl( *((qint32*)(data+idx)) );
}

quint32 CQtWrapper::GetMsgDataU32(QVariant msgdata, int idx)
{
    const char *data = (const char*)msgdata.value<void*>();
    return (quint32)ntohl( *((quint32*)(data+idx)) );
}

qint64 CQtWrapper::GetMsgDataS64(QVariant msgdata, int idx)
{
    const char *data = (const char*)msgdata.value<void*>();
    return (qint64)ntohl( *((qint64*)(data+idx)) );
}

quint64 CQtWrapper::GetMsgDataU64(QVariant msgdata, int idx)
{
    const char *data = (const char*)msgdata.value<void*>();
    return (quint64)ntohl( *((quint64*)(data+idx)) );
}

QString CQtWrapper::GetMsgDataStr(QVariant msgdata, int idx, int len)
{
    const char *data = (const char*)msgdata.value<void*>();
    return QString( QLatin1String(data+idx, len) );
}


char s[10]= {12,96,0,59,29,5};
void CQtWrapper::TestCppCall()
{
    qDebug("CQtWrapper::TestCppCall");

    QVariant v = qVariantFromValue((void *)s);
    emit processGameMessage(111, v);
}

void CQtWrapper::TestCppCall2(QVariant msgdata)
{
    qDebug("CQtWrapper::TestCppCall2");
    const char *data = (const char*)msgdata.value<void *>();
    for(int i=0; i<5; ++i)
    {
        qDebug("data[%d]=%d", i, (int)data[i]);
    }
}

