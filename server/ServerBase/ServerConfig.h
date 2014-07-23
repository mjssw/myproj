#ifndef _BASE_SERVER_CONFIG_H_
#define _BASE_SERVER_CONFIG_H_

#include "Define.h"
#include <tinyxml.h>
#include <string>
#include <vector>
#include "Log4cxxCreater.h"

#define DB_TYPE_USER	"user"
#define DB_TYPE_GROUP	"group"
#define DB_TYPE_GAME	"game"

struct ConnectConfig
{
	ConnectConfig():
		m_Id(0),
		m_strSvrName(""),
		m_strIp(""),
		m_nPort(0),
		m_bDefault(false)
	{}

	s32  m_Id;
	std::string m_strSvrName;
	std::string m_strIp;
	s32 m_nPort;
	bool m_bDefault;
};

struct MysqlConfig
{
	s32 m_id;
	std::string m_type;
	std::string m_ip;
	s32 m_port;
	std::string m_db;
	std::string m_user;
	std::string m_password;
};

class CServerConfig
{
public:
	CServerConfig(const char *cfgFile) : 
		m_ConfigFile(cfgFile),
		m_ConnectCount(0),
		m_defaultIdx(-1),
		m_LogConfig(),
		m_nCenterServerId(0),
		m_nGameManagerServerId(0),
		m_nGroupManagerServerId(0)
	{}

	virtual ~CServerConfig()
	{}

	bool LoadConfigFile()
	{
		return _LoadConfigFile(); 
	}

	s32 GetConnectCount()
	{ 
		return (s32)m_Connects.size(); 
	}
	const ConnectConfig* GetConnectConfigs(s32 idx) const
	{
		if( idx >= 0 && idx < (s32)m_Connects.size() )
		{
			return &m_Connects[ idx ];
		}
		return NULL;
	}
	const ConnectConfig* GetDefaultConnectConfig() const
	{
		if( m_defaultIdx >= 0 && m_defaultIdx < (s32)m_Connects.size() )
		{
			return &m_Connects[ m_defaultIdx ];
		}
		return NULL;
	}
	const LogConfig& GetLogConfig() const
	{
		return m_LogConfig;
	}
	const ServerConfig& GetServerConfig() const
	{
		return m_ServerConfig;
	}

	virtual bool LoadOtherConfig(TiXmlElement *root){ return true; }

	s32 GetCenterServerId()
	{ 
		return m_nCenterServerId; 
	}

	s32 GetGameManagerServerId()
	{ 
		return m_nGameManagerServerId; 
	}

	s32 GetGroupManagerServerId()
	{ 
		return m_nGroupManagerServerId; 
	}

	bool AddConnectItem(s32 id, const char *ip, s32 port, bool isDefault=false)
	{
		return _AddConnectItem( id, ip, port, (isDefault?"true":"false") );
	}

	s32 GetDbConfigCount() const
	{
		return (s32)m_mysqlCfgs.size();
	}
	const MysqlConfig* GetMysqlDbConfig(s32 idx) const
	{
		if( idx >= 0 && idx < (s32)m_mysqlCfgs.size() )
		{
			return &m_mysqlCfgs[ idx ];
		}
		return NULL;
	}
	const MysqlConfig* GetMysqlDbConfigById(s32 id) const
	{
		s32 count = (s32)m_mysqlCfgs.size();
		for( s32 i=0; i<count; ++i )
		{
			if( m_mysqlCfgs[i].m_id == id )
			{
				return &m_mysqlCfgs[i];
			}
		}
		return NULL;
	}

private:

	int _GetConnectCount(TiXmlElement *elem)
	{
		int count = 0;
		TiXmlElement *e = elem->FirstChildElement();
		while( e != NULL && e->Value() != NULL )
		{
			if( strcmp(e->Value(), "Connect") == 0 )
			{
				++count;
			}

			e = e->NextSiblingElement();
		}

		return count;
	}

	TiXmlElement* _FindChildElement(TiXmlElement *elem, const char *child_name)
	{
		if( elem == NULL || child_name == NULL ) 
		{
			return NULL;
		}

		TiXmlElement *e = elem->FirstChildElement();
		while( e != NULL && e->Value() != NULL )
		{
			if( strcmp(e->Value(), child_name) == 0 )
			{
				return e;
			}
			else
			{
				e = e->NextSiblingElement();
			}
		}

		return NULL;
	}

	bool _LoadServerConfig(TiXmlElement *root)
	{
		TiXmlElement *elem = _FindChildElement(root, "Server");
		if( elem != NULL )
		{
			const char *val = elem->Attribute( "Id" );
			if( val != NULL )
			{
				m_ServerConfig.m_Id = atoi( val );
			}

			val = elem->Attribute( "DisplayName" );
			if( val != NULL )
			{
				strncpy( m_ServerConfig.m_DisplayName, val, MAX_NAME_LEN );
			}

			val = elem->Attribute( "Ip" );
			if( val != NULL )
			{
				strncpy( m_ServerConfig.m_Ip, val, MAX_IP_LEN );
			}

			val = elem->Attribute( "Port" );
			if( val != NULL )
			{
				m_ServerConfig.m_Port = atoi( val );
			}

			return true;
		}

		return false;
	}

	bool _LoadConnectConfig(TiXmlElement *root)
	{
		TiXmlElement *elem = _FindChildElement(root, "Connects");
		if( elem != NULL )
		{
			const char *val = elem->Attribute( "CenterServerId" );
			if( val != NULL )
			{
				m_nCenterServerId = atoi( val );
			}

			val = elem->Attribute( "GameManagerId" );
			if( val != NULL )
			{
				m_nGameManagerServerId = atoi( val );
			}

			val = elem->Attribute( "GroupManagerId" );
			if( val != NULL )
			{
				m_nGroupManagerServerId = atoi( val );
			}

			m_ConnectCount = _GetConnectCount( elem );
			if( m_ConnectCount <= 0 )
			{
				return true;
			}

			m_Connects.clear();
			TiXmlElement *e = elem->FirstChildElement();
			while( e != NULL && e->Value() != NULL )
			{
				if( strcmp(e->Value(), "Connect") == 0 )
				{
					ConnectConfig cfg;
					const char *val = e->Attribute( "Id" );
					if( val != NULL )
					{
						cfg.m_Id = atoi( val );
					}

					val = e->Attribute( "IsDefault" );
					if( val != NULL )
					{
						cfg.m_bDefault = 
							(strcmp(val, "true")==0) ? true : false;
					}

					val = e->Attribute( "Name" );
					if( val != NULL )
					{
						cfg.m_strSvrName = val;
					}

					val = e->Attribute( "Ip" );
					if( val != NULL )
					{
						cfg.m_strIp = val;
					}

					val = e->Attribute( "Port" );
					if( val != NULL )
					{
						cfg.m_nPort = atoi( val );
					}

					m_Connects.push_back( cfg );
					if( cfg.m_bDefault )
					{
						m_defaultIdx = ((s32)m_Connects.size() - 1);
					}
				}

				e = e->NextSiblingElement();
			}

			return true;
		}

		return false;
	}

	bool _LoadMysqlConfig(TiXmlElement *root)
	{
		m_mysqlCfgs.clear();
		TiXmlElement *elem = _FindChildElement(root, "Databases");
		if( elem != NULL )
		{
			TiXmlElement *e = elem->FirstChildElement();
			while( e != NULL && e->Value() != NULL )
			{
				if( strcmp(e->Value(), "Database") == 0 )
				{
					MysqlConfig cfg;
					const char *val = e->Attribute( "Id" );
					if( val != NULL )
					{
						cfg.m_id = atoi( val );
					}
					
					val = e->Attribute( "Type" );
					if( val != NULL )
					{
						cfg.m_type = val;
					}

					val = e->Attribute( "Ip" );
					if( val != NULL )
					{
						cfg.m_ip = val;
					}

					val = e->Attribute( "Port" );
					if( val != NULL )
					{
						cfg.m_port = atoi( val );
					}

					val = e->Attribute( "User" );
					if( val != NULL )
					{
						cfg.m_user = val;
					}

					val = e->Attribute( "Password" );
					if( val != NULL )
					{
						cfg.m_password = val;
					}

					val = e->Attribute( "Db" );
					if( val != NULL )
					{
						cfg.m_db = val;
					}

					m_mysqlCfgs.push_back( cfg );
				}

				e = e->NextSiblingElement();
			}

			return true;
		}

		return true;
	}

	bool _LoadLogConfig(TiXmlElement *root)
	{
		TiXmlElement *elem = _FindChildElement(root, "Log");
		if( elem != NULL )
		{
			TiXmlElement *e = elem->FirstChildElement();
			while( e != NULL && e->Value() != NULL )
			{
				if( strcmp(e->Value(), "File") == 0 )
				{
					const char *val = e->Attribute( "Path" );
					if( val != NULL )
					{
						m_LogConfig.m_File = val;
					}

					val = e->Attribute( "Enable" );
					if( val != NULL )
					{
						m_LogConfig.m_bEnableFile = 
							(strcmp(val, "true")==0) ? true : false;
					}

					val = e->Attribute( "Layout" );
					if( val != NULL )
					{
						m_LogConfig.m_FileLayout = val;
					}

					val = e->Attribute( "MinLevel" );
					if( val != NULL )
					{
						m_LogConfig.m_FileMinLevel = val;
					}

					val = e->Attribute( "MaxLevel" );
					if( val != NULL )
					{
						m_LogConfig.m_FileMaxLevel = val;
					}
				}
				else if( strcmp(e->Value(), "Console") == 0 )
				{
					const char *val = e->Attribute( "Enable" );
					if( val != NULL )
					{
						m_LogConfig.m_bEnableConsole = 
							(strcmp(val, "true")==0) ? true : false;
					}

					val = e->Attribute( "Layout" );
					if( val != NULL )
					{
						m_LogConfig.m_ConsoleLayout = val;
					}

					val = e->Attribute( "MinLevel" );
					if( val != NULL )
					{
						m_LogConfig.m_ConsoleMinLevel = val;
					}

					val = e->Attribute( "MaxLevel" );
					if( val != NULL )
					{
						m_LogConfig.m_ConsoleMaxLevel = val;
					}
				}

				e = e->NextSiblingElement();
			}

			return true;
		}

		return false;
	}

	bool _LoadConfigFile()
	{
		TiXmlDocument doc( m_ConfigFile.c_str() );
		if( doc.LoadFile() )
		{
			TiXmlElement *root = doc.RootElement();
			if( root == NULL ) 
			{
				return false;
			}

			bool ret = _LoadServerConfig( root );
			if( !ret )
			{
				return false;
			}

			ret = _LoadConnectConfig( root );
			if( !ret )
			{
				return false;
			}

			ret = _LoadMysqlConfig( root );
			if( !ret )
			{
				return false;
			}

			return (_LoadLogConfig( root ) && LoadOtherConfig( root ));
		}

		return true;
	}

	bool _AddConnectItem(s32 id, const char *ip, s32 port, const char *isDefault)
	{
		TiXmlDocument doc( m_ConfigFile.c_str() );
		if( doc.LoadFile() )
		{
			TiXmlElement *root = doc.RootElement();
			if( root == NULL ) 
			{
				return false;
			}

			bool flag = false;
			TiXmlElement *elem = _FindChildElement( root, "Connects" );
			TiXmlElement connectsNode( "Connects" );
			if( elem == NULL )
			{
				flag = true;
				elem = &connectsNode;
				elem->SetAttribute( "CenterServerId", "0" );
				elem->SetAttribute( "GameManagerId", "0" );
				elem->SetAttribute( "GroupManagerId", "0" );
			}

			TiXmlElement connectNode( "Connect" );
			connectNode.SetAttribute( "Id", id );
			connectNode.SetAttribute( "Name", "" );
			connectNode.SetAttribute( "Ip", ip );
			connectNode.SetAttribute( "Port", port );
			connectNode.SetAttribute( "IsDefault", isDefault );
			elem->InsertEndChild( connectNode );

			if( flag )
			{
				root->InsertEndChild( *elem );
			}

			doc.SaveFile( m_ConfigFile.c_str() );

			ConnectConfig cfg;
			cfg.m_Id = id;
			cfg.m_strSvrName = "";
			cfg.m_strIp = ip;
			cfg.m_nPort = port;
			cfg.m_bDefault = (strcmp(isDefault, "true") == 0) ? true : false;
			m_Connects.push_back( cfg );

			return true;
		}

		return false;
	}

protected:
	std::string m_ConfigFile;
	ServerConfig m_ServerConfig;
	s32 m_ConnectCount;
	std::vector<ConnectConfig> m_Connects;
	s32 m_defaultIdx;
	LogConfig	m_LogConfig;
	s32 m_nCenterServerId;
	s32 m_nGameManagerServerId;
	s32 m_nGroupManagerServerId;

	std::vector<MysqlConfig> m_mysqlCfgs;
};

#endif
