#ifndef _CLIENT_LIB_H_
#define _CLIENT_LIB_H_

#include "Thread.h"
#include "Log.h"
#include <map>

namespace SGLib
{
	class CClientBase;
	class CClientGroupBase
	{
	public:
		virtual ~CClientGroupBase(){}
		virtual void CloseClient(CClientBase *client){}
	};

	class CClientBase
	{
	public:
		enum
		{
			E_Client_Recv_Buf_Size = (1<<14),	// 16kb
			E_Max_Wait_Conn_Sec = 5,			// 10秒连接超时
		};

		template<class TClient>
		friend class CClientGroup;

		CClientBase(int id) : 
			m_id(id),
			m_hSock(0), 
			m_bConnect(false), 
			m_head(0), 
			m_len(0),
			m_bConnecting(false),
			m_bHasConnWillProc(false),
			m_clientGroup( NULL )
		{}

		virtual ~CClientBase()
		{
			Fini();
		}

		virtual int OnRecv(char *buf, int len)
		{
			return len;
		}

		virtual void OnConnect()
		{
		}

		virtual void OnClose()
		{
		}

		virtual void OnConnectError()
		{
		}

		int GetId(){ return m_id; }
		int GetSocket(){ return m_hSock; }
		bool IsConnect(){ return m_bConnect; }
		bool Send(char *buf, int len)
		{
			if( m_hSock == 0 ) 
			{
				return false;
			}
			char *pBuf = buf;
			while( true )
			{
				int sendLen = send( m_hSock, pBuf, len, 0 );
				if( sendLen == SOCKET_ERROR )
				{
#if _USE_WIN32
					SGDEBUG("send error:%d\n", WSAGetLastError());
#else
					SGDEBUG("send error:%d\n", errno);
#endif
					return false;
				}
				if( sendLen < len )
				{
					pBuf += sendLen;
					len -= sendLen;
				}
				else
				{
					return true;
				}
			}
		}

		bool Init(const char *ip, int port)
		{
			m_bConnecting = true;
			m_head = 0;
			m_len = 0;
			m_hSock = socket( AF_INET, SOCK_STREAM, 0 );
			if( m_hSock == INVALID_SOCKET )
			{
				return false;
			}
#ifdef _USE_WIN32
			int timeout = 2000;
			// 设置接收超时
			int ret = setsockopt( m_hSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int) );
			if( ret == SOCKET_ERROR )
			{
                SGDEBUG("setsockopt failed, errno=%d, errmsg=%s\n", errno,
                    strerror(errno));
                _CloseSocket( m_hSock );
				m_hSock = 0;
				return false;
			}

			// 设置发送超时
			ret = setsockopt( m_hSock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(int) );
			if( ret == SOCKET_ERROR )
			{
                _CloseSocket( m_hSock );
				m_hSock = 0;
				return false;
			}
#endif

			//设置非阻塞方式连接
			if( _SetSockBlockingMode( false ) )
			{
				return _DoConnect(ip, port);
			}

			return false;
		}

		void Fini()
		{
			if( m_clientGroup )
			{
				m_clientGroup->CloseClient( this );
			}
		}

		void ClearHasConnFlag()
		{
			m_bHasConnWillProc = false;
		}

		bool HasConnWillProc()
		{
			return m_bHasConnWillProc;
		}

	private:
		void _SetClientGroup(CClientGroupBase *clientGroup)
		{
			m_clientGroup = clientGroup;
		}

		void _Fini()
		{
			CLog::InfoLog( "client[%d:%d] Fini begin", GetId(), GetSocket() );
			OnClose();
			CGuardLock<CLock> g( m_socketLock );
			if( m_hSock != 0 )//&& m_bConnect )
			{
				CLog::InfoLog( "client[%d:%d] Fini", GetId(), GetSocket() );

                _CloseSocket( m_hSock );
				m_hSock = 0;
				m_bConnect = false;
				m_bConnecting = false;
				m_head = 0;
				m_len = 0;

				CLog::InfoLog( "client[%d:%d] Fini success", GetId(), GetSocket() );
			}
		}

        void _CloseSocket( int hSock )
        {
#if _USE_WIN32
            closesocket( hSock );
#else
            close( hSock );
#endif
        }

		void _ResetRecvBuff()
		{
			memmove( m_bufRecv, m_bufRecv+m_head, m_len );
			m_head = 0;
		}

		bool _SetSockBlockingMode(bool isSetBlock)
		{
#if _USE_WIN32
			unsigned long ul = 1;
			if( isSetBlock )
			{
				ul = 0;
			}
			int ret = ioctlsocket( m_hSock, FIONBIO, (unsigned long*)&ul );
			if( ret == SOCKET_ERROR )
			{
                _CloseSocket( m_hSock );
				m_hSock = 0;
				return false;
			}
#else
            int opt = fcntl( m_hSock, F_GETFL );
            if( opt < 0 )
            {
                _CloseSocket( m_hSock );
				m_hSock = 0;
				return false;
            }
            if( isSetBlock )
            {
                opt &= ~O_NONBLOCK;
            }
            else
            {
                opt |= O_NONBLOCK;
            }
            if( fcntl( m_hSock, F_SETFL, opt ) == -1 )
            {
                _CloseSocket( m_hSock );
				m_hSock = 0;
				return false;
            }
#endif
			return true;
		}

		bool _DoConnect(const char *ip, int port)
		{
#if _USE_WIN32
			// 准备开始连接
			SOCKADDR_IN	sa;
			memset( &sa, 0, sizeof(sa) );
			sa.sin_family = AF_INET;
			sa.sin_port = htons( port );
			sa.sin_addr.s_addr = inet_addr( ip );

			// 连接
			int ret = connect( m_hSock, (SOCKADDR*)&sa, sizeof(sa) );
			if( ret == 0 )
			{
				// 连接成功了
				m_bHasConnWillProc = true;
				return true;
			}
			if( ret == SOCKET_ERROR )
			{
				int errorNo = WSAGetLastError();
				if( errorNo == WSAEWOULDBLOCK )
				{
					return true;
				}
			}

            _CloseSocket( m_hSock );
			m_hSock = 0;
			return false;
#else
            struct sockaddr_in addr;
            memset( &addr, 0, sizeof(addr) );
            addr.sin_family = AF_INET;
            addr.sin_port = htons( port );
            if( inet_pton(AF_INET, ip, &addr.sin_addr) == 1 )
            {
                int ret = connect( m_hSock, (struct sockaddr*)&addr, sizeof(addr) );
                if( ret == 0 || (ret == -1 && errno == EINPROGRESS ) )
                {
                    return true;
                }
            }

            _CloseSocket( m_hSock );
			m_hSock = 0;
            return false;
#endif
		}

	private:
	public:
		int		m_id;
		int		m_hSock;
		bool	m_bConnect;
		char	m_bufRecv[E_Client_Recv_Buf_Size];
		int		m_head;
		int		m_len;

		bool	m_bConnecting;
		bool	m_bHasConnWillProc;
		CLock	m_socketLock;
		CClientGroupBase *m_clientGroup;
	};

	template<class TClient>
	class CClientGroup : public IRunnable, public CClientGroupBase 
	{
	public:
		enum
		{
			E_Client_Count_Pre_Group = 64,
		};

		CClientGroup(int id, int nClientCount) : 
			m_id(id),
			m_Worker(0), 
			m_StartLock(),
			m_fdSetLock()
		{
			m_nCount = nClientCount;
			if( m_nCount > E_Client_Count_Pre_Group )
			{
				m_nCount = E_Client_Count_Pre_Group;
			}

			for(int i=0; i<E_Client_Count_Pre_Group; ++i)
			{
				if( i < m_nCount )
				{
					m_ClientList[i] = new TClient( (id * E_Client_Count_Pre_Group + i) );
					if( m_ClientList[i] )
					{
						m_ClientList[i]->_SetClientGroup( this );
					}
				}
				else
				{
					m_ClientList[i] = 0;
				}
			}

			m_nMaxFd = 0;
			FD_ZERO( &m_fdConnSet );
			FD_ZERO( &m_fdReadSet );
			FD_ZERO( &m_fdExpSet );

			m_StartLock.Lock();
			m_StopWoker = false;
			m_Worker = new CThread(this);
			m_Worker->Start();

            //SGDEBUG("ClientGroup constructor, workerrun\n");
		}

		~CClientGroup()
		{
			for(int i=0; i<m_nCount; ++i)
			{
				if( m_ClientList[i] != 0 )
				{
					delete m_ClientList[i];
					m_ClientList[i] = 0;
				}
			}

			if( m_Worker != 0 )
			{
				if( m_Worker->IsRun() )
				{
					m_Worker->Stop();
				}
				delete m_Worker;
			}
		}

		virtual void CloseClient(CClientBase *client)
		{
			TClient *tclient = (TClient*)client; 
			_ClientClose( tclient ); 
		}

		virtual void Run()
		{
			m_StartLock.Lock();
            //SGDEBUG("CClientGroup run, m_StopWorker=%d\n", m_StopWoker );

			CLog::DebugLog( "ClientGroupWorker:%d Start", m_id );
			while( !m_StopWoker )
			{
				struct timeval stimeout;
				stimeout.tv_sec = 1;
				stimeout.tv_usec = 0;

				fd_set rdSet, wtSet, expSet;
				{
					CGuardLock<CLock> g( m_fdSetLock );
					rdSet = m_fdReadSet;
					wtSet = m_fdConnSet;
					expSet = m_fdExpSet;
				}
				int ret = select( m_nMaxFd+1, &rdSet, &wtSet, &expSet, &stimeout );
				if( ret > 0 )
				{
					for(int i=0; i<m_nCount; ++i)
					{
						int fd = m_ClientList[i]->GetSocket();
						if( FD_ISSET( fd, &rdSet ) )
						{
							//if( m_ClientList[i]->IsConnect() )
							{
								int end = m_ClientList[i]->m_head + m_ClientList[i]->m_len;
								char *buf = m_ClientList[i]->m_bufRecv + end;
								int len = CClientBase::E_Client_Recv_Buf_Size - end;
								int recvLen = recv( fd, buf, len, 0 );
								if( recvLen == SOCKET_ERROR )
								{
									// ERROR
									CLog::InfoLog( "client[%d:%d] recv error will _ClientClose", 
										m_ClientList[i]->GetId(), m_ClientList[i]->GetSocket() );
									_ClientClose( m_ClientList[i] );
								}
								//else if( recvLen == 0 )
								//{
								//	// SOCKET CLOSE
								//	m_ClientList[i]->Fini();
								//	FD_CLR( fd, &m_fdReadSet );
								//}
								else
								{
									if( recvLen > 0 || (recvLen == 0 && len == 0) )
									{
										m_ClientList[i]->m_len += recvLen;
										int procLen = m_ClientList[i]->OnRecv(
											m_ClientList[i]->m_bufRecv + m_ClientList[i]->m_head,
											m_ClientList[i]->m_len);
										m_ClientList[i]->m_head += procLen;
										m_ClientList[i]->m_len -= procLen;
										m_ClientList[i]->_ResetRecvBuff();
									}
									else
									{
										// SOCKET CLOSE
										CLog::InfoLog( "client[%d:%d] close normal will _ClientClose", 
											m_ClientList[i]->GetId(), m_ClientList[i]->GetSocket() );
										_ClientClose( m_ClientList[i] );
									}
								}
							}
						}

						if( FD_ISSET( fd, &wtSet ) )
						{
							_ClientConnectSuccess( m_ClientList[i] );
						}

						if( FD_ISSET( fd, &expSet ) )
						{
							SGDEBUG("socket error\n");
							if( m_ClientList[i]->m_bConnecting )
							{
								m_ClientList[i]->OnConnectError();
							}
							CLog::InfoLog( "client[%d:%d] exception will _ClientClose", 
								m_ClientList[i]->GetId(), m_ClientList[i]->GetSocket() );
							_ClientClose( m_ClientList[i] );
						}
					}
				}
				else if( ret == 0 )
				{
					// TIME OUT
					for(int i=0; i<m_nCount; ++i)
					{
						if( m_ClientList[i]->m_bConnecting )
						{
							if( m_ClientList[i]->HasConnWillProc() )
							{
								m_ClientList[i]->ClearHasConnFlag();
								_ClientConnectSuccess( m_ClientList[i] );
								continue;
							}
						}
					}
				}
				else
				{
					// ERROR
#ifdef _USE_WIN32
					int errorno = WSAGetLastError();
#else
					int errorno = errno;
#endif
					CLog::ErrorLog( "ClientGroupWorker:%d select error=%d, Stop", 
						 m_id, errorno );
					//break;
				}
			}
			
			CLog::DebugLog( "ClientGroupWorker:%d End", m_id );
		}

		bool Start(const char *ip, int port)
		{
			if( _Init(ip, port) )
			{
				m_StartLock.UnLock();
				return true;
			}
			return false;
		}

		void Stop()
		{
			m_StartLock.UnLock();
			m_StopWoker = true;
			m_Worker->Stop();
		}

	private:
		bool _Init(const char *ip, int port)
		{
			m_nMaxFd = 0;
			for(int i=0; i<m_nCount; ++i)
			{
				if( m_ClientList[i]->Init(ip, port) )
				{
					int fd = m_ClientList[i]->GetSocket();
					FD_SET( fd, &m_fdConnSet );
					FD_SET( fd, &m_fdExpSet );
					if( fd > m_nMaxFd )
					{
						m_nMaxFd = fd;
					}
				}
				else
				{
					return false;
				}
			}
			return true;
		}

		void _ClientConnectSuccess(TClient *client)
		{
			if( client )
			{
				int fd = client->GetSocket();
				client->m_bConnect = true;
				client->m_bConnecting = false;
				// 重新设置为阻塞模式
				client->_SetSockBlockingMode( true );
				{
					CGuardLock<CLock> g( m_fdSetLock );
					FD_CLR( fd, &m_fdConnSet );
					FD_SET( fd, &m_fdReadSet );
				}
				client->OnConnect();
			}
		}

		void _ClientClose(TClient *client)
		{
			if( client )
			{
				CLog::InfoLog( "client[%d:%d] close by call _ClientClose", 
					client->GetId(), client->GetSocket() );
				int fd = client->GetSocket();
				{
					CGuardLock<CLock> g( m_fdSetLock );
					FD_CLR( fd, &m_fdConnSet );
					FD_CLR( fd, &m_fdReadSet );
					FD_CLR( fd, &m_fdExpSet );
				}
				client->_Fini();
			}
		}

	private:
	public:
		int		 m_id;
		int		 m_nCount;
		TClient *m_ClientList[E_Client_Count_Pre_Group];
		CThread *m_Worker;
		bool     m_StopWoker;
		CLock    m_StartLock;
		CLock    m_fdSetLock;
		fd_set	 m_fdConnSet;
		fd_set	 m_fdReadSet;
		fd_set	 m_fdExpSet;
		int		 m_nMaxFd;
	};

	template<class TClient>
	class CClientManager
	{
	public:
		enum
		{
			E_Max_Group_Count = 100,
		};

		CClientManager(int nClientCount)
		{
#if _USE_WIN32
			WSADATA wsaData;
			WSAStartup( MAKEWORD(2,2), &wsaData );
#endif

			m_nCount = nClientCount / CClientGroup<TClient>::E_Client_Count_Pre_Group;
			if( (nClientCount % CClientGroup<TClient>::E_Client_Count_Pre_Group) != 0 )
			{
				++m_nCount;
			}
			if( m_nCount > E_Max_Group_Count )
			{
				m_nCount = E_Max_Group_Count;
			}

			for(int i=0; i<E_Max_Group_Count; ++i)
			{
				if( i < m_nCount )
				{
					m_ClientGroupList[i] = new CClientGroup<TClient>( i, nClientCount );
					nClientCount -= CClientGroup<TClient>::E_Client_Count_Pre_Group;
				}
				else
				{
					m_ClientGroupList[i] = 0;
				}
			}
		}

		~CClientManager()
		{
			for(int i=0; i<m_nCount; ++i)
			{
				if( m_ClientGroupList[i] != 0 )
				{
					delete m_ClientGroupList[i];
					m_ClientGroupList[i] = 0;
				}
			}

#if _USE_WIN32
			WSACleanup();
#endif
		}

		bool Start(const char *ip, int port)
		{
			int i = 0;
			bool ret = true;
			for(i=0; i<m_nCount; ++i)
			{
				if( !m_ClientGroupList[i]->Start(ip, port) )
				{
					ret = false;
					break;
				}
			}

			if( !ret )
			{
				for(int j=0; j<i; ++j)
				{
					m_ClientGroupList[i]->Stop();
				}
			}

			return ret;
		}

		void Stop()
		{
			for(int i=0; i<m_nCount; ++i)
			{
				m_ClientGroupList[i]->Stop();
			}
		}

	private:
	public:
		int m_nCount;
		CClientGroup<TClient> *m_ClientGroupList[E_Max_Group_Count];
	};
};
#endif
