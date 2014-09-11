#ifndef _QT_WRAPPER_H_
#define _QT_WARPPER_H_

#include <QObject>

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
        QObject(parent)
    {}
    static CQtWrapper *m_pInstance;

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

    // net process wrapper, called by qml
public slots:
    bool UserLogin(QString user, QString pwd);
    bool UserRegister(QString user, QString pwd);
};

#endif
