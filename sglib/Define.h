#ifndef _SG_DEFINE_H_
#define _SG_DEFINE_H_

//#define _USE_WIN32
//
// !!!
// Any Define in this file must not be modify.
// Or will make unknown error .
//


typedef unsigned char        byte;
typedef char                 s8;
typedef short                s16;
typedef int                  s32;
typedef long long            s64;
typedef unsigned char        u8;
typedef unsigned short       u16;
typedef unsigned int         u32;
typedef unsigned long long   u64;

#define SAFE_DELETE(p)       do{\
    if( p )\
    {\
        delete p;\
        p = 0;\
    }\
}while(0);

#define SAFE_DELETE_ARRAY(p) do{\
    if( p )\
    {\
        delete [] p;\
        p = 0;\
    }\
}while(0);

#define SINGLETON_CLASS_DEFINE(class_name)	\
public:\
	static class_name& Instance()\
	{\
		if( m_pInstance == NULL )\
		{\
			m_pInstance = new class_name();\
		}\
		return *m_pInstance;\
	}\
	~class_name(){ delete m_pInstance; };\
private:\
	class_name(){};\
	static class_name *m_pInstance;\

#define SIGNLETON_CLASS_INIT(class_name)	\
class_name* class_name::m_pInstance = NULL;


#include "ErrDef.h"
//#include "Config.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _DEBUG
    #define SGDEBUG printf
#else
    #define SGDEBUG(...)
#endif

#ifdef _USE_WIN32
	//#define _WIN32_WINNT    0x0400
	#define _WIN32_WINNT    0x0501

	#include <winsock2.h>
	#include <windows.h>
	#pragma comment(lib, "ws2_32.lib")
	#include <process.h>
    #include <shlobj.h>
    #include <time.h>
	#include <dbghelp.h>
	#include <shlwapi.h>
	#pragma comment(lib, "shlwapi.lib")
	#pragma comment(lib, "Winmm.lib")

	#define MAX_ACCEPT_COUNT				128
	#define MAX_WORK_THREAD_NUM				32	
	#define WORKER_THREADS_PER_PROCESSOR	2 

	enum EEventIocp
	{
		IOCP_RECV = 0,
		IOCP_SEND,
		IOCP_ACCEPT,
	};

	typedef struct sCompletionKey
	{
		void	*m_pData;
	}CompletionKey_t;

	typedef struct sOverlapped
	{
		OVERLAPPED m_sOverlapped;
		WSABUF     m_sWsabuf;
		EEventIocp m_eEvtIocp;
		SOCKET     m_hSock;
		byte	   m_Buf[128];//此buf只在投递和接受连接请求有用,后续可使用此缓冲区
	}Overlapped_t;
#else
	#include <pthread.h>
	#include <sys/time.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/epoll.h>
	#include <sys/stat.h>
    #include <sys/eventfd.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <stdarg.h>
	#include <string.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
    #include <netinet/tcp.h>
	#define MAX_ACCEPT_COUNT		128 
	#define MAX_WORK_THREAD_NUM		1
    #ifndef INVALID_SOCKET 
        #define INVALID_SOCKET          (-1)
    #endif
    #ifndef SOCKET_ERROR 
        #define SOCKET_ERROR            (-1)
    #endif
#endif

#define __ASSERT    assert

#define SG_ASSERT(cond) do{\
    if( !(cond) )\
    {\
	    SGLib::CStackTrace::Instance().DumpStack();\
        __ASSERT( (cond) );\
    }\
}while(0);

#define SELF_ASSERT(cond, expr) do{\
    if( !(cond) )\
    {\
	    SGLib::CStackTrace::Instance().DumpStack();\
        __ASSERT( (cond) );\
		expr;\
    }\
}while(0);

#define _EXCEPTION_LOG_DIR    "Exception"
#define _DEBUG_LOG_DIR		  "Debug"
#define _ERROR_LOG_DIR		  "Error"
#define _INFO_LOG_DIR		  "Info"

#define INVALID_VAL       (-1)

#endif
