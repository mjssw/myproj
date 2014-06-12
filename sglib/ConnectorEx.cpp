#include "ConnectorEx.h"
#include "IoServiceEx.h"
#include "IoSocketExManager.h"
#include "StackTrace.h"
#include "Log.h"
using namespace SGLib;

CConnectorEx::CConnectorEx(CIoServiceExPool &ioServicePool) : 
	m_freeConnQueue( E_Connector_QueueSize ),
	m_connectExecutor( E_Connector_ExecutorThreadCount, E_Connector_QueueSize ),
	m_ioServicePool( ioServicePool )
{
}

CConnectorEx::~CConnectorEx()
{
}

bool CConnectorEx::Start()
{
	return m_connectExecutor.Start();
}

void CConnectorEx::Stop()
{
	m_connectExecutor.Stop();
}

void CConnectorEx::Connect(const char *ip, s32 port)
{
	CConnectObj *connObj;
	if( !m_freeConnQueue.Pop( connObj ) )
	{
		connObj = new CConnectObj( this );
	}
	SELF_ASSERT( connObj, return; );
	strncpy( connObj->m_ip, ip, E_Connector_MaxIp_Len );
	connObj->m_port = port;
	m_connectExecutor.Execute( connObj );
}

void CConnectorEx::CConnectObj::Run()
{
	_Connect( m_ip, (u16)m_port );
	bool ret = m_pthis->m_freeConnQueue.Push( this );
	SELF_ASSERT( ret, return; );
}

void CConnectorEx::CConnectObj::_Connect(const char *ip, u16 port)
{
	SELF_ASSERT( m_pthis, return; ); 

	CIoServiceEx *ioService = m_pthis->m_ioServicePool.GetIoService();
	SELF_ASSERT( ioService, return; ); 

	CIoSocketEx *ioSocket = ioService->GetIoSocketExMgr().GetFreeIoSocket();
	if( !ioSocket )
	{
		CLog::ErrorLog( "CConnectorEx::CConnectObj::_Connect GetFreeIoSocket Error" );
		// TODO call back OnConnectErr
		return;
	}

	ioSocket->_SetClientInfo( 0, inet_addr(ip), 0, port );
	ioSocket->_SetIoService( *ioService );

	s32 sock = _DoConnect( ip, port );
	if( sock != (s32)INVALID_SOCKET )
	{
		ioSocket->_SetSocket( sock );
		
		SEventObj eventObj;
		eventObj.type = SEventObj::E_Event_ConnectSuccess;
		if( !ioService->TryPushEvent( ioSocket->GetId(), eventObj, ioSocket ) )
		{
			ioSocket->OnConnect();
		}

#ifdef _USE_WIN32
		if( false == ioService->BindIoObject( ioSocket->GetSocket(), ioSocket->_GetCompletionKey() ) )
#else
        if( false == ioService->BindIoObject(sock, *ioSocket) )
#endif
		{
			CLog::ErrorLog( "CConnectorEx::CConnectObj::_Connect BindIoObject Error" );
			ioSocket->Close();
		}
		else
		{
#ifdef _USE_WIN32
			if( ioSocket->_PostRecv() == false )
			{
				CLog::ErrorLog( "CConnectorEx::CConnectObj::_Connect _PostRecv Error" );
				ioSocket->Close();
			}
#endif
		}
	}
	else
	{
		SEventObj eventObj;
		eventObj.type = SEventObj::E_Event_ConnectError;
		if( !ioService->TryPushEvent( ioSocket->GetId(), eventObj, ioSocket ) )
		{
			ioSocket->OnConnectError();
		}
	}
}

s32 CConnectorEx::CConnectObj::_DoConnect(const char *ip, u16 port)
{
	s32 sock = (s32)socket( AF_INET, SOCK_STREAM, 0 );
	if( INVALID_SOCKET == sock ) 
	{
		CLog::ErrorLog( "CConnectorEx::CConnectObj::_DoConnect socket Error" );
		return (s32)INVALID_SOCKET;
	}

	struct sockaddr_in sa;
	memset( &sa, 0, sizeof(sa) );
	sa.sin_family = AF_INET;
	sa.sin_port = htons( port );
#ifdef _USE_WIN32
	sa.sin_addr.s_addr = inet_addr( ip );
#else
	inet_pton( AF_INET, ip, &sa.sin_addr );
#endif

	if( SOCKET_ERROR == connect( sock, (struct sockaddr*)(&sa), sizeof(sa)) ) 
	{
#ifdef _USE_WIN32
		closesocket( sock );
#else
		close( sock );
#endif
		CLog::ErrorLog( "CConnectorEx::CConnectObj::_DoConnect connect Error" );
		return (s32)INVALID_SOCKET;
	}

	return sock;
}
