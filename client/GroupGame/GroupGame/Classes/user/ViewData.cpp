#include "user/GameRoomInfo.h"
#include "ViewData.h"
using namespace std;

CUserViewData::CUserViewData() : 
	m_isSavePwd( false ),
	m_isAutoLogin( false ),
	m_selectGroup( 0 ),
	m_curGameRoom( NULL )
{
}

CUserViewData::~CUserViewData()
{
}

void CUserViewData::LoadFromConfig()
{
	// TODO 
	SetSavePwd( false );
	SetAutoLogin( false );
}

void CUserViewData::SetSavePwd(bool isSavePwd)
{
	m_isSavePwd = isSavePwd;
}

bool CUserViewData::GetSavePwd()
{
	return m_isSavePwd;
}

void CUserViewData::SetAutoLogin(bool isAutoLogin)
{
	m_isAutoLogin = isAutoLogin;
}

bool CUserViewData::GetAutoLogin()
{
	return m_isAutoLogin;
}

void CUserViewData::SetSelectGroup(CGroup *group)
{
	m_selectGroup = group;
}

CGroup* CUserViewData::GetSelectGroup()
{
	return m_selectGroup;
}

void CUserViewData::SetNewGroupName(const string &name)
{
	m_newGroupName = name;	
}

const string& CUserViewData::GetNewGroupName()
{
	return m_newGroupName;
}

void CUserViewData::SetNewGroupDone(bool done)
{
	m_isNewGroupDone = done;
}

bool CUserViewData::IsNewGroupDone()
{
	return m_isNewGroupDone;
}

void CUserViewData::SetInviteUser(const string &user)
{
	m_inviteUser = user;
}

const string& CUserViewData::GetInviteUser()
{
	return m_inviteUser;
}

void CUserViewData::SetJoinGroupInfo(u64 id, const string &name)
{
	m_joinGroupId = id;
	m_joinGroupName = name;
}

u64 CUserViewData::GetJoinGroupId()
{
	return m_joinGroupId;
}

const string& CUserViewData::GetJoinGroupName()
{
	return m_joinGroupName;
}

void CUserViewData::SetChatText(const string &text)
{
	m_chatText = text;
}

const string& CUserViewData::GetChatText()
{
	return m_chatText;
}


void CUserViewData::PushChatMsg(u64 groupid, const std::string &sender, const std::string &content)
{
	m_chatmsg.push_back( Chatmsg(groupid, sender, content) );
}

Chatmsg* CUserViewData::GetTopChatMsg()
{
	if( m_chatmsg.empty() )
	{
		return NULL;
	}
	return &m_chatmsg.front();
}

void CUserViewData::PopChatMsg()
{
	m_chatmsg.pop_front();
}

void CUserViewData::SetCurGameRoom(CGameRoomInfo *room)
{
	m_curGameRoom = room;
}

CGameRoomInfo* CUserViewData::GetCurGameRoom()
{
	return m_curGameRoom;
}
