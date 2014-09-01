#include <QApplication>
#include <QQmlApplicationEngine>
#include <QTextCodec>
//#include <QFont>

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));

    QApplication app(argc, argv);
   // QFont newFont("宋体", 8, QFont::Bold, true);
    //app.setFont();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///groupgame.qml")));

    return app.exec();
}
