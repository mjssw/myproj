#include "TestRpcClient.h"
#include "ServerManager.h"
#include "TestClient.h"
#include "msgid.pb.h"
#include "client.pb.h"

CTestRpcClient::CTestRpcClient(int nId) : 
	CRpcClient(nId)
{
}

CTestRpcClient::~CTestRpcClient()
{
}

void CTestRpcClient::OnRecv(const byte *pPkg, s32 nPkgLen)
{
	s32 msgId = *((s32*)pPkg);

	SERVER_LOG_DEBUG( "client:" << GetId() << " recv rpc pkg, len=" << nPkgLen << " ,msgId=" << msgId );

	map<int, ProtoProc>::iterator it = m_mapProtoProc.find( msgId );
	if( it != m_mapProtoProc.end() )
	{
		(this->*it->second)( pPkg+MSG_ID_LEN, nPkgLen-MSG_ID_LEN );
	}
	else
	{
		SERVER_LOG_ERROR( "CTestRpcClient,OnRecv," << GetId() << "," << msgId );
	}
}

void CTestRpcClient::_RegisterProc(int id, ProtoProc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}

void CTestRpcClient::_SendMsgToClient(u64 cid, u64 gate_cid, const ::google::protobuf::Message &Msg, s32 nMsgId)
{
	CClient *client = CServerManager::Instance().FindClient( cid );
	if( client != NULL )
	{
		CTestClient *cli = (CTestClient*)client;
		cli->SendMsgToClient( gate_cid, Msg, nMsgId );
	}
	else
	{
		SERVER_LOG_ERROR( "CTestRpcClient,_SendMsgToClient," << cid );
	}
}
