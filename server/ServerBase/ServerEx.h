#ifndef _SERVER_EX_H_
#define _SERVER_EX_H_

#include "IoSocketExManager.h"
#include "AccepterEx.h"
#include "ConnectorEx.h"
#include "IoServiceExPool.h"
#include "EventWorkerEx.h"
#include "EventWorkerExPool.h"
#include "Timer.h"
using namespace SGLib;

#include <string>
using namespace std;

#include "Server.h"
#include "Log4cxxCreater.h"
#include "ServerConfig.h"

template<class TClient, class TRpcClient, class TServerConfig>
class CServerEx : public CServerBase
{
public:
    struct IoConfig
    {
        s32 m_ioServiceCount;
        s32 m_connCount;
        s32 m_sendSize;
        s32 m_recvSize;
    };

    struct CallBackData
    {
        void *pData;
		u64 ip;
        s32 port;
    };

	enum
	{
		E_Reconnect_Timer = 1000,
        E_EventQueue_Size = 4096,
	};

	CServerEx(const char *serverCfgFile, s32 workerCount, const IoConfig &ctCfg, const IoConfig &rctCfg) : 
        m_id( 0 ),
		m_serverCfgFile( serverCfgFile ),
        m_cfgLock(),
        m_serverConfig( NULL ),
        m_workerCount( workerCount ),
        m_eventWorkerPool( workerCount, E_EventQueue_Size ),

        m_ctIoServiceCount( ctCfg.m_ioServiceCount ),
        m_ctConnCount( ctCfg.m_connCount ),
        m_ctSendSize( ctCfg.m_sendSize ),
        m_ctRecvSize( ctCfg.m_recvSize ),
        m_ctIoSocketMgr(),
        m_ctIoSerivcePool( m_ctIoServiceCount, m_ctIoSocketMgr, &m_eventWorkerPool ),
	    m_accepter( m_ctIoSerivcePool ),

        m_rctIoServiceCount( rctCfg.m_ioServiceCount ),
        m_rctConnCount( rctCfg.m_connCount ),
        m_rctSendSize( rctCfg.m_sendSize ),
        m_rctRecvSize( rctCfg.m_recvSize ),
        m_rctIoSocketMgr(),
        m_rctIoSerivcePool( m_rctIoServiceCount, m_rctIoSocketMgr, &m_eventWorkerPool ),
	    m_connector( m_rctIoSerivcePool ),

		m_timer( m_eventWorkerPool )
	{
	}

	friend class CServerManager;

	static u64 GetProcessId()
	{
#ifdef _USE_WIN32
		return (u64)GetCurrentProcessId();
#else
		return (u64)getpid();
#endif
	}

public:
	~CServerEx()
	{
		Stop();
	}

	virtual bool Start()
	{
		m_serverConfig = new TServerConfig( m_serverCfgFile.c_str() );
		if( !m_serverConfig->LoadConfigFile() )
		{
			return false;
		}
		m_id = m_serverConfig->GetServerConfig().m_Id; 
        s32 port = m_serverConfig->GetServerConfig().m_Port;

        // Start event worker
        if( !m_eventWorkerPool.Start() )
        {
            return false;
        }

        // Start client server
        bool ret = false;
        TClient ctSession(0);
        if( m_ctIoSocketMgr.Init(m_ctConnCount, m_ctSendSize, m_ctRecvSize, ctSession) &&
            m_ctIoSerivcePool.Start() && 
            m_accepter.Start( port ) )
        {
            ret = true;
        }
        if( !ret )
        {
            return false;
        }

        // Start rpc client server
        ret = false;
        TRpcClient rctSession(0);
        if( m_rctIoSocketMgr.Init(m_rctConnCount, m_rctSendSize, m_rctRecvSize, rctSession) &&
            m_rctIoSerivcePool.Start() && 
            m_connector.Start() )
        {
            ret = true;
        }
        if( !ret )
        {
            return false;
        }

        // Start timer
		if( !m_timer.Start() )
        {
            return false;
        }

		m_log = CLog4cxxCreater::GetLogger( m_serverConfig->GetLogConfig(), "ServerLog" );

        // Try to connect servers
        s32 count = m_serverConfig->GetConnectCount();
        for( s32 i=0; i<count; ++i )
        {
            const ConnectConfig *cfg = m_serverConfig->GetConnectConfigs( i );
            if( cfg == NULL )
            {
                continue;
            }

            m_connector.Connect( cfg->m_strIp.c_str(), cfg->m_nPort );
        }

        LOG4CXX_INFO( m_log, "Server:(" << m_serverConfig->GetServerConfig().m_Id << \
                ":" << m_serverConfig->GetServerConfig().m_DisplayName << \
                ":" << m_serverConfig->GetServerConfig().m_Ip << \
                ":" << m_serverConfig->GetServerConfig().m_Port << ") Start Success!" << \
                "[PID=" << GetProcessId() << "]" );

		return true;
	}

	virtual void Stop()
	{
		m_timer.Stop();
        m_connector.Stop();
        m_accepter.Stop();
        m_ctIoSerivcePool.Stop();
        m_rctIoSerivcePool.Stop();
        m_eventWorkerPool.Stop();
	}

	virtual void Connect(const char *ip, int port)
	{
        m_connector.Connect( ip, port );
	}

	virtual void Reconnect(const char *ip, int port)
	{
		LOG4CXX_INFO( m_log, "CServer::ReConnect," << ip << "," << port );

		CallBackData data;
		data.pData = this;
		data.ip = ConvertIpToInt( ip );
		data.port = port;
		m_timer.AddTimer(
			E_Reconnect_Timer, 
			TimerCallBack, 
			&data, 
			sizeof(data), 
			false );
	}

	virtual s32 GetDefaultForwardServerId()
	{
		if( m_serverConfig != NULL )
		{
			SGLib::CGuardLock<SGLib::CLock> g(m_cfgLock);
			const ConnectConfig *cfg = m_serverConfig->GetDefaultConnectConfig();
			if( cfg != NULL )
			{
				return cfg->m_Id;
			}
		}
		
		return INVALID_VAL;
	}

    virtual s32 GetServerId(const char *ip, s32 port)
    {
		SGLib::CGuardLock<SGLib::CLock> g(m_cfgLock);
		s32 count = m_serverConfig->GetConnectCount();
		for( s32 i=0; i<count; ++i )
		{
			const ConnectConfig *cfg = m_serverConfig->GetConnectConfigs( i );
			if( cfg == NULL )
			{
				continue;
			}

			if( cfg->m_strIp == ip &&
				cfg->m_nPort == port )
			{
				return cfg->m_Id;
			}
		}

		return INVALID_VAL;
    }

	virtual LoggerPtr GetLogger()
	{
		return m_log;
	}

	static char* ConvertIpToStr(u64 ip)
	{
		struct in_addr in;
#if _USE_WIN32
		in.S_un.S_addr = ip;
#else
		in.s_addr = ip;
#endif
		return inet_ntoa( in );
	}

	static u64 ConvertIpToInt(const char *ip)
	{
#ifdef _USE_WIN32
		return (u64)inet_addr( ip );
#else
		u64 uip;
		inet_pton( AF_INET, ip, &uip );
		return uip;
#endif
	}

	static void TimerCallBack(void *pData, s32 nDataLen)
	{
		CallBackData *data = (CallBackData*)(pData);
		CServerEx<TClient, TRpcClient, TServerConfig> *pThis = (CServerEx<TClient, TRpcClient, TServerConfig>*)data->pData;
		if( pThis != NULL )
		{
			pThis->Connect( ConvertIpToStr(data->ip), data->port );
		}
	}

	virtual s32 AddTimer(u32 uElapse, CallBackFunc Func, void *pUserData, s32 nUserDataLen, bool bPersist)
	{
		return m_timer.AddTimer(
			uElapse, 
			Func, 
			pUserData, 
			nUserDataLen, 
			bPersist );
	}

	virtual void DelTimer(s32 timerId)
	{
		m_timer.DelTimer( timerId );
	}

	virtual s32 ServerId()
	{ 
		return m_id;
	}

	virtual s32 MaxConnectionCount()
	{
        return m_ctConnCount;
	}

	virtual const CServerConfig* GetServerConfig()
	{ 
		SGLib::CGuardLock<SGLib::CLock> g(m_cfgLock);
		return m_serverConfig;
	}

	virtual s32 GetCenterServerId()
	{ 
		SGLib::CGuardLock<SGLib::CLock> g(m_cfgLock);
		if( m_serverConfig )
		{
			return m_serverConfig->GetCenterServerId();
		}
		return 0; 
	}

	virtual s32 GetGameManagerServerId()
	{ 
		SGLib::CGuardLock<SGLib::CLock> g(m_cfgLock);
		if( m_serverConfig )
		{
			return m_serverConfig->GetGameManagerServerId();
		}
		return 0; 
	}

	virtual s32 GetGroupManagerServerId()
	{ 
		SGLib::CGuardLock<SGLib::CLock> g(m_cfgLock);
		if( m_serverConfig )
		{
			return m_serverConfig->GetGroupManagerServerId();
		}
		return 0; 
	}

	virtual bool AddConnectItem(s32 id, const char *ip, s32 port, bool isDefault)
	{
		SGLib::CGuardLock<SGLib::CLock> g(m_cfgLock);
		if( m_serverConfig )
		{
			return m_serverConfig->AddConnectItem( id, ip, port, isDefault );
		}

		return false;
	}
	
	virtual SGLib::CEventWorkerExPool* GetEventWorkerExPool()
	{ 
		return &m_eventWorkerPool;
	}

protected:
	s32 m_id;

    // server config
	string m_serverCfgFile;
	SGLib::CLock m_cfgLock;
	CServerConfig* m_serverConfig;
	
    // event worker
    s32 m_workerCount;
    SGLib::CEventWorkerExPool m_eventWorkerPool;

    // client
    s32 m_ctIoServiceCount;
    s32 m_ctConnCount;
    s32 m_ctSendSize;
    s32 m_ctRecvSize;
    CIoSocketExMgr m_ctIoSocketMgr;
    CIoServiceExPool m_ctIoSerivcePool;
    CAccepterEx m_accepter;

    // rpc client
    s32 m_rctIoServiceCount;
    s32 m_rctConnCount;
    s32 m_rctSendSize;
    s32 m_rctRecvSize;
    CIoSocketExMgr m_rctIoSocketMgr;
    CIoServiceExPool m_rctIoSerivcePool;
    CConnectorEx m_connector;
	
	LoggerPtr m_log;

    class CCallbackTimer : public IEventEx
    {
    public:
        struct CallBackHead
        {
            CallBackFunc func;
            s32 len;
            void *obj;
        };

        enum
        {
            E_Timer_DataLen = CEventWorkerEx::E_Max_SessionEvent_ParamSize - sizeof(CallBackHead) - sizeof(void*), 
		    E_Max_Timer_Count = 1000,
        };

        struct CallBackData
        {
            CallBackHead head;
            char data[E_Timer_DataLen];
        };

        CCallbackTimer(SGLib::CEventWorkerExPool &eventWorkerPool) : 
            m_eventWorkerPool(eventWorkerPool), m_timer()
        {
            SG_ASSERT( sizeof(CallBackData) < CTimer<E_Max_Timer_Count>::E_Timer_UserDataLen );
        }

        virtual void HandleEvent(s32 len, char *data)
        {
            SELF_ASSERT( data!=NULL && len>0, return; );
            CallBackData *cbData = (CallBackData*)data;
            cbData->head.func( cbData->data, cbData->head.len );
        }

        bool Start(){ return m_timer.Start(); }
        void Stop(){ m_timer.Stop(); }

        s32 AddTimer(u32 elapse, CallBackFunc func, void *data, s32 len, bool isPersist)
        {
			if( len > E_Timer_DataLen )
			{
				return INVALID_VAL;
			}

			CallBackData cbData;
			cbData.head.obj = this;
            cbData.head.func = func;
            cbData.head.len = (len>E_Timer_DataLen) ? E_Timer_DataLen : len;
            if( cbData.head.len > 0 )
            {
                memcpy( cbData.data, data, cbData.head.len );
            }

            return m_timer.AddTimer( elapse, TimerCallBack, &cbData, sizeof(cbData), isPersist );
        }

        void DelTimer(s32 timerId)
        {
            return m_timer.DelTimer( timerId );
        }

        static void TimerCallBack(void *data, s32 len)
        {
            SELF_ASSERT( data!=NULL && len>0, return; );
            CallBackData *cbData = (CallBackData*)data;
            CCallbackTimer *pthis = (CCallbackTimer*)(cbData->head.obj);
            pthis->m_eventWorkerPool.PushEvent(
                    0, len, data, (IEventEx*)cbData->head.obj );
        }
    protected:
        SGLib::CEventWorkerExPool &m_eventWorkerPool;
        CTimer<E_Max_Timer_Count> m_timer;
    } m_timer;

};

#endif
