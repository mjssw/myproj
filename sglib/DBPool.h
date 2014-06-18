#ifndef _SG_DB_POOL_H_
#define _SG_DB_POOL_H_

#include "executor.h"
#include "DBQuery.h"
#include "DBCallbackFunctor.h"
#include <string>

namespace SGLib
{
	class CDBTask : public IRunnable
	{
	public:
		enum
		{
			E_DBTask_Param_Len = 128,
		};

		CDBTask(IDBQuery &DBQuery, const char *sql, CDBCallbackFunctor *pFunctor) : 
		  m_sql(sql),
		  m_pFunctor(pFunctor),
		  m_DBQuery(DBQuery),
		  m_paramLen( 0 )
		{
		}

		~CDBTask()
		{
			SAFE_DELETE( m_pFunctor );
		}

		void SetParam(void *param, s32 len)
		{
			if( len > E_DBTask_Param_Len )
			{
				// TODO
				return;
			}

			memcpy( m_param, param, len );
			m_paramLen = len;
		}

		void Run()
		{
			// TODO add ErrMsg
			IDBRecordSet *RecordSet = NULL;
			char errMsg[1024] = {0};
			bool ret = m_DBQuery.Execute( m_sql.c_str(), &RecordSet, errMsg );
			(*m_pFunctor)( RecordSet, NULL, m_param, m_paramLen );
			if( ret )
			{
				//SAFE_DELETE(RecordSet);
			}
			delete this;
		}

	private:
		std::string m_sql;
		CDBCallbackFunctor *m_pFunctor;
		IDBQuery		   &m_DBQuery;
		s32 m_paramLen;
		char m_param[E_DBTask_Param_Len]; 
	};

	template<class TDBQuery>
	class CDBPool : private NonCopyable
	{
	public :
		CDBPool(s32 nCount, s32 nSize) : 
		   m_DBExecutor(nCount, nSize),
		   m_pDBQuery(NULL)
		{}

		~CDBPool()
		{
			Stop();
		}

		bool Start(void *ConnectDBInfo, char *CharacterSet=0);
		void Stop();

		template<typename Func>
		bool Execute(const char *sql, Func callbackFunc, void *param, s32 len);

		template<class ClassObj, class ClassMemFunc>
		bool Execute(const char *sql, const ClassObj &classObj, ClassMemFunc memCallbackFunc, void *param, s32 len);
	
		bool Execute(const char *sql, CDBCallbackFunctor *callback, void *param, s32 len);

	private:
		bool _DoExecute(const char *sql, CDBCallbackFunctor *callback, void *param, s32 len);

	private:
		CExecutor m_DBExecutor;
		IDBQuery  *m_pDBQuery;
	};

	template<class TDBQuery>
	bool CDBPool<TDBQuery>::Start(void *ConnectDBInfo, char *CharacterSet)
	{
		m_pDBQuery = new TDBQuery();
		if( m_pDBQuery == NULL )
		{
			return false;
		}

		if( !m_pDBQuery->SetConnection( ConnectDBInfo, CharacterSet ) )
		{
			return false;
		}

		if( !m_pDBQuery->Connect() )
		{
			return false;
		}

		return m_DBExecutor.Start();
	}

	template<class TDBQuery>
	void CDBPool<TDBQuery>::Stop()
	{
		if( m_pDBQuery )
		{
			if( m_pDBQuery->IsConnected() )
			{
				m_pDBQuery->Close();
			}
			SAFE_DELETE( m_pDBQuery );
		}
		m_DBExecutor.Stop();
	}

	template<class TDBQuery>
	template<typename Func>
	bool CDBPool<TDBQuery>::Execute(const char *sql, Func callbackFunc, void *param, s32 len)
	{
		CDBCallbackFunctor *callback = new CDBCallbackFunctor( callbackFunc );
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

	template<class TDBQuery>
	template<class ClassObj, class ClassMemFunc>
	bool CDBPool<TDBQuery>::Execute(const char *sql, const ClassObj &classObj, ClassMemFunc memCallbackFunc, void *param, s32 len)
	{
		CDBCallbackFunctor *callback = new CDBCallbackFunctor( classObj, memCallbackFunc );
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
	
	template<class TDBQuery>
	bool CDBPool<TDBQuery>::Execute(const char *sql, CDBCallbackFunctor *callback, void *param, s32 len)
	{
		if( callback == NULL )
		{
			return false;
		}
		
		return _DoExecute( sql, callback, param, len );
	}

	template<class TDBQuery>
	bool CDBPool<TDBQuery>::_DoExecute(const char *sql, CDBCallbackFunctor *callback, void *param, s32 len)
	{
		CDBTask *pTask = new CDBTask( *m_pDBQuery, sql, callback );
		if( pTask == NULL )
		{
			return false;
		}
		if( len > 0 )
		{
			pTask->SetParam( param, len );
		}
		m_DBExecutor.Execute( pTask );
		return true;
	}
};

#endif
