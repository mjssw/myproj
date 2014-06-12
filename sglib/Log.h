#ifndef _SG_LOG_H_
#define _SG_LOG_H_

#include "Define.h"
#include "Lock.h"

namespace SGLib
{
    class CLog
    {
    public:
        static bool IsDirExist(const char *pstrDir);
        static bool CreateDir(const char *pstrDir);
        static void Log(const char *pstrDir, const char *szFormat, ... );

	private:
		template<class TIoSocket>
		friend class CIoService;
		friend class CDispatchEvent;
		friend class CIoSocketBase;
		friend class CStackTrace;
        static void SafeLog(const char *pstrDir, const char *szFormat, ... );

    private:
        static CLock m_Lock;
        static void _WriteLog(const char *pstrLogFile, const char *pstrLog, const char *pstrTime);

	private:
		static CLock m_printLock;
		static CLock m_exceptionLock;
		static CLock m_errorLock;
		static CLock m_infoLock;
		static CLock m_debugLock;

	public:
		static void ExceptionLog(const char *szFormat, ... );
		static void ErrorLog(const char *szFormat, ... );
		static void InfoLog(const char *szFormat, ... );
		static void DebugLog(const char *szFormat, ... );
    };
};

#endif

