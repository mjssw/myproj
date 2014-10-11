#include "MsgGameModule.h"
#include "net/NetManager.h"
#include "user/UserManager.h"
#include "wrapper/QtWrapper.h"
#include "CommDef.h"
#include "errno.pb.h"
using namespace std;

CMsgConnectGame::CMsgConnectGame()
{
}
void CMsgConnectGame::Process()
{
	CCLog( "[CMsgConnectGame::Process] connect game(%s:%d) success", LOGIN_IP, LOGIN_PORT );

}

CMsgGameMessage::CMsgGameMessage(int msgid, int msglen, char *data) :
	m_msgid( msgid ),
    m_msglen( msglen )
{
	if( msglen>0 && data )
	{
        m_msgData.append( data, msglen );
	}
}
void CMsgGameMessage::Process()
{
    QVariant v = qVariantFromValue( (void*)m_msgData.c_str() );
    emit CQtWrapper::Instance().processGameMessage( m_msgid, v );
}
