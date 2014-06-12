#include "IoServiceExPool.h"
#include "IoServiceEx.h"
#include "IoSocketExManager.h"
#include "OverlappedPoolEx.h"
#include "StackTrace.h"
using namespace SGLib;

CIoServiceExPool::CIoServiceExPool(s32 count, CIoSocketExMgr &ioSocketMgr, CEventWorkerExPool *eventWorkerPool) : 
	m_index( 0 ),
	m_count( count ),
	m_ioServicePool( NULL ),
	m_lock(),
	m_ioSocketMgr( ioSocketMgr ),
	m_eventWorkerPool( eventWorkerPool ),
	m_isStart( false )
{
	if( m_count <= 0 )
	{
		m_count = 1;
	}
	if( m_count > E_Max_IoService_Count )
	{
		m_count = E_Max_IoService_Count;
	}
}

CIoServiceExPool::~CIoServiceExPool()
{
	Stop();
}

bool CIoServiceExPool::Start()
{
	if( m_ioServicePool != NULL )
	{
		return true;
	}

	m_ioServicePool = new CIoServiceEx*[ m_count ];
	if( m_ioServicePool == NULL )
	{
		return false;
	}

	bool ret = true;
	for( s32 i=0; i<m_count; ++i )
	{
		m_ioServicePool[i] = new CIoServiceEx( m_ioSocketMgr, m_eventWorkerPool );
		SELF_ASSERT( m_ioServicePool[i]!=NULL, return false; );
		ret &= m_ioServicePool[i]->Start();
	}

	m_isStart = ret;
	return ret;
}

void CIoServiceExPool::Stop()
{
	for( s32 i=0; i<m_count; ++i )
	{
		m_ioServicePool[i]->Stop();
		SAFE_DELETE( m_ioServicePool[i] );
	}
	SAFE_DELETE_ARRAY( m_ioServicePool );
	m_count = 0;
	m_index = -1;
	m_isStart = false;
}

CIoServiceEx* CIoServiceExPool::GetIoService()
{
	CIoServiceEx *ioService = NULL;
	CGuardLock<CLock> g( m_lock );
	if( m_ioServicePool == NULL )
	{
		return NULL;
	}
	ioService = m_ioServicePool[ m_index ];
	++m_index;
	if( m_index >= m_count )
	{
		m_index = 0;
	}

	return ioService;
}
