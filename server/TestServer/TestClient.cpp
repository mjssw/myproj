#include "TestClient.h"
#include "ServerManager.h"
#include "msgid.pb.h"
#include "client.pb.h"
using namespace std;

CTestClient::CTestClient(int nId) : CGateClientBase(nId)
{
	_RegisterClientProc( sglib::msgid::CS_TEST_MSG_REQ, &CTestClient::_TestMsg );
}

#ifndef _FACE_TO_GATE
void CTestClient::OnRecv(const byte *pPkg, s32 nPkgLen)
{
	s32 msgId = *((s32*)pPkg);
	sglib::clientproto::CSTestMsgReq req;
	if( req.ParseFromArray(pPkg+MSG_ID_LEN, nPkgLen-MSG_ID_LEN) )
	{
		printf( "recv msg:%s\n", req.msg().c_str() );

		sglib::clientproto::SCTestMsgRsp rsp;
		rsp.set_msg( req.msg() );
		
		int count = rsp.ByteSize();
		byte buf[MAX_MSG_LEN] = {0};
		*((s32*)buf) = sglib::msgid::SC_TEST_MSG_RSP;
		if( rsp.SerializeToArray(buf+MSG_ID_LEN, MAX_MSG_LEN) )
		{
			if( !Send( buf, count + MSG_ID_LEN ) )
			{
				Close();
				SERVER_LOG_ERROR( "CTestClient,Send,Close" );
			}
		}
		else
		{
			SERVER_LOG_ERROR( "CTestClient,SCTestMsgRsp,SerializeToArray" );
		}
	}
	else
	{
		SERVER_LOG_ERROR( "CTestClient,CSTestMsgReq,ParseFromArray," << msgId );
	}
}
#endif

void CTestClient::ProcPlayerMessage(u64 gateid, u64 clientid, s32 msgid, const byte *pkg, s32 len)
{
#ifdef _FACE_TO_GATE
	map<int, ClientProtoProcFunc>::iterator it = m_mapProtoProc.find( msgid );
	if( it != m_mapProtoProc.end() )
	{
		(this->*it->second)(
			clientid,
			pkg, 
			len);
	}
#endif
}

void CTestClient::OnClientClose(u64 clientid)
{
	CGateClientBase::OnClientClose( clientid );
}

void CTestClient::_RegisterClientProc(s32 id, ClientProtoProcFunc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}

void CTestClient::_TestMsg(u64 id, const byte *pkg, s32 len)
{
	sglib::clientproto::CSTestMsgReq req;
	if( req.ParseFromArray(pkg, len) )
	{
		sglib::clientproto::SCTestMsgRsp rsp;
		rsp.set_msg( req.msg() );
	
		SendMsgToClient( id, rsp, sglib::msgid::SC_TEST_MSG_RSP );
	}
	else
	{
		SERVER_LOG_ERROR( "CTestClient,CSTestMsgReq,ParseFromArray" );
	}
}
