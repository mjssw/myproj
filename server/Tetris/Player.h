#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "PlayerBase.h"
#include "GameScene.h"
#include <map>

class CPlayer;
typedef void (CPlayer::*PlayerMsgProcFunc)(s32 msgid, const byte *pkg, s32 len);

class CPlayer : public CPlayerBase
{
public:
	CPlayer();
	CPlayer(u64 gateId, u64 clientId, const std::string &userId);
	~CPlayer();
	
	virtual CPlayerBase* Clone(u64 gateId, u64 clientId, const std::string &userId);
	virtual void OnEnterHall();
	virtual void OnEnterRoom();
	virtual void OnLeaveRoom();
	virtual void ProcMessage(s32 msgid, const byte *pkg, s32 len);

private:
	void _Register(s32 msgid, PlayerMsgProcFunc func);
	void _ReadyProc(s32 msgid, const byte *pkg, s32 len);
	void _GameLostProc(s32 msgid, const byte *pkg, s32 len);
	void _PutBlockProc(s32 msgid, const byte *pkg, s32 len);
	void _ClearBlockProc(s32 msgid, const byte *pkg, s32 len);

	void _Ready();
	void _Lost();
	void _PutBlock(s32 blockid, s32 column);
	void _ClearBlock(const s32 *rows, s32 count);

public:
	s32 Ready();
	bool IsReady();
	void GameStart();
	void GameEnd();
	bool PutBlock(s32 blockid, s32 column);
	void ClearBlock(const s32 *rows, s32 count);
	s32 CurState();

private:
	CGameScene m_GameScene;	
	std::map<s32, PlayerMsgProcFunc> m_mapMsgProc;
};

#endif
