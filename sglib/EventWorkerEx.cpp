#include "EventWorkerEx.h"
#include "IEventEx.h"
#include "StackTrace.h"
#include "Log.h"

using namespace SGLib;

CEventWorkerEx::CEventWorkerEx(s32 eventQueueSize) : 
    m_workSignal(),
    m_worker( NULL ),
	m_isStop( true ),
	m_eventQueue( eventQueueSize ),
    m_freeEventQueue( eventQueueSize )
{
    bool ret = _Init( eventQueueSize );
    SG_ASSERT( ret );
}

CEventWorkerEx::~CEventWorkerEx()
{
    Stop();
    _Fini();
}

void CEventWorkerEx::PushEvent(s32 paramLen, void *paramData, void *obj)
{
    if( obj == NULL || 
        (paramLen>0 && paramData==NULL) )
    {
        return;
    }

    SEvent *evt = NULL;
    if( !m_freeEventQueue.Pop( evt ) )
    {
        evt = new SEvent(); 
        SELF_ASSERT( evt!=NULL, return; );
    }
    SELF_ASSERT( paramLen<=E_Max_SessionEvent_ParamSize, return; );
    evt->paramLen = paramLen;
    evt->obj = obj;
    memcpy( evt->paramData, paramData, paramLen );

	bool ret = m_eventQueue.Push( evt );
	SELF_ASSERT( ret, return; );
    m_workSignal.SetEvent();
}

bool CEventWorkerEx::Start()
{
	m_isStop = false;
	m_worker = new CThread( this );
	if( m_worker != NULL )
	{
		if( m_worker->Start() )
		{
			return true;
		}
	}
	m_isStop = true;
	return false;
}

void CEventWorkerEx::Stop()
{
	if( !m_isStop && m_worker )
	{
		m_isStop = true;
		m_workSignal.SetEvent();
		m_worker->Stop();
	    SAFE_DELETE( m_worker );
	}
}

void CEventWorkerEx::Run()
{
	CLog::DebugLog("CEventWorker::Run Start");

	while( !m_isStop )
	{
		m_workSignal.Wait(); 

		if( m_isStop )
		{
			break;
		}

		while( !m_eventQueue.Empty() )
		{
            SEvent *evt = NULL;
			if( m_eventQueue.Pop( evt ) )
            {
                SELF_ASSERT( evt->obj!=NULL, continue; );
                IEventEx *e = (IEventEx*)evt->obj;
                e->HandleEvent( evt->paramLen, evt->paramData );

				bool ret = m_freeEventQueue.Push( evt );
                SELF_ASSERT( ret, continue; );
            }
		}
	}
	
	CLog::DebugLog( "CEventWorker::Run End" );
}

bool CEventWorkerEx::_Init(s32 count)
{
    for( s32 i=0; i<count; ++i )
    {
        SEvent *evt = new SEvent();
        SELF_ASSERT( evt!=NULL, return false; );
        bool ret = m_freeEventQueue.Push( evt );
        SELF_ASSERT( ret, return false; );
    }
    return true;
}

void CEventWorkerEx::_Fini()
{
    SEvent *evt = NULL;
    while( !m_eventQueue.Empty() )
    {
        if( m_eventQueue.Pop( evt ) )
        {
            SAFE_DELETE( evt );
        }
    }

    while( !m_freeEventQueue.Empty() )
    {
        if( m_freeEventQueue.Pop( evt ) )
        {
            SAFE_DELETE( evt );
        }
    }
}

