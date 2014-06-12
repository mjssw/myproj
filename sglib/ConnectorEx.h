#ifndef _CONNECTOR_EX_H_
#define _CONNECTOR_EX_H_

#include "Executor.h"
#include "IoServiceExPool.h"

namespace SGLib
{
	class CConnectorEx
	{
	public:
		enum
		{
			E_Connector_MaxIp_Len = 128,
			E_Connector_QueueSize = 128,
			E_Connector_ExecutorThreadCount = 2,
		};

		CConnectorEx(CIoServiceExPool &ioServicePool);
		~CConnectorEx();

		bool Start();
		void Stop();

		void Connect(const char *ip, s32 port);

	private:
		class CConnectObj : public IRunnable
		{
		public:
			CConnectObj(CConnectorEx *pthis) : m_pthis(pthis)
			{}
			virtual void Run();

			char m_ip[E_Connector_MaxIp_Len];
			s32 m_port;
			CConnectorEx *m_pthis;

		private:
			void _Connect(const char *ip, u16 port);
			s32 _DoConnect(const char *ip, u16 port);
		};
		
		CSafeQueue<CConnectObj*> m_freeConnQueue;
		CExecutor m_connectExecutor;
		CIoServiceExPool &m_ioServicePool;
	};
};

#endif
