#ifndef _ROOM_H_
#define _ROOM_H_

#include "RoomBase.h"

template<class StateOwner>
class CRoomStateMachine;


class CPlayer;
class CRoom : public CRoomBase
{
public:
	CRoom(s32 id);
	~CRoom();

	virtual void OnInit();
	virtual CRoomBase* Clone(s32 id);

	void PlayerReady(CPlayer *player);
	void PlayerLost(CPlayer *player);
	void PlayerPutBlock(CPlayer *player, s32 blockid, s32 column);
	void PlayerClearBlock(CPlayer *player, const s32 *rows, s32 count);
	void PlayerForceLeave(CPlayer *player);
	void NotifyRoomInfo(CPlayer *player);

private:
	void _CheckGameStart();
	void _PlayerReadyResponse(CPlayer *player, s32 result);
	void _GameStart();
	void _NotifyGameStart();
	void _GameEnd(CPlayer *loser);
	void _NotifyGameEnd(const std::string &loser);
	void _PutBlock(CPlayer *player, s32 blockid, s32 column);
	void _ClearBlock(CPlayer *player, const s32 *rows, s32 count);
	void _NotifyPutBlock(CPlayer *player, s32 blockid, s32 column);
	void _NotifyClearBlock(CPlayer *player, const s32 *rows, s32 count);
	void _NotifyPlayerReady(CPlayer *player);
};

#endif
