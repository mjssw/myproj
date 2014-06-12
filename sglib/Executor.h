#ifndef _SG_EXECUTOR_H_
#define _SG_EXECUTOR_H_

#include "Define.h"
#include "NonCopyable.h"
#include "Lock.h"
#include "Thread.h"
#include "Queue.h"

namespace SGLib
{
    // 线程池,由一组线程执行任务队列中的任务.
    // 任务统一由Execute接口提交.
    class CExecutor : private NonCopyable
    {
    public:
        // nCount 工作线程数
        // nSize  工作队列初始大小
        CExecutor(s32 nCount, s32 nSize);
        ~CExecutor();

        bool Start();
        void Stop();
        void Execute( IRunnable* );

    private:
        void WakeUpWorker();

    private:
        friend class CWorker;
        class CWorker : public IRunnable
        {
        public:
            CWorker();
            ~CWorker();

            void Init( CExecutor* );
            bool Start();
            void Stop();
            void WakeUp();

            virtual void Run();

        private:
            CExecutor*  m_pThis;
            CThread     m_WorkThread;
            CEvent      m_WorkEvent;
            bool        m_bStop;
        };

        // 一组工作线程,所有线程启动后等待信号
        CWorker*     m_pWorkers;
        s32          m_nWorkerCount; 

        // 任务队列,工作线程从此队列中取数据执行
        CSafeQueue<IRunnable*, CLock>   m_TaskQueue;

        // 空闲工作者队列
        CSafeQueue<CWorker*, CLock>     m_FreeWorkerQueue;
    };
};

#endif
