#include "PlayerBase.h"
#include "StackTrace.h"
#include "ServerManager.h"
#include "GateClientBase.h"
#include "RoomBase.h"
#include "RoomStateBase.h"
#include "PlayerStateBase.h"
#include "PlayerStateManager.h"

CPlayerBase::CPlayerBase(u64 gateId, u64 clientId, const std::string &userId) : 
	m_GateId(gateId),
	m_ClientId(clientId),
	m_UserId(userId),
	m_pRoom(NULL),
	m_pStateMachine(NULL)
{
}
CPlayerBase::~CPlayerBase()
{
	SAFE_DELETE( m_pStateMachine );
}

u64 CPlayerBase::GateId() const
{
	return m_GateId;
}

u64 CPlayerBase::ClientId() const
{
	return m_ClientId;
}

const std::string& CPlayerBase::UserId() const
{
	return m_UserId;
}

void CPlayerBase::EnterRoom(CRoomBase *pRoom)
{
	if( pRoom )
	{
		SG_ASSERT( m_pRoom == NULL );
		m_pRoom = pRoom;
		ChangeState( E_PlayerState_InRoom_Free );	
		OnEnterRoom();
	
		SERVER_LOG_INFO( "CPlayerBase,EnterRoom," << pRoom->Id() << "," << ClientId() << "," << UserId().c_str() );
	}
}

void CPlayerBase::LeaveRoom()
{
	SERVER_LOG_INFO( "CPlayerBase,LeaveRoom," << m_pRoom->Id() << "," << ClientId() << "," << UserId().c_str() );

	m_pRoom = NULL;
	ChangeState( E_PlayerState_Free );	
	OnLeaveRoom();
}

CRoomBase* CPlayerBase::GetRoom()
{
	return m_pRoom;
}

void CPlayerBase::ChangeState(s32 stateId)
{
	CStateBase<CPlayerBase> *state = CPlayerStateManager::Instance().FindState( stateId );
	if( state )
	{
		SG_ASSERT( m_pStateMachine );
		m_pStateMachine->ChangeState( state, this );
	}
	else
	{
		SERVER_LOG_ERROR( "CPlayerBase,ChangeState," << stateId << "," << ClientId() << "," << UserId().c_str() );
	}
}

void CPlayerBase::Update(s32 msgid, const byte *pkg, s32 len)
{
	SG_ASSERT( m_pStateMachine );
	m_pStateMachine->Update( this, msgid, pkg, len );
}

void CPlayerBase::SendMsg(const ::google::protobuf::Message &msg, s32 msgId)
{
	CClient *pClient = CServerManager::Instance().FindClient( m_GateId );
	SG_ASSERT( pClient != NULL );

	CGateClientBase *pGateClient = (CGateClientBase*)pClient;
	pGateClient->SendMsgToClient( m_ClientId, msg, msgId );
}
