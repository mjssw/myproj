#include "GroupManager.h"
#include "ServerManager.h"
#include "GateClientManager.h"
#include "GroupClient.h"
#include "group.pb.h"
#include "msgid.pb.h"
using namespace std;
using namespace SGLib;

INIT_SIGNLETON_CLASS(CGroupManager);

CGroupManager::~CGroupManager()
{
}

CGroupInfo* CGroupManager::AddGroup(u64 id, const char *name, const char *icon)
{
	CGuardLock<CLock> g(m_Lock);
	map<u64, CGroupInfo*>::iterator it = m_Groups.find( id );
	if( it == m_Groups.end() )
	{
		CGroupInfo *group = new CGroupInfo( id, name, icon );
		if( group )
		{
			m_Groups[ id ] = group;
			return group;
		}
		else
		{
			SERVER_LOG_ERROR( "CGroupManager,AddGroup,new CGroupInfo" );
		}
	}

	return NULL;
}

void CGroupManager::DelGroup(u64 id)
{
	CGuardLock<CLock> g(m_Lock);
	map<u64, CGroupInfo*>::iterator it = m_Groups.find( id );
	if( it != m_Groups.end() )
	{
		SAFE_DELETE( it->second );
		m_Groups.erase( it );
	}
}

CGroupInfo* CGroupManager::FindGroup(u64 id)
{
	CGuardLock<CLock> g(m_Lock);
	map<u64, CGroupInfo*>::iterator it = m_Groups.find( id );
	if( it != m_Groups.end() )
	{
		return it->second;
	}

	return NULL;
}

s32 CGroupManager::TotalGroupCount()
{
	CGuardLock<CLock> g(m_Lock);
	return (s32)m_Groups.size();
}

s32 CGroupManager::TotalMemberCount()
{
	s32 count = 0;
	CGuardLock<CLock> g(m_Lock);
	map<u64, CGroupInfo*>::iterator it = m_Groups.begin();
	for( ; it != m_Groups.end(); ++it )
	{
		SG_ASSERT( it->second != NULL );
		count += it->second->MemberCount();
	}

	return count;
}

s32 CGroupManager::GroupMemberCount(u64 id)
{
	CGroupInfo *group = FindGroup( id );
	if( group )
	{
		return group->MemberCount();
	}

	return 0;
}

void CGroupManager::SetGroupReady()
{
	m_bIsReady = true;
}

void CGroupManager::SendMsgToClient(u64 gateInstanceId, u64 clientid, const ::google::protobuf::Message &msg, s32 msgid)
{
    CClient *client = CServerManager::Instance().FindClient( gateInstanceId );
    if( client != NULL )
    {
        CGroupClient *cli = (CGroupClient*)client;
        cli->SendMsgToClient( clientid, msg, msgid );
    }
    else
    {
        SERVER_LOG_ERROR( "CGroupManager,SendMsgToClient,FindClient,GateId:" << gateInstanceId << ",ClientId:" << clientid << ",MsgId:" << msgid );
    }
}

void CGroupManager::SendMsgToClient(s32 gateResId, u64 clientid, const ::google::protobuf::Message &msg, s32 msgid)
{
	const CGateClientInfo *info = CGateClientManager::Instance().GetGateInfo( gateResId );
    if( info )
    {
        u64 gateid = info->m_GateInstanceId;
        SendMsgToClient( gateid, clientid, msg, msgid );
    }
    else
    {
        SERVER_LOG_ERROR( "CGroupManager,SendMsgToClient,GetGateInfo,GateResId:" << gateResId << ",CliendId:" << clientid << ",MsgId:" << msgid );
    }
}

void CGroupManager::BroadcastMsgToGates(const ::google::protobuf::Message &msg, s32 msgid)
{
	vector<u64> vecClient;
	CServerManager::Instance().GetClientList( vecClient );
	vector<u64>::iterator it = vecClient.begin();
	for( ; it != vecClient.end(); ++it )
	{
		CClient *client = CServerManager::Instance().FindClient( *it );
		if( !client )
		{
			continue;
		}

		CGroupClient *pClient = (CGroupClient*)client;
		pClient->SendMsg( msg, msgid );
	}
}

void CGroupManager::StartReportStatusTimer(bool isDynamicStart)
{
	m_nReportTimerId = CServerManager::Instance().AddTimer(
			E_ReportStatusInfo_Timer,
			ReportStatusInforCallBack,
			NULL,
			0,
			true );

		// 设置动态启动
		m_bIsDynamicStart = isDynamicStart;
		m_bIsReady = (!m_bIsDynamicStart);
		if( m_bIsDynamicStart )
		{
			m_bHasReported = false;
		}
}

void CGroupManager::ReportStatusInforCallBack(void *pData, s32 nDataLen)
{
	if( !CGroupManager::Instance().m_bIsReady )
	{
		// 动态启动的如果其gate还没连接上来，则不要汇报自身的状态

		if( !CGroupManager::Instance().m_bHasReported )
		{
			// 如果还没有回报过，在汇报给GameManager
			CGroupManager::Instance().m_bHasReported = CGroupManager::Instance()._ReportDynamicStarted();
		}

		return;
	}

	vector<s32> vecGateId;
	CGateClientManager::Instance().GetGateIdList( vecGateId );

	if( vecGateId.size() > 0 )
	{
		sglib::groupproto::GroupGroupmanagerReportStatusInfo reportInfo;
		reportInfo.set_serverid( CServerManager::Instance().ServerId() );
		reportInfo.set_totalgroup( CGroupManager::Instance().TotalGroupCount() );	
		reportInfo.set_totalmember( CGroupManager::Instance().TotalMemberCount() );	

		vector<s32>::iterator it = vecGateId.begin();
		for( ; it != vecGateId.end(); ++it )
		{
			const CGateClientInfo *info = CGateClientManager::Instance().GetGateInfo( *it );
			if( info == NULL )
			{
				continue;
			}

			sglib::publicproto::GateServerInfo *pInfo = reportInfo.add_gateservers();
			pInfo->set_id( info->m_GateId );
			pInfo->set_ip( info->m_GateIp );
			pInfo->set_port( info->m_GatePort );
			pInfo->set_cur_count( info->m_GateCurConnCount );
			pInfo->set_max_count( info->m_GateMaxConnCount );
		}

		CServerManager::Instance().SendRpcMsg( 
			CServerManager::Instance().GetGroupManagerServerId(),
			reportInfo, sglib::msgid::GPGM_REPORT_STATUS_INFO );
	}
}

void CGroupManager::NotifyMemberJoinGroup(u64 groupid, const std::string &newMember)
{
	CGroupInfo *group = FindGroup( groupid );
	if( !group )
	{
		SG_ASSERT( false );
		return;
	}
	CGroupMember *member = group->FindMember( newMember.c_str() );
	SG_ASSERT( member );

	map<s32, vector<u64> > alluser;
	vector<string> vecUser;
	group->DumpMemberInfo(  alluser, newMember, vecUser );

	// 通知新玩家新的群详细信息 
	_NotifyGroupInfoToMember( *group, vecUser, *member );

	// 通知此群中的所有在线成员有新成员加入
	_NotifyNewMemberJoin( groupid, alluser, *member );
}

void CGroupManager::BroadcastGroupMessage(u64 groupid, const string &sender, const string &content)
{
	CGroupInfo *group = FindGroup( groupid );
	if( !group )
	{
		SERVER_LOG_ERROR( "CGroupManager,BroadcastGroupMessage,FindGroup," << groupid );
		return;
	}

	if( !group->FindMember( sender.c_str() ) )
	{
		SERVER_LOG_ERROR( "CGroupManager,BroadcastGroupMessage,SenderNotInGroup," << groupid << "," << sender );
		return;
	}

	sglib::groupproto::SCGroupMessageNtf ntf;
	ntf.set_groupid( groupid );
	ntf.set_content( content );
	ntf.set_sender( sender );

	BroadcastMsgInGroup( groupid, ntf, sglib::msgid::SC_GROUP_MESSAGE_NTF );
}

void CGroupManager::NotifyGroupInfoToMember(u64 groupid, const std::string &member)
{
	CGroupInfo *group = FindGroup( groupid );
	if( !group )
	{
		SG_ASSERT( false );
		return;
	}
	CGroupMember *pMember = group->FindMember( member.c_str() );
	SG_ASSERT( pMember );

	map<s32, vector<u64> > alluser;
	vector<string> vecUser;
	group->DumpMemberInfo(  alluser, member, vecUser );

	// 通知新玩家新的群详细信息 
	_NotifyGroupInfoToMember( *group, vecUser, *pMember );
}

void CGroupManager::NotifyAllGroupGateAddGroupInfo(u64 groupid, s32 serverid)
{
	sglib::groupproto::GroupGateAddGroupNtf ntf;
	ntf.set_groupid( groupid );
	ntf.set_serverid( serverid );

	CGroupManager::Instance().BroadcastMsgToGates( ntf, sglib::msgid::GPGG_ADD_GROUPID_NTF );
}

void CGroupManager::BroadcastMsgInGroup(u64 groupid, const ::google::protobuf::Message &msg, s32 msgid)
{
	CGroupInfo *group = FindGroup( groupid );
	if( !group )
	{
		SERVER_LOG_ERROR( "CGroupManager,BroadcastMsgInGroup,FindGroup," << groupid );
		return;
	}

	map<s32, vector<u64> > alluser;
	vector<string> vecUser;
	group->DumpMemberInfo(  alluser, "", vecUser );

	map<s32, vector<u64> >::iterator it = alluser.begin();
	for( ; it != alluser.end(); ++it )
	{
		const CGateClientInfo *pGate = CGateClientManager::Instance().GetGateInfo( it->first );
		if( pGate )
		{
			CGroupClient *client = (CGroupClient*)CServerManager::Instance().FindClient( pGate->m_GateInstanceId );
			SG_ASSERT( client != NULL );
			client->SendMsgToClients( it->second, msg, msgid );
		}
		else
		{
			SERVER_LOG_WARN( "CGroupManager,BroadcastMsgInGroup,GetGateInfo," << it->first );
		}
	}
}

void CGroupManager::DisplayInfo()
{
#if _DEBUG
	printf( "========= Group Info ===========\n" );
	SGLib::CGuardLock<SGLib::CLock> g(m_Lock);
	std::map<u64, CGroupInfo*>::iterator it = m_Groups.begin();
	for( ; it != m_Groups.end(); ++it )
	{
		it->second->DisplayInfo();
		printf( "\n" );
	}
#endif
}

bool CGroupManager::_ReportDynamicStarted()
{
	sglib::groupproto::GroupGroupmanagerDynamicStartNtf ntf;
	
	const CServerConfig *cfg = CServerManager::Instance().GetServerConfig();
	if( !cfg )
	{
		SERVER_LOG_ERROR( "CGroupManager,_ReportDynamicStarted,GetServerConfig" );
		return false;
	}

	const ServerConfig &_cfg = cfg->GetServerConfig();
	ntf.set_id( _cfg.m_Id );
	ntf.set_ip( _cfg.m_Ip );
	ntf.set_port( _cfg.m_Port );
	
	CServerManager::Instance().SendRpcMsg(  
		CServerManager::Instance().GetGroupManagerServerId(),
		ntf, sglib::msgid::GPGM_DYNAMIC_START_NTF );

	return true;
}

void CGroupManager::_NotifyNewMemberJoin(u64 groupid, std::map<s32, std::vector<u64> > &alluser, CGroupMember &newMember)
{
	sglib::groupproto::SCGroupMemberJoinNtf ntf;
	ntf.set_groupid( groupid );
	sglib::publicproto::GroupMemberInfo *member = ntf.mutable_member();
	SG_ASSERT( member != NULL );
	member->set_user( newMember.GetUser() );
	member->set_nickname( newMember.GetNickName() );
	member->set_head( newMember.GetHead() );
	member->set_online( 1 );

	map<s32, vector<u64> >::iterator it = alluser.begin();
	for( ; it != alluser.end(); ++it )
	{
		const CGateClientInfo *pGate = CGateClientManager::Instance().GetGateInfo( it->first );
		if( pGate )
		{
			CGroupClient *client = (CGroupClient*)CServerManager::Instance().FindClient( pGate->m_GateInstanceId );
			SG_ASSERT( client != NULL );
			client->SendMsgToClients(
				it->second,
				ntf,
				sglib::msgid::SC_GROUP_MEMBER_JOIN_NTF );
		}
		else
		{
			SERVER_LOG_WARN( "CGroupManager,_NotifyNewMemberJoin,GetGateInfo," << it->first );
		}
	}
}

void CGroupManager::_NotifyGroupInfoToMember(CGroupInfo &group, vector<string> &vecUser, CGroupMember &member)
{
	const CGateClientInfo *pGate = CGateClientManager::Instance().GetGateInfo( member.GetGateResId() );
	SG_ASSERT( pGate != NULL );
	CGroupClient *client = (CGroupClient*)CServerManager::Instance().FindClient( pGate->m_GateInstanceId );
	SG_ASSERT( client != NULL );

	// 通知玩家群列表更新信息
	sglib::groupproto::SCGroupListUpdate groupListUpdate;
	sglib::publicproto::GroupInfo *info = groupListUpdate.add_groups();
	SG_ASSERT( info != NULL );
	info->set_id( group.GetId() );
	info->set_name( group.GetName() );
	info->set_icon( group.GetIcon() );
	info->set_count( group.MemberCount() );
	client->SendMsgToClient( member.GetClientId(), groupListUpdate, sglib::msgid::SC_GROUP_LIST_UPDATE );

	// 通知玩家群内成员列表信息
	sglib::groupproto::SCGroupInfoUpdate groupInfoUpdate;
	groupInfoUpdate.set_id( group.GetId() );
	vector<string>::iterator it = vecUser.begin();
	for( ; it != vecUser.end(); ++it )
	{
		CGroupMember *member = group.FindMember( it->c_str() );
		if( member )
		{
			sglib::publicproto::GroupMemberInfo *info = groupInfoUpdate.add_members();
			SG_ASSERT( info != NULL );
			info->set_user( member->GetUser() );
			info->set_nickname( member->GetNickName() );
			info->set_head( member->GetHead() );
			info->set_online( member->IsOnline() ? 1 : 0 );
		}
		else
		{
			// 有可能此玩家正好离开群
			SERVER_LOG_WARN( "CGroupManager,_NotifyGroupInfoToNewMember,FindMember," << it->c_str() );
		}
	}
	client->SendMsgToClient( member.GetClientId(), groupInfoUpdate, sglib::msgid::SC_GROUP_INFO_UPDATE );
}
