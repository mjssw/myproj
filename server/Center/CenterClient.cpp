#include "CenterClient.h"
#include "ServerManager.h"
#include "GameManager.h"
#include "GroupInfoManager.h"
#include "UserManager.h"
#include <map>
using namespace std;

#include "msgid.pb.h"
#include "errno.pb.h"
#include "center.pb.h"
#include "login.pb.h"
#include "gamemanager.pb.h"
#include "group.pb.h"
#include "public.pb.h"

CCenterClient::CCenterClient(int nId) : CClient(nId)
{
	_RegisterProc( sglib::msgid::LCT_USER_LOGIN_REQ,		&CCenterClient::_UserLogin );
	_RegisterProc( sglib::msgid::GMCT_REPORT_GAME_INFO,		&CCenterClient::_GameInfoReport );
	_RegisterProc( sglib::msgid::SCT_USER_LOGOUT_NOTIFY,	&CCenterClient::_UserLogout );
	_RegisterProc( sglib::msgid::SCT_ENTER_GAME_REQ,		&CCenterClient::_UserAskEnterGame );
	_RegisterProc( sglib::msgid::GMCT_GATE_CLOSE_NTF,		&CCenterClient::_GameGateClose );
	_RegisterProc( sglib::msgid::GMCT_GAME_SERVER_CLOSE_NTF,&CCenterClient::_GameServerClose );
	_RegisterProc( sglib::msgid::GMCT_GP_GATE_CLOSE_NTF,	&CCenterClient::_GroupGateClose );
	_RegisterProc( sglib::msgid::GMCT_GP_SERVER_CLOSE_NTF,	&CCenterClient::_GroupServerClose );
	_RegisterProc( sglib::msgid::GMCT_GP_GROUP_GATE_INFO,	&CCenterClient::_GroupGateInfoReport );
}

void CCenterClient::OnRecv(const byte *pPkg, s32 nPkgLen)
{
	s32 msgId = *((s32*)pPkg);

	//SERVER_LOG_DEBUG( "client:" << GetClientId() << " recv rpc pkg, len=" << nPkgLen << " ,msgId=" << msgId );

	map<int, ProtoProc>::iterator it = m_mapProtoProc.find( msgId );
	if( it != m_mapProtoProc.end() )
	{
		(this->*it->second)( pPkg+MSG_ID_LEN, nPkgLen-MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CCenterClient,OnRecv," << GetClientId() << "," << msgId );
	}
}

void CCenterClient::_RegisterProc(int id, ProtoProc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}

void CCenterClient::SendMsg(const ::google::protobuf::Message &Msg, s32 nMsgId)
{
	int count = Msg.ByteSize();
	byte buf[MAX_MSG_LEN] = {0};
	*((s32*)buf) = nMsgId;
	if( Msg.SerializeToArray(buf+MSG_ID_LEN, MAX_MSG_LEN) )
	{
		bool ret = Send( buf, count + MSG_ID_LEN );
		if( !ret )
		{
			SERVER_LOG_ERROR( "CCenterClient,SendMsg," << nMsgId << "Send" );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CCenterClient,SendMsg," << nMsgId << ",SerializeToArray" );
	}
}

void CCenterClient::_UserLogin(const byte *pPkg, s32 nPkgLen)
{
	sglib::loginproto::LoginCenterUserLoginReq req;
	if( req.ParseFromArray(pPkg, nPkgLen) )
	{
		CUserManager::Instance().AddUser( req.user(), req.flag() );
		CUserManager::Instance().NotifyGameListToUser( *this, req.gateid(), req.clientid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CCenterClient,_UserLogin,ParseFromArray" );
	}
}

void CCenterClient::_UserLogout(const byte *pPkg, s32 nPkgLen)
{
	sglib::centerproto::ServerCenterUserLogoutNotify ntf;
	if( ntf.ParseFromArray(pPkg, nPkgLen) )
	{
		CUserManager::Instance().DelUser( ntf.user() );
	}
	else
	{
		SERVER_LOG_ERROR( "CCenterClient,_UserLogout,ParseFromArray" );
	}
}

void CCenterClient::_GameInfoReport(const byte *pPkg, s32 nPkgLen)
{
	sglib::gamemanagerproto::GamemanagerCenterReportGameInfo report;
	if( report.ParseFromArray(pPkg, nPkgLen) )
	{
		//SERVER_LOG_DEBUG( "===> GameInfoReport: GameID=" << report.gameid() );

		for( int i=0; i<report.gateservers_size(); ++i )
		{
			const sglib::publicproto::GateServerInfo &info = report.gateservers( i );
			CGameManager::Instance().SetGateInfo(
				report.gameid(),
				info.id(),
				info.ip().c_str(),
				info.port(),
				info.cur_count(),
				info.max_count() );

			/*SERVER_LOG_DEBUG( "\t---> GateId:" << info.id() 
				<< " [" << info.ip().c_str() << ":" << info.port() << "] " 
				<< info.cur_count() << "/" << info.max_count() );
			//*/
		}

		for( int i=0; i<report.gameservers_size(); ++i )
		{
			const sglib::publicproto::GameServerInfo &info = report.gameservers( i );
			CGameManager::Instance().SetGameServerInfo( 
				report.gameid(), 
				info.serverid(), 
				info.cur_count(), 0 );
			
			/*SERVER_LOG_DEBUG( "\t---> GameServerId:" << info.serverid() << ", CurCount:" << info.cur_count() );
			//*/
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CCenterClient,_GateInfoReport,ParseFromArray" );
	}
}

void CCenterClient::_UserAskEnterGame(const byte *pPkg, s32 nPkgLen)
{
	sglib::centerproto::ServerCenterEnterGameReq req;
	if( req.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_DEBUG( "ServerCenterEnterGameReq : GameID=" << req.gameid() );

		CUserManager::Instance().UserAskEnterGameInfo(
			*this, req.gateid(), req.clientid(), req.gameid(), req.gsid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CCenterClient,_UserAskEnterGame,ParseFromArray" );
	}
}

void CCenterClient::_GameGateClose(const byte *pPkg, s32 nPkgLen)
{
	sglib::gamemanagerproto::GamemanagerCenterGateCloseNtf ntf;
	if( ntf.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_DEBUG( "GameGateClose GameId:" << ntf.gameid() << ", GateId:" << ntf.gateid() );

		CGameManager::Instance().DelGateInfo(
			ntf.gameid(), ntf.gateid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CCenterClient,_GameGateClose,ParseFromArray" );
	}
}

void CCenterClient::_GameServerClose(const byte *pPkg, s32 nPkgLen)
{
	sglib::gamemanagerproto::GamemanagerCenterServerCloseNtf ntf;
	if( ntf.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_DEBUG( "GameGateClose GameId:" << ntf.gameid() << ", ServerId:" << ntf.serverid() );

		CGameManager::Instance().DelGameServerInfo(
			ntf.gameid(), ntf.serverid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CCenterClient,_GameServerClose,ParseFromArray" );
	}
}

void CCenterClient::_GroupGateClose(const byte *pPkg, s32 nPkgLen)
{
	sglib::groupproto::GroupmanagerCenterGateCloseNtf ntf;
	if( ntf.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_DEBUG( "GroupGateClose GateId:" << ntf.gateid() );

		CGroupInfoManager::Instance().DelGroupInfo(
			ntf.gateid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CCenterClient,_GroupGateClose,ParseFromArray" );
	}
}

void CCenterClient::_GroupServerClose(const byte *pPkg, s32 nPkgLen)
{
	sglib::groupproto::GroupmanagerCenterGroupCloseNtf ntf;
	if( ntf.ParseFromArray(pPkg, nPkgLen) )
	{
		SERVER_LOG_DEBUG( "GroupServerClose ServerId:" << ntf.serverid() );
	}
	else
	{
		SERVER_LOG_ERROR( "CCenterClient,_GroupServerClose,ParseFromArray" );
	}
}

void CCenterClient::_GroupGateInfoReport(const byte *pPkg, s32 nPkgLen)
{
	sglib::groupproto::GroupmanagerCenterReportGroupGateInfo report;
	if( report.ParseFromArray(pPkg, nPkgLen) )
	{
		//SERVER_LOG_DEBUG( "===> GroupGateInfoReport" );

		for( int i=0; i<report.gateservers_size(); ++i )
		{
			const sglib::publicproto::GateServerInfo &info = report.gateservers( i );

			CGroupInfoManager::Instance().SetGateInfo(
				info.id(),
				info.ip().c_str(),
				info.port(),
				info.cur_count(),
				info.max_count() );

			/*SERVER_LOG_DEBUG( "\t---> GroupGateId:" << info.id() 
				<< " [" << info.ip().c_str() << ":" << info.port() << "] " 
				<< info.cur_count() << "/" << info.max_count() );
			//*/
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CCenterClient,_GroupGateInfoReport,ParseFromArray" );
	}
}

