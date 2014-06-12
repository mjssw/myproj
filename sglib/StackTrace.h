#ifndef _STACK_TRACE_H_
#define _STACK_TRACE_H_

#include "Define.h"
#include "Lock.h"

namespace SGLib
{
#ifdef _USE_WIN32
	typedef BOOL    (__stdcall *StackWalk64_t)(DWORD, HANDLE, HANDLE, LPSTACKFRAME64, PVOID, PREAD_PROCESS_MEMORY_ROUTINE64, PFUNCTION_TABLE_ACCESS_ROUTINE64, PGET_MODULE_BASE_ROUTINE64, PTRANSLATE_ADDRESS_ROUTINE64);
	typedef PVOID   (__stdcall *SymFunctionTableAccess64_t)(HANDLE, DWORD64);
	typedef DWORD64 (__stdcall *SymGetModuleBase64_t)(HANDLE, DWORD64);
	typedef BOOL    (__stdcall *SymCleanup_t)(HANDLE);
	typedef BOOL    (__stdcall *SymFromAddr_t)(HANDLE, DWORD64, PDWORD64, PSYMBOL_INFO);
	typedef BOOL    (__stdcall *SymGetLineFromAddr64_t)(HANDLE, DWORD64, PDWORD, PIMAGEHLP_LINE64);
	typedef BOOL    (__stdcall *SymInitialize_t)(HANDLE, PCTSTR, BOOL);
	typedef DWORD   (__stdcall *SymSetOptions_t)(DWORD);
	typedef DWORD   (__stdcall *SymGetOptions_t)();
	typedef BOOL	(__stdcall *SymGetSymFromAddr64_t)(HANDLE, DWORD64, PDWORD64, PIMAGEHLP_SYMBOL64);
#endif

	class CStackTrace
	{
	public:
		static CStackTrace& Instance();
		void DumpStack(const char *pOutFile=0);
		~CStackTrace();
	private:
		CStackTrace();
		#ifdef _USE_WIN32
			bool _Init();
			void _Fini();
			const char* BuildSymbolSearchPath();
			void _DumpStack(const char *pOutFile);
		#else
			bool _Init();
			void _Fini();
			void _DumpStack(const char *pOutFile);
		#endif
	private:
		bool    m_bInit;
        CLock   m_Lock;
		#ifdef _USE_WIN32
			HMODULE					   m_hDbgHelp;
			HANDLE					   m_hProcess;
			StackWalk64_t              m_pfnStackWalk64;
			SymCleanup_t               m_pfnSymCleanup;
			SymFromAddr_t              m_pfnSymFromAddr;
			SymFunctionTableAccess64_t m_pfnSymFunctionTableAccess64;
			SymGetModuleBase64_t       m_pfnSymGetModuleBase64;
			SymGetLineFromAddr64_t     m_pfnSymGetLineFromAddr64;
			SymInitialize_t            m_pfnSymInitialize;
			SymSetOptions_t            m_pfnSymSetOptions;
			SymGetOptions_t			   m_pfnSymGetOptions;
			SymGetSymFromAddr64_t	   m_pfnSymGetSymFromAddr64;
		#else
		#endif
	};
};

#endif
