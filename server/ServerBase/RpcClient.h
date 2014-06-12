#ifndef _BASE_RPC_CLIENT_H_
#define _BASE_RPC_CLIENT_H_

#include "IoService.h"
#include "IoAdvSocketEx.h"
#include "globalcfg.h"
using namespace SGLib;
#include "Utils.h"

#ifdef _USE_IOSERVICE_EX_
class CRpcClient : public CIoAdvSocketEx
#else
class CRpcClient : public CIoSocketBase
#endif
{
public:
	CRpcClient(s32 nId) : 
#ifdef _USE_IOSERVICE_EX_
		CIoAdvSocketEx(nId),
#else
        CIoSocketBase(nId),
#endif
        m_ServerId(INVALID_VAL)
	{}

	virtual ~CRpcClient(){}
		
	virtual void OnConnect();
	virtual void OnClose();
	virtual void OnRecv(const byte *pPkg, s32 nPkgLen);
	virtual void OnConnectError();

protected:
    char* ConvertIpToStr(u32 ip);

protected:
	char m_ip[MAX_IP_LEN];	// 连接的IP
	s32  m_port;			// 连接的PORT
    s32  m_ServerId;		// 连接的ServerId
};

#endif
