#ifndef _ROOM_H_
#define _ROOM_H_

#include "RoomBase.h"

class CRoom : public CRoomBase
{
public:
	CRoom(int id) : CRoomBase(id)
	{}

	void Enter();
	void EnemyEnter();
};

#endif
