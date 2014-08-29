#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QColor>
#include "user/UserManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    CUserManger *mgr = &CUserManger::Instance();

    engine.rootContext()->setContextProperty("mgr", mgr);
    engine.rootContext()->setContextProperty("cppColor", QColor(Qt::yellow));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));


    return app.exec();
}
