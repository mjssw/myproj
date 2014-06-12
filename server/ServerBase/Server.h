#ifndef _BASE_SERVER_H_
#define _BASE_SERVER_H_

#include "globalcfg.h"
#include "IoService.h"
#include "Timer.h"
#include "EventWorkerExPool.h"
using namespace SGLib;

#include <string>
using namespace std;

#include "Log4cxxCreater.h"
#include "ServerConfig.h"
#include "Client.h"
#include "RpcClient.h"


class CServerBase
{
public:
	virtual bool Start(){ return false; }
	virtual void Stop(){}
	virtual void Connect(const char *ip, int port){}
	virtual void Reconnect(const char *ip, int port){}
	virtual s32 GetDefaultForwardServerId(){ return INVALID_VAL; }
	virtual s32 GetServerId(const char *ip, s32 port){ return INVALID_VAL; }
	virtual s32 ServerId(){ return INVALID_VAL; }
	virtual LoggerPtr GetLogger(){ return LoggerPtr(); }
	virtual s32 AddTimer(u32 uElapse, CallBackFunc Func, void *pUserData, s32 nUserDataLen, bool bPersist){ return 0; }
	virtual void DelTimer(s32 timerId){}
	virtual s32 MaxConnectionCount(){ return INVALID_VAL; }
	virtual const CServerConfig* GetServerConfig(){ return NULL; }
	virtual s32 GetCenterServerId(){ return 0; }
	virtual s32 GetGameManagerServerId(){ return 0; }
	virtual s32 GetGroupManagerServerId(){ return 0; }
	virtual bool AddConnectItem(s32 id, const char *ip, s32 port, bool isDefault){ return true; }
	virtual SGLib::CEventWorkerExPool* GetEventWorkerExPool(){ return NULL; }
};

template<class TClient, class TRpcClient, class TServerConfig>
class CServer : public CServerBase
{
public:
	enum
	{
		RECONNECT_TIMER = 1000,
		MAX_TIMER_COUNT = 100,
	};

	struct CallBackData
	{
		void *pData;
		char strip[MAX_IP_LEN];
		int port;
	};

	CServer(const char *server_cfg_file, const char *netcfg_file, const char *cfg_name, const char *cfg_name2) : 
		m_strServerCfgFile(server_cfg_file),
		m_NetcfgFile(netcfg_file),
		m_CfgName(cfg_name),
		m_CfgName2(cfg_name2),
		m_pCliSvr(NULL),
		m_pRpcSvr(NULL),
		m_timer(),
		m_pServerConfig(NULL)
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
	~CServer()
	{
		Stop();
	}

	virtual bool Start()
	{
		m_pServerConfig = new TServerConfig( m_strServerCfgFile.c_str() );
		if( !m_pServerConfig->LoadConfigFile() )
		{
			return false;
		}
		m_SvrId = m_pServerConfig->GetServerConfig().m_Id; 

		m_timer.Start();

		CNetConfigMgr netCfgMgr1( m_NetcfgFile.c_str(), m_CfgName.c_str() );
		m_pCliSvr = new CIoService<TClient>( netCfgMgr1.GetNetConfig() );
		if( m_pCliSvr!=NULL && m_pCliSvr->Init()==E_Error_Success )
		{
			if( m_pCliSvr->StartListen( m_pServerConfig->GetServerConfig().m_Port ) == E_Error_Success )
			{
				//log4cxx::xml::DOMConfigurator::configure("xmlconfig");
				//log4cxx::LoggerPtr log = log4cxx::Logger::getLogger("logtest");

				// get logger
				m_Log = CLog4cxxCreater::GetLogger(
					m_pServerConfig->GetLogConfig(),
					"ServerLog" );

				m_pCliSvr->Dispatch();

				// create rpc service
				CNetConfigMgr netCfgMgr2( m_NetcfgFile.c_str(), m_CfgName2.c_str() );
				m_pRpcSvr = new CIoService<TRpcClient>( netCfgMgr2.GetNetConfig() );
				if( m_pRpcSvr!=NULL && m_pRpcSvr->Init()==E_Error_Success )
				{
					m_pRpcSvr->Dispatch();

					s32 count = m_pServerConfig->GetConnectCount();
					for( s32 i=0; i<count; ++i )
					{
						const ConnectConfig *cfg = m_pServerConfig->GetConnectConfigs( i );
						if( cfg == NULL )
						{
							continue;
						}

						m_pRpcSvr->Connect(
							cfg->m_strIp.c_str(),
							cfg->m_nPort );
					}

					LOG4CXX_INFO( m_Log, "Server:(" << m_pServerConfig->GetServerConfig().m_Id << \
						":" << m_pServerConfig->GetServerConfig().m_DisplayName << \
						":" << m_pServerConfig->GetServerConfig().m_Ip << \
						":" << m_pServerConfig->GetServerConfig().m_Port << ") Start Success!" << \
						"[PID=" << GetProcessId() << "]" );

					return true;
				}
			}
		}

		return false;
	}

	virtual void Stop()
	{
		m_timer.Stop();
		if( m_pCliSvr != NULL )
		{
			m_pCliSvr->Fini();
			SAFE_DELETE( m_pCliSvr );
		}

		if( m_pRpcSvr != NULL )
		{
			m_pRpcSvr->Fini();
			SAFE_DELETE( m_pRpcSvr );
		}
	}

	virtual void Connect(const char *ip, int port)
	{
		if( m_pRpcSvr != NULL )
		{
			m_pRpcSvr->Connect( ip, port );
		}
	}

	virtual void Reconnect(const char *ip, int port)
	{
		LOG4CXX_INFO( m_Log, "CServer::ReConnect," << ip << "," << port );

		CallBackData data;
		data.pData = this;
		strcpy( data.strip, ip );
		data.port = port;
		m_timer.AddTimer(
			RECONNECT_TIMER, 
			TimerCallBack, 
			&data, 
			sizeof(data), 
			false );
	}

	virtual s32 GetDefaultForwardServerId()
	{
		if( m_pServerConfig != NULL )
		{
			SGLib::CGuardLock<SGLib::CLock> g(m_CfgLock);
			const ConnectConfig *cfg = m_pServerConfig->GetDefaultConnectConfig();
			if( cfg != NULL )
			{
				return cfg->m_Id;
			}
		}
		
		return INVALID_VAL;
	}

    virtual s32 GetServerId(const char *ip, s32 port)
    {
		SGLib::CGuardLock<SGLib::CLock> g(m_CfgLock);
		s32 count = m_pServerConfig->GetConnectCount();
		for( s32 i=0; i<count; ++i )
		{
			const ConnectConfig *cfg = m_pServerConfig->GetConnectConfigs( i );
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
		return m_Log;
	}

	static void TimerCallBack(void *pData, s32 nDataLen)
	{
		CallBackData *data = (CallBackData*)(pData);
		CServer<TClient, TRpcClient, TServerConfig> *pThis = (CServer<TClient, TRpcClient, TServerConfig>*)data->pData;
		if( pThis != NULL && pThis->m_pCliSvr != NULL )
		{
			pThis->m_pRpcSvr->Connect(data->strip, data->port);
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
		return m_SvrId;
	}

	virtual s32 MaxConnectionCount()
	{ 
		if( m_pCliSvr )
		{
			return m_pCliSvr->GetMaxConnectionCount();
		}

		return INVALID_VAL;
	}

	virtual const CServerConfig* GetServerConfig()
	{ 
		SGLib::CGuardLock<SGLib::CLock> g(m_CfgLock);
		return m_pServerConfig;
	}

	virtual s32 GetCenterServerId()
	{ 
		SGLib::CGuardLock<SGLib::CLock> g(m_CfgLock);
		if( m_pServerConfig )
		{
			return m_pServerConfig->GetCenterServerId();
		}
		return 0; 
	}

	virtual s32 GetGameManagerServerId()
	{ 
		SGLib::CGuardLock<SGLib::CLock> g(m_CfgLock);
		if( m_pServerConfig )
		{
			return m_pServerConfig->GetGameManagerServerId();
		}
		return 0; 
	}

	virtual s32 GetGroupManagerServerId()
	{ 
		SGLib::CGuardLock<SGLib::CLock> g(m_CfgLock);
		if( m_pServerConfig )
		{
			return m_pServerConfig->GetGroupManagerServerId();
		}
		return 0; 
	}

	virtual bool AddConnectItem(s32 id, const char *ip, s32 port, bool isDefault)
	{
		SGLib::CGuardLock<SGLib::CLock> g(m_CfgLock);
		if( m_pServerConfig )
		{
			return m_pServerConfig->AddConnectItem( id, ip, port, isDefault );
		}

		return false;
	}

protected:
	s32 m_SvrId;
	string m_strServerCfgFile;
	string m_NetcfgFile;
	string m_CfgName;
	string m_CfgName2;

	CIoService<TClient> *m_pCliSvr;
	CIoService<TRpcClient> *m_pRpcSvr;
	CTimer<MAX_TIMER_COUNT> m_timer;
	
	SGLib::CLock m_CfgLock;
	CServerConfig* m_pServerConfig;
	LoggerPtr m_Log;
};

#endif
