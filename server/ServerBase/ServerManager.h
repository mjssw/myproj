#ifndef _BASE_SERVER_MANAGER_H_
#define _BASE_SERVER_MANAGER_H_

#ifdef _USE_IOSERVICE_EX_
#include "ServerEx.h"
#else
#include "Server.h"
#endif
#include <map>
#include "google/protobuf/message.h"
#include "MySqlManager.h"

#define MSG_ID_LEN			sizeof(s32)
#define MAX_MSG_LEN			4096

#define SERVER_LOG_DEBUG(msg)	LOG4CXX_DEBUG( CServerManager::Instance().GetLogger(), msg );
#define SERVER_LOG_INFO(msg)	LOG4CXX_INFO( CServerManager::Instance().GetLogger(), msg );
#define SERVER_LOG_ERROR(msg)	LOG4CXX_ERROR( CServerManager::Instance().GetLogger(), msg );
#define SERVER_LOG_WARN(msg)	LOG4CXX_WARN( CServerManager::Instance().GetLogger(), msg );
#define SERVER_LOG_FATAL(msg)	LOG4CXX_FATAL( CServerManager::Instance().GetLogger(), msg );

class CServerManager
{
public:
	enum
	{
		E_Mysql_ExecWorker_Count = 3,
		E_Mysql_Queue_Size = 1024,
	};

	static std::string TimeNow()
	{
		char strTime[64] = {0};
		time_t now;
		time( &now );
		strftime( strTime, sizeof(strTime), "%Y-%m-%d", localtime(&now) );
		return std::string( strTime );
	}

	~CServerManager()
	{
		SAFE_DELETE( m_pServer );
		_DeleteMysqlMgr();
	}

	static CServerManager& Instance()
	{
		if( m_pInstance == NULL )
		{
			m_pInstance = new CServerManager();
		}
		return *m_pInstance;
	}

	template<class TClient, class TRpcClient, class TServerConfig>
	bool InitServer(const char *server_cfg_file, const char *netcfg_file, const char *cfg_name, const char *cfg_name2)
	{
#ifdef _USE_IOSERVICE_EX_
		CNetConfigMgr ctNetCfg( netcfg_file, cfg_name );
		typename CServerEx<TClient, TRpcClient, TServerConfig>::IoConfig ctCfg;
		ctCfg.m_ioServiceCount = 1;
		ctCfg.m_connCount = ctNetCfg.GetNetConfig().m_maxClientCount;
		ctCfg.m_sendSize = ctNetCfg.GetNetConfig().m_sendBufferSize;
		ctCfg.m_recvSize = ctNetCfg.GetNetConfig().m_recvBufferSize;
		
		CNetConfigMgr rctNetCfg( netcfg_file, cfg_name2 );
		typename CServerEx<TClient, TRpcClient, TServerConfig>::IoConfig rctCfg;
		rctCfg.m_ioServiceCount = 1;
		rctCfg.m_connCount = rctNetCfg.GetNetConfig().m_maxClientCount;
		rctCfg.m_sendSize = rctNetCfg.GetNetConfig().m_sendBufferSize;
		rctCfg.m_recvSize = rctNetCfg.GetNetConfig().m_recvBufferSize;
		m_pServer = new CServerEx<TClient, TRpcClient, TServerConfig>(
			server_cfg_file, 1, ctCfg, rctCfg );
#else
		m_pServer = new CServer<TClient, TRpcClient, TServerConfig>(
			server_cfg_file,
			netcfg_file,
			cfg_name,
			cfg_name2 );
#endif

		return ( m_pServer != NULL );
	}

	bool Start()
	{
		if( m_pServer != NULL )
		{
			return m_pServer->Start() && _InitMysqlMgr() && _StartMysqlMgr();
		}

		return false;
	}

	void Stop()
	{
		if( m_pServer != NULL )
		{
			_StopMysqlMgr();
			m_pServer->Stop();
		}
	}

	void Connect(const char *ip, int port)
	{
		if( m_pServer != NULL )
		{
			m_pServer->Connect( ip, port );
		}
	}

	void Reconnect(const char *ip, int port)
	{
		if( m_pServer != NULL )
		{
			m_pServer->Reconnect( ip, port );
		}
	}

	s32 GetDefaultForwardServerId()
	{
		if( m_pServer != NULL )
		{
			return m_pServer->GetDefaultForwardServerId();
		}

		return INVALID_VAL;
	}

	LoggerPtr GetLogger()
	{
		if( m_pServer != NULL )
		{
			return m_pServer->GetLogger();
		}

		return LoggerPtr();
	}

	void AddClient(u64 id, CClient *client)
	{
		if( client != NULL )
		{
			SERVER_LOG_DEBUG( "CServerManager,AddClient," << id );

			CGuardLock<CLock> g(m_ClientLock);
			m_ClientMap[id] = client;
		}
	}

	void DelClient(u64 id)
	{
		SERVER_LOG_DEBUG( "CServerManager,DelClient," << id );

		CGuardLock<CLock> g(m_ClientLock);
		std::map<u64, CClient*>::iterator it = m_ClientMap.find( id );
		if( it != m_ClientMap.end() )
		{
			m_ClientMap.erase( it );
		}
	}

    void AddRpcClient(s32 id, CRpcClient *client)
	{
		if( client != NULL )
		{
			SERVER_LOG_DEBUG( "CServerManager,AddRpcClient," << id );

			CGuardLock<CLock> g(m_RpcClientLock);
			m_RpcClientMap[id] = client;
		}
	}

	void DelRpcClient(s32 id)
	{
		SERVER_LOG_DEBUG( "CServerManager,DelRpcClient," << id );

		CGuardLock<CLock> g(m_RpcClientLock);
		std::map<s32, CRpcClient*>::iterator it = m_RpcClientMap.find( id );
		if( it != m_RpcClientMap.end() )
		{
			m_RpcClientMap.erase( it );
		}
	}

	void SendRpcMsg(s32 serverId, const ::google::protobuf::Message &Msg, s32 nMsgId)
	{
		//SERVER_LOG_DEBUG( "CServerManager,SendRpcMsg," << serverId );

		int count = Msg.ByteSize();
		byte buf[MAX_MSG_LEN] = {0};
		*((s32*)buf) = nMsgId;
		if( Msg.SerializeToArray(buf+MSG_ID_LEN, MAX_MSG_LEN) )
		{
			bool ret = _SendRpcMsg( serverId, buf, count+MSG_ID_LEN );
			if( !ret )
			{
				SERVER_LOG_ERROR( "CServerManager,SendRpcMsg," << nMsgId );
			}
		}
		else
		{
			SERVER_LOG_ERROR( "CServerManager,SendRpcMsg," << nMsgId << ",SerializeToArray" );
		}
	}

    s32 GetServerId(const char *ip, s32 port)
    {
		if( m_pServer != NULL )
        {
            return m_pServer->GetServerId( ip, port );
        }

        return INVALID_VAL;
    }

	void CloseClient(u64 id)
	{
		SERVER_LOG_DEBUG( "CServerManager,CloseClient," << id );

		CGuardLock<CLock> g(m_ClientLock);
		std::map<u64, CClient*>::iterator it = m_ClientMap.find( id );
		if( it != m_ClientMap.end() )
		{
			it->second->Close();
		}
	}

	void SetClientForwardServerId(u64 id, s32 serverId)
	{
		SERVER_LOG_DEBUG( "CServerManager,SetClientForwardServerId," << id << "," << serverId );

		CGuardLock<CLock> g(m_ClientLock);
		std::map<u64, CClient*>::iterator it = m_ClientMap.find( id );
		if( it != m_ClientMap.end() )
		{
			it->second->SetForwardId( serverId );
		}
		else
		{
			SERVER_LOG_ERROR( "CServerManager,SetClientForwardServerId,FindClient," << id );
		}
	}

	void SendClientMsg(u64 id, const byte *pPkg, s32 pkgLen)
	{
		//SERVER_LOG_DEBUG( "CServerManager,SendClientMsg," << id );

		CGuardLock<CLock> g(m_ClientLock);
		std::map<u64, CClient*>::iterator it = m_ClientMap.find( id );
		if( it != m_ClientMap.end() )
		{
			bool ret = it->second->SendPkg( pPkg, pkgLen );
			if( !ret )
			{
				SERVER_LOG_ERROR( "CServerManager,SendClientMsg," << id << "," << pkgLen );
			}
		}
		else
		{
			SERVER_LOG_ERROR( "CServerManager,SendClientMsg,FindClient," << id );
		}
	}
	
	void SendClientMsg(u64 id, const ::google::protobuf::Message &msg, s32 msgid)
	{
		int count = msg.ByteSize();
		byte buf[MAX_MSG_LEN] = {0};
		*((s32*)buf) = msgid;
		if( msg.SerializeToArray(buf+MSG_ID_LEN, MAX_MSG_LEN) )
		{
			SendClientMsg( id, buf, count+MSG_ID_LEN );
		}
		else
		{
			SERVER_LOG_ERROR( "CGroupManagerClient,SendMessageToGroupServer,SerializeToArray" );
		}
	}

	CClient* FindClient(u64 id)
	{
		CGuardLock<CLock> g(m_ClientLock);
		std::map<u64, CClient*>::iterator it = m_ClientMap.find( id );
		if( it != m_ClientMap.end() )
		{
			return it->second;
		}
		return NULL;
	}

	CRpcClient* FindRpcClient(s32 id)
	{
		CGuardLock<CLock> g(m_RpcClientLock);
		std::map<s32, CRpcClient*>::iterator it = m_RpcClientMap.find( id );
		if( it != m_RpcClientMap.end() )
		{
			return it->second;
		}
		return NULL;
	}

	s32 AddTimer(u32 uElapse, CallBackFunc Func, void *pUserData, s32 nUserDataLen, bool bPersist)
	{
		if( m_pServer )
		{
			return m_pServer->AddTimer(
				uElapse, 
				Func, 
				pUserData, 
				nUserDataLen, 
				bPersist );
		}
		return INVALID_VAL;
	}

	void DelTimer(s32 timerId)
	{
		if( m_pServer )
		{
			m_pServer->DelTimer( timerId );
		}
	}

	u32 GetClientCount()
	{
		CGuardLock<CLock> g(m_ClientLock);
		return (u32)m_ClientMap.size();
	}

	s32 ServerId()
	{
		if( m_pServer )
		{
			return m_pServer->ServerId();
		}

		return INVALID_VAL;
	}
	
	s32 MaxConnectionCount()
	{
		if( m_pServer )
		{
			return m_pServer->MaxConnectionCount();
		}

		return INVALID_VAL;
	}

	u64 NextClientId()
	{
		u64 id = 0;
		{
			CGuardLock<CLock> g( m_ClientIdLock );
			id = m_nextClientId;
			++m_nextClientId;
		}
		return id;
	}

	const CServerConfig* GetServerConfig()
	{
		if( m_pServer )
		{
			return m_pServer->GetServerConfig();
		}

		return NULL;
	}

	s32 GetCenterServerId()
	{ 
		if( m_pServer )
		{
			return m_pServer->GetCenterServerId(); 
		}
		return 0;
	}

	s32 GetGameManagerServerId()
	{ 
		if( m_pServer )
		{
			return m_pServer->GetGameManagerServerId(); 
		}
		return 0;
	}

	s32 GetGroupManagerServerId()
	{ 
		if( m_pServer )
		{
			return m_pServer->GetGroupManagerServerId(); 
		}
		return 0;
	}

	void GetClientList(std::vector<u64> &vecClient)
	{
		vecClient.clear();
		CGuardLock<CLock> g(m_ClientLock);
		std::map<u64, CClient*>::iterator it = m_ClientMap.begin();
		for( ; it != m_ClientMap.end(); ++it )
		{
			vecClient.push_back( it->first );
		}
	}
	
	bool AddConnectItem(s32 id, const char *ip, s32 port, bool isDefault)
	{
		if( m_pServer )
		{
			return m_pServer->AddConnectItem( id, ip, port, isDefault ); 
		}
		return false;
	}

	CMysqlManager* GetMysqlManager(s32 id)
	{
		std::map<s32, CMysqlManager*>::iterator it = m_mysqlMgr.find(id);
		if( it != m_mysqlMgr.end() )
		{
			return it->second;
		}
		return NULL;
	}

	s32 GetMysqlManagerIdByDbName(const std::string &dbname)
	{
		std::map<std::string, s32>::iterator it = m_mysqlDbnameIndex.find( dbname );
		if( it != m_mysqlDbnameIndex.end() )
		{
			return it->second;
		}
		return INVALID_VAL;
	}

	template<class ClassObj, class ClassMemFunc>
	bool ExecSql(s32 id, const std::string &sql, const ClassObj &classObj, ClassMemFunc memCallbackFunc, void *param, s32 len)
	{
		CMysqlManager *sqlMgr = GetMysqlManager( id );
		if( !sqlMgr )
		{
			return false;
		}

		return sqlMgr->Execute( sql.c_str(), classObj, memCallbackFunc, param, len );
	}
	
	s32 HashUser(const std::string &user)
	{
		// 注意user数据库始终是ud1 ... udN
		// 这里由user值hash到某一个 ud?，然后根据ud?找到对应的id
		const static string dbhead = "ud";
		s32 dbid = _hashId( user );
		char strdbid[32] = {0};
		sprintf( strdbid, "%d", dbid );
		string dbname = dbhead + strdbid;
		return GetMysqlManagerIdByDbName( dbname );
	}

	std::string BuildUserGroupTableName(const std::string &user)
	{
		return (std::string("user_") + user + "_group");
	}

	std::string BuildGroupTableName(u64 groupid)
	{
		char table[128] = {0};
		sprintf( table, "group_%llu", groupid );
		return std::string(table);
	}

	s32 GetGroupDbId()
	{
		return GetMysqlManagerIdByDbName( "groups" );
	}

private:
	
	s32 _hashId(const string &user)
	{
		// TODO
		return 1;
	}

	bool _SendRpcMsg(s32 serverId, const byte *pPkg, s32 pkgLen)
	{
		//SERVER_LOG_DEBUG( "CServerManager,SendRpcMsg," << serverId );

		CGuardLock<CLock> g(m_RpcClientLock);
		std::map<s32, CRpcClient*>::iterator it = m_RpcClientMap.find( serverId );
		if( it != m_RpcClientMap.end() )
        {
			return it->second->Send( pPkg, pkgLen );
        }

        return false;
	}

	bool _InitMysqlMgr()
	{
		s32 count = m_pServer->GetServerConfig()->GetDbConfigCount();
		for( s32 i=0; i<count; ++i )
		{
			const MysqlConfig *cfg = m_pServer->GetServerConfig()->GetMysqlDbConfig( i );
			SELF_ASSERT( cfg, return false; );
			CMysqlManager *mysql = new CMysqlManager( 
				E_Mysql_ExecWorker_Count, E_Mysql_Queue_Size, m_pServer->GetEventWorkerExPool() );
			if( !mysql )
			{
				return false;
			}
			m_mysqlMgr[ cfg->m_id ] = mysql;
			m_mysqlTypeIndex[ cfg->m_type ][ cfg->m_id ] = mysql;
			m_mysqlDbnameIndex[ cfg->m_db ] = cfg->m_id;
		
			SERVER_LOG_INFO( "Init mysqlmgr:" << cfg->m_id << " success!" );
		}

		return true;
	}

	bool _StartMysqlMgr()
	{
		bool ret = true;
		std::map<s32, CMysqlManager*>::iterator it = m_mysqlMgr.begin();
		for( ; it != m_mysqlMgr.end(); ++it )
		{
			const MysqlConfig *cfg = m_pServer->GetServerConfig()->GetMysqlDbConfigById( it->first );
			SELF_ASSERT( cfg, return false; );
			ret &= it->second->Start(
				cfg->m_ip.c_str(), 
				cfg->m_db.c_str(), 
				cfg->m_user.c_str(), 
				cfg->m_password.c_str(), 
				cfg->m_port );
			
			if( ret )
			{
				SERVER_LOG_INFO( "start mysqlmgr:" << cfg->m_id << " [" <<\
					cfg->m_ip.c_str() << ":" << cfg->m_port << ":" << cfg->m_db.c_str() << "] success!" );
			}
			else
			{
				SERVER_LOG_ERROR( "start mysqlmgr:" << cfg->m_id << " [" <<\
					cfg->m_ip.c_str() << ":" << cfg->m_port << ":" << cfg->m_db.c_str() << "] failed!" );
			}
		}
		return ret;
	}

	void _StopMysqlMgr()
	{
		std::map<s32, CMysqlManager*>::iterator it = m_mysqlMgr.begin();
		for( ; it != m_mysqlMgr.end(); ++it )
		{
			it->second->Stop();
		}
	}

	void _DeleteMysqlMgr()
	{
		std::map<s32, CMysqlManager*>::iterator it = m_mysqlMgr.begin();
		for( ; it != m_mysqlMgr.end(); ++it )
		{
			SAFE_DELETE( it->second );
		}
		m_mysqlMgr.clear();
		m_mysqlTypeIndex.clear();
		m_mysqlDbnameIndex.clear();
	}

private:
	CServerManager() : 
		m_pServer(NULL),
		m_ClientLock(),
		m_ClientMap(),
		m_RpcClientLock(),
		m_RpcClientMap(),
		m_ClientIdLock(),
		m_nextClientId(1)
	{
	}

	static CServerManager *m_pInstance;

	CServerBase *m_pServer;

	CLock m_ClientLock;
	std::map<u64, CClient*> m_ClientMap;

	CLock m_RpcClientLock;
	std::map<s32, CRpcClient*> m_RpcClientMap;

	CLock m_ClientIdLock;
	u64 m_nextClientId;

	std::map<s32, CMysqlManager*> m_mysqlMgr;
	std::map<std::string, map<s32, CMysqlManager*> > m_mysqlTypeIndex;
	std::map<std::string, s32> m_mysqlDbnameIndex;
};

#endif
