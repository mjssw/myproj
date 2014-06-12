#include "AccepterEx.h"
#include "StackTrace.h"
#include "IoServiceEx.h"
#include "IoServiceExPool.h"
#include "IoSocketExManager.h"
#include "EventWorkerExPool.h"
#include "Log.h"
using namespace SGLib;

#ifdef _USE_WIN32
#include <MSWSock.h>
static LPFN_ACCEPTEX				g_lpfnAcceptEx = NULL;
static LPFN_GETACCEPTEXSOCKADDRS	g_lpfnGetAcceptExSockAddrs = NULL;
#endif

CAccepterEx::CAccepterEx(CIoServiceExPool &ioServicePool) : 
	m_listenPort( 0 ),
	m_listenSock( (s32)INVALID_SOCKET ),
	m_ioServicePool( ioServicePool ),
    m_ioService( NULL )
{
#ifdef _USE_WIN32
	m_completionKey.m_pData = this;
	memset( m_overlappArray, 0, sizeof(Overlapped_t) * MAX_ACCEPT_COUNT );
#endif
}

CAccepterEx::~CAccepterEx()
{
}

void CAccepterEx::HandleRead(s32 size, void *data)
{
	CLog::DebugLog( "CAccepterEx::HandleRead accept client"	);

#ifdef _USE_WIN32
	SELF_ASSERT( data != NULL, return; );
	_DoAcceptWindows( *((Overlapped_t*)data) );
#else
	_DoAcceptLinux();
#endif
}

void CAccepterEx::HandleWrite(s32 size, void *data)
{
	SELF_ASSERT( false, return; );
}

void CAccepterEx::HandleError(void *data)
{
	CLog::DebugLog( "CAccepterEx::HandleError error occur"	);

#ifdef _USE_WIN32
	SG_ASSERT( data != NULL );
	_PostAcceptEx( *((Overlapped_t*)data) );
#else
    SG_ASSERT( false );
#endif
}

bool CAccepterEx::Start(s32 port)
{
	m_ioService = m_ioServicePool.GetIoService();
	if( m_ioService == NULL )
	{
		CLog::ErrorLog( "CAccepterEx::Start->GetIoService Error:null ioservice" );
		return false;
	}

	m_listenSock = (s32)socket( AF_INET, SOCK_STREAM, 0 );
	if( m_listenSock == INVALID_SOCKET )
	{
		CLog::ErrorLog( "CAccepterEx::Start create socket Error" );
		return false;
	}

	struct sockaddr_in addr;
	memset( (void*)&addr, 0, sizeof(addr) );
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl( INADDR_ANY );
	addr.sin_port = htons( port );

	if( bind(m_listenSock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR )
	{
		CLog::ErrorLog( "CAccepterEx::Start bind Error" );
		return false;
	}

	bool ret = _BindToIoService();
	if( !ret )
	{
		CLog::ErrorLog( "CAccepterEx::Start _BindToIoService Error" );
		return false;
	}

	if( listen(m_listenSock, MAX_ACCEPT_COUNT) == SOCKET_ERROR )
	{
		CLog::ErrorLog( "CAccepterEx::Start listen Error" );
		return false;
	}

	_Start();
	m_listenPort = port;

	CLog::DebugLog( "CAccepterEx start listen port:%d Success", port );

	return true;
}

void CAccepterEx::Stop()
{
    if( m_listenSock != INVALID_SOCKET )
    {
#ifdef _USE_WIN32
        CancelIo( (HANDLE)m_listenSock );
        closesocket( m_listenSock );
#else
        bool ret = m_ioService->DelEpollEvent( m_listenSock );
        SELF_ASSERT( ret, ; );
        close( m_listenSock );
#endif
        m_listenSock = (s32)INVALID_SOCKET;
    }
}

bool CAccepterEx::_BindToIoService()
{
	SELF_ASSERT( m_ioService != NULL, return false; );
#ifdef _USE_WIN32
	return m_ioService->BindIoObject( m_listenSock, m_completionKey );
#else
    bool ret = _SetSocketNonblock( m_listenSock );
	if( !ret )
	{
		CLog::ErrorLog( "CAccepterEx::_BindToIoService _SetSocketNonblock Error" );
		return false;
	}
    return m_ioService->BindIoObject( m_listenSock, *this );
#endif
}

void CAccepterEx::_Start()
{
#ifdef _USE_WIN32
	DWORD dwBytes = 0;
	GUID guidAcceptEx = WSAID_ACCEPTEX;
	s32 ret = WSAIoctl( m_listenSock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidAcceptEx, sizeof(guidAcceptEx),
		&g_lpfnAcceptEx, sizeof(LPFN_ACCEPTEX), &dwBytes, NULL, NULL );
	SELF_ASSERT( ret != SOCKET_ERROR, return; );

	dwBytes = 0;
	GUID guidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
	ret = WSAIoctl( m_listenSock, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidGetAcceptExSockAddrs, sizeof(guidGetAcceptExSockAddrs),
		&g_lpfnGetAcceptExSockAddrs, sizeof(LPFN_GETACCEPTEXSOCKADDRS),
		&dwBytes, NULL, NULL );
	SELF_ASSERT( ret != SOCKET_ERROR, return; );
	
	for( s32 i=0; i<MAX_ACCEPT_COUNT; ++i )
	{
		_PostAcceptEx( m_overlappArray[i] );
	}
#else
#endif
}

#ifdef _USE_WIN32
void CAccepterEx::_PostAcceptEx(Overlapped_t &overlapped)
{
	SOCKET hSock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	if( hSock == INVALID_SOCKET )
	{
		CLog::ErrorLog( "CAccepterEx::_PostAcceptEx WSASocket Error:%d", (s32)WSAGetLastError() );
		return;
	}

	memset( (void*)&overlapped.m_sOverlapped, 0, sizeof(overlapped.m_sOverlapped) );
	overlapped.m_hSock = hSock;
	overlapped.m_eEvtIocp = IOCP_ACCEPT;

	/*
	* 经过AcceptEx操作之后，地址信息已经填入指定的缓冲区
	* 注意这里指定的长度
	*/
	int nAddrLen = sizeof(SOCKADDR_IN) + 16;
	DWORD dwBytes = 0;
	BOOL bRet = g_lpfnAcceptEx(
		m_listenSock,
		hSock,
		overlapped.m_Buf,
		0,
		nAddrLen,
		nAddrLen,
		&dwBytes,
		&overlapped.m_sOverlapped );
	if( bRet != TRUE )
	{
		if( WSAGetLastError() != WSA_IO_PENDING )
		{
			CLog::ErrorLog( "CAccepterEx::_PostAcceptEx AcceptEx Error:%d", (s32)WSAGetLastError() );
			return;
		}
	}
}

void CAccepterEx::_DoAcceptWindows(Overlapped_t &overlapped)
{
	SELF_ASSERT( m_ioService, return; );
	CIoSocketEx *ioSocket = m_ioService->GetIoSocketExMgr().GetFreeIoSocket();
	if( ioSocket )
	{
		SOCKET hSock = overlapped.m_hSock;
        //把listen套结字一些属性（包括socket内部接受/发送缓存大小等等）拷贝到新建立的套结字
        setsockopt( hSock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (const char*)m_listenSock, sizeof(SOCKET) );
		ioSocket->_SetSocket( (s32)hSock );
        
        /*
         * 通过GetAcceptExSockaddrs 获取本地地址和远程地址，
         * 需要lpOutputBuffer指向当初调用AcceptEx时指定的一个缓冲区，
         * 并且在 dwReceiveDataLength中指定相同的大小。
         * 注意这里设置的长度
         * 注意iAddrLen 长度必须和AcceptEx中传入的长度一致
         */
        sockaddr_in *pRemoteAddr = NULL;
        sockaddr_in *pLocalAddr = NULL;
        s32 iRemoteAddrLen = sizeof(sockaddr_in);
        s32 iLocalAddrLen = sizeof(sockaddr_in);
        s32 iAddrLen = sizeof(sockaddr_in) + 16;
        g_lpfnGetAcceptExSockAddrs( overlapped.m_Buf, 0, iAddrLen, iAddrLen,
            (SOCKADDR**)&pLocalAddr, &iLocalAddrLen, (SOCKADDR**)&pRemoteAddr, &iRemoteAddrLen );

		ioSocket->_SetClientInfo( pLocalAddr->sin_addr.s_addr, pRemoteAddr->sin_addr.s_addr, 
			pLocalAddr->sin_port, pRemoteAddr->sin_port );

		CIoServiceEx *ioService = m_ioServicePool.GetIoService();
		SELF_ASSERT( ioService != NULL, return; );
		ioSocket->_SetIoService( *ioService );
			
		// 如果存在event worker ， 则交给其处理
		SEventObj eventObj;
		eventObj.type = SEventObj::E_Event_AcceptClient;
		if( !m_ioService->TryPushEvent( ioSocket->GetId(), eventObj, ioSocket ) )
		{
			ioSocket->OnAccept();
		}

		if( false == ioService->BindIoObject( ioSocket->GetSocket(), ioSocket->_GetCompletionKey() ) )
		{
			CLog::ErrorLog( "CAccepterEx::_DoAcceptWindows BindIoObject Error" );
			ioSocket->Close();
		}
		else
		{
			if( ioSocket->_PostRecv() == false )
			{
				ioSocket->Close();
			}
		}
	}
	else
	{
		CLog::ErrorLog( "CAccepterEx::_DoAcceptWindows Error:Reach max accept count" );
		closesocket( overlapped.m_hSock );
	}

	// 重新投递连接
	_PostAcceptEx( overlapped );
}
#else
s32 CAccepterEx::_DoHandleRead()
{
	return 0;
}

s32 CAccepterEx::_DoHandleWrite()
{
	return 0;
}
		
void CAccepterEx::_DoAcceptLinux()
{
    do
    {
        s32 sock = _EpollAcceptSocket();
        if( sock == INVALID_SOCKET )
        {
            break;
        }

	    CIoSocketEx *ioSocket = m_ioService->GetIoSocketExMgr().GetFreeIoSocket();
        if( ioSocket )
        {
            ioSocket->_SetSocket( sock );
            CIoServiceEx *ioService = m_ioServicePool.GetIoService();
            SG_ASSERT( ioService != NULL );
            ioSocket->_SetIoService( *ioService );
			
			SEventObj eventObj;
			eventObj.type = SEventObj::E_Event_AcceptClient;
			if( !m_ioService->TryPushEvent( ioSocket->GetId(), eventObj, ioSocket ) )
			{
				ioSocket->OnAccept();
			}

            if( false == ioService->BindIoObject(sock, *ioSocket) )
            {
				CLog::ErrorLog( "CAccepterEx::_DoAcceptLinux BindIoObject Error" );
                ioSocket->Close();
            }
        }
        else
        {
            // reach max connection
			CLog::ErrorLog( "CAccepterEx::_DoAcceptLinux Error:Reach max accept count" );
            close( sock );
            break;
        }

    } while( 1 );
}

s32 CAccepterEx::_EpollAcceptSocket()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    s32 sock = accept( m_listenSock, (struct sockaddr*)&addr, &len );
    if( sock == -1 )
    {
		CLog::ErrorLog( "CAccepterEx::_EpollAcceptSocket accept Error:%d", errno );
        return INVALID_SOCKET;
    }

    if( !_SetSocketNonblock(sock) )
    {
		CLog::ErrorLog( "CAccepterEx::_EpollAcceptSocket _SetSocketNonblock Error" );
        close( sock );
        return INVALID_SOCKET;
    }

    return sock;
}

bool CAccepterEx::_SetSocketNonblock(s32 sock)
{
    s32 opt = fcntl( sock, F_GETFL );
    if( opt == -1 )
    {
		CLog::ErrorLog( "CAccepterEx::_SetSocketNonblock fcntl get Error:%d", (s32)errno );
        return false;
    }
    opt |= O_NONBLOCK;
    if( fcntl( sock, F_SETFL, opt ) == -1 )
    {
		CLog::ErrorLog( "CAccepterEx::_SetSocketNonblock fcntl set Error:%d", (s32)errno );
        return false;
    }
    return true;
}
#endif
