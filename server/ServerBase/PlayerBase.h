#ifndef _PLAYER_BASE_H_
#define _PLAYER_BASE_H_

#include "Define.h"
#include <string>
#include "google/protobuf/message.h"
#include "StateBase.h"

class CRoomBase; 
class CPlayerBase
{
public:
	CPlayerBase() : m_pStateMachine(NULL) {}
	CPlayerBase(u64 gateId, u64 clientId, const std::string &userId);
	virtual ~CPlayerBase();
	virtual CPlayerBase* Clone(u64 gateId, u64 clientId, const std::string &userId){ return NULL; }
	void EnterRoom(CRoomBase *pRoom);
	void LeaveRoom();
	virtual void OnEnterHall(){}
	virtual void OnEnterRoom(){}
	virtual void OnLeaveRoom(){}
	virtual void ProcMessage(s32 msgid, const byte *pkg, s32 len){}

	u64 GateId() const;
	u64 ClientId() const;
	const std::string& UserId() const;
	CRoomBase* GetRoom();
	void ChangeState(s32 stateId);
	void Update(s32 msgid, const byte *pkg, s32 len);

	void SendMsg(const ::google::protobuf::Message &msg, s32 msgId);

protected:
	u64 m_GateId;
	u64 m_ClientId;
	std::string m_UserId;
	CRoomBase *m_pRoom;

	CStateMachineBase<CPlayerBase> *m_pStateMachine;
};

#endif
