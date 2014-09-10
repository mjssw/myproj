#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTextCodec>
//#include <QFont>
#include <QObject>
#include <QDebug>

#include "../../protomsg/msg/group.pb.h"
#include "UserManager.h"

void testprotobuf()
{
    sglib::groupproto::CSGroupCreateReq msg;
    msg.set_name("abc");
    msg.set_head("default");
    int total = msg.ByteSize();
    char buf[1024] = {0};
    int ret = msg.SerializeToArray(buf, 1024);
    qDebug("totalLen=%d ret=%d", total, ret);
}

#if MYDEBUG
int a = 10;
#endif

#if QT_VERSION
int b = 11;
#endif

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
    //testprotobuf();

    QApplication app(argc, argv);
   // QFont newFont("宋体", 8, QFont::Bold, true);
    //app.setFont();

    QQmlApplicationEngine engine;

    //CTest1 *mgr = new CTest1();
    CUserManger *mgr = &CUserManger::Instance();

    engine.rootContext()->setContextProperty("mgr", mgr);

    engine.load(QUrl(QStringLiteral("qrc:///groupgame.qml")));

    qDebug("==============run now(%d,%d)=================\n", a, b);
    return app.exec();
}
