#ifndef _MSG_GROUP_H_
#define _MSG_GROUP_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "GameList.h"
#include "GroupClient.h"
#include "GameManager.h"
#include "GroupMainScene.h"

class CMsgGroupConnect : public CMsgBase
{
public:
	CMsgGroupConnect(void *client) : m_pClient(client)
	{}

	virtual void Proc()
	{
		printf( "CMsgGroupConnect::Proc\n" );
		CNetManager::Instance().SetGroupClient( (CGroupClient*)m_pClient );
		
		CNetManager::Instance().SetState( CNetManager::E_State_Connected_GroupServer );
		CNetManager::Instance().LoginGroup( 
			CGameManager::Instance().GetUser() );
	}

private:
	void *m_pClient;
};

class CMsgGroupConnectError : public CMsgBase
{
public:
	virtual void Proc()
	{
	}
};

class CMsgLoginGroupRsp : public CMsgBase
{
public:
	CMsgLoginGroupRsp(int result) : 
		m_ret(result)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgLoginGroupRsp:%d", m_ret );
		if( m_ret == sglib::errorcode::E_ErrorCode_Success )
		{
			cocos2d::CCDirector::sharedDirector()->replaceScene( CGroupMainScene::scene() );
		}
	}

private:
	int m_ret;
};

class CMsgGroupMessageNtf : public CMsgBase
{
public:
	CMsgGroupMessageNtf(const std::string &sender, const std::string &content, u64 id) : 
		m_Sender(sender),
		m_Content(content),
		m_GroupId( id )
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgGroupMessageNtf:%s:%s:%llu", m_Sender.c_str(), m_Content.c_str(), m_GroupId);
	}

private:
	std::string m_Sender;
	std::string m_Content;
	u64 m_GroupId;
};

class CMsgGroupInfoUpdate : public CMsgBase
{
public:
	CMsgGroupInfoUpdate(u64 id, std::vector<std::string> &members)
	{
		m_GroupId = id;
		m_Members = members;
	}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgGroupInfoUpdate " );
		int count = m_Members.size();
		for( int i=0; i<count; ++i )
		{
			CGameManager::Instance().AddGroupMember( m_GroupId, m_Members[i] );
		}
	}

private:
	u64 m_GroupId;
	std::vector<std::string> m_Members;
};

class CMsgGroupListUpdate : public CMsgBase
{
public:
	CMsgGroupListUpdate(std::vector<u64> &vecId, std::vector<std::string> &vecName)
	{
		m_GroupIds = vecId;
		m_Names = vecName;
	}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgGroupInfoUpdate " );

		int count = m_GroupIds.size();
		for( int i=0; i<count; ++i )
		{
			CGameManager::Instance().AddGroup( m_GroupIds[i], m_Names[i] );
		}
	}

private:
	std::vector<u64> m_GroupIds;
	std::vector<std::string> m_Names;
};

class CMsgGroupCreateGameRoomResult : public CMsgBase
{
public:
	CMsgGroupCreateGameRoomResult(s32 result, u64 groupid, s32 game, s32 roomid, const std::string &ip, s32 port)
	{
		m_result = result;
		m_groupid = groupid;
		m_game = game;
		m_roomid = roomid;
		m_ip = ip;
		m_port = port;
	}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgGroupCreateGameRoomResult" );

	}

private:
	s32 m_result;
	u64 m_groupid;
	s32 m_game;
	s32 m_roomid;
	std::string m_ip;
	s32 m_port;
};

class CMsgGroupCreateGameRoomNtf : public CMsgBase
{
public:
	CMsgGroupCreateGameRoomNtf(const std::string &creater, u64 groupid, s32 game, s32 roomid, const std::string &ip, s32 port)
	{
		m_creater = creater;
		m_groupid = groupid;
		m_game = game;
		m_roomid = roomid;
		m_ip = ip;
		m_port = port;
	}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgGroupCreateGameRoomNtf" );

		CGameManager::Instance().AddGroupGameRoom(
			m_groupid, m_game, m_roomid, m_ip, m_port, m_creater );
		CGameManager::Instance().CreateGroupGameRoom();
	}

private:
	std::string m_creater;
	u64 m_groupid;
	s32 m_game;
	s32 m_roomid;
	std::string m_ip;
	s32 m_port;
};

#endif
