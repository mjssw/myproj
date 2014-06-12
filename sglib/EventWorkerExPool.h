#ifndef _EVENT_WORKER_EX_POOL_H_
#define _EVENT_WORKER_EX_POOL_H_

#include "Define.h"
#include "Lock.h"

namespace SGLib
{
	class IEventEx;
	class CEventWorkerEx;
	class CEventWorkerExPool
	{
	public:
		enum
		{
			E_Max_EventWorker_Count = 32,
		};

		CEventWorkerExPool(s32 count, s32 queueSize);
		~CEventWorkerExPool();

		bool Start();
		void Stop();
        void PushEvent(s32 worker, s32 paramLen, void *paramData, IEventEx *obj);

	private:
		s32 m_count;
		s32 m_queueSize;
		CEventWorkerEx **m_eventWorkerPool;
		CLock m_lock;
	};
};

#endif
