#ifndef _SG_IO_SERVICE_H_
#define _SG_IO_SERVICE_H_

#include "Define.h"
#include "Thread.h"
#include "Runnable.h"
#include "IoSocketBase.h"
#include "Queue.h"
#include "Lock.h"
#include "Event.h"
#include "Log.h"
#include "StackTrace.h"
#include "SetupSocket.h"
#include "List.h"
#include "IoSocketManager.h"
#include "EventDispatch.h"

#ifdef _USE_WIN32
#include <MSWSock.h>
#endif

namespace SGLib
{
	template<class TIoSocket>
	class CIoService;

#ifdef _USE_WIN32
#else
    class CBasicEpollOp
    {
    public:
        static s32 CreatEpollFd(s32 &nEpollFd, s32 nCount)
        {
            nEpollFd = epoll_create( nCount );
            if( nEpollFd == -1 )
            {
                return E_Error_EpollCreateFailed; 
            }

            return E_Error_Success;
        }

        static void CloseEpollFd(s32 nEpollFd)
        {
            close( nEpollFd );
        }

        static s32 EpollEventAdd(s32 nEpollFd, s32 nSocket, u32 uEvent, s32 nId)
        {
            CIoSocketBase::UEpollData EpollData;
            EpollData.sData.n32Id = nId;
            EpollData.sData.n32Fd = nSocket;

            struct epoll_event ev;
            ev.data.u64 = EpollData.u64Data;
            ev.events = uEvent;
            s32 nRet = epoll_ctl( nEpollFd, EPOLL_CTL_ADD, nSocket, &ev );
            if( nRet == -1 )
            {
                return E_Error_EpollCtlFailed;
            }

            return E_Error_Success;
        }

        static s32 EpollEventDel(s32 nEpollFd, s32 nSocket)
        {
            struct epoll_event ev;
            s32 nRet = epoll_ctl( nEpollFd, EPOLL_CTL_DEL, nSocket, &ev );
            if( nRet == -1 )
            {
                return E_Error_EpollCtlFailed;
            }

            return E_Error_Success;
        }

        static s32 EpollEventMod(s32 nEpollFd, s32 nSocket, s32 nId, bool bAddOutEvent)
        {
            CIoSocketBase::UEpollData EpollData;
            EpollData.sData.n32Id = nId;
            EpollData.sData.n32Fd = nSocket;

            struct epoll_event ev;
            ev.data.u64 = EpollData.u64Data;
            ev.events = EPOLLIN | EPOLLET;
            if( bAddOutEvent == true )
            {
                ev.events |= EPOLLOUT;
            }
            s32 nRet = epoll_ctl( nEpollFd, EPOLL_CTL_MOD, nSocket, &ev );
            if( nRet != 0 )
            {
                return E_Error_EpollCtlFailed;
            }

            return E_Error_Success;
        }

        static s32 EpollWait(s32 nEpollFd, struct epoll_event *EpollEvents, s32 nMaxEvents, s32 nTimeOut)
        {
            return epoll_wait(
                nEpollFd,
                EpollEvents,
                nMaxEvents, 
                nTimeOut );
        }
    };
#endif

	template<class TIoSocket>
	class CIoService : public IRunnable
	{
	public:
		CIoService(const CNetConfig &netCfg);
		~CIoService();

		s32   Init();
		void  Fini();
		void  Dispatch();
		s32   StartListen(s32 nPort);
		bool  Connect(const char *pstrIp, u16 uPort);
		s32   GetMaxConnectionCount(){ return m_netConfig.m_maxClientCount; }
        void  DisplayDetailInfo();

	protected:
		virtual void Run();

	private:
		bool		m_bInit;
		bool		m_bStop;
		s32	        m_nWorkThreadNum;
        CThread     *m_pWorkThread[MAX_WORK_THREAD_NUM];
		u32	        m_uTimeOut;
		s32			m_nDispatchEventNum;
		CDispatchEvent **m_DispatchEvent;
		const CNetConfig  &m_netConfig;

		friend class CListener;
		class CListener
		{
		public:
			CListener(CIoService<TIoSocket> &IoService);
			~CListener();
			s32   StartListen(s32 nPort);
			void  StopListen();
			#ifdef _USE_WIN32
				void PostAcceptEx(Overlapped_t &Overlapped);
			    void DoAcceptEx(Overlapped_t &Overlapped, CIoSocketBase &IoSocket);
            #else
                s32  GetListenSocket(){ return m_hListenSock; }
			#endif
		private:
			CIoService<TIoSocket>  &m_refIoService;
			s32         m_nListenPort;
			#ifdef _USE_WIN32
				SOCKET						m_hListenSock;
				CompletionKey_t				m_ListenCompleteKey;
				LPFN_ACCEPTEX				m_lpfnAcceptEx;
				LPFN_GETACCEPTEXSOCKADDRS	m_lpfnGetAcceptExSockAddrs;
				Overlapped_t				m_OverlappArray[MAX_ACCEPT_COUNT];
				s32 _StartWinListen();
			#else
				s32					        m_hListenSock;
			#endif
		} m_Listener;

		friend class CIoSocketMgr;
		CIoSocketMgr m_IoSocketMgr;

		friend class CConnector;
		class CConnector : public IRunnable
		{
		public:
			CConnector(CIoService<TIoSocket> &IoService) : 
				m_refIoService(IoService),
				m_connQueue(m_refIoService.m_netConfig.m_connectingQueueSize),
				m_workThread(this, true)
			{
				if( !m_workThread.Start() )
				{
					CLog::ErrorLog( "CConnector work thread start failed" );
				}
			}

			~CConnector()
			{
				m_workThread.Stop();
			}
	        
			bool Connect(const char *pstrIp, u16 uPort);
			virtual void Run();
		private:
			bool _Connect(const char *pstrIp, u16 uPort);
			bool _DoConnect(s32 &Sock, const char *pstrIp, u16 uPort);
		private:
			struct SConnInfo
			{
				char ip[STR_IP_LEN];
				u16 port;
			};
			CIoService<TIoSocket> &m_refIoService;
			CSafeQueue<SConnInfo> m_connQueue;
			CAdvThread m_workThread;
		} m_Connector;

        // 命令行线程,just for test
        /*class CCmdTask : public IRunnable
        {
        public:
            CCmdTask(CIoService<TIoSocket> &IoService) : m_refIoService(IoService), m_CmdThread(this)
            {
                m_CmdThread.Start();
            }
            ~CCmdTask()
            {
                m_CmdThread.Stop();
            }
            virtual void Run()
            {
                while( 1 )
                {
                    char cmd[32] = {0};
                    scanf( "%s", cmd );
                    if( strcmp(cmd, "info") == 0 )
                    {
                        m_refIoService.DisplayDetailInfo();
                    }
                    else if( strcmp(cmd, "quit") == 0 )
                    {
                        break;
                    }
                    else
                    {
                        printf( "未知命令: %s\n", cmd );
                    }
                };
            }
        private:
			CIoService<TIoSocket> &m_refIoService;
            CThread     m_CmdThread;
        } m_CmdTask;*/


	private:
		s32 _InitWorkThread();
		s32 _InitDispatchEvent();
		CDispatchEvent* _GetDispatchEvent();
		void  _Stop();

		#ifdef _USE_WIN32
			HANDLE m_hCompletePort;
			s32    _InitWin();
			void   _StopWin();
			void   _IoRunWin();
			bool   _BindToCompletionPort(SOCKET hSock, const CompletionKey_t &Key);
			void   _DoIocpEventAccept(CompletionKey_t &Key, Overlapped_t &Overlapped);
			void   _DoIocpEventRecv(CompletionKey_t &Key, Overlapped_t &Overlapped, DWORD dwSize);
			void   _DoIocpEventSend(CompletionKey_t &Key, Overlapped_t &Overlapped, DWORD dwSize);
			void   _DoCompletionIoError(CompletionKey_t &Key, Overlapped_t &Overlapped, u32 uErrCode);
		#else
            s32    m_nEpollFd;
	        struct epoll_event *m_EpollEvents;
			s32    _InitLinux();
			void   _IoRunLinux();
            s32    _SetSocketNonBlock(s32 nSocket);
            void   _DoEpollAccept();
            void   _DoEpollRecv(s32 nSocket, s32 nId);
            void  _DoEpollSend(s32 nSocket, s32 nId);
            s32   _EpollAcceptSocket();
		#endif
	};

    //===========================================================================================

    #ifdef _USE_WIN32
	static int GetProcessorsCount()
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

	template<class TIoSocket>
	CIoService<TIoSocket>::CListener::CListener(CIoService<TIoSocket> &IoService) : m_refIoService(IoService)
	{
		m_nListenPort = 0;
		m_hListenSock = INVALID_SOCKET;
        #ifdef _USE_WIN32
		    m_lpfnAcceptEx = 0;
		    m_lpfnGetAcceptExSockAddrs = 0;
        #endif
	}

	template<class TIoSocket>
	CIoService<TIoSocket>::CListener::~CListener()
	{
		StopListen();
	}

	template<class TIoSocket>
	s32 CIoService<TIoSocket>::CListener::StartListen(s32 nPort)
	{
		m_hListenSock = socket( AF_INET, SOCK_STREAM, 0 );
		if( m_hListenSock == INVALID_SOCKET )
		{
			CLog::ErrorLog( "CIoService<TIoSocket>::CListener::StartListen socket Error" );
			return E_Error_CreateSocketFailed;
		}

        #ifndef _USE_WIN32
            s32 nRet = m_refIoService._SetSocketNonBlock( m_hListenSock );
            if( nRet != E_Error_Success )
            {
				CLog::ErrorLog( "CIoService<TIoSocket>::CListener::StartListen _SetSocketNonBlock Error" );
                return nRet;
            }
        #endif

		struct sockaddr_in addr;
		memset( (void*)&addr, 0, sizeof(addr) );
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl( INADDR_ANY );
		addr.sin_port = htons( nPort );

		if( bind(m_hListenSock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR )
		{
			CLog::ErrorLog( "CIoService<TIoSocket>::CListener::StartListen bind Error" );
			return E_Error_BindListenSocketFailed;
		}

        #ifndef _USE_WIN32
            nRet = CBasicEpollOp::EpollEventAdd( m_refIoService.m_nEpollFd, m_hListenSock, EPOLLIN | EPOLLET, 0 );
            if( nRet != E_Error_Success )
            {
				CLog::ErrorLog( "CIoService<TIoSocket>::CListener::StartListen EpollEventAdd Error" );
                return nRet;
            }
        #endif

		if( listen(m_hListenSock, MAX_ACCEPT_COUNT) == SOCKET_ERROR )
		{
			CLog::ErrorLog( "CIoService<TIoSocket>::CListener::StartListen listen Error" );
			return E_Error_ListenSocketFailed;
		}
		
		#ifdef _USE_WIN32
			s32 nRet = _StartWinListen();
			if( nRet != E_Error_Success )
			{
				CLog::ErrorLog( "CIoService<TIoSocket>::CListener::StartListen _StartWinListen Error" );
				return nRet;
			}
		#endif

		m_nListenPort = nPort;

		return E_Error_Success;
	}

	template<class TIoSocket>
	void CIoService<TIoSocket>::CListener::StopListen()
	{
		if( m_hListenSock != INVALID_SOCKET )
		{
		    #ifdef _USE_WIN32
			    CancelIo( (HANDLE)m_hListenSock );
			    closesocket( m_hListenSock );
            #else
                s32 nRet = CBasicEpollOp::EpollEventDel( m_refIoService.m_nEpollFd, m_hListenSock );
                SG_ASSERT( nRet == E_Error_Success );
                close( m_hListenSock );
            #endif
			m_hListenSock = INVALID_SOCKET;
		}
	}

	template<class TIoSocket>
	void CIoService<TIoSocket>::CConnector::Run()
	{
		SConnInfo info;
		while( m_connQueue.Pop(info) )
		{
			if( !_Connect( info.ip, info.port ) )
			{
				CLog::ErrorLog( "CIoService::CConnector::Run connect client[%s,%d] failed", 
                    info.ip, info.port );

				TIoSocket tmpObj(-1);
				tmpObj.SetClientInfo( 0, inet_addr(info.ip), 0, info.port );
				tmpObj.OnConnectError();
			}
		}
	}

	template<class TIoSocket>
	bool CIoService<TIoSocket>::CConnector::Connect(const char *pstrIp, u16 uPort)
	{
		SConnInfo info;
		strncpy( info.ip, pstrIp, STR_IP_LEN );
		info.port = uPort;
		if( m_connQueue.Push(info) )
		{
			m_workThread.WakeUp();
			return true;
		}
		
		CLog::ErrorLog( "CIoService<TIoSocket>::CConnector::Connect Error:push connect event[%s,%d]", 
			info.ip, info.port );

		return false;
	}

	template<class TIoSocket>
	bool CIoService<TIoSocket>::CConnector::_Connect(const char *pstrIp, u16 uPort)
	{
		s32 Sock = (s32)INVALID_SOCKET;
		if( _DoConnect(Sock, pstrIp, uPort) )
		{
			CLog::DebugLog( "CIoService<TIoSocket>::CConnector::_Connect  _DoConnect %s:%d success", pstrIp, uPort );

			CIoSocketBase *pIoSocket = m_refIoService.m_IoSocketMgr.GetFreeIoSocket();
			if( pIoSocket )
			{
				CLog::DebugLog( "CIoService<TIoSocket>::CConnector::_Connect   %s:%d success, iosocketId=%d", 
					pstrIp, uPort, pIoSocket->GetId() );

				pIoSocket->SetSocket( Sock );
                pIoSocket->SetClientInfo( 0, inet_addr(pstrIp), 0, uPort );
				pIoSocket->SetDispatchEvent( m_refIoService._GetDispatchEvent() );
	
				SG_ASSERT( pIoSocket->m_pDispatchEvent );
				CLog::DebugLog( "CIoService<TIoSocket>::CConnector::_Connect %s:%d success, push connect event, id=%d", 
					pstrIp, uPort, pIoSocket->GetId() );
				pIoSocket->PushEvent( 
					CDispatchEvent::E_Event_Connect, pIoSocket->GetId() );

#ifdef _USE_WIN32
				if( false == m_refIoService._BindToCompletionPort( pIoSocket->GetSocket(), pIoSocket->GetCompletionKey()) )
#else
                if( 0 != CBasicEpollOp::EpollEventAdd( m_refIoService.m_nEpollFd, Sock, EPOLLIN | EPOLLET, pIoSocket->GetId() ) )
#endif
				{
					CLog::ErrorLog( "CIoService<TIoSocket>::CConnector::_Connect attach Error" );
					pIoSocket->Close();
				}
				else
				{
                    #ifdef _USE_WIN32
					if( pIoSocket->PostRecv() == false )
					{
						pIoSocket->Close();
					}
                    #endif
				}
			}
			else
			{
				#ifdef _USE_WIN32
				closesocket( Sock );
				#else
				close( Sock );
				#endif
				return false;
			}

			return true;
		}
		return false;
	}
			
	template<class TIoSocket>
	bool CIoService<TIoSocket>::CConnector::_DoConnect(s32 &Sock, const char *pstrIp, u16 uPort)
	{
		Sock = (s32)socket( AF_INET, SOCK_STREAM, 0 );
		if( INVALID_SOCKET == Sock ) 
		{
			return false;
		}

		struct sockaddr_in sa;
		memset( &sa, 0, sizeof(sa) );
		sa.sin_family = AF_INET;
		sa.sin_port = htons( uPort );
		#ifdef _USE_WIN32
		sa.sin_addr.s_addr = inet_addr( pstrIp );
		#else
		inet_pton( AF_INET, pstrIp, &sa.sin_addr );
		#endif

		if( SOCKET_ERROR == connect(Sock, (struct sockaddr*)(&sa), sizeof(sa)) ) 
		{
			CLog::ErrorLog( "CIoService<TIoSocket>::CConnector::_DoConnect connect(%s:%d) Error", pstrIp, uPort );
			#ifdef _USE_WIN32
			closesocket( Sock );
			#else
			close( Sock );
			#endif
			return false;
		}

		return true;
	}

	template<class TIoSocket>
	CIoService<TIoSocket>::CIoService(const CNetConfig &netCfg) : 
		m_netConfig(netCfg),
		m_Listener(*this), 
        m_IoSocketMgr(m_netConfig),
		m_Connector(*this)
	{
        srand( (unsigned int)time(0) );

		m_bStop = false;
		m_bInit = false;
		m_nWorkThreadNum = MAX_WORK_THREAD_NUM;	
		m_uTimeOut = m_netConfig.m_ioTimeOut;
		m_nDispatchEventNum = 0;
		m_DispatchEvent = NULL;

		#ifdef _USE_WIN32
			m_nWorkThreadNum = WORKER_THREADS_PER_PROCESSOR * GetProcessorsCount() + 2;
			if( m_nWorkThreadNum > MAX_WORK_THREAD_NUM )
			{
				m_nWorkThreadNum = MAX_WORK_THREAD_NUM;
			}
			m_hCompletePort = INVALID_HANDLE_VALUE;

        
            CSetupSocket::Instance().Setup();
		#else
            m_nEpollFd = -1;
		#endif
	}

	template<class TIoSocket>
	CIoService<TIoSocket>::~CIoService()
	{
		_Stop();
        CSetupSocket::Instance().Clean();
	}

	template<class TIoSocket>
	s32 CIoService<TIoSocket>::Init()
	{
		if( m_bInit == true )
		{
			return E_Error_Success;
		}

		int nRet = E_Error_Success;
		#ifdef _USE_WIN32
			nRet = _InitWin();
		#else
			nRet = _InitLinux();
		#endif

		if( nRet != E_Error_Success )
		{
			m_bInit = false;
		}
		else
		{
			nRet = m_IoSocketMgr.Init<TIoSocket>();
			SG_ASSERT( nRet == E_Error_Success );
			nRet = _InitWorkThread();
			if( nRet == E_Error_Success )
			{
				nRet = _InitDispatchEvent();
			}
		}

		if( nRet == E_Error_Success )
		{
			m_bInit = true;
		}

		return nRet;
	}

	template<class TIoSocket>
	void CIoService<TIoSocket>::Fini()
	{
		_Stop();
	}

	template<class TIoSocket>
	void CIoService<TIoSocket>::Dispatch()
	{
		for( s32 i=0; i<m_nDispatchEventNum; ++i )
		{
			bool bRet = m_DispatchEvent[i]->Start();
			SG_ASSERT( bRet == true );
		}
	}

	template<class TIoSocket>
	s32 CIoService<TIoSocket>::StartListen(s32 nPort)
	{
		return m_Listener.StartListen( nPort );
	}
		
	template<class TIoSocket>
	bool CIoService<TIoSocket>::Connect(const char *pstrIp, u16 uPort)
	{
		return m_Connector.Connect( pstrIp, uPort );
	}

	template<class TIoSocket>
    void CIoService<TIoSocket>::DisplayDetailInfo()
    {
        /*
        printf( "**********************************************************************************\n\n" );
        printf( "current connect socket info: %d\n", m_IoSocketMgr.m_SocketMgr.m_nInuseSize );
        for( s32 i=0, id=m_IoSocketMgr.m_SocketMgr.m_nInuseIoSocket; i<m_IoSocketMgr.m_SocketMgr.m_nInuseSize; ++i )
        {
            s32 nPrev = m_IoSocketMgr.m_SocketMgr.m_IoSocketArray[id].m_nPrevIoSocket;
            s32 nNext = m_IoSocketMgr.m_SocketMgr.m_IoSocketArray[id].m_nNextIoSocket;

            printf( "curren id:%d\tprev:%d\tnext:%d\n", id, nPrev, nNext );
			
            id = nNext;
        }
        
        printf( "\n\n" );
        for( s32 i=0, id=m_IoSocketMgr.m_SocketMgr.m_nInuseIoSocket; i<m_IoSocketMgr.m_SocketMgr.m_nInuseSize; ++i )
        {
            s32 nNext = m_IoSocketMgr.m_SocketMgr.m_IoSocketArray[id].m_nNextIoSocket;

            m_IoSocketMgr.m_SocketMgr.m_IoSocketArray[id].m_pIoSocket->DisplayDetailInfo();
            printf( "\n" );

            id = nNext;
        }
        
        printf( "\n\n" );
        printf( "**********************************************************************************\n\n\n" );//*/
	}

	template<class TIoSocket>
	void CIoService<TIoSocket>::Run()
	{
        //SGDEBUG("Ioserivce::run\n");
		#ifdef _USE_WIN32
			_IoRunWin();
		#else
			_IoRunLinux();
		#endif
	}

	template<class TIoSocket>
	s32 CIoService<TIoSocket>::_InitWorkThread()
	{
		for( s32 i=0; i<MAX_WORK_THREAD_NUM; ++i )
		{
			m_pWorkThread[i] = NULL;
		}

		for( s32 i=0; i<m_nWorkThreadNum; ++i )
		{
			m_pWorkThread[i] = new CThread( this );
			if( !m_pWorkThread[i] )
			{
				return E_Error_CreateWorkThreadFailed;	
			}
		}

		for( s32 i=0; i<m_nWorkThreadNum; ++i )
		{
			bool bRet = m_pWorkThread[i]->Start();
			SELF_ASSERT( bRet == true, ; );
		}

		return E_Error_Success;
	}

	template<class TIoSocket>
	s32 CIoService<TIoSocket>::_InitDispatchEvent()
	{
		m_nDispatchEventNum = m_netConfig.m_eventWorkerCount;
		m_DispatchEvent = new CDispatchEvent*[m_nDispatchEventNum];

		for( s32 i=0; i<m_nDispatchEventNum; ++i )
		{
			m_DispatchEvent[i] = NULL;
		}

		for( s32 i=0; i<m_nDispatchEventNum; ++i )
		{
			m_DispatchEvent[i] = new CDispatchEvent(m_IoSocketMgr, m_netConfig);
			if( !m_DispatchEvent[i] )
			{
				return E_Error_CreateDispatchEventFailed;	
			}
		}

		return E_Error_Success;
	}

	template<class TIoSocket>
	CDispatchEvent* CIoService<TIoSocket>::_GetDispatchEvent()
	{
		if( m_nDispatchEventNum > 1 )
		{
			return m_DispatchEvent[ rand() % m_nDispatchEventNum ];
		}
		else
		{
			return m_DispatchEvent[0];
		}
	}

	template<class TIoSocket>
	void CIoService<TIoSocket>::_Stop()
	{
		if( m_bStop )
		{
			return;
		}

		m_bStop = true;
		m_Listener.StopListen();

		#ifdef _USE_WIN32
			_StopWin();
		#endif

		for( s32 i=0; i<m_nWorkThreadNum; ++i )
		{
			SELF_ASSERT( m_pWorkThread[i] != NULL, continue; );
			m_pWorkThread[i]->Stop( m_netConfig.m_workThreadQuitTimeOut );

			SAFE_DELETE( m_pWorkThread[i] );
		}

		for( s32 i=0; i<m_nDispatchEventNum; ++i )
		{
			SELF_ASSERT( m_DispatchEvent[i] != NULL, continue; );
			m_DispatchEvent[i]->Stop( m_netConfig.m_workThreadQuitTimeOut );

			SAFE_DELETE( m_DispatchEvent[i] );
		}
		m_nDispatchEventNum = 0;
		SAFE_DELETE_ARRAY( m_DispatchEvent );

		#ifdef _USE_WIN32
			CloseHandle( m_hCompletePort );
			m_hCompletePort = INVALID_HANDLE_VALUE;
        #else
            CBasicEpollOp::CloseEpollFd( m_nEpollFd );
		#endif

		m_IoSocketMgr.Release();
	}

	/////////////////////////////////////////////////////////////////////////////

#ifdef _USE_WIN32

	template<class TIoSocket>
	s32 CIoService<TIoSocket>::_InitWin()
	{	
		m_hCompletePort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
		if( NULL == m_hCompletePort )
		{
			return E_Error_CreateCompletePortFailed;
		}

		return E_Error_Success;
	}

	template<class TIoSocket>
	void CIoService<TIoSocket>::_StopWin()
	{
		for( s32 i=0; i<m_nWorkThreadNum; ++i )
		{
			//Help threads get out of blocking - GetQueuedCompletionStatus()
			PostQueuedCompletionStatus( m_hCompletePort, 0, (DWORD)NULL, NULL );
		}
	}

	template<class TIoSocket>
	void CIoService<TIoSocket>::_IoRunWin()
	{
		while( m_bStop != true )
		{
			CompletionKey_t *pCompletionKey = NULL;
			Overlapped_t *pOverlapped = NULL;
			DWORD dwBytesTransfered = 0;

			//从完成队列中取数据
			//dwBytesTransfered是完成当次IO操作所发送或接收到得字节数
			BOOL bRet = GetQueuedCompletionStatus(
				m_hCompletePort,
				&dwBytesTransfered,
				(PULONG_PTR)&pCompletionKey,
				(LPOVERLAPPED*)&pOverlapped,
				m_uTimeOut
			);

			if( m_bStop )
			{
				break;
			}

			// 错误情况
			if( bRet == FALSE )
			{
				// 注意考虑超时的情况, Timer需要使用
				u32 uErrCode = GetLastError();
                if( uErrCode == WAIT_TIMEOUT )
                {
					// Time out
                }
                else
                {
                    // 如果不是超时
					if( pCompletionKey == NULL )
					{
						//服务器关闭,线程退出
						break;
					}

                    if( pOverlapped )
                    {
                        _DoCompletionIoError( *pCompletionKey, *pOverlapped, uErrCode );
                    }
                }
				continue;
			}

			//正常情况
			switch( pOverlapped->m_eEvtIocp )
			{
			case IOCP_ACCEPT:
                _DoIocpEventAccept( *pCompletionKey, *pOverlapped );
				break;
			case IOCP_RECV:
                _DoIocpEventRecv( *pCompletionKey, *pOverlapped, dwBytesTransfered );
				break;
			case IOCP_SEND:
                _DoIocpEventSend( *pCompletionKey, *pOverlapped, dwBytesTransfered );
				break;
			default:
				SG_ASSERT( false );
				break;
			};
		}
		
		m_bStop = true;
	}

	template<class TIoSocket>
	bool CIoService<TIoSocket>::_BindToCompletionPort(SOCKET hSock, const CompletionKey_t &Key)
	{
		if( NULL == CreateIoCompletionPort(
			(HANDLE)hSock, 
			m_hCompletePort, 
			(DWORD)&Key, 
			0) )
		{
			return false;
		}
		return true;
	}
	
	template<class TIoSocket>
	void CIoService<TIoSocket>::_DoIocpEventAccept(CompletionKey_t &Key, Overlapped_t &Overlapped)
	{
        // 建立新连接 
		CLog::DebugLog( "CIoService::_DoIocpEventAccept begin" );

        CIoSocketBase *pIoSocket = m_IoSocketMgr.GetFreeIoSocket();
        if( pIoSocket )
        {
			CLog::DebugLog( "CIoService::_DoIocpEventAccept iosocketId=%d", pIoSocket->GetId() );

			pIoSocket->SetDispatchEvent( _GetDispatchEvent() );
            m_Listener.DoAcceptEx( Overlapped, *pIoSocket );

			CLog::DebugLog( "CIoService<TIoSocket>::_DoIocpEventAccept Accept a client[%d,%d]", pIoSocket->GetId(), pIoSocket->GetSocket() );
			SG_ASSERT( pIoSocket->m_pDispatchEvent != NULL );
			pIoSocket->PushEvent( CDispatchEvent::E_Event_Accept, pIoSocket->GetId() );

            if( false == _BindToCompletionPort( pIoSocket->GetSocket(), pIoSocket->GetCompletionKey()) )
            {
				CLog::ErrorLog( "CIoService<TIoSocket>::_DoIocpEventAccept bind client[%d,%d] to io completionport failed, close", 
                    pIoSocket->GetId(), pIoSocket->GetSocket() );
		        pIoSocket->Close();
            }
            else
            {
                // 投递一个WSARecv
                if( pIoSocket->PostRecv() == false )
                {
					CLog::ErrorLog( "CIoService<TIoSocket>::_DoIocpEventAccept client[%d,%d] PostRecv failed, close", 
                        pIoSocket->GetId(), pIoSocket->GetSocket() );
		            pIoSocket->Close();
                    //return;
                }
            }
        }
        else
        {
            // 达到最大连接数
            closesocket( Overlapped.m_hSock );
			CLog::ErrorLog( "CIoService<TIoSocket>::_DoIocpEventAccept Reach max accept count" );
        }

        // 重新投递连接
        m_Listener.PostAcceptEx( Overlapped );
	}

	template<class TIoSocket>
	void CIoService<TIoSocket>::_DoIocpEventRecv(CompletionKey_t &Key, Overlapped_t &Overlapped, DWORD dwSize)
	{
        CIoSocketBase *pIoSocket = (CIoSocketBase*)Key.m_pData;
		if( dwSize == 0 && Overlapped.m_sWsabuf.len > 0 )
        {
            // 断开连接, 注意这里为了防止因为本身接受缓冲区已经满了，
			// 但是WSARecv出去的时候给的缓冲区的长度本身也是0，最后导致dwSize也是0
            CLog::DebugLog( "CIoService<TIoSocket>::_DoIocpEventRecv client[%d,%d] close", pIoSocket->GetId(), pIoSocket->GetSocket() );
		    pIoSocket->Close();
        }
        else
        {
            // 调整数据缓冲区,通知事件处理线程收到数据了
            pIoSocket->RecvDataSize( dwSize );

			SG_ASSERT( pIoSocket->m_pDispatchEvent );
            pIoSocket->PushEvent( CDispatchEvent::E_Event_Recv, pIoSocket->GetId() );
            
            // 投递一个接数据异步操作
            if( pIoSocket->PostRecv() == false )
            {
                CLog::ErrorLog( "CIoService<TIoSocket>::_DoIocpEventRecv client[%d,%d] PostRecv failed. close", 
                    pIoSocket->GetId(), pIoSocket->GetSocket() );
		        pIoSocket->Close();
            }
        }
	}

	template<class TIoSocket>
	void CIoService<TIoSocket>::_DoIocpEventSend(CompletionKey_t &Key, Overlapped_t &Overlapped, DWORD dwSize)
	{
        CIoSocketBase *pIoSocket = (CIoSocketBase*)Key.m_pData;
		if( dwSize == 0 && Overlapped.m_sWsabuf.len > 0 )
        {
            // 断开连接
			CLog::ErrorLog( "CIoService<TIoSocket>::_DoIocpEventSend client[%d,%d] close2", pIoSocket->GetId(), pIoSocket->GetSocket() );
		    pIoSocket->Close();
        }
        else
        {
            pIoSocket->SendDataSize( dwSize );
			SELF_ASSERT( pIoSocket->m_pDispatchEvent, return; );
            pIoSocket->PushEvent( CDispatchEvent::E_Event_OnSend, pIoSocket->GetId() );
        }
	}

	template<class TIoSocket>
	void CIoService<TIoSocket>::_DoCompletionIoError(CompletionKey_t &Key, Overlapped_t &Overlapped, u32 uErrCode)
    {
        if( Overlapped.m_eEvtIocp == IOCP_ACCEPT )
        {
            //如果连接请求失败,需要重新投递一个接受连接请求
            m_Listener.PostAcceptEx( Overlapped );
        }
        else
        {
            // 断开连接
            CIoSocketBase *pIoSocket = (CIoSocketBase*)Key.m_pData;
			CLog::ErrorLog( "CIoService<TIoSocket>::_DoCompletionIoError socket[%d,%d] io error, errcode=%u. close",
				pIoSocket->GetId(), pIoSocket->GetSocket(), uErrCode );
		    pIoSocket->Close();
        }
    }

	template<class TIoSocket>
	s32 CIoService<TIoSocket>::CListener::_StartWinListen()
	{
		m_ListenCompleteKey.m_pData = this;
		if( false == m_refIoService._BindToCompletionPort( 
			m_hListenSock, 
			m_ListenCompleteKey) ) 
		{
			return E_Error_BindToCompletePortFailed;
		}

		DWORD dwBytes = 0;
		GUID m_GuidAcceptEx = WSAID_ACCEPTEX;
		s32 iRet = WSAIoctl(
			m_hListenSock,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&m_GuidAcceptEx,
			sizeof(m_GuidAcceptEx),
			&m_lpfnAcceptEx,
			sizeof(LPFN_ACCEPTEX),
			&dwBytes,
			NULL,
			NULL );
		if( iRet == SOCKET_ERROR )
		{
			return E_Error_GetAcceptExPtrFailed; 
		}

		dwBytes = 0;
		GUID m_GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
		iRet = WSAIoctl(
			m_hListenSock,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&m_GuidGetAcceptExSockAddrs,
			sizeof(m_GuidGetAcceptExSockAddrs),
			&m_lpfnGetAcceptExSockAddrs,
			sizeof(LPFN_GETACCEPTEXSOCKADDRS),
			&dwBytes,
			NULL,
			NULL );
		if( iRet == SOCKET_ERROR )
		{
			return E_Error_GetAcceptExSockAddrFailed; 
		}

		for( s32 i=0; i<MAX_ACCEPT_COUNT; ++i )
		{
			PostAcceptEx( m_OverlappArray[i] );
		}

		return E_Error_Success;
	}

	template<class TIoSocket>
    void CIoService<TIoSocket>::CListener::DoAcceptEx(Overlapped_t &Overlapped, CIoSocketBase &IoSocket)
    {
        SOCKET hSock = Overlapped.m_hSock;

        //把listen套结字一些属性（包括socket内部接受/发送缓存大小等等）拷贝到新建立的套结字
        setsockopt( 
            hSock, 
            SOL_SOCKET, 
            SO_UPDATE_ACCEPT_CONTEXT, 
            (const char*)m_hListenSock, 
            sizeof(SOCKET) );
        IoSocket.SetSocket( (s32)hSock );
        
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
        m_lpfnGetAcceptExSockAddrs(
            Overlapped.m_Buf,
            0,
            iAddrLen,
            iAddrLen,
            (SOCKADDR**)&pLocalAddr,
            &iLocalAddrLen,
            (SOCKADDR**)&pRemoteAddr,
            &iRemoteAddrLen );

		IoSocket.SetClientInfo(
			pLocalAddr->sin_addr.s_addr,
			pRemoteAddr->sin_addr.s_addr,
			pLocalAddr->sin_port,
			pRemoteAddr->sin_port );
    }

	template<class TIoSocket>
	void CIoService<TIoSocket>::CListener::PostAcceptEx(Overlapped_t &Overlapped)
	{
		SOCKET hSock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
		if( hSock == INVALID_SOCKET )
		{
			CLog::ErrorLog( "PostAcceptEx::WSASocket failed. ErrCode=%d", WSAGetLastError() );
			return;
		}

		memset( (void*)&Overlapped.m_sOverlapped, 0, sizeof(Overlapped.m_sOverlapped) );
		Overlapped.m_hSock = hSock;
		Overlapped.m_eEvtIocp = IOCP_ACCEPT;

		/*
		 * 经过AcceptEx操作之后，地址信息已经填入指定的缓冲区
		 * 注意这里指定的长度
		 */
		int nAddrLen = sizeof(SOCKADDR_IN) + 16;
		DWORD dwBytes = 0;
		BOOL bRet = m_lpfnAcceptEx(
			m_hListenSock,
			hSock,
			Overlapped.m_Buf,
			0,
			nAddrLen,
			nAddrLen,
			&dwBytes,
			&Overlapped.m_sOverlapped );
		if( bRet != TRUE )
		{
			if( WSAGetLastError() != WSA_IO_PENDING )
			{
				CLog::ErrorLog( "PostAcceptEx::AcceptEx failed. ErrCode=%d", WSAGetLastError() );
				return;
			}
		}
	}


#else

	template<class TIoSocket>
	s32 CIoService<TIoSocket>::_InitLinux()
	{
		m_EpollEvents = new struct epoll_event[ m_netConfig.m_maxClientCount ];
        return CBasicEpollOp::CreatEpollFd( m_nEpollFd, m_netConfig.m_maxClientCount );
	}

	template<class TIoSocket>
	void CIoService<TIoSocket>::_IoRunLinux()
	{
        //SGDEBUG("IorunLinux\n");
		while( m_bStop != true )
		{
            s32 nFds = CBasicEpollOp::EpollWait(
                    m_nEpollFd,
                    m_EpollEvents,
                    m_netConfig.m_maxClientCount, 
                    m_uTimeOut );
            //SGDEBUG("EpollWait ret=%d\n", nFds);
            if( nFds == -1 )
            {
                if( errno == EINTR )
                {
                    continue;
                }

                // Error
                CLog::ErrorLog( "_IoRunLinux::EpollWait failed. ErrCode=%d ErrMsg=%s", errno, strerror(errno) );
                break;
            }

            if( nFds == 0 )
            {
                continue;
            }

            for( s32 i=0; i<nFds; ++i )
            {
                s32 nListenFd = m_Listener.GetListenSocket();
                CIoSocketBase::UEpollData EpollData;
                EpollData.u64Data = m_EpollEvents[i].data.u64;
                s32 nFd = EpollData.sData.n32Fd;
                s32 nId = EpollData.sData.n32Id;

                if( nFd==nListenFd && (m_EpollEvents[i].events & EPOLLIN) ) 
                {
                    // accept clients
                    _DoEpollAccept();
                }
                else
                {
                    // read data
                    if( m_EpollEvents[i].events & EPOLLIN )
                    {
                        _DoEpollRecv( nFd, nId );
                    }

                    // send data
                    if( m_EpollEvents[i].events & EPOLLOUT )
                    {
                        _DoEpollSend( nFd, nId );
                    }
                }
            }
        }

        m_bStop = true;
	}

	template<class TIoSocket>
    s32 CIoService<TIoSocket>::_SetSocketNonBlock(s32 nSocket)
    {
        s32 opt = fcntl( nSocket, F_GETFL );
        if( opt == -1 )
        {
            return E_Error_SetSocketNonBlockFailed;
        }
        opt |= O_NONBLOCK;
        if( fcntl( nSocket, F_SETFL, opt ) == -1 )
        {
            return E_Error_SetSocketNonBlockFailed;
        }

        return E_Error_Success;
    }

	template<class TIoSocket>
    void CIoService<TIoSocket>::_DoEpollAccept()
    {
        do
        {
            s32 nSockFd = _EpollAcceptSocket();
            if( nSockFd == INVALID_SOCKET )
            {
                break;
            }

            CIoSocketBase *pIoSocket = m_IoSocketMgr.GetFreeIoSocket();
            if( pIoSocket )
            {
                pIoSocket->SetSocket( nSockFd );
                pIoSocket->InitEpollFd( m_nEpollFd );
				pIoSocket->SetDispatchEvent( _GetDispatchEvent() );

				CLog::DebugLog( "CIoService::_DoEpollAccept accept a client. sockfd=%d sockid=%d IoSocket::GetSocket()=%d",
                        nSockFd, pIoSocket->GetId(), pIoSocket->GetSocket() );
				SG_ASSERT( pIoSocket->m_pDispatchEvent );
                pIoSocket->PushEvent( CDispatchEvent::E_Event_Accept, pIoSocket->GetId() );

                s32 nRet = CBasicEpollOp::EpollEventAdd( m_nEpollFd, nSockFd, EPOLLIN | EPOLLET, pIoSocket->GetId() );
                if( nRet != E_Error_Success )
                {
                    CLog::ErrorLog( "_DoEpollAccept::EpollEventAdd failed. ErrCode=%d", errno );
		            pIoSocket->Close();
                    continue;
                }
            }
            else
            {
                // reach max connection
                CLog::ErrorLog( "_DoEpollAccept failed. Reach max connection." );
                close( nSockFd );
                break;
            }
        } while( 1 );
    }

	template<class TIoSocket>
    void CIoService<TIoSocket>::_DoEpollRecv(s32 nSocket, s32 nId)
    {
        CIoSocketBase *pIoSocket = m_IoSocketMgr.FindIoSocket( nId );
        if( !pIoSocket )
        {
            return;
        }

        SG_ASSERT( nSocket == pIoSocket->GetSocket() );
        
        s32 nLen = pIoSocket->GetRecvFreeLen();
        byte *pData = pIoSocket->GetRecvFreePtr();

        do
        {
            if( nLen == 0 )
            {
                // 没有空间了
                break;
            }

            s32 nRecvLen = recv( pIoSocket->GetSocket(), pData, nLen, 0 );
            if( nRecvLen==-1 && (errno==EAGAIN || errno==EWOULDBLOCK) )
            {
                break;
            }
            else if( nRecvLen == 0 )
            {
                CLog::DebugLog( "CIoService::_DoEpollRecv remote client[sockid=%d, socket=%d] close", nId, nSocket );
		        pIoSocket->Close();
                break;
            }
            else if( nRecvLen > 0 )
            {
                //SGDEBUG( "[%d]recv buff:%d\n", pIoSocket->GetId(), nRecvLen);

                pIoSocket->RecvDataSize( nRecvLen );
				SG_ASSERT( pIoSocket->m_pDispatchEvent );
                pIoSocket->PushEvent( CDispatchEvent::E_Event_Recv, pIoSocket->GetId() );

                if( nRecvLen == nLen )
                {
                    nLen = pIoSocket->GetRecvFreeLen();
                    pData = pIoSocket->GetRecvFreePtr();
                }
                else
                {
                    break;
                }
            }
            else
            {
                CLog::ErrorLog( "_DoEpollRecv recv failed. Remote client error sockid=%d socket=%d. Close",
                        pIoSocket->GetId(), pIoSocket->GetSocket() );
		        pIoSocket->Close();
                break;
            }
        } while( 1 );
    }

	template<class TIoSocket>
    void CIoService<TIoSocket>::_DoEpollSend(s32 nSocket, s32 nId)
    {
        CIoSocketBase *pIoSocket = m_IoSocketMgr.FindIoSocket( nId );
        if( !pIoSocket )
        {
            return;
        }

        SG_ASSERT( nSocket == pIoSocket->GetSocket() );

        s32 nSendSize = 0;

        s32 nSendLen = pIoSocket->GetSendDataLen();
        byte *pSendData = pIoSocket->GetSendDataPtr();

        do
        {
            if( nSendLen == 0 )
            {
                break;
            }

            s32 nLen = send( nSocket, pSendData, nSendLen, 0 );
            if( nLen == 0 )
            {
                break;
            }
            else if( nLen == -1 )
            {
                if( errno==EAGAIN || errno==EWOULDBLOCK )
                {
                    break;
                }
                else
                {
                    CLog::ErrorLog( "_DoEpollSend send failed. Remote client error sockid=%d socket=%d. Close",
                            pIoSocket->GetId(), pIoSocket->GetSocket() );
		            pIoSocket->Close();
                    return;
                }
            }
    
            nSendSize += nLen;

            if( nLen < nSendLen )
            {
                nSendLen -= nLen;
                pSendData += nLen;
            }
            else
            {
                pIoSocket->SendDataSize( nSendSize );
                nSendSize = 0;

                nSendLen = pIoSocket->GetSendDataLen();
                pSendData = pIoSocket->GetSendDataPtr();
            }

        } while( 1 );

        if( nSendSize > 0 )
        {
            pIoSocket->SendDataSize( nSendSize );
        }

        // if all data is send, need to remove EPOLLOUT event 
        if( pIoSocket->GetSendDataSize() == 0 )
        {
            s32 nRet = CBasicEpollOp::EpollEventMod( m_nEpollFd, pIoSocket->GetSocket(), pIoSocket->GetId(), false );
            if( nRet != E_Error_Success )
            {
                CLog::ErrorLog( "_DoEpollSend::EpollEventMod failed. sockid=%d socket=%d. Close",
                        pIoSocket->GetId(), pIoSocket->GetSocket() );
		        pIoSocket->Close();
            }
        }
    }

	template<class TIoSocket>
    s32 CIoService<TIoSocket>::_EpollAcceptSocket()
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        s32 nFd = accept( 
                m_Listener.GetListenSocket(),
                (struct sockaddr*)&client_addr, 
                &client_len );
        if( nFd == -1 )
        {
            if( errno==EAGAIN || errno==EWOULDBLOCK )
            {
            }
            else
            {
                CLog::ErrorLog( "_EpollAcceptSocket accept failed, errno=%d errstr=%s", errno, strerror(errno) );
            }
            return INVALID_SOCKET;
        }

        s32 nRet = _SetSocketNonBlock( nFd );
        if( nRet != E_Error_Success )
        {
            CLog::ErrorLog( "_EpollAcceptSocket::_SetSocketNonBlock failed. socket=%d", nFd );
            close( nFd );
            return INVALID_SOCKET;
        }

        return nFd;
    }

#endif
};

#endif
