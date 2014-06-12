#ifndef _BASIC_SERVER_EX_H_
#define _BASIC_SERVER_EX_H_

#include "IoSocketExManager.h"
#include "AccepterEx.h"
#include "ConnectorEx.h"
#include "IoServiceExPool.h"

namespace SGLib
{
	// TIoSocketEx MUST inherit from CIoSocketEx
	template<typename TIoSocketEx>
	class CBasicServerEx
	{
	public:
		CBasicServerEx(s32 ioServiceCount, s32 maxConnCount, s32 sendSize, s32 recvSize) : 
			m_ioServiceCount( ioServiceCount ),
			m_maxConnectionCount( maxConnCount ),
			m_maxSendSize( sendSize ),
			m_maxRecvSize( recvSize ),
			m_ioSocketMgr(),
			m_ioSerivcePool( ioServiceCount, m_ioSocketMgr ), 
			m_accepter( m_ioSerivcePool ),
			m_connector( m_ioSerivcePool )
		{
		}

		bool Start()
		{
			TIoSocketEx ioSocket(0);
			CIoSocketEx *ptest = ioSocket.Clone( 0 );
			if( !ptest )
			{
				return false;
			}
			SAFE_DELETE( ptest );
			if( !m_ioSocketMgr.Init( m_maxConnectionCount, m_maxSendSize, m_maxRecvSize, ioSocket ) )
			{
				return false;
			}
			return ( m_ioSerivcePool.Start() && m_connector.Start() );
		}

		bool StartListen(s32 port)
		{
			if( m_ioSerivcePool.IsStart() )
			{
				return m_accepter.Start( port );
			}
			return false;
		}

		void Stop()
		{
			m_connector.Stop();
			m_accepter.Stop();
			m_ioSerivcePool.Stop();
		}

		void Connect(const char *ip, s32 port)
		{
			m_connector.Connect( ip, port );
		}

		void DebugShowClientInfo(s32 id)
		{
			CIoSocketEx *ioSocket = m_ioSocketMgr.FindIoSocket( id );
			if( ioSocket )
			{
				ioSocket->DebugShowInfo();
			}
		}
		
		void DebugShowSocketExManagerUseInfo()
		{
			m_ioSocketMgr.DebugShowUseInfo();
		}
		
		void DebugShowSocketExManagerFreeInfo()
		{
			m_ioSocketMgr.DebugShowFreeInfo();
		}

	private:
		s32 m_ioServiceCount;
		s32 m_maxConnectionCount;
		s32 m_maxSendSize;
		s32 m_maxRecvSize;
		CIoSocketExMgr m_ioSocketMgr;
		CIoServiceExPool m_ioSerivcePool;
		CAccepterEx m_accepter;
		CConnectorEx m_connector;
	};

};
#endif
