#ifndef _SG_THREAD_H_
#define _SG_THREAD_H_

#include "Define.h"
#include "NonCopyable.h"
#include "Runnable.h"
#include "Event.h"
#include "Lock.h"

namespace SGLib
{
    class CThread;

    class CBasicThreadOps
    {
    public:
        static u32    _SuspendThread( CThread *pThread );
        static u32    _ResumeThread( CThread *pThread );
        static bool   _TerminateThread( CThread *pThread, u32 exitCode=0 );
        static void   _Sleep(u32 uMilliSec);
		static u64	  _GetThreadId();
    };

    // 基本线程对象
    class CThread : private NonCopyable
    {
    public:
        friend class CBasicThreadOps;

		// linux下 auto_run设置使用一个Event实现
        CThread( IRunnable *task, bool auto_run = true );
        ~CThread();
        
        bool Start();
        void Stop( s32 waitTime = -1 );
	    bool IsRun(){ return m_bRun; }

    private:
        void _Dispatch();

    private:
        IRunnable* m_pTask;
#ifdef _USE_WIN32
        HANDLE     m_hHandle;
        static unsigned int WINAPI ThreadFunc(void *pParam);
#else
	    pthread_t  m_Thread;
		CLock      m_StartLock;
	    static void* ThreadFunc(void *pParam);
#endif
        u32        m_uiThreadId;
        bool       m_bRun;
    };

    // 高级线程对象,循环执行m_pTask任务.
    // 每次执行前等待事件对象(bWaitForEvent设置为true).
    class CAdvThread : public IRunnable, private NonCopyable
    {
    public:
        CAdvThread( IRunnable*, bool bWaitForEvent=true );
        ~CAdvThread();

        bool Start();
        void Stop();
        void WakeUp();

        virtual void Run();

    private:
        IRunnable*  m_pTask;
        CThread     m_WorkThread;
        CEvent      m_WorkEvent;
        bool        m_bStop;
        bool        m_bWaitForEvent;
    };
};

#endif
