#ifndef _ADVANCE_SERVER_EX_H_
#define _ADVANCE_SERVER_EX_H_

#include "IoSocketExManager.h"
#include "AccepterEx.h"
#include "ConnectorEx.h"
#include "IoServiceExPool.h"
#include "EventWorkerEx.h"
#include "EventWorkerExPool.h"
#include "Timer.h"

namespace SGLib
{

	// TIoAdvSocketEx MUST inherit from CIoAdvSocketEx
	template<typename TIoAdvSocketEx, int queueSize=4096, int timerCount=4096>
	class CAdvanceServerEx
	{
	public:
		CAdvanceServerEx(s32 ioServiceCount, s32 maxConnCount, s32 sendSize, s32 recvSize, s32 workerCount) : 
			m_ioServiceCount( ioServiceCount ),
			m_maxConnectionCount( maxConnCount ),
			m_maxSendSize( sendSize ),
			m_maxRecvSize( recvSize ),
			m_workerCount( workerCount ),
			m_ioSocketMgr(),
			m_eventWorkerPool( workerCount, queueSize ),
			m_ioSerivcePool( ioServiceCount, m_ioSocketMgr, &m_eventWorkerPool ), 
			m_accepter( m_ioSerivcePool ),
			m_connector( m_ioSerivcePool ),
			m_timer( m_eventWorkerPool )
		{
		}
		
		bool Start(s32 port)
		{
			TIoAdvSocketEx ioSession(0);

			CIoSocketEx *ptest = ioSession.Clone( 0 );
			if( !ptest )
			{
				return false;
			}
			SAFE_DELETE( ptest );

			if( !m_ioSocketMgr.Init( 
				m_maxConnectionCount, m_maxSendSize, m_maxRecvSize, ioSession ) )
			{
				return false;
			}

			if( !m_timer.Start() )
			{
				return false;
			}

			return ( m_eventWorkerPool.Start() && 
					 m_ioSerivcePool.Start() && 
					 m_accepter.Start( port ) &&
					 m_connector.Start() );
		}

		void Stop()
		{
			m_timer.Stop();
			m_connector.Stop();
			m_accepter.Stop();
			m_ioSerivcePool.Stop();
			m_eventWorkerPool.Stop();
		}

		s32 AddTimer(u32 elapse, CallBackFunc func, void *data, s32 len, bool isPersist)
		{
			return m_timer.AddTimer( elapse, func, data, len, isPersist );
		}

        void DelTimer(s32 timerId)
		{
			return m_timer.DelTimer( timerId );
		}
		
		void Connect(const char *ip, s32 port)
		{
			m_connector.Connect( ip, port );
		}

	private:
		s32 m_ioServiceCount;
		s32 m_maxConnectionCount;
		s32 m_maxSendSize;
		s32 m_maxRecvSize;
		s32 m_workerCount;
		CIoSocketExMgr m_ioSocketMgr;
		CEventWorkerExPool m_eventWorkerPool;
		CIoServiceExPool m_ioSerivcePool;
		CAccepterEx m_accepter;
		CConnectorEx m_connector;

		class CCallbackTimer : public IEventEx
		{
		public:
			
			struct CallBackHead
			{
				CallBackFunc func;
				s32 len;
				void *obj;
			};

			enum
			{
				E_Timer_DataLen = CEventWorkerEx::E_Max_SessionEvent_ParamSize - sizeof(CallBackHead) - sizeof(void*), 
			};
			
			struct CallBackData
			{
				CallBackHead head;
				char data[E_Timer_DataLen];
			};

			CCallbackTimer(CEventWorkerExPool &eventWorkerPool) : 
				m_eventWorkerPool(eventWorkerPool), m_timer()
			{
				SG_ASSERT( sizeof(CallBackData) < CTimer<timerCount>::E_Timer_UserDataLen );
			}

			virtual void HandleEvent(s32 len, char *data)
			{
				SELF_ASSERT( data!=NULL && len>0, return; );
				CallBackData *cbData = (CallBackData*)data;
				cbData->head.func( cbData->data, cbData->head.len );
			}
			
			bool Start(){ return m_timer.Start(); }
			void Stop(){ m_timer.Stop(); }

			s32 AddTimer(u32 elapse, CallBackFunc func, void *data, s32 len, bool isPersist)
			{
				if( len > E_Timer_DataLen )
				{
					return INVALID_VAL;
				}

				CallBackData cbData;
				cbData.head.obj = this;
				cbData.head.func = func;
				cbData.head.len = (len>E_Timer_DataLen) ? E_Timer_DataLen : len;
				if( cbData.head.len > 0 )
				{
					memcpy( cbData.data, data, cbData.head.len );
				}

				return m_timer.AddTimer( elapse, TimerCallBack, &cbData, sizeof(cbData), isPersist );
			}

			void DelTimer(s32 timerId)
			{
				return m_timer.DelTimer( timerId );
			}

			static void TimerCallBack(void *data, s32 len)
			{
				SELF_ASSERT( data!=NULL && len>0, return; );
				CallBackData *cbData = (CallBackData*)data;
				CCallbackTimer *pthis = (CCallbackTimer*)(cbData->head.obj);
				pthis->m_eventWorkerPool.PushEvent(
					0, len, data, (IEventEx*)cbData->head.obj );
			}
		protected:
			CEventWorkerExPool &m_eventWorkerPool;
			CTimer<timerCount> m_timer;
		} m_timer;
	};

};

#endif
