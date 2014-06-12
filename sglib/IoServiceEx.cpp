#include "IoServiceEx.h"
#include "IoSocketExManager.h"
#include "SetupSocket.h"
#include "IoObjectEx.h"
#include "IEventEx.h"
#include "EventWorkerExPool.h"
#include "OverlappedPoolEx.h"
#include "StackTrace.h"
#include "Log.h"
using namespace SGLib;

#ifdef _USE_WIN32
static int _GetProcessorsCount()
{
	int nProcessors = 0;
	if( 0 == nProcessors )
	{
		SYSTEM_INFO si;
		GetSystemInfo( &si );
		nProcessors = si.dwNumberOfProcessors;
	}
	return nProcessors;
}
#endif

CIoServiceEx::CIoServiceEx(CIoSocketExMgr &ioSocketExMgr, CEventWorkerExPool *eventWorkerPool) : 
	m_isRun( false )
	, m_ioSocketExMgr( ioSocketExMgr )
	, m_eventWorkerPool( eventWorkerPool )
{
#ifdef _USE_WIN32
	m_ioThreadCount = 2 * _GetProcessorsCount() + 2;
#else
	m_ioThreadCount = 1;
#endif
	if( m_ioThreadCount > E_Max_IoThread_Count )
	{
		m_ioThreadCount = E_Max_IoThread_Count;
	}
	
	for( s32 i=0; i<E_Max_IoThread_Count; ++i )
	{
		m_ioThread[i] = NULL;
	}

	CSetupSocket::Instance().Setup();
}

CIoServiceEx::~CIoServiceEx()
{
	Stop();
	CSetupSocket::Instance().Clean();
}

bool CIoServiceEx::Start()
{
	if( IsRun() )
	{
		CLog::DebugLog( "CIoServiceEx::Start aleardy start" );
		return true;
	}

	if( !_Init() )
	{
		CLog::ErrorLog( "CIoServiceEx::Start->_Init Error" );
		return false;
	}

	_SetRun( true );
	SGDEBUG( "%d IoThread Start\n", m_ioThreadCount );
	for( s32 i=0; i<m_ioThreadCount; ++i )
	{
		m_ioThread[i] = new CThread( this );
		SELF_ASSERT( m_ioThread[i], return false; );
		bool ret = m_ioThread[i]->Start();
		SELF_ASSERT( ret, return false; );
	}

	return true;
}

void CIoServiceEx::Stop()
{
	if( !IsRun() )
	{
		return;
	}

	_SetRun( false );
	_InterruptIoThread();
	for( s32 i=0; i<m_ioThreadCount; ++i )
	{
		SG_ASSERT( m_ioThread[i] != NULL );
		m_ioThread[i]->Stop();
		SAFE_DELETE( m_ioThread[i] );
	}
    
#ifdef _USE_WIN32
    CloseHandle( m_completionHandle );
    m_completionHandle = INVALID_HANDLE_VALUE;
#else
    m_interrupter.Fini( m_epollFd );
    close( m_epollFd );
#endif
}

bool CIoServiceEx::IsRun()
{
	return m_isRun;
}

bool CIoServiceEx::TryPushEvent(s32 worker, const SEventObj &eventObj, IEventEx *obj)
{
	if( m_eventWorkerPool )
	{
		m_eventWorkerPool->PushEvent(
			worker, sizeof(eventObj), (char*)&eventObj, obj );

		return true;
	}

	return false;
}

void CIoServiceEx::Run()
{
	while( IsRun() )
	{
#ifdef _USE_WIN32
		_IoRunWindows();
#else
		_IoRunLinux();
#endif
	}

	_SetRun( false );
}

#ifdef _USE_WIN32
bool CIoServiceEx::BindIoObject(s32 sock, const CompletionKey_t &key)
{
	if( NULL == CreateIoCompletionPort(
		(HANDLE)sock, 
		m_completionHandle, 
		(DWORD)&key, 
		0) )
	{
		CLog::ErrorLog( "CIoServiceEx::BindIoObject->CreateIoCompletionPort Error:%d", (s32)GetLastError() );
		return false;
	}
	_InterruptIoThread();
	return true;
}
#else
bool CIoServiceEx::BindIoObject(s32 sock, const CIoObjectEx &ioObj)
{
    u32 event = EPOLLIN | EPOLLET;
	return AddEpollEvent( sock, event, ioObj );
}

bool CIoServiceEx::AddEpollEvent(s32 sock, u32 event, const CIoObjectEx &ioObj)
{
    struct epoll_event ev;
    ev.data.ptr = (void*)&ioObj;
    ev.events = event;
    s32 ret = epoll_ctl( m_epollFd, EPOLL_CTL_ADD, sock, &ev );
    if( ret == -1 )
    {
		CLog::ErrorLog( "CIoServiceEx::AddEpollEvent->epoll_ctl Error:%d", (s32)errno );
        return false;
    }

    return true;
}

bool CIoServiceEx::DelEpollEvent(s32 sock)
{
    struct epoll_event ev;
    s32 ret = epoll_ctl( m_epollFd, EPOLL_CTL_DEL, sock, &ev );
    if( ret == -1 )
    {
		CLog::ErrorLog( "CIoServiceEx::DelEpollEvent->epoll_ctl Error:%d", (s32)errno );
        return false;
    }

    return true;
}

bool CIoServiceEx::ModEpollEvent(s32 sock, const CIoObjectEx &ioObj, bool isAddOutEvent)
{
    struct epoll_event ev;
    ev.data.ptr = (void*)&ioObj;
    ev.events = EPOLLIN | EPOLLET;
    if( isAddOutEvent )
    {
        ev.events |= EPOLLOUT;
    }
    s32 ret = epoll_ctl( m_epollFd, EPOLL_CTL_MOD, sock, &ev );
    if( ret != 0 )
    {
		CLog::ErrorLog( "CIoServiceEx::ModEpollEvent->epoll_ctl Error:%d", (s32)errno );
        return false;
    }

    return true;
}

CIoServiceEx::CEpoolWaitInterrupter::CEpoolWaitInterrupter() : 
    m_fdRead( -1 ),
    m_fdWrite( -1 )
{
}

CIoServiceEx::CEpoolWaitInterrupter::~CEpoolWaitInterrupter()
{
}

bool CIoServiceEx::CEpoolWaitInterrupter::Init(s32 epollfd)
{
    m_fdRead = m_fdWrite = eventfd( 0, EFD_CLOEXEC | EFD_NONBLOCK );
    if( m_fdRead == -1 )
    {
		CLog::ErrorLog( "CIoServiceEx::CEpoolWaitInterrupter::Init->eventfd Error:%d", (s32)errno );
        return false;
    }

    epoll_event ev = { 0, { 0 } };
    ev.events = EPOLLIN | EPOLLERR | EPOLLET;
    ev.data.ptr = this;
    s32 ret = epoll_ctl( epollfd, EPOLL_CTL_ADD, m_fdRead, &ev );
    if( ret != 0 )
    {
		CLog::ErrorLog( "CIoServiceEx::CEpoolWaitInterrupter::Init->epoll_ctl Error:%d", (s32)errno );
        return false;
    }

    uint64_t counter(1UL);
    write( m_fdWrite, &counter, sizeof(uint64_t) );

    return true;
}

void CIoServiceEx::CEpoolWaitInterrupter::Fini(s32 epollfd)
{
    struct epoll_event ev;
    epoll_ctl( epollfd, EPOLL_CTL_DEL, m_fdRead, &ev );
    if( m_fdRead != -1 )
    {
        close( m_fdRead );
        m_fdRead = -1; 
    }
    if( m_fdWrite != -1 )
    {
        close( m_fdWrite );
        m_fdWrite = -1 ; 
    }
}

void CIoServiceEx::CEpoolWaitInterrupter::Interrupt(s32 epollfd)
{
    epoll_event ev = { 0, { 0 } };
    ev.events = EPOLLIN | EPOLLERR | EPOLLET;
    ev.data.ptr = this;
    s32 ret = epoll_ctl( epollfd, EPOLL_CTL_MOD, m_fdRead, &ev );
    if( ret != 0 )
    {
		CLog::ErrorLog( "CIoServiceEx::CEpoolWaitInterrupter::Interrupt->epoll_ctl Error:%d", (s32)errno );
    }
}
#endif

void CIoServiceEx::_SetRun(bool isRun)
{
	m_isRun = isRun;
}

bool CIoServiceEx::_Init()
{
#ifdef _USE_WIN32
	m_completionHandle = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( NULL == m_completionHandle )
	{
		CLog::ErrorLog( "CIoServiceEx::_Init->CreateIoCompletionPort Error:%d", (s32)GetLastError() );
		return false;
	}
	return true;
#else		
	m_epollFd = epoll_create( E_Max_EpollEvent_Count );
	if( m_epollFd == -1 )
	{
		CLog::ErrorLog( "CIoServiceEx::_Init->epoll_create Error:%d", (s32)errno );
		return false; 
	}
    return m_interrupter.Init( m_epollFd );
#endif
}

void CIoServiceEx::_IoRunWindows()
{
#ifdef _USE_WIN32
	CompletionKey_t *completionKey = NULL;
	Overlapped_t *overlapped = NULL;
	DWORD bytesTransfered = 0;

	BOOL ret = GetQueuedCompletionStatus(
		m_completionHandle,
		&bytesTransfered,
		(PULONG_PTR)&completionKey,
		(LPOVERLAPPED*)&overlapped,
		INFINITE
	);

	if( !IsRun() )
	{
		return;
	}

	// 错误情况
	if( ret == FALSE )
	{
		u32 err = GetLastError();
		if( err == WAIT_TIMEOUT )
		{
			// Time out
			return;
		}
		else
		{
			// 如果不是超时
			if( completionKey == NULL )
			{
				//服务器关闭,线程退出
				_SetRun( false );
				return;
			}

			if( overlapped )
			{
				CIoObjectEx *obj = (CIoObjectEx*)completionKey->m_pData;
				SELF_ASSERT( obj != NULL, return; );
				CLog::ErrorLog( "CIoServiceEx::_IoRunWindows->GetQueuedCompletionStatus client[%d,%d] Error:%d", 
					obj->GetObjectId(), obj->GetObjectSocket(), (s32)GetLastError() );
				obj->HandleError( overlapped );	
			}
			else
			{
				CLog::ErrorLog( "CIoServiceEx::_IoRunWindows->GetQueuedCompletionStatus Error:%d", (s32)GetLastError() );
			}
		}
		return;
	}

	// 被中断情况
	if( completionKey == NULL && overlapped == NULL )
	{
		return;
	}

	// 正常情况
	SELF_ASSERT( completionKey != NULL, return; );
	SELF_ASSERT( overlapped != NULL, return; );
	CIoObjectEx *obj = (CIoObjectEx*)completionKey->m_pData;
	SELF_ASSERT( obj != NULL, return; );
	switch( overlapped->m_eEvtIocp )
	{
	case IOCP_ACCEPT:
	case IOCP_RECV:
		obj->HandleRead( (s32)bytesTransfered, overlapped );
		break;
	case IOCP_SEND:
		obj->HandleWrite( (s32)bytesTransfered, overlapped );
		break;
	default:
		SELF_ASSERT( false, return; );
		break;
	};
#endif
}

void CIoServiceEx::_IoRunLinux()
{
#ifndef _USE_WIN32
	s32 nfds = epoll_wait( m_epollFd, m_epollEvents, E_Max_EpollEvent_Count, -1 );
	if( nfds == -1 )
	{
		if( errno == EINTR )
		{
			return;
		}

		// Error
		_SetRun( false );
		CLog::ErrorLog( "CIoServiceEx::_IoRunLinux->epoll_wait Error:%d", (s32)errno );
		return;
	}

	if( nfds == 0 )
	{
		return;
	}

	for( s32 i=0; i<nfds; ++i )
	{
		CIoObjectEx *obj = (CIoObjectEx*)m_epollEvents[i].data.ptr;
		SELF_ASSERT( obj != NULL, continue; );
		if( m_epollEvents[i].events & EPOLLIN )
		{
			s32 size = obj->_DoHandleRead();
			obj->HandleRead( size, NULL );
		}

		if( m_epollEvents[i].events & EPOLLOUT )
		{
			s32 size = obj->_DoHandleWrite();
			obj->HandleWrite( size, NULL );
		}
	}
#endif
}

void CIoServiceEx::_InterruptIoThread()
{
#ifdef _USE_WIN32
	for( s32 i=0; i<m_ioThreadCount; ++i )
	{
		//Help threads get out of blocking - GetQueuedCompletionStatus()
		BOOL ret = PostQueuedCompletionStatus( m_completionHandle, 0, (DWORD)NULL, NULL );
		if( ret != TRUE )
		{
			CLog::ErrorLog( "CIoServiceEx::_InterruptIoThread->PostQueuedCompletionStatus Error:%d", (s32)GetLastError() );
		}
	}
#else
    m_interrupter.Interrupt( m_epollFd );
#endif
}
