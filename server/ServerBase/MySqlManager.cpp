#include "MySqlManager.h"
#include "EventWorkerExPool.h"
#include "StackTrace.h"
using namespace SGLib;
using namespace std;

CMysqlManager::CMysqlManager(s32 execCount, s32 queueSize, SGLib::CEventWorkerExPool *eventWorkerPool) : 
	m_eventWorkerPool( eventWorkerPool ),
	m_dbPool( execCount, queueSize )
{
}

CMysqlManager::~CMysqlManager()
{
	Stop();
}

bool CMysqlManager::Start(const char *addr, const char *dbname, const char *user, const char *pass, u32 port)
{
	MySqlDBInfo dbInfo( addr, dbname, user, pass, port );
	return m_dbPool.Start( &dbInfo );
}

void CMysqlManager::Stop()
{
	m_dbPool.Stop();
}

struct _dbEventParam
{
	void *recordSet;
	void *callbackObj;
};

void CMysqlManager::HandleEvent(s32 paramLen, char *paramData)
{
	SELF_ASSERT( paramLen == sizeof(_dbEventParam), return; );
	_dbEventParam *dbparam = (_dbEventParam*)paramData; 
	SELF_ASSERT( dbparam && dbparam->callbackObj, return; );

	CDBCallbackFunctor *func = (CDBCallbackFunctor*)(dbparam->callbackObj);
	IDBRecordSet *record = (IDBRecordSet*)(dbparam->recordSet);
	(*func)( record, NULL, NULL, 0 );
	SAFE_DELETE( func );
	SAFE_DELETE( record );
}

bool CMysqlManager::_DoExecute(const char *sql, CDBCallbackFunctor *callback)
{
	SDBParam param = { callback };
	return m_dbPool.Execute( sql, this, &CMysqlManager::_mysql_callback, &param, sizeof(SDBParam) );
}
	
void CMysqlManager::_mysql_callback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len)
{
	SELF_ASSERT( len == sizeof(SDBParam), return; );
	SDBParam *dbparam = (SDBParam*)param; 
	SELF_ASSERT( dbparam && dbparam->data, return; );
	CDBCallbackFunctor *func = (CDBCallbackFunctor*)(dbparam->data);

	if( m_eventWorkerPool )
	{
		_dbEventParam _param = { RecordSet, func };
		m_eventWorkerPool->PushEvent( 0, sizeof(_dbEventParam), &_param, this );
	}
	else
	{
		(*func)( RecordSet, NULL, NULL, 0 );
		SAFE_DELETE( func );
		SAFE_DELETE( RecordSet );			
	}
}
