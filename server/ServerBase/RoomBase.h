#ifndef _ROOM_BASE_H_
#define _ROOM_BASE_H_

#include "Define.h"
#include "StateBase.h"
#include "google/protobuf/message.h"

class CPlayerBase;
class CRoomBase
{
public:
	friend class CHall;

	CRoomBase(s32 id);
	virtual ~CRoomBase();

	virtual void OnInit(){}
	virtual CRoomBase* Clone(s32 id){ return NULL; }
	s32 PlayerEnter(CPlayerBase *player);
	s32 PlayerLeave(CPlayerBase *player);
	void ChangeState(s32 stateid);
	s32 CurState();

	s32 Id();
	void Broadcast(const ::google::protobuf::Message &msg, s32 msgId, CPlayerBase *exclude=NULL);
	bool PlayerInRoom(CPlayerBase *player);

private:
	void _Init();
	void _PlayerEnter(CPlayerBase *player);
	void _NotifyPlayerEnter(const std::string &userId);
	void _PlayerLeave(CPlayerBase *player);
	void _NotifyPlayerLeave(const std::string &userId);
	void _NotifyEnterRoomSuccess(CPlayerBase *player, s32 roomid);

protected:
	s32 m_Id;
	s32 m_PlayerCount;
	CPlayerBase **m_Players;
	CStateMachineBase<CRoomBase> *m_pStateMachine;
};

#endif
