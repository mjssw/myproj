#ifndef _MYSQL_MANAGER_H_
#define _MYSQL_MANAGER_H_

#include "DBPool.h"
#include "MySqlDBQuery.h"
#include "IEventEx.h"
#include "EventWorkerEx.h"
#include "EventWorkerExPool.h"

#define DB_PARAM_MAX_LEN (SGLib::CEventWorkerEx::E_Max_SessionEvent_ParamSize - 3*sizeof(void*))
class CMysqlManager : public SGLib::IEventEx
{
public:
	CMysqlManager(s32 execCount, s32 queueSize, SGLib::CEventWorkerExPool *eventWorkerPool);
	~CMysqlManager();

	bool Start(const char *addr, const char *dbname, const char *user, const char *pass, u32 port);
	void Stop();

	template<typename Func>
	bool Execute(const char *sql, Func callbackFunc, void *param, s32 len)
	{
		if( len > DB_PARAM_MAX_LEN )
		{
			return false;
		}

		SGLib::CDBCallbackFunctor *callback = new SGLib::CDBCallbackFunctor( callbackFunc );
		if( callback == NULL )
		{
			return false;
		}
		bool ret = _DoExecute( sql, callback, param, len );
		if( !ret )
		{
			SAFE_DELETE( callback );
		}
		return ret;
	}

	template<class ClassObj, class ClassMemFunc>
	bool Execute(const char *sql, const ClassObj &classObj, ClassMemFunc memCallbackFunc, void *param, s32 len)
	{
		if( len > DB_PARAM_MAX_LEN )
		{
			return false;
		}

		SGLib::CDBCallbackFunctor *callback = new SGLib::CDBCallbackFunctor( classObj, memCallbackFunc );
		if( callback == NULL )
		{
			return false;
		}
		bool ret = _DoExecute( sql, callback, param, len );
		if( !ret )
		{
			SAFE_DELETE( callback );
		}
		return ret;
	}

	virtual void HandleEvent(s32 paramLen, char *paramData);
	SGLib::IDBQuery* GetDBQuery(){ return m_dbPool.GetDBQuery(); }
	void Ping();

private:
	struct SDBParam
	{
		void *data;
		char param[DB_PARAM_MAX_LEN];
		s32 len;
	};

private:
	bool _DoExecute(const char *sql, SGLib::CDBCallbackFunctor *callback, void *data, s32 len); 
	void _mysql_callback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len);

private:
    SGLib::CEventWorkerExPool *m_eventWorkerPool;
	SGLib::CDBPool<SGLib::MySqlDBQuery> m_dbPool;
	SGLib::CLock m_connLock;
};

#endif
