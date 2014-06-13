#include "Log.h"
#include "Config.h"
using namespace SGLib;

CLock CLog::m_Lock;
CLock CLog::m_exceptionLock;
CLock CLog::m_debugLock;
CLock CLog::m_infoLock;
CLock CLog::m_errorLock;
CLock CLog::m_printLock;

#define MAX_DIR_LEN 1024

#ifdef _USE_WIN32
#define BUILD_LOG_INFO  do\
{\
    GetCurrentDirectoryA( sizeof(strDir), strDir );\
    sprintf( strDir, "%s\\Log\\%s", strDir, pstrDir );\
    if( CLog::IsDirExist(strDir)==false && CLog::CreateDir(strDir)==false )\
    {\
		SGDEBUG( "BUILD_LOG_INFO return\n" );\
        return;\
    }\
    time_t tNow;\
    time( &tNow );\
    strftime( strTime, sizeof(strTime), "%Y-%m-%d",localtime(&tNow) );\
    sprintf( strDir, "%s\\%s.log", strDir, strTime );\
    va_list args;\
    va_start( args, szFormat );\
    _vsnprintf( strLog, sizeof(strLog), szFormat, args );\
    strLog[LOG_BUFFER_MAX_LEN-1] = 0;\
    va_end( args );\
    strftime( strTime, sizeof(strTime), "%Y-%m-%d %X", localtime(&tNow) );\
} while(0);
#else
#define BUILD_LOG_INFO  do\
{\
    sprintf( strDir, "./Log/%s", pstrDir );\
    if( CLog::IsDirExist(strDir)==false && CLog::CreateDir(strDir)==false )\
    {\
		SGDEBUG( "BUILD_LOG_INFO return\n" );\
        return;\
    }\
    time_t tNow;\
    time( &tNow );\
    strftime( strTime, sizeof(strTime), "%Y-%m-%d",localtime(&tNow) );\
    sprintf( strDir, "%s\\%s.log", strDir, strTime );\
    va_list args;\
    va_start( args, szFormat );\
    vsnprintf( strLog, sizeof(strLog), szFormat, args );\
    strLog[LOG_BUFFER_MAX_LEN-1] = 0;\
    va_end( args );\
    strftime( strTime, sizeof(strTime), "%Y-%m-%d %X", localtime(&tNow) );\
} while(0);
#endif

bool CLog::IsDirExist(const char *pstrDir)
{
#ifdef _USE_WIN32
    DWORD dwRet = GetFileAttributesA( pstrDir );
    if( !((dwRet!=0xFFFFFFFF) && (dwRet & FILE_ATTRIBUTE_DIRECTORY)) )
    {
        return false;
    }
#else
    struct stat sFileStat;
    if( stat( pstrDir, &sFileStat ) != 0 )
    {
        return false;
    }
    if( !S_ISDIR(sFileStat.st_mode) )
    {
        return false;
    }
#endif
    return true;
}

bool CLog::CreateDir(const char *pstrDir)
{
#ifdef _USE_WIN32
    s32 nRet = SHCreateDirectoryExA( NULL, pstrDir, NULL );
    if( nRet==ERROR_SUCCESS || nRet==ERROR_FILE_EXISTS || nRet==ERROR_ALREADY_EXISTS )
    {
        return true;
    }
    return false; 
#else
    s32 nRet = 0;
    char strDir[MAX_DIR_LEN] = "";
    strncpy( strDir, pstrDir, MAX_DIR_LEN );
    s32 nLen = strlen( strDir );
    for( s32 i=0; i<nLen; ++i )
    {
        if( strDir[i] == '/' )
        {
            strDir[i] = 0;
            
            nRet = access( strDir, 0 );
            if( nRet != 0 )
            {
                nRet = mkdir( strDir, 0755 );
                if( nRet != 0 )
                {
                    return false;
                }
            }
            
            strDir[i] = '/';
        }
    }
    
    nRet = mkdir( strDir, 0755 );
    if( nRet != 0 )
    {
        return false;
    }

    return true;
#endif
} 

void CLog::Log(const char *pstrDir, const char *szFormat, ... )
{
    char strDir[MAX_DIR_LEN] = "";
    char strTime[64] = {0};
    char strLog[LOG_BUFFER_MAX_LEN] = {0};
    BUILD_LOG_INFO;

    _WriteLog( strDir, strLog, strTime );
}

void CLog::SafeLog(const char *pstrDir, const char *szFormat, ... )
{
    char strDir[MAX_DIR_LEN] = "";
    char strTime[64] = {0};
    char strLog[LOG_BUFFER_MAX_LEN] = {0};
    BUILD_LOG_INFO;

    CGuardLock<CLock> g( m_Lock );
    _WriteLog( strDir, strLog, strTime );
}

void CLog::ExceptionLog(const char *szFormat, ... )
{
	const char *pstrDir = _EXCEPTION_LOG_DIR; 
	
	char strDir[MAX_DIR_LEN] = "";
    char strTime[64] = {0};
    char strLog[LOG_BUFFER_MAX_LEN] = {0};
    BUILD_LOG_INFO;

	{
		//CGuardLock<CLock> g( m_exceptionLock );
		_WriteLog( strDir, strLog, strTime );
	}
	{
		//CGuardLock<CLock> g( m_printLock );
        printf( "[Exception] %s %s\n", strTime, strLog );
	}
}

void CLog::DebugLog(const char *szFormat, ... )
{
#ifdef _DEBUG
	const char *pstrDir = _DEBUG_LOG_DIR; 
	
	char strDir[MAX_DIR_LEN] = "";
    char strTime[64] = {0};
    char strLog[LOG_BUFFER_MAX_LEN] = {0};
    BUILD_LOG_INFO;

	{
		CGuardLock<CLock> g( m_debugLock );
		_WriteLog( strDir, strLog, strTime );
	}

	{
		CGuardLock<CLock> g( m_printLock );
        printf( "[Debug] %s %s\n", strTime, strLog );
	}
#endif
}

void CLog::InfoLog(const char *szFormat, ... )
{
	const char *pstrDir = _INFO_LOG_DIR; 
	
	char strDir[MAX_DIR_LEN] = "";
    char strTime[64] = {0};
    char strLog[LOG_BUFFER_MAX_LEN] = {0};
    BUILD_LOG_INFO;

	{
		CGuardLock<CLock> g( m_infoLock );
		_WriteLog( strDir, strLog, strTime );
	}
	
	{
		CGuardLock<CLock> g( m_printLock );
        printf( "[Info] %s %s\n", strTime, strLog );
	}
}

void CLog::ErrorLog(const char *szFormat, ... )
{
	const char *pstrDir = _ERROR_LOG_DIR; 
	
	char strDir[MAX_DIR_LEN] = "";
    char strTime[64] = {0};
    char strLog[LOG_BUFFER_MAX_LEN] = {0};
    BUILD_LOG_INFO;

	{
		CGuardLock<CLock> g( m_errorLock );
		_WriteLog( strDir, strLog, strTime );
	}
	
	{
		CGuardLock<CLock> g( m_printLock );
        printf( "[Error] %s %s\n", strTime, strLog );
	}
}

void CLog::_WriteLog(const char *pstrLogFile, const char *pstrLog, const char *pstrTime)
{
    FILE *fp = fopen( pstrLogFile, "a+" );
    if( fp )
    {
        fprintf( fp, "%s %s\n", pstrTime, pstrLog );
        fclose( fp );
    }
	else
	{
		SGDEBUG( "_WriteLog open file failed.\n" );
	}
}
