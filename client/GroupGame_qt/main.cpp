#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTextCodec>
#include <QObject>
#include <QDebug>
#include <QtCore>

#include "../../protomsg/msg/group.pb.h"
#include "net/NetManager.h"
#include "user/UserManager.h"
#include "wrapper/QtWrapper.h"
#include "wrapper/MainApplication.h"

void testprotobuf()
{
    sglib::groupproto::CSGroupCreateReq msg;
    msg.set_name("abc");
    msg.set_head("default");
    int total = msg.ByteSize();
    char buf[1024] = {0};
    bool ret = msg.SerializeToArray(buf, 1024);
    qDebug("totalLen=%d ret=%s", total, ret?"true":"false");

    ret = CNetManager::Instance().StartLogin(LOGIN_IP, LOGIN_PORT);
    qDebug("connect login result:%s", ret?"true":"false");

}

#if MYDEBUG
int a = 10;
#endif

#if QT_VERSION
int b = 11;
#endif

int main(int argc, char *argv[])
{
    CUserManager::Instance().Instance();
    CNetManager::Instance().Init();

    CMainQApplication app( argc, argv );
    QQmlApplicationEngine engine;

    // let wrapper can be used in qml
    CQtWrapper *wrapper = &CQtWrapper::Instance();
    engine.rootContext()->setContextProperty( "wrapper", wrapper );

    // set net msg process in main thread
    QObject::connect( wrapper, SIGNAL(newMessageCome()), &app, SLOT(messagePushed()), Qt::QueuedConnection);

    engine.load(QUrl(QStringLiteral("qrc:///groupgame.qml")));

    qDebug("==============[T:%d]run now(%d,%d)=================\n", QThread::currentThreadId(), a, b);

    return app.exec();
}
