#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QApplication>

class CMainQApplication : public QApplication
{
    Q_OBJECT
public:
    CMainQApplication(int &argc, char **argv, int flag= ApplicationFlags) :
        QApplication(argc, argv, flag)
    {}
    virtual ~CMainQApplication(){}

public slots:
    void messagePushed();
};

#endif // MAINAPPLICATION_H
