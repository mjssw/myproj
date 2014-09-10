#include "GameRoomInfo.h"
#include "user/UserManager.h"
using namespace std;

CGameRoomList::CGameRoomList()
{
}

CGameRoomList::~CGameRoomList()
{
	map<s32, map<s32, CGameRoomInfo*> >::iterator it = m_gameRooms.begin();
	for( ; it != m_gameRooms.end(); ++it )
	{
		map<s32, CGameRoomInfo*>::iterator _it = it->second.begin();
		for( ; _it != it->second.end(); ++_it )
		{
			delete _it->second;
		}
	}
	m_gameRooms.clear();
}

bool CGameRoomList::AddGameRoomInfo(s32 gameid, u64 groupid, const string &creater, const string &ip, s32 port, s32 roomid, const string &roompwd)
{
	CGameRoomInfo *room = new CGameRoomInfo(
		gameid, groupid, creater, ip, port, roomid, roompwd );
	if( room )
	{
		m_gameRooms[ gameid ][ roomid ] = room;
		return true;
	}
	return false;
}

CGameRoomInfo* CGameRoomList::GetGameRoomInfo(s32 gameid, s32 roomid)
{
	map<s32, map<s32, CGameRoomInfo*> >::iterator it = m_gameRooms.find( gameid );
	if( it != m_gameRooms.end() )
	{
		map<s32, CGameRoomInfo*>::iterator _it = it->second.find( roomid );
		if( _it != it->second.end() )
		{
			return _it->second;
		}
	}
	return NULL;
}

void CGameRoomList::DelGameRoomInfo(s32 gameid, s32 roomid)
{
	map<s32, map<s32, CGameRoomInfo*> >::iterator it = m_gameRooms.find( gameid );
	if( it != m_gameRooms.end() )
	{
		map<s32, CGameRoomInfo*>::iterator _it = it->second.find( roomid );
		if( _it != it->second.end() )
		{
			delete _it->second;
			it->second.erase( _it );
		}
	}
}

const std::map<s32, std::map<s32, CGameRoomInfo*> >& CGameRoomList::GetRooms()
{
	return m_gameRooms;
}

/*
void DumpRoomList(u64 groupid, CGameRoomList &roomList, std::vector<TableViewData> &vecData)
{
	vecData.clear();
	const map<s32, map<s32, CGameRoomInfo*> > &rooms = roomList.GetRooms();
	map<s32, map<s32, CGameRoomInfo*> >::const_iterator it = rooms.begin();
	for( ; it != rooms.end(); ++it )
	{
		map<s32, CGameRoomInfo*>::const_iterator _it = it->second.begin();
		for( ; _it != it->second.end(); ++_it )
		{
			if( _it->second->m_groupid != groupid )
			{
				continue;
			}

			TableViewData data;

			string dir,name,icon;
			CUserManager::Instance().GetGameInfo( _it->second->m_gameid, dir, icon, name );
			data.icon = dir + "/" + icon;
			char strroom[64] = {0};
			sprintf( strroom, "room(%d)", _it->second->m_roomid );
			data.text = a2u(strroom);
			data.data = _it->second;
			vecData.push_back( data ); 
		}
	}
}
//*/
