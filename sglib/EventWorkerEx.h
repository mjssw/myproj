#ifndef _EVENT_WORKER_EX_H_
#define _EVENT_WORKER_EX_H_

#include "Queue.h"
#include "Thread.h"

namespace SGLib
{
	class CEventWorkerEx : public IRunnable
    {
    public:
        enum
        {
            E_Max_SessionEvent_ParamSize = 64,
        };

        struct SEvent
        {
            s32 paramLen;
            char paramData[E_Max_SessionEvent_ParamSize];
            void *obj;
        };

        CEventWorkerEx(s32 eventQueueSize);
		~CEventWorkerEx();

        void PushEvent(s32 paramLen, void *paramData, void *obj);
		bool Start();
		void Stop();
		void Run();

    private:
        bool _Init(s32 count);
        void _Fini();

    private:	
        CEvent m_workSignal;
		CThread *m_worker;
		bool m_isStop;
		CSafeQueue<SEvent*, CLock> m_eventQueue;
		CSafeQueue<SEvent*, CLock> m_freeEventQueue;
    };
}

#endif

