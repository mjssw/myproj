#include <stdio.h>
#include "ClientLib.h"
#include "Timer.h"
#include "Log.h"
using namespace SGLib;

#include <vector>
#include <string>
#include <map>
using namespace std;

#define TEST_MSG2 "abcdefghijklmnopqrstuvwxyz01234567890|abcdefghijklmnopqrstuvwxyz01234567890"

class ctClient1;

#ifdef _USE_WIN32
static LONG64 volatile g_TotalPkgCount = 0;
#else
static s64 g_TotalPkgCount = 0;
static CLock g_pkgCountLock;
#endif
static CLock g_ioLock;
static CTimer<1000> g_timer;
static int g_FreqSendPkg = 1000;

static CLock g_clientLock;
static map<s32, ctClient1*> g_clients; 
static void AddClient(ctClient1 *client);
static void DelClient(ctClient1 *client);
static void TimerCallBack(void *pData, s32 nDataLen);
static CLock g_msglock;
static bool g_showmsg = false;
static void OnMsg()
{
	printf( "OnMsg\n" );
	CGuardLock<CLock> g(g_msglock);
	g_showmsg = true;
}
static void OffMsg()
{
	printf( "OffMsg\n" );
	CGuardLock<CLock> g(g_msglock);
	g_showmsg = false;
}
static bool IsOnMsg()
{
	CGuardLock<CLock> g(g_msglock);
	return g_showmsg;
}

class ctClient1 : public CClientBase
{
public:
	enum
	{
		MSG_ID_LEN = sizeof(s32),
		MSG_HEAD_LEN = MSG_ID_LEN,
		MAX_MSG_LEN = 4096,
	};

	ctClient1(int id) : CClientBase(id), m_sendTimer(-1)
    {
    }

	virtual int OnRecv(char *buf, int len)
	{
		int totalProc = 0;
		char *pbuf = buf;
		while(true)
		{
			int procLen = 0;
			if( len < sizeof(int) )
			{
				return totalProc;
			}

			int pkgLen = *((int*)pbuf);
			if( len < pkgLen )
			{
				return totalProc;
			}

			_TestMsgRsp( pbuf+sizeof(int), pkgLen-sizeof(int) );
			
			procLen = (pkgLen);
			pbuf += procLen;
			len -= procLen;
			totalProc += procLen;
		}

		return totalProc;
	}

	s32 m_sendTimer;
	virtual void OnConnect()
	{
		s64 thisAddr = (s64)this;
		m_sendTimer = g_timer.AddTimer(g_FreqSendPkg, TimerCallBack, &thisAddr, sizeof(s64), true);
		//printf("%d connect success, add timerid=%d\n", GetSocket(), ret);

		AddClient( this );
	}

	virtual void OnClose()
	{
		//printf("%d close\n", GetSocket());
		CLog::Log("Debug1", "%d close, timerid=%d", GetSocket(), m_sendTimer);
		if( m_sendTimer != -1 )
		{
			CLog::Log("Debug1", "%d close, del timerid=%d", GetSocket(), m_sendTimer);
			g_timer.DelTimer(m_sendTimer);
			m_sendTimer = -1;
		}

		DelClient( this );
	}

	void TestMsg()
	{
		SendMsg( TEST_MSG2 );
	}


private:
	void _TestMsgRsp(char *msg, int len)
	{
#ifdef _USE_WIN32
		LONG64 ret = InterlockedIncrement64( &g_TotalPkgCount );
#else
        g_pkgCountLock.Lock();
        ++g_TotalPkgCount;
        s64 ret = g_TotalPkgCount;
        g_pkgCountLock.UnLock();
#endif

		if( IsOnMsg() )
		{
			char str[MAX_MSG_LEN] = {0};
			strncpy( str, msg, len );
			printf("recv %s\n", str );
		}
		if( ret % 20000 == 0 )
		{
			char strTime[256] = {0};
			time_t tNow;
			time( &tNow );
			strftime( strTime, sizeof(strTime), "%Y-%m-%d %X",localtime(&tNow) );
			g_ioLock.Lock();
			printf( "%s\t Total reach: %lld\n", strTime, ret );
			g_ioLock.UnLock();
		}
	}

private:
public:
	void SendMsg(const char *pmsg)
	{
		const char *msg = pmsg;
		int total = strlen(msg);
		char buf[MAX_MSG_LEN] = {0};
		*((s32*)(buf)) = total + sizeof(int);
		memcpy( buf+sizeof(int), msg, total );

		if( !Send(buf, total+sizeof(int)) )
		{
			CLog::Log("Debug1", "Send msg failed , then Fini, socket=%d", GetSocket());
			Fini();
		}
	}
};

static void TimerCallBack(void *pData, s32 nDataLen)
{
	s64 addrThis = *((s64*)pData);
	ctClient1 *pThis = (ctClient1*)(addrThis);
	pThis->TestMsg();
}

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

static void AddClient(ctClient1 *client)
{
	if( !client ) return;
	CGuardLock<CLock> g(g_clientLock);
	g_clients[ client->GetId() ] = client;
}
static void DelClient(ctClient1 *client)
{
	if( !client ) return;
	CGuardLock<CLock> g(g_clientLock);
	map<s32, ctClient1*>::iterator it = g_clients.find( client->GetId() );
	if( it != g_clients.end() )
	{
		g_clients.erase( it );
	}
}
void TestForIoService(const char *ip, int port, int clientNum, int freq)
{
	if( !g_timer.Start() )
	{
		printf( "start timer failed\n" );
		return;
	}
	
	char strTime[256] = {0};
	time_t tNow;
	time( &tNow );
	strftime( strTime, sizeof(strTime), "%Y-%m-%d %X",localtime(&tNow) );
	printf( "%d clients start at %s\tFREQ:%d\n", clientNum, strTime, freq );

	g_FreqSendPkg = freq;
	CClientManager<ctClient1> mgr(clientNum);
	mgr.Start(ip, port);

	while(true)
	{
		char cmd[64] = {0};
		gets(cmd);
		vector<string> vCmd;
		SplitCmd(cmd, vCmd );
		if( vCmd.size() == 0 )
		{
			printf("no cmd here\n");
			continue;
		}

		if( vCmd[0] == "quit" )
		{
			break;
		}
		else if( vCmd[0] == "all" )
		{
			char strTime[256] = {0};
			time_t tNow;
			time( &tNow );
			strftime( strTime, sizeof(strTime), "%Y-%m-%d %X",localtime(&tNow) );
			g_ioLock.Lock();
			printf( "%s\t FREQ:%d Total reach: %lld\n", strTime, g_FreqSendPkg, g_TotalPkgCount );
			g_ioLock.UnLock();
		}
		else if( vCmd[0] == "senddata" )
		{
			char *data = "hehehe";
			CGuardLock<CLock> g(g_clientLock);			
			map<s32, ctClient1*>::iterator it = g_clients.begin(); 
			for( ; it != g_clients.end(); ++it )
			{
				it->second->SendMsg( data );
			}
		}
		else if( vCmd[0] == "onmsg" )
		{
			OnMsg();
		}
		else if( vCmd[0] == "offmsg" )
		{
			OffMsg();
		}
	}

	mgr.Stop();
	g_timer.Stop();
}
