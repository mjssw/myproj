#ifndef _HALL_CONFIG_H_
#define _HALL_CONFIG_H_

#include "Define.h"
#include <string>
#include "Utils.h"
#include "Config.h"
#include <tinyxml.h>

class CHallConfig
{
public:
	CHallConfig() : 
		m_Id(INVALID_VAL),
		m_Name(""),
		m_RoomCount(0),
		m_RoomIdFrom(INVALID_VAL),
		m_RoomPlayerCount(0),
		m_IsDynamicStart(0)
	{}

	bool LoadHallConfig(TiXmlElement *root)
	{ 
		if( root == NULL )
		{
			return false;
		}

		TiXmlElement *elem = _FindChildElement( root, "HallCfg" );
		if( elem == NULL )
		{
			return false;
		}

		const char *val = elem->Attribute( "Id" );
		if( val != NULL )
		{
			m_Id = atoi( val );
		}

		val = elem->Attribute( "Name" );
		if( val != NULL )
		{
			m_Name = CUtils::utf82s( val );
		}

		val = elem->Attribute( "RoomIdFrom" );
		if( val != NULL )
		{
			m_RoomIdFrom = atoi( val );
		}

		val = elem->Attribute( "RoomCount" );
		if( val != NULL )
		{
			m_RoomCount = atoi( val );
		}

		val = elem->Attribute( "RoomPlayerCount" );
		if( val != NULL )
		{
			m_RoomPlayerCount = atoi( val );
		}

		val = elem->Attribute( "IsDynamicStart" );
		if( val != NULL )
		{
			m_IsDynamicStart = atoi( val );
		}

		return true; 
	}

	s32 HallId() const
	{
		return m_Id;
	}

	const std::string& HallName() const
	{
		return m_Name;
	}

	s32 RoomCount() const
	{
		return m_RoomCount;
	}

	s32 RoomIdFrom() const
	{
		return m_RoomIdFrom;
	}

	s32 RoomPlayerCount() const
	{
		return m_RoomPlayerCount;
	}

	bool IsDynamicStart() const
	{
		return (m_IsDynamicStart == 1);
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
	s32 m_Id;
	std::string m_Name;
	s32 m_RoomCount;
	s32 m_RoomIdFrom;
	s32 m_RoomPlayerCount;
	// 是否是动态启动游戏服 1:是 0:否
	s32 m_IsDynamicStart;
};

#endif
