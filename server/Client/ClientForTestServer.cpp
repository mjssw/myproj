#include <stdio.h>
#include "ClientLib.h"
#include "Timer.h"
#include "client.pb.h"
#include "msgid.pb.h"
using namespace SGLib;

#include <vector>
using namespace std;

#define TEST_MSG "abcdefghijklmnopqrstuvwxyz01234567890|abcdefghijklmnopqrstuvwxyz01234567890"

static LONG64 volatile g_TotalPkgCount = 0;
static CLock g_ioLock;
static CTimer<500> g_timer;
static int g_FreqSendPkg = 1000;

static void TimerCallBack(void *pData, s32 nDataLen);

class ctClient : public CClientBase
{
public:
	enum
	{
		MSG_ID_LEN = sizeof(s32),
		MSG_HEAD_LEN = MSG_ID_LEN,
		MAX_MSG_LEN = 4096,
	};

    ctClient(int id) : CClientBase(id)
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

			int _id = *((int*)(pbuf+sizeof(int)));
			switch(_id)
			{
			case sglib::msgid::SC_TEST_MSG_RSP:
				{
					sglib::clientproto::SCTestMsgRsp rsp;
					if( rsp.ParseFromArray(pbuf+MSG_ID_LEN+sizeof(int), pkgLen-MSG_ID_LEN-sizeof(int)) )
					{
						_TestMsgRsp( rsp );
					}
				}
				break;
			default:
				break;
			}
			
			procLen = (pkgLen);
			pbuf += procLen;
			len -= procLen;
			totalProc += procLen;
		}

		return totalProc;
	}

	virtual void OnConnect()
	{
		s64 thisAddr = (s64)this;
		s32 ret = g_timer.AddTimer(g_FreqSendPkg, TimerCallBack, &thisAddr, sizeof(s64), true);
		//printf("%d connect success, add timerid=%d\n", GetSocket(), ret);
	}

	virtual void OnClose()
	{
		printf("%d close\n", GetSocket());
	}

	void TestMsg()
	{
		sglib::clientproto::CSTestMsgReq req;
		req.set_msg( TEST_MSG );
		SendMsg( req, sglib::msgid::CS_TEST_MSG_REQ );

		//printf( "%d send pkg\n", GetSocket() );
	}


private:
	void _TestMsgRsp(const sglib::clientproto::SCTestMsgRsp &rsp)
	{
		LONG64 ret = InterlockedIncrement64( &g_TotalPkgCount );
		printf("recv %s\n", rsp.msg().c_str() );
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
	void SendMsg(const ::google::protobuf::Message &Msg, s32 nMsgId)
	{
		int total = Msg.ByteSize();
		char buf[MAX_MSG_LEN] = {0};
		*((s32*)(buf)) = total + MSG_ID_LEN + MSG_HEAD_LEN;
		*((s32*)(buf + MSG_HEAD_LEN)) = nMsgId;
		if( Msg.SerializeToArray(buf+MSG_ID_LEN+MSG_HEAD_LEN, MAX_MSG_LEN) )
		{
			if( !Send(buf, total+MSG_ID_LEN+MSG_HEAD_LEN) )
			{
				Fini();
			}
		}
		else
		{
			printf( "Msg:%d SerializeToArray failed\n", nMsgId );
		}
	}
};

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

static void TimerCallBack(void *pData, s32 nDataLen)
{
	s64 addrThis = *((s64*)pData);
	ctClient *pThis = (ctClient*)(addrThis);
	pThis->TestMsg();
}

void TestForTestServer(const char *ip, int port, int clientNum, int freq)
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
	CClientManager<ctClient> mgr(clientNum);
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
	}

	mgr.Stop();
	g_timer.Stop();
}
