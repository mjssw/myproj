#include "EventWorkerExPool.h"
#include "EventWorkerEx.h"
#include "IEventEx.h"
#include "StackTrace.h"
using namespace SGLib;

CEventWorkerExPool::CEventWorkerExPool(s32 count, s32 queueSize) : 
	m_count( count ),
	m_queueSize( queueSize ),
	m_eventWorkerPool( NULL ),
	m_lock()
{
	if( m_count <= 0 )
	{
		m_count = 1;
	}
	if( m_count > E_Max_EventWorker_Count )
	{
		m_count = E_Max_EventWorker_Count;
	}
}

CEventWorkerExPool::~CEventWorkerExPool()
{
	Stop();
}

bool CEventWorkerExPool::Start()
{
	if( m_eventWorkerPool != NULL )
	{
		return true;
	}

	m_eventWorkerPool = new CEventWorkerEx*[ m_count ];
	SELF_ASSERT( m_eventWorkerPool!=NULL, return false; );

	bool ret = true;
	for( s32 i=0; i<m_count; ++i )
	{
		m_eventWorkerPool[i] = new CEventWorkerEx( m_queueSize );
		SELF_ASSERT( m_eventWorkerPool[i]!=NULL, return false; );
		ret &= m_eventWorkerPool[i]->Start();
	}

	return ret;
}

void CEventWorkerExPool::Stop()
{
	if( m_eventWorkerPool )
	{
		for( s32 i=0; i<m_count; ++i )
		{
			m_eventWorkerPool[i]->Stop();
			SAFE_DELETE( m_eventWorkerPool[i] );
		}
		SAFE_DELETE_ARRAY( m_eventWorkerPool );
		m_count = 0;
	}
}

void CEventWorkerExPool::PushEvent(s32 worker, s32 paramLen, void *paramData, IEventEx *obj)
{
	worker %= m_count;
	SELF_ASSERT( m_eventWorkerPool[worker] != NULL, return; );
	m_eventWorkerPool[ worker ]->PushEvent( paramLen, paramData, obj );
}
