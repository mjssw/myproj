#ifndef _GAME_ROOM_INFO_H_
#define _GAME_ROOM_INFO_H_

#include "Define.h"
#include <map>
#include <vector>
#include <string>

class CGameRoomInfo
{
public:
	CGameRoomInfo(s32 gameid, u64 groupid, const std::string &creater, const std::string &ip, s32 port, s32 roomid, const std::string &roompwd) : 
		m_gameid( gameid ),
		m_groupid( groupid ),
		m_creater( creater ),
		m_ip( ip ),
		m_port( port ),
		m_roomid( roomid ),
		m_roompwd( roompwd )
	{}

	s32 m_gameid;
	u64 m_groupid;
	std::string m_creater;
	std::string m_ip;
	s32 m_port;
	s32 m_roomid;
	std::string m_roompwd;
};

class CGameRoomList
{
public:
	CGameRoomList();
	~CGameRoomList();

	bool AddGameRoomInfo(s32 gameid, u64 groupid, const std::string &creater, const std::string &ip, s32 port, s32 roomid, const std::string &roompwd);
	CGameRoomInfo* GetGameRoomInfo(s32 gameid, s32 roomid);
	void DelGameRoomInfo(s32 gameid, s32 roomid);

	const std::map<s32, std::map<s32, CGameRoomInfo*> >& GetRooms();
private:
	std::map<s32, std::map<s32, CGameRoomInfo*> > m_gameRooms;
};

#endif
