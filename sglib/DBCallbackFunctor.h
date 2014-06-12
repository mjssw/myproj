#ifndef _SG_DB_CALLBACK_FUNCTOR_
#define _SG_DB_CALLBACK_FUNCTOR_

namespace SGLib
{
	class IDBRecordSet;

	class CFunctorImpl
	{
	public:
		virtual ~CFunctorImpl(){}
		virtual void operator()(IDBRecordSet *pRecordSet, char *pErrMsg, void *param, s32 len) = 0;
	};

	// 普通函数指针
	template<typename Func>
	class CFuncHandler : public CFunctorImpl
	{
	public:
		CFuncHandler(const Func &func) : m_Func(func)
		{}

		~CFuncHandler(){}

		void operator()(IDBRecordSet *pRecordSet, char *pErrMsg, void *param, s32 len)
		{
			return m_Func(pRecordSet, pErrMsg, param, len);
		}
	private:
		Func m_Func;
	};

	// 成员函数
	template<typename ClassObj, typename ClassMemFunc>
	class CMemFuncHandler : public CFunctorImpl
	{
	public:
		CMemFuncHandler(const ClassObj &Obj, ClassMemFunc MemFunc) : 
		  m_Obj(Obj),
		  m_MemFunc(MemFunc)
		{}

		~CMemFuncHandler(){}

		void operator()(IDBRecordSet *pRecordSet, char *pErrMsg, void *param, s32 len)
		{
			return ((*m_Obj).*m_MemFunc)(pRecordSet, pErrMsg, param, len);
		}
	private:
		ClassObj	 m_Obj;
		ClassMemFunc m_MemFunc;
	};

	class CDBCallbackFunctor
	{
	public:
		CDBCallbackFunctor() : m_pImpl(NULL)
		{}

		template<class Func>
		CDBCallbackFunctor(Func func)
		{
			m_pImpl = new CFuncHandler<Func>(func);
		}

		template<class ClassObj, class ClassMemFunc>
		CDBCallbackFunctor(const ClassObj &classObj, ClassMemFunc memFunc)
		{
			m_pImpl = new CMemFuncHandler<ClassObj, ClassMemFunc>(
				classObj, memFunc);
		}

		~CDBCallbackFunctor(){}

		void operator()(IDBRecordSet *pRecordSet, char *pErrMsg, void *param, s32 len)
		{
			if( !m_pImpl )
			{
				return;
			}
			return (*m_pImpl)(pRecordSet, pErrMsg, param, len);
		}

	private:
		CFunctorImpl *m_pImpl;
	};

	

}
#endif