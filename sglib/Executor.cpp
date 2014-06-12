#include "Executor.h"
using namespace SGLib;

CExecutor::CExecutor(s32 nCount, s32 nSize) :
    m_pWorkers( NULL )
  , m_nWorkerCount( nCount )
  , m_TaskQueue( nSize )
  , m_FreeWorkerQueue( nCount+1 )
{
    if( m_nWorkerCount > 0 )
    {
        m_pWorkers = new CWorker[ m_nWorkerCount ];

        if( m_pWorkers )
        {
            for( s32 i=0; i<m_nWorkerCount; ++i )
            {
                m_pWorkers[i].Init( this );
            }
        }
    }
}
 
CExecutor::~CExecutor()
{
    Stop();

    if( m_pWorkers )
    {
        delete [] m_pWorkers;
        m_pWorkers = NULL;
    }
}
 
bool CExecutor::Start()
{
    if( m_nWorkerCount>0 && m_pWorkers )
    {
        for( s32 i=0; i<m_nWorkerCount; ++i )
        {
            m_pWorkers[i].Start();

            m_FreeWorkerQueue.Push( &m_pWorkers[i] );
        }

        return true;
    }

    return false;
}
 
void CExecutor::Stop()
{
    if( m_nWorkerCount>0 && m_pWorkers )
    {
        for( s32 i=0; i<m_nWorkerCount; ++i )
        {
            m_pWorkers[i].Stop();
        }
    }
}
 
void CExecutor::Execute( IRunnable* task )
{
    m_TaskQueue.Push( task );

    WakeUpWorker();
}

void CExecutor::WakeUpWorker()
{
    // 如果不存在空闲worker,新的任务会由当前正在执行的某一个worker执行
    CWorker *worker = NULL;
    if( true == m_FreeWorkerQueue.Pop( worker ) )
    {
        if( worker )
        {
            worker->WakeUp();
        }
    }
}

////////////////////////////////////////////////////////////////

CExecutor::CWorker::CWorker() :
    m_pThis( NULL )
  , m_WorkThread( this, false )
  , m_bStop( true )
{
}
            
CExecutor::CWorker::~CWorker()
{
    Stop();
}
 
void CExecutor::CWorker::Init( CExecutor* pExecutor )
{
    m_pThis = pExecutor;
}

bool CExecutor::CWorker::Start()
{
    m_bStop = false;
    return m_WorkThread.Start();
}
            
void CExecutor::CWorker::Stop()
{
    m_bStop = true;
    WakeUp();
    m_WorkThread.Stop();
}

void CExecutor::CWorker::WakeUp()
{
    m_WorkEvent.SetEvent();
}
            
void CExecutor::CWorker::Run()
{
    while( 1 )
    {
        m_WorkEvent.Wait();

        if( m_bStop == true )
        {
            break;
        }

        if( m_pThis )
        {
            while( 1 )
            {
                IRunnable *task = NULL;
                if( true == m_pThis->m_TaskQueue.Pop( task ) )
                {
                    if( task )
                    {
                        task->Run();
                        continue;
                    }
                }
                break;
            }
        }

        // 当前工作者已经完成这阶段任务
        // 加入到空闲队列中
        m_pThis->m_FreeWorkerQueue.Push( this );
    }
}
