#include "Config.h"
#include <tinyxml.h>
using namespace SGLib;

CNetConfigMgr::CNetConfigMgr(const char *netcfg_file, const char *cfg_name) : m_bInit(false)
{
	_Init( netcfg_file, cfg_name );
}

CNetConfigMgr::~CNetConfigMgr(){}

const CNetConfig& CNetConfigMgr::GetNetConfig()
{
	return m_netCfg;
}

void CNetConfigMgr::_Init(const char *netcfg_file, const char *cfg_name)
{
	m_bInit = true;

	m_netCfg.m_workThreadQuitTimeOut = WAIT_WORK_THREAD_QUIT_TIME;
	m_netCfg.m_ioTimeOut				= DEFAULT_MS_TIME_OUT;
	m_netCfg.m_maxClientCount		= MAX_CLIENT_COUNT;
	m_netCfg.m_eventQueueSize		= MAX_EVENT_COUNT;
	m_netCfg.m_waitEventTimeOut		= WAIT_EVENT_TIME_OUT;
	m_netCfg.m_maxPackageLen			= MAX_RECV_PKG_LEN;
	m_netCfg.m_eventWorkerCount		= MAX_DISPATCH_EVENT_NUM;
	m_netCfg.m_connectingQueueSize	= CONNECTING_QUEUE_SIZE;
	m_netCfg.m_sendBufferSize		= MAX_SEND_BUFFER_LEN;
	m_netCfg.m_recvBufferSize		= MAX_RECV_BUFFER_LEN;

	_LoadNetConfig( netcfg_file, cfg_name );
}

void CNetConfigMgr::_LoadNetConfig(const char *netcfg_file, const char *cfg_name)
{
	TiXmlDocument doc( netcfg_file );
	if( doc.LoadFile() )
	{
		TiXmlElement *root = doc.RootElement();
		if( root == NULL ) 
		{
			return;
		}

		bool bFind = false;
		TiXmlElement *elem = root->FirstChildElement();
		while( elem != NULL && elem->Value() != NULL )
		{
			if( strcmp(elem->Value(), cfg_name) == 0 )
			{
				bFind = true;
				break;
			}
			else
			{
				elem = elem->NextSiblingElement();
			}
		}

		if( bFind )
		{
			const char *val = elem->Attribute( "WorkThreadQuitTimeOut" );
			if( val != NULL )
			{
				m_netCfg.m_workThreadQuitTimeOut = atoi( val );
			}

			val = elem->Attribute( "IoTimeOut" );
			if( val != NULL )
			{
				m_netCfg.m_ioTimeOut = atoi( val );
			}

			val = elem->Attribute( "MaxClientCount" );
			if( val != NULL )
			{
				m_netCfg.m_maxClientCount = atoi( val );
			}

			val = elem->Attribute( "EventQueueSize" );
			if( val != NULL )
			{
				m_netCfg.m_eventQueueSize = atoi( val );
			}

			val = elem->Attribute( "WaitEventTimeOut" );
			if( val != NULL )
			{
				m_netCfg.m_waitEventTimeOut = atoi( val );
			}

			val = elem->Attribute( "MaxPackageLen" );
			if( val != NULL )
			{
				m_netCfg.m_maxPackageLen = atoi( val );
			}

			val = elem->Attribute( "EventWorkerCount" );
			if( val != NULL )
			{
				m_netCfg.m_eventWorkerCount = atoi( val );
			}

			val = elem->Attribute( "ConnectingQueueSize" );
			if( val != NULL )
			{
				m_netCfg.m_connectingQueueSize = atoi( val );
			}

			val = elem->Attribute( "SendBufferSize" );
			if( val != NULL )
			{
				m_netCfg.m_sendBufferSize = atoi( val );
			}

			val = elem->Attribute( "RecvBufferSize" );
			if( val != NULL )
			{
				m_netCfg.m_recvBufferSize = atoi( val );
			}
		}
	}
}
