#ifndef _MYSQL_MANAGER_H_
#define _MYSQL_MANAGER_H_

#include "DBPool.h"
#include "MySqlDBQuery.h"
#include "IEventEx.h"
#include "EventWorkerExPool.h"

class CMysqlManager : public SGLib::IEventEx
{
public:
	CMysqlManager(s32 execCount, s32 queueSize, SGLib::CEventWorkerExPool *eventWorkerPool);
	~CMysqlManager();

	bool Start(const char *addr, const char *dbname, const char *user, const char *pass, u32 port);
	void Stop();

	struct SDBParam
	{
		void *data;
	};

	template<typename Func>
	bool Execute(const char *sql, Func callbackFunc)
	{
		SGLib::CDBCallbackFunctor *callback = new SGLib::CDBCallbackFunctor( callbackFunc );
		if( callback == NULL )
		{
			return false;
		}
		bool ret = _DoExecute( sql, callback );
		if( !ret )
		{
			SAFE_DELETE( callback );
		}
		return ret;
	}

	template<class ClassObj, class ClassMemFunc>
	bool Execute(const char *sql, const ClassObj &classObj, ClassMemFunc memCallbackFunc)
	{
		SGLib::CDBCallbackFunctor *callback = new SGLib::CDBCallbackFunctor( classObj, memCallbackFunc );
		if( callback == NULL )
		{
			return false;
		}
		bool ret = _DoExecute( sql, callback );
		if( !ret )
		{
			SAFE_DELETE( callback );
		}
		return ret;
	}

	virtual void HandleEvent(s32 paramLen, char *paramData);

private:
	bool _DoExecute(const char *sql, SGLib::CDBCallbackFunctor *callback); 
	void _mysql_callback(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len);

private:
    SGLib::CEventWorkerExPool *m_eventWorkerPool;
	SGLib::CDBPool<SGLib::MySqlDBQuery> m_dbPool;
};

#endif
