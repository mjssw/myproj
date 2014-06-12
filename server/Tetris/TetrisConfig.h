#ifndef _TETRIS_CONFIG_H_
#define _TETRIS_CONFIG_H_

#include "ServerConfig.h"
#include "HallConfig.h"

class CTetrisConfig : public CServerConfig
{
public:
	CTetrisConfig(const char *cfgFile) : 
		CServerConfig(cfgFile),
		m_HallCfg()
	{}

	~CTetrisConfig(){}

	virtual bool LoadOtherConfig(TiXmlElement *root)
	{ 
		if( root == NULL )
		{
			return false;
		}

		return m_HallCfg.LoadHallConfig( root );
	}

	const CHallConfig& GetHallConfig()
	{
		return m_HallCfg;
	}

private:
	CHallConfig m_HallCfg;
};

#endif
