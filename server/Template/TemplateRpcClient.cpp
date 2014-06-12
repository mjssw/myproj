#include "TemplateRpcClient.h"
#include "ServerManager.h"
#include "TemplateClient.h"
#include "msgid.pb.h"

CTemplateRpcClient::CTemplateRpcClient(s32 nId) : 
	CRpcClient(nId)
{
}

CTemplateRpcClient::~CTemplateRpcClient()
{
}

void CTemplateRpcClient::OnRecv(const byte *pkg, s32 len)
{
	s32 msgId = *((s32*)pkg);

	map<s32, ProtoProc>::iterator it = m_mapProtoProc.find( msgId );
	if( it != m_mapProtoProc.end() )
	{
		(this->*it->second)( pkg+MSG_ID_LEN, len-MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CTemplateRpcClient,OnRecv," << GetId() << "," << msgId );
	}
}

void CTemplateRpcClient::_RegisterProc(s32 id, ProtoProc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}

void CTemplateRpcClient::_SendMsgToClient(u64 gateid, u64 userid, const ::google::protobuf::Message &msg, s32 msgid)
{
	CClient *client = CServerManager::Instance().FindClient( gateid );
	if( client != NULL )
	{
		CTemplateClient *cli = (CTemplateClient*)client;
		cli->SendMsgToClient( userid, msg, msgid );
	}
	else
	{
		SERVER_LOG_ERROR( "CTemplateRpcClient,_SendMsgToClient," << gateid );
	}
}
