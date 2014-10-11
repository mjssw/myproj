#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
//#include <QTextCodec>
//#include <QObject>
#include <QDebug>
#include <QtCore>
#include <QString>
#include <QChar>
//#include <QtDeclarative/QDeclarativeContext>

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

void teststring()
{
    char s[10]= {12,96,0,59,29,5};
    std::string data;
    //data.resize(10);
    //data.copy(s, 5);
    data.append(s, 5);
    qDebug("len=%d", data.length());
    for(int i=0; i<5; ++i)
    {
        int a = (int)data.at(i);
        qDebug("a[%d]=%d", i, a );
    }

    /*
    QString qdata;
    qdata.append(s);
    qDebug("len2=%d", qdata.length());
    for(int i=0; i<5; ++i)
    {
        int a = (int)qdata.at(i).toLatin1();
        qDebug("a[%d]=%d", i, a );
    }
    //*/
}

int main(int argc, char *argv[])
{
    teststring();

    CUserManager::Instance().Instance();
    CNetManager::Instance().Init();

    CMainQApplication app( argc, argv );
    QQmlApplicationEngine engine;
    //QDeclarativeView view;

    /*
    QWidget *w = app.activeWindow();
    w->lower();
    //*/

    // let wrapper can be used in qml
    CQtWrapper *wrapper = &CQtWrapper::Instance();
    wrapper->SetAppObject( &app );
    engine.rootContext()->setContextProperty( "wrapper", wrapper );

    // set net msg process in main thread
    QObject::connect( wrapper, SIGNAL(newMessageCome()), &app, SLOT(messagePushed()), Qt::QueuedConnection);

    engine.load(QUrl(QStringLiteral("qrc:///groupgame.qml")));

    qDebug("==============[T:%d]run now(%d,%d)=================\n", QThread::currentThreadId(), a, b);

    return app.exec();
}
