#ifndef _QT_WRAPPER_H_
#define _QT_WARPPER_H_

#include <QObject>
#include <QVariant>

class CQtWrapper : public QObject
{
    Q_OBJECT

public:
    static CQtWrapper& Instance()
    {
        if( m_pInstance == NULL )
        {
            m_pInstance = new CQtWrapper();
        }
        return *m_pInstance;
    }
    ~CQtWrapper(){ delete m_pInstance; }
private:
    CQtWrapper(QObject* parent = Q_NULLPTR ) :
        QObject(parent),
        m_appObj(NULL)
    {}
    static CQtWrapper *m_pInstance;

private:
    QObject *m_appObj;
public:
    void SetAppObject(QObject *app);

    // signal to main thread to process net message
signals:
    void newMessageCome();

    // call qml function
signals:
    void userLoginFailed(QString errmsg);
    void userLoginSuccess();
    void userLoginGroupDone();
    void addGroup(quint64 id, QString name);
    void registerResult(bool result);
    void processGameMessage(int msgid, QVariant msgdata);

    // net process wrapper, called by qml
public slots:
    bool UserLogin(QString user, QString pwd);
    bool UserRegister(QString user, QString pwd);
    void SendHomeSignal();


    void TestCppCall();
    void TestCppCall2(QVariant msgdata);

    // get variant msg field
public slots:
    qint8 GetMsgDataS8(QVariant msgdata, int idx);
    quint8 GetMsgDataU8(QVariant msgdata, int idx);
    qint16 GetMsgDataS16(QVariant msgdata, int idx);
    quint16 GetMsgDataU16(QVariant msgdata, int idx);
    qint32 GetMsgDataS32(QVariant msgdata, int idx);
    quint32 GetMsgDataU32(QVariant msgdata, int idx);
    qint64 GetMsgDataS64(QVariant msgdata, int idx);
    quint64 GetMsgDataU64(QVariant msgdata, int idx);
    QString GetMsgDataStr(QVariant msgdata, int idx, int len);
};

#endif
