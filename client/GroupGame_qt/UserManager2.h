#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QDebug>
#include <string>

extern void testprotobuf();

class CUserManger2 : public QObject
{
    Q_OBJECT

    // 注册属性
    Q_PROPERTY( QString User READ GetUser WRITE SetUser )

    // 单件
public:
    static CUserManger2& Instance()
    {
        if( m_pInstance == NULL )
        {
            m_pInstance = new CUserManger2();
        }
        return *m_pInstance;
    }
    ~CUserManger2(){ delete m_pInstance; }
private:
    CUserManger2(QObject* parent = Q_NULLPTR ) :
        QObject(parent)
    {}
    static CUserManger2 *m_pInstance;

public:
    // (1) qml 调用函数
    Q_INVOKABLE QString testmethod( void )
    {
        qDebug( "[C++]%s is called.", __FUNCTION__ );
        testprotobuf();
        return "call_cpp_testmethon";
    }

    // (2) qml 属性测试
    const QString& GetUser();
    void SetUser(const QString &user);

signals:
    // (3) c++ 调用qml 函数
    void resetLabel(QString text);//用于调用qml的javascript 方法,首字母必须小写

public slots:
    // (4) qml 调用函数(第二种方式)
    QString getText1(){ return QString("cpptext1");}
    QString getText2(){ return "text2";}
    void func1(QString text)
    {
        qDebug( "[C++]%s is called, param:%s", __FUNCTION__, text.toLatin1().data() );
        emit resetLabel(text);
    }

private:
    QString m_user;
};

#endif // USERMANAGER_H
