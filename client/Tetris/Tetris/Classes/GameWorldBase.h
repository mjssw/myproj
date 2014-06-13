#ifndef _GAME_WORLD_BASE_H_
#define _GAME_WORLD_BASE_H_

#include <string>

class CGameWorldBase
{
public:
	virtual ~CGameWorldBase(){}
	virtual void ConnectError(){}
	virtual void EnterRoomFailed(){}
	virtual void EnterRoom(const std::string& user){}
	virtual void LeaveRoom(const std::string& user){}
	virtual void DisplayGameWorld(){}
	virtual void PlayerReady(){}
	virtual void EnemyReady(){}
	virtual void RoomInfo(const std::string &enemy, bool isReady){}
	virtual void GameEnd(bool bWin){}
	virtual void EnemyClearRows(std::vector<int> &clearRows){}
	virtual void CreateGroupGameRoom(){}
};

#endif
