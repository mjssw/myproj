#ifndef _GROUP_CONFIG_H_
#define _GROUP_CONFIG_H_

#include "ServerConfig.h"

class CGroupConfig : public CServerConfig
{
public:
	CGroupConfig(const char *cfgFile) : 
		CServerConfig(cfgFile),
		m_DynamicStart(0)
	{}

	~CGroupConfig(){}

	virtual bool LoadOtherConfig(TiXmlElement *root)
	{ 
		if( root == NULL )
		{
			return false;
		}

		TiXmlElement *elem = _FindChildElement( root, "Param" );
		if( elem == NULL )
		{
			return false;
		}

		const char *val = elem->Attribute( "IsDynamicStart" );
		if( val != NULL )
		{
			m_DynamicStart = atoi( val );
		}

		return true;
	}

	bool IsDynamicStart() const
	{
		return (m_DynamicStart == 1);
	}

private:
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

private:
	s32 m_DynamicStart;
};

#endif
