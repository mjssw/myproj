#ifndef _SG_EXECUTOR_H_
#define _SG_EXECUTOR_H_

#include "Define.h"
#include "NonCopyable.h"
#include "Lock.h"
#include "Thread.h"
#include "Queue.h"

namespace SGLib
{
    // �̳߳�,��һ���߳�ִ����������е�����.
    // ����ͳһ��Execute�ӿ��ύ.
    class CExecutor : private NonCopyable
    {
    public:
        // nCount �����߳���
        // nSize  �������г�ʼ��С
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

        // һ�鹤���߳�,�����߳�������ȴ��ź�
        CWorker*     m_pWorkers;
        s32          m_nWorkerCount; 

        // �������,�����̴߳Ӵ˶�����ȡ����ִ��
        CSafeQueue<IRunnable*, CLock>   m_TaskQueue;

        // ���й����߶���
        CSafeQueue<CWorker*, CLock>     m_FreeWorkerQueue;
    };
};

#endif
