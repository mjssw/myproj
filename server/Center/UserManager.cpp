#include "UserManager.h"
#include "User.h"
#include "CenterClient.h"
#include "GameManager.h"
#include "ServerManager.h"
#include "StackTrace.h"
#include "center.pb.h"
#include "msgid.pb.h"
#include "errno.pb.h"
using namespace std;

SIGNLETON_CLASS_INIT(CUserManager);

void CUserManager::AddUser(const std::string &user, const std::string &flag)
{
	map<string, CUser*>::iterator it = m_mapUsers.find( user );
	if( it != m_mapUsers.end() )
	{
		SERVER_LOG_ERROR( "CUserManager,AddUser," << user.c_str() << "AleardyAdd," << flag.c_str() \
			<< "," << it->second->GetFlag().c_str() );
		return;
	}

	CUser *puser = new CUser( user );
	SELF_ASSERT( puser, return; );
	puser->SetFlag( flag );
	m_mapUsers[ user ] = puser;

	SERVER_LOG_INFO( "CUserManager,AddUser," << user.c_str() << "," << flag.c_str() );
}

void CUserManager::DelUser(const std::string &user)
{
	map<string, CUser*>::iterator it = m_mapUsers.find( user );
	if( it == m_mapUsers.end() )
	{
		SERVER_LOG_ERROR( "CUserManager,DelUser," << user.c_str() << "NotFound" );
		return;
	}

	SAFE_DELETE( it->second );
	m_mapUsers.erase( it );
	
	SERVER_LOG_INFO( "CUserManager,DelUser," << user.c_str() );
}

void CUserManager::NotifyGameListToUser(CCenterClient &client, u64 gateid, u64 clientid)
{
	vector<s32> vecGameList;
	CGameManager::Instance().GetGameList( vecGameList );
	sglib::centerproto::CenterLoginGameInfoNotify ntf;
	ntf.set_clientid( clientid );
	ntf.set_gateid( gateid );
	for( vector<s32>::iterator it=vecGameList.begin(); it!=vecGameList.end(); ++it )
	{
		const CGameInfo *pInfo = CGameManager::Instance().GetGameInfo( *it );
		if( pInfo != NULL )
		{
			sglib::publicproto::GameInfo *info = ntf.add_games();
			if( info != NULL )
			{
				info->set_gameid( pInfo->GameId() );
				info->set_cur_count( pInfo->GetPlayerCount() );
			}
		}
	}
	client.SendMsg( ntf, sglib::msgid::CTL_GAME_INFO_NOTIFY );
}

void CUserManager::UserAskEnterGameInfo(CCenterClient &client, u64 gateid, u64 clientid, s32 gameid, u64 gsid)
{
	sglib::centerproto::CenterServerEnterGameRsp rsp;
	s32 result = sglib::errorcode::E_ErrorCode_Success; 
	rsp.set_clientid( clientid );
	rsp.set_gateid( gateid );
	rsp.set_gameid( gameid );
	rsp.set_gsid( gsid );

	const CGameInfo *pGame = CGameManager::Instance().GetGameInfo( gameid );
	if( pGame != NULL )
	{
		std::string gateIp = "";
		s32 gatePort = INVALID_VAL;
		bool ret = pGame->GetOptimalGateInfo( gateIp, gatePort );
		if( ret )
		{
			rsp.set_ip( gateIp );
			rsp.set_port( gatePort );
		}
		else
		{
			result = sglib::errorcode::E_ErrorCode_GameUserFull;
		}
	}
	else
	{
		result = sglib::errorcode::E_ErrorCode_NoSuchGame;
	}
	rsp.set_result( result );

	client.SendMsg( rsp, sglib::msgid::CTS_ENTER_GAME_RSP );
}
