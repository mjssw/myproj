#include "MainApplication.h"
#include "net/NetManager.h"
#include <QDebug>

void CMainQApplication::messagePushed()
{
    CNetManager::Instance().update( 0 );
}
