#ifndef _SG_GLOBAL_CONFIG_H_
#define _SG_GLOBAL_CONFIG_H_

#include "Define.h"
#include <tinyxml.h>

namespace SGLib
{
	enum
	{
		MAX_FILE_PATH_LEN = 1024,
		MAX_NAME_LEN = 64,
		MAX_IP_LEN = 128,
	};

	struct ServerConfig
	{
		s32  m_Id;
		char m_DisplayName[MAX_NAME_LEN];

		char m_Ip[MAX_IP_LEN];
		s32  m_Port;
		
		char m_ConfigFile[MAX_FILE_PATH_LEN];
	};

	struct GlobalConfig
	{
		GlobalConfig():m_nServerCount(0), m_pServerConfig(NULL){}

		s32  m_SiteId;
		char m_DisplayName[MAX_NAME_LEN];

		s32 m_nServerCount;
		ServerConfig *m_pServerConfig; 
	};

	class CGlobalConfigure
	{
	public:
		CGlobalConfigure(const char *global_file) : m_bInit(false)
		{
			memset( m_Cfgfile, 0, MAX_FILE_PATH_LEN );
			strncpy( m_Cfgfile, global_file, MAX_FILE_PATH_LEN );
		}
		~CGlobalConfigure(){}
	
		bool Init()
		{
			return _LoadNetConfig();
		}

		const ServerConfig* GetServerConfig(s32 svr_id) const
		{
			return _GetServerConfig( svr_id );
		}

        const s32 GetServerId(const char *ip, s32 port) const
        {
            return _GetServerId( ip, port );
        }
	
	private:
		bool m_bInit;
		char m_Cfgfile[MAX_FILE_PATH_LEN];
		GlobalConfig m_GlobalCfg;

		const ServerConfig* _GetServerConfig(s32 svr_id) const
		{
			for(int i=0; i<m_GlobalCfg.m_nServerCount; ++i)
			{
				if( m_GlobalCfg.m_pServerConfig[i].m_Id == svr_id )
				{
					return &m_GlobalCfg.m_pServerConfig[i];
				}
			}

			return NULL;
		}

        const s32 _GetServerId(const char *ip, s32 port) const
		{
            s32 id = INVALID_VAL;
			for(int i=0; i<m_GlobalCfg.m_nServerCount; ++i)
			{
				if( m_GlobalCfg.m_pServerConfig[i].m_Port == port &&
                    strcmp(m_GlobalCfg.m_pServerConfig[i].m_Ip, ip) == 0 )
				{
					id = m_GlobalCfg.m_pServerConfig[i].m_Id;
				}
			}

			return id;
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

		int _GetServerCount(TiXmlElement *elem)
		{
			int count = 0;
			TiXmlElement *e = elem->FirstChildElement();
			while( e != NULL && e->Value() != NULL )
			{
				if( strcmp(e->Value(), "Server") == 0 )
				{
					++count;
				}

				e = e->NextSiblingElement();
			}

			return count;
		}

		bool _LoadNetConfig()
		{
			TiXmlDocument doc( m_Cfgfile );
			if( doc.LoadFile() )
			{
				TiXmlElement *root = doc.RootElement();
				if( root == NULL ) 
				{
					return false;
				}

				TiXmlElement *elem = _FindChildElement(root, "Site");
				if( elem != NULL )
				{
					// 找到一条记录
					const char *val = elem->Attribute( "Id" );
					if( val != NULL )
					{
						m_GlobalCfg.m_SiteId = atoi( val );
					}

					val = elem->Attribute( "DisplayName" );
					if( val != NULL )
					{
						strncpy( m_GlobalCfg.m_DisplayName, val, MAX_NAME_LEN );
					}

					elem = _FindChildElement(elem, "Servers");
					if( elem != NULL )
					{
						int serverCount = _GetServerCount( elem );
						m_GlobalCfg.m_nServerCount = serverCount;
						m_GlobalCfg.m_pServerConfig = new ServerConfig[serverCount];
						if( m_GlobalCfg.m_pServerConfig == NULL )
						{
							return false;
						}

						int idx = 0;
						TiXmlElement *e = elem->FirstChildElement();
						while( e != NULL && e->Value() != NULL )
						{
							if( strcmp(e->Value(), "Server") == 0 )
							{
								// 找到一条记录

								const char *val = e->Attribute( "Id" );
								if( val != NULL )
								{
									m_GlobalCfg.m_pServerConfig[idx].m_Id = atoi( val );
								}

								val = e->Attribute( "DisplayName" );
								if( val != NULL )
								{
									strncpy( m_GlobalCfg.m_pServerConfig[idx].m_DisplayName, val, MAX_NAME_LEN );
								}

								val = e->Attribute( "IP" );
								if( val != NULL )
								{
									strncpy( m_GlobalCfg.m_pServerConfig[idx].m_Ip, val, MAX_IP_LEN );
								}

								val = e->Attribute( "Port" );
								if( val != NULL )
								{
									m_GlobalCfg.m_pServerConfig[idx].m_Port = atoi(val);
								}

								val = e->Attribute( "Config" );
								if( val != NULL )
								{
									strncpy(
										m_GlobalCfg.m_pServerConfig[idx].m_ConfigFile,
										val,
										MAX_FILE_PATH_LEN );
								}

								++idx;
							}
							e = e->NextSiblingElement();
						}

						return true;
					}
				}
			}

			return false;
		}
	};

};

#endif
