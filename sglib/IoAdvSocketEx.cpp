#include "IoAdvSocketEx.h"
#include "IoServiceEx.h"
#include "IoSocketExManager.h"
#include "StackTrace.h"
#include "Log.h"
using namespace SGLib;

CIoAdvSocketEx::CIoAdvSocketEx(s32 id) :
	CIoSocketEx( id )
{
}

CIoAdvSocketEx::~CIoAdvSocketEx()
{
}

void CIoAdvSocketEx::HandleEvent(s32 paramLen, char *paramData)
{
	SELF_ASSERT( paramLen>0 && paramData!=NULL, return; );
	SEventObj *obj = (SEventObj*)paramData;
	//CLog::DebugLog( "CIoAdvSocketEx::HandleEvent get event:%d", obj->type );
	switch( obj->type )
	{
	case SEventObj::E_Event_AcceptClient:
		{
			OnAccept();
		}
		break;
	case SEventObj::E_Event_ConnectSuccess:
		{
			OnConnect();
		}
		break;
	case SEventObj::E_Event_ConnectError:
		{
			OnConnectError();
			m_ioService->GetIoSocketExMgr().PutIoSocket( *this );
		}
		break;
	case SEventObj::E_Event_ClientClose:
		{
			OnClose();
			SELF_ASSERT( m_ioService, return; );
			m_ioService->GetIoSocketExMgr().PutIoSocket( *this );
		}
		break;
	case SEventObj::E_Event_RecvData:
		{
			CIoSocketEx::_ProcessRecvData();
		}
		break;
	case SEventObj::E_Event_Timer:
		break;
	default:
		SELF_ASSERT( false, return; );
		break;
	}
}

void CIoAdvSocketEx::Close()
{
	SELF_ASSERT( m_ioService, return; );

	{
		CGuardLock<CLock> g( m_socketLock );
		bool ret = _Close();
		CLog::DebugLog( "CIoAdvSocketEx::Close socket[id:%d] close:%s", m_id, (ret?"true":"false") );
	}
	
	SEventObj eventObj;
	eventObj.type = SEventObj::E_Event_ClientClose;
	bool ret = m_ioService->TryPushEvent( GetId(), eventObj, this );
	SELF_ASSERT( ret, return; );
}

void CIoAdvSocketEx::_ProcessRecvData()
{
	SEventObj eventObj;
	eventObj.type = SEventObj::E_Event_RecvData;
	bool ret = m_ioService->TryPushEvent( GetId(), eventObj, this );
	SELF_ASSERT( ret, return; );
}
