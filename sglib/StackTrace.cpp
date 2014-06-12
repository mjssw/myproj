#include "StackTrace.h"
#include "Log.h"
using namespace SGLib;

#include <string>
using namespace std;

#ifdef _USE_WIN32
#include <WinNT.h>
#else
#include <execinfo.h>
#endif

CStackTrace& CStackTrace::Instance()
{
	static CStackTrace StackTrace;
	return StackTrace;
}

CStackTrace::CStackTrace() : m_bInit(false)
{
	m_bInit = _Init();
}

CStackTrace::~CStackTrace()
{
	if( m_bInit == true )
	{
		_Fini();
		m_bInit = false;
	}
}

void CStackTrace::DumpStack(const char *pOutFile)
{
	if( m_bInit != true )
	{
		return;
	}

	_DumpStack( pOutFile );
}

#ifdef _USE_WIN32

bool CStackTrace::_Init()
{
	m_hDbgHelp = LoadLibrary( "dbghelp.dll" );
    if( !m_hDbgHelp ) 
	{
		return false;
	}

	string strFuncName;
	bool bLoad = true;
	do
	{
		strFuncName = "StackWalk64";
        m_pfnStackWalk64 = (StackWalk64_t)GetProcAddress( m_hDbgHelp, strFuncName.c_str() );
        if( m_pfnStackWalk64 == NULL)
		{
			bLoad = false;
			break;
        }

        strFuncName = "SymFunctionTableAccess64";
        m_pfnSymFunctionTableAccess64 = (SymFunctionTableAccess64_t)GetProcAddress( m_hDbgHelp, strFuncName.c_str() );
        if( m_pfnSymFunctionTableAccess64 == NULL )
		{
			bLoad = false;
			break;
        }

        strFuncName = "SymGetModuleBase64";
        m_pfnSymGetModuleBase64 = (SymGetModuleBase64_t)GetProcAddress( m_hDbgHelp, strFuncName.c_str() );
        if( m_pfnSymGetModuleBase64 == NULL )
		{
			bLoad = false;
			break;
        }

        strFuncName = "SymCleanup";
        m_pfnSymCleanup = (SymCleanup_t)GetProcAddress( m_hDbgHelp, strFuncName.c_str() );
        if( m_pfnSymCleanup == NULL)
		{
			bLoad = false;
			break;
        }

        strFuncName = "SymFromAddr";
        m_pfnSymFromAddr = (SymFromAddr_t)GetProcAddress( m_hDbgHelp, strFuncName.c_str() );
        if( m_pfnSymFromAddr == NULL )
		{
			bLoad = false;
			break;
        }

        strFuncName = "SymGetLineFromAddr64";
        m_pfnSymGetLineFromAddr64 = (SymGetLineFromAddr64_t)GetProcAddress( m_hDbgHelp, strFuncName.c_str() );
        if( m_pfnSymGetLineFromAddr64 == NULL )
		{
			bLoad = false;
			break;
        }

        strFuncName = "SymInitialize";
        m_pfnSymInitialize = (SymInitialize_t)GetProcAddress( m_hDbgHelp, strFuncName.c_str() );
        if( m_pfnSymInitialize == NULL )
		{
			bLoad = false;
			break;
        }

        strFuncName = "SymSetOptions";
        m_pfnSymSetOptions = (SymSetOptions_t)GetProcAddress( m_hDbgHelp, strFuncName.c_str() );
        if( m_pfnSymSetOptions == NULL )
		{
			bLoad = false;
			break;
        }
		
        strFuncName = "SymGetOptions";
		m_pfnSymGetOptions = (SymGetOptions_t)GetProcAddress( m_hDbgHelp, strFuncName.c_str() );
		if( m_pfnSymGetOptions == NULL )
		{
			bLoad = false;
			break;
        }
		
        strFuncName = "SymGetSymFromAddr64";
		m_pfnSymGetSymFromAddr64 = (SymGetSymFromAddr64_t)GetProcAddress( m_hDbgHelp, strFuncName.c_str() );
		if( m_pfnSymGetOptions == NULL )
		{
			bLoad = false;
			break;
        }
	
		m_hProcess = GetCurrentProcess();

		string strPath = BuildSymbolSearchPath();
		if( !m_pfnSymInitialize(m_hProcess, strPath.c_str(), TRUE) )
		{
			bLoad = false;
			break;
		}

		DWORD dwOptions = m_pfnSymGetOptions();
		dwOptions |= (SYMOPT_LOAD_LINES | SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME );
		m_pfnSymSetOptions( dwOptions );

	} while( 0 );

	if( bLoad == false )
	{
		FreeLibrary( m_hDbgHelp );
		m_hDbgHelp = NULL;
	}

	return bLoad;
}

void CStackTrace::_Fini()
{
	m_pfnSymCleanup( m_hProcess );
	FreeLibrary( m_hDbgHelp );
	m_hDbgHelp = NULL;
}

const char* CStackTrace::BuildSymbolSearchPath()
{
	static string strPath;
	strPath = "";

    HMODULE  module = GetModuleHandle( NULL );
	char path[MAX_PATH] = {0};
    GetModuleFileName( module, path, MAX_PATH );
    PathRemoveFileSpec( path );
	
	strPath = path;
    strPath += ";.\\";

    // Append %SYSTEMROOT% and %SYSTEMROOT%\system32.
    char *env = getenv( "SYSTEMROOT" );
    if( env )
	{
		strPath += ";";
        strPath += env;
        strPath += ";";
        strPath += env;
        strPath += "\\system32";
    }

    // Append %_NT_SYMBOL_PATH% and %_NT_ALT_SYMBOL_PATH%.
    env = getenv( "_NT_SYMBOL_PATH" );
    if( env )
	{
        strPath += ";";
        strPath += env;
    }

    env = getenv( "_NT_ALT_SYMBOL_PATH" );
    if( env )
	{
        strPath += ";";
        strPath += env;
    }

    // Remove any quotes from the path. The symbol handler doesn't like them.
    size_t length = strPath.length();
    size_t pos = 0;
	string strTmpPath( strPath );
	strPath = "";
    while( pos < length )
	{
		const char ch = *(strTmpPath.c_str() + pos);
        if( ch != '\"')
		{
			strPath += ch;
        }
        ++pos;
    }

    return strPath.c_str();
}

void CStackTrace::_DumpStack(const char *pOutFile)
{
	CONTEXT Context;
	memset( &Context, 0, sizeof(CONTEXT) );
    Context.ContextFlags = CONTEXT_FULL;
    RtlCaptureContext( &Context );

	STACKFRAME64 StackFrame;
	memset( &StackFrame, 0, sizeof(STACKFRAME64) );
	DWORD imageType;
#ifdef _M_IX86
	imageType = IMAGE_FILE_MACHINE_I386;
	StackFrame.AddrPC.Offset = Context.Eip;
	StackFrame.AddrPC.Mode = AddrModeFlat;
	StackFrame.AddrFrame.Offset = Context.Ebp;
	StackFrame.AddrFrame.Mode = AddrModeFlat;
	StackFrame.AddrStack.Offset = Context.Esp;
	StackFrame.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
	imageType = IMAGE_FILE_MACHINE_AMD64;
	StackFrame.AddrPC.Offset = Context.Rip;
	StackFrame.AddrPC.Mode = AddrModeFlat;
	StackFrame.AddrFrame.Offset = Context.Rsp;
	StackFrame.AddrFrame.Mode = AddrModeFlat;
	StackFrame.AddrStack.Offset = Context.Rsp;
	StackFrame.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
	imageType = IMAGE_FILE_MACHINE_IA64;
	StackFrame.AddrPC.Offset = Context.StIIP;
	StackFrame.AddrPC.Mode = AddrModeFlat;
	StackFrame.AddrFrame.Offset = Context.IntSp;
	StackFrame.AddrFrame.Mode = AddrModeFlat;
	StackFrame.AddrBStore.Offset = Context.RsBSP;
	StackFrame.AddrBStore.Mode = AddrModeFlat;
	StackFrame.AddrStack.Offset = Context.IntSp;
	StackFrame.AddrStack.Mode = AddrModeFlat;
#else
#error "Platform not supported!"
#endif

	const s32 MAX_NAMELEN = 1024;
	char strSymData[ sizeof(IMAGEHLP_SYMBOL64) + MAX_NAMELEN ];
	memset( strSymData, 0, sizeof(strSymData) );
	IMAGEHLP_SYMBOL64 *pSym = (IMAGEHLP_SYMBOL64*)strSymData;
	pSym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
	pSym->MaxNameLength = MAX_NAMELEN;

	IMAGEHLP_LINE64 Line;
	memset( &Line, 0, sizeof(Line) );
	Line.SizeOfStruct = sizeof(Line);

	CGuardLock<CLock> g( CLog::m_exceptionLock );
	CLog::ExceptionLog( "Execption Info:" );
        
	HANDLE hThread = GetCurrentThread();
	for( s32 nFrameNum=0; ; ++nFrameNum )
	{
		// get next stack frame (StackWalk64(), SymFunctionTableAccess64(), SymGetModuleBase64())
		// if this returns ERROR_INVALID_ADDRESS (487) or ERROR_NOACCESS (998), you can
		// assume that either you are done, or that the stack is so hosed that the next
		// deeper frame could not be found.
		// CONTEXT need not to be suplied if imageType is IMAGE_FILE_MACHINE_I386!
		if ( !m_pfnStackWalk64(
				imageType,
				m_hProcess,
				hThread,
				&StackFrame,
				&Context,
				NULL,
				m_pfnSymFunctionTableAccess64,
				m_pfnSymGetModuleBase64,
				NULL) )
		{
			break;
		}

		if( StackFrame.AddrPC.Offset == StackFrame.AddrReturn.Offset )
		{
			//SGDEBUG( "StackWalk64-Endless-Callstack!\n" );
			break;
		}

		if( StackFrame.AddrPC.Offset != 0 )
		{
			DWORD dwDisplacement;
			BOOL bFound = m_pfnSymGetLineFromAddr64(
					m_hProcess,
					StackFrame.AddrPC.Offset,
					&dwDisplacement,
					&Line );
			if( bFound == TRUE)
			{
				//SGDEBUG( "SrcFile:%s Line:%d\n", Line.FileName, Line.LineNumber );
			}

			string strFuncName( "Function name unavailable" );
			DWORD64 dwDisplacement64;
			if( m_pfnSymGetSymFromAddr64(m_hProcess, StackFrame.AddrPC.Offset, &dwDisplacement64, pSym) != FALSE)
			{
				strFuncName = pSym->Name;
			}

			if( bFound )
			{
				CLog::ExceptionLog( "%s (%d): %s", Line.FileName, Line.LineNumber, strFuncName.c_str() );
			}
			else 
			{
				//SGDEBUG( "ADDRESSFORMAT (File and line number not available): " );
				//SGDEBUG( "%s\n", strFuncName.c_str() );
			}
		}
	}

    SGDEBUG( "Exception Happens! More detial info in '%s'\n", _EXCEPTION_LOG_DIR );
}

#else

bool CStackTrace::_Init()
{
	return true;
}

void CStackTrace::_Fini()
{
}

void CStackTrace::_DumpStack(const char *pOutFile)
{
    const s32 nSize = 128;
    s32 nRealAddrSize = 0;
    void *buffer[nSize]; 

    nRealAddrSize = backtrace( buffer, nSize );

    char **strAddr = backtrace_symbols( buffer, nRealAddrSize );
    if( strAddr )
    {
		CGuardLock<CLock> g( CLog::m_exceptionLock );
        CLog::ExceptionLog( "Execption Info:" );
        for( s32 i=0; i<nRealAddrSize; ++i )
        {
            CLog::ExceptionLog( "%s", strAddr[i] );
        }

        free( strAddr );

        SGDEBUG( "Exception Happens! More detial info in '%s'\n", _EXCEPTION_LOG_DIR );
    }
}

#endif
