#ifndef _IO_SERVICE_EX_POOL_H_
#define _IO_SERVICE_EX_POOL_H_

#include "Define.h"
#include "Lock.h"

namespace SGLib
{
	class CIoSocketExMgr;
	class CIoServiceEx;
	class CEventWorkerExPool;
	class CIoServiceExPool
	{
	public:
		enum
		{
			E_Max_IoService_Count = 32,
		};

		CIoServiceExPool(s32 count, CIoSocketExMgr &ioSocketMgr, CEventWorkerExPool *eventWorkerPool=NULL);
		~CIoServiceExPool();

		bool Start();
		void Stop();
		bool IsStart(){ return m_isStart; }
		CIoServiceEx* GetIoService();

	private:
		s32 m_index;
		s32 m_count;
		CIoServiceEx **m_ioServicePool;
		CLock m_lock;
		CIoSocketExMgr &m_ioSocketMgr;
		CEventWorkerExPool *m_eventWorkerPool;
		bool m_isStart;
	};
};

#endif
