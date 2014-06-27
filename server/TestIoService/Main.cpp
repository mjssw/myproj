#include <stdio.h>
#include "IoAdvSocketEx.h"
#include "BasicServerEx.h"
#include "AdvanceServerEx.h"
#include "IoService.h"
#include "CycleBuffer.h"
#include "BufferPool.h"
using namespace SGLib;
#include <vector>
#include <string>
using namespace std;

extern void _DisplayCountInfo();



static void SplitCmd(const char *cmd, vector<string> &vecCmd)
{
	const char *ptr = cmd;
	while(ptr)
	{
		const char *pos = strchr(ptr, ' ');
		if( pos == NULL )
		{
			vecCmd.push_back(ptr);
			break;
		}

		vecCmd.push_back( string(ptr, pos) );
		ptr = pos + 1;
	}
}

static bool g_showMsg = false;
static CLock g_sessionDataLock;
static bool g_sessionCloseFlag = false;
static s32 g_sessionId = 0;
class CSession : public CIoSocketEx 
{
public:
	bool m_needSend;
	CSession(s32 id) : CIoSocketEx(id), m_needSend(true)
	{}

	virtual void OnAccept()
	{
		printf( "[%llu]accept client: id=%d sock=%d\n", CBasicThreadOps::_GetThreadId(),
			GetId(), GetSocket() );
	}
	virtual void OnConnect()
	{
		printf( "[%llu]connected success\n", CBasicThreadOps::_GetThreadId() );
	}
	virtual void OnConnectError()
	{
		printf( "[%llu]connected failed\n", CBasicThreadOps::_GetThreadId() );
	}

	virtual void OnClose()
	{
		printf( "[%llu]close client: id=%d sock=%d\n", CBasicThreadOps::_GetThreadId(),
			GetId(), GetSocket() );
	}
	virtual void OnRecv(const byte *pkg, s32 len)
	{
		if( g_showMsg )
		{
			char msg[128] = {0};
			memcpy( msg, pkg, len );
			printf( "[%llu]recv:%s\n", CBasicThreadOps::_GetThreadId(),
				msg );
		}
		//*/

		bool isneedClose = g_sessionCloseFlag;
		s32 exId = g_sessionId;
		{
			CGuardLock<CLock> g( g_sessionDataLock );
			isneedClose = g_sessionCloseFlag;
			exId = g_sessionId;
		}
		if( isneedClose && GetId() != exId )
		{
			Close();
			return;
		}

		if( m_needSend && !Send( pkg, len ) )
		{
			//Close();
			printf( "[%llu]Send error, not send data any more\n", CBasicThreadOps::_GetThreadId() );
			m_needSend = false;
		}
	}
	virtual CIoSocketEx* Clone(s32 id)
	{
		return new CSession(id);
	}
};

class CAdvSession : public CIoAdvSocketEx
{
public:
	CAdvSession(s32 id) : CIoAdvSocketEx(id){}
	virtual CIoSocketEx* Clone(s32 id)
	{
		return new CAdvSession(id);
	}
	virtual void OnAccept()
	{
		printf( "[%llu]accept client: id=%d sock=%d\n", CBasicThreadOps::_GetThreadId(),
			GetId(), GetSocket() );
	}
	virtual void OnClose()
	{
		printf( "[%llu]close client: id=%d sock=%d\n", CBasicThreadOps::_GetThreadId(),
			GetId(), GetSocket() );
	}
	virtual void OnRecv(const byte *pkg, s32 len)
	{
		if( g_showMsg )
		{
			char msg[128] = {0};
			memcpy( msg, pkg, len );
			printf( "[%llu]recv:%s\n", CBasicThreadOps::_GetThreadId(), msg );// */
		}

		if( !Send( pkg, len ) )
		{
			Close();
		}
	}
};

class COldSession : public CIoSocketBase 
{
public:
	COldSession(s32 id) : CIoSocketBase(id)
	{}

	virtual void OnAccept()
	{
		printf( "[%llu]accept client: id=%d\n", CBasicThreadOps::_GetThreadId(),
			GetId() );
	}
	virtual void OnConnect()
	{
		printf( "[%llu]connected success\n", CBasicThreadOps::_GetThreadId() );
	}
	virtual void OnConnectError()
	{
		printf( "[%llu]connected failed\n", CBasicThreadOps::_GetThreadId() );
	}

	virtual void OnClose()
	{
		printf( "[%llu]close client: id=%d \n", CBasicThreadOps::_GetThreadId(),
			GetId() );
	}
	virtual void OnRecv(const byte *pkg, s32 len)
	{
		if( g_showMsg )
		{
			char msg[128] = {0};
			memcpy( msg, pkg, len );
			printf( "[%llu]recv:%s\n", CBasicThreadOps::_GetThreadId(),
				msg );
		}
		//*/

		if( !Send( pkg, len ) )
		{
			Close();
		}
	}
};

void TimerCallBack123(void *data, s32 len)
{
	s32 count = *((s32*)data);
#ifdef _USE_WIN32
	printf( "[%llu][%llu]timer:%d\n", CBasicThreadOps::_GetThreadId(), GetTickCount(), count );
#else
	printf( "[%llu][]timer:%d\n", CBasicThreadOps::_GetThreadId(), count );
#endif

	*((s32*)data) += 1;
}

void testAdvServer(s32 listenPort, s32 workerCount, s32 bufsize, s32 maxcount)
{
	CAdvanceServerEx<CAdvSession> *server = new CAdvanceServerEx<CAdvSession>(1,maxcount,bufsize,bufsize,workerCount);
	bool ret = server->Start( listenPort );

	s32 g_callcount = 0;
	server->AddTimer(2000, TimerCallBack123, &g_callcount, sizeof(s32), true );
	printf( "adv Server start:%d. press enter to quit\n", (ret?1:0) );
	while(1)
	{
		char cmd[32] = {0};
		gets( cmd );
		if( strcmp(cmd, "quit") == 0 )
		{
			break;
		}
		else if( strcmp(cmd, "onmsg") == 0 )
		{
			g_showMsg = true;
		}
		else if( strcmp(cmd, "offmsg") == 0 )
		{
			g_showMsg = false;
		}
	}

	server->Stop();

	printf( "count=%d\n", g_callcount );
}

void TimerCallBackFunc(void *pData, s32 nDataLen)
{
	//_DisplayCountInfo();
}

void testBasicServer(s32 listenport, s32 nouse, s32 bufsize, s32 maxcount)
{
	CBasicServerEx<CSession> server(1,maxcount,bufsize,bufsize);
	bool ret = server.Start();
	if( !ret ) return;
	ret = server.StartListen( listenport );

	CTimer<5> timer;
	ret = timer.Start();
	if( !ret ) return;
	timer.AddTimer( 10000, TimerCallBackFunc, NULL, 0, true );

	printf( "Server start:%d. press enter to quit\n", (ret?1:0) );
	while(1)
	{
		char cmd[32] = {0};
		gets( cmd );
		vector<string> vecCmd;
		SplitCmd(cmd, vecCmd);
		if( strcmp(cmd, "quit") == 0 )
		{
			break;
		}
		else if( strcmp(cmd, "onmsg") == 0 )
		{
			g_showMsg = true;
		}
		else if( strcmp(cmd, "offmsg") == 0 )
		{
			g_showMsg = false;
		}
		else if( strcmp(cmd, "counter") == 0 )
		{
			_DisplayCountInfo();
		}
		else if( vecCmd.size() == 2 && vecCmd[0]=="showclient" )
		{
			s32 id = atoi( vecCmd[1].c_str() );
			server.DebugShowClientInfo( id );
		}
		else if( vecCmd[0] == "showmgruse" )
		{
			server.DebugShowSocketExManagerUseInfo();
		}
		else if( vecCmd[0] == "showmgrfree" )
		{
			server.DebugShowSocketExManagerFreeInfo();
		}
		else if( vecCmd[0] == "closeallex" )
		{
			if( vecCmd.size() != 2 ) continue;
			s32 exid = atoi( vecCmd[1].c_str() );
   
			CGuardLock<CLock> g( g_sessionDataLock );
			g_sessionId = exid;
			g_sessionCloseFlag = true;
		}
	}

	timer.Stop();
	server.Stop();
}

void testOldServer(s32 listenport, s32 workerCount, s32 bufsize, s32 maxcount)
{
	CNetConfig netConfig;
	netConfig.m_connectingQueueSize = 128;
	netConfig.m_eventQueueSize = 4096;
	netConfig.m_eventWorkerCount = workerCount;
	netConfig.m_ioTimeOut = 1000;
	netConfig.m_maxClientCount = maxcount;
	netConfig.m_maxPackageLen = 4096;
	netConfig.m_recvBufferSize = bufsize;
	netConfig.m_sendBufferSize = bufsize;
	netConfig.m_waitEventTimeOut = 1000;
	netConfig.m_workThreadQuitTimeOut = 8000;

	CIoService<COldSession> ioService(netConfig);
	bool ret = false;
	if( ioService.Init()==E_Error_Success )
	{
		if( ioService.StartListen( listenport ) == E_Error_Success )
		{
			ioService.Dispatch();
			ret = true;
		}
	}
	
	printf( "Server start:%d. press enter to quit\n", (ret?1:0) );
	while(1)
	{
		char cmd[32] = {0};
		gets( cmd );
		if( strcmp(cmd, "quit") == 0 )
		{
			break;
		}
		else if( strcmp(cmd, "onmsg") == 0 )
		{
			g_showMsg = true;
		}
		else if( strcmp(cmd, "offmsg") == 0 )
		{
			g_showMsg = false;
		}
	}

	ioService.Fini();
}


void testConnectToBasicServer()
{	
	CBasicServerEx<CSession> server(1,1000,512,512);
	bool ret = server.Start();

	printf( "Server start:%d. press enter to quit\n", (ret?1:0) );
	server.Connect( "127.0.0.1", 10001 );
	printf( "try connect 127.0.0.1:10001\n" );
	getchar();

	server.Stop();
}


enum 
{
	len = 10,
};
void testCycleBuffer()
{
#ifdef _USE_WIN32
	CCycleBuffer buffer;
	bool ret = buffer.Initialize(1024);
	if( !ret  )return ;
	buffer.Init();
	char *buf = "1234567890abcdefghijklmnopqrstuvwxyz+_)(*&^%$#@!~}{\":?><[];',./";
	int len = strlen(buf);
	int count = 10000000;
	vector<int> vecLen;
	printf( "prepare to run...\n" );
	for( int i=0; i<count; ++i )
	{
		vecLen.push_back( (rand() % (len-1)) + 1 );
	}

	printf( "Start...\n" );
	DWORD _beginTm = GetTickCount();

	for( int i=0; i<count; ++i )
	{
		buffer.AddDataSize( vecLen[i], (const byte*)buf );
		buffer.AddFreeSize( vecLen[i] );
	}

	DWORD use = GetTickCount() - _beginTm;
	printf( "End! use time:%u\n", use );
#endif
}

static std::string TimeNow()
{
	char strTime[64] = {0};
	time_t now;
	time( &now );
    strftime( strTime, sizeof(strTime), "%Y-%m-%d %X", localtime(&now) );
	return std::string( strTime );
}
static void timerCallBackFunc(void *pData, s32 nDataLen)
{
	int *param = (int*)pData;
	printf( "[%s] param=%d\n", TimeNow().c_str(), *param );
}
void TestTimer(int tcount)
{
	CTimer<2000> *timer = new CTimer<2000>();
	if( !timer )
	{
		printf( "new timer failed.\n" );
		return;
	}
	bool ret = timer->Start();
	if( !ret )
	{
		printf( "start timer failed.\n" );
		return;
	}
	bool flag = true;
	srand( (unsigned int)time(NULL) );
	for( int i=0; i<tcount; ++i )
	{
		int param = i;
		s32 timerId = timer->AddTimer( 5000, timerCallBackFunc, &param, sizeof(int), true );
		if( timerId == CTimer<2000>::E_Invalid_TimerId )
		{
			flag = false;
			break;
		}

		SGLib::CBasicThreadOps::_Sleep( (rand() % 5) * 1000 );
	}
	if( flag )
	{
		while(1)
		{
			char cmd[32] = {0};
			gets( cmd );
			if( strcmp(cmd, "quit") == 0 )
			{
				break;
			}
		}
	}
	timer->Stop();
	SAFE_DELETE( timer );
}

int main(int argc, char *argv[])
{
	if( argc == 2 )
	{
		TestTimer( atoi(argv[1]) ); 
		return 0;
	}
	return 0;

	//testCycleBuffer(); getchar(); return 0;

	if( argc == 5 )
	{
		testAdvServer( atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]) );
		//testBasicServer( atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]) );
		//testOldServer( atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]) );
	}
	else
	{
		printf( "param error\n" );
	}
	//*/

	//testConnectToBasicServer();

	return 0;
}
