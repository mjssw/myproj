#ifndef _BASE_CLIENT__H_
#define _BASE_CLIENT__H_

#include "IoService.h"
#include "IoAdvSocketEx.h"
using namespace SGLib;
#include "Utils.h"

#ifdef _USE_IOSERVICE_EX_
class CClient : public CIoAdvSocketEx
#else
class CClient : public CIoSocketBase
#endif
{
public:
	CClient(s32 nId) : 
#ifdef _USE_IOSERVICE_EX_
		CIoAdvSocketEx(nId),
#else
		CIoSocketBase(nId),
#endif
		m_ForwardId(-1),
		m_Lock(),
		m_SendLock(),
		m_ClientId(0)
	{}

	virtual ~CClient(){}
		
	virtual void OnAccept();
	virtual void OnClose();
	virtual void OnRecv(const byte *pPkg, s32 nPkgLen);

	void SetForwardId(s32 fid);
	s32 GetForwardId();
	u64 GetClientId();
	
	bool SendPkg(const byte *pPkg, s32 nPkgLen);

private:
	void _SetClientId(u64 id);

protected:
	s32 m_ForwardId;
	CLock m_Lock;
	CLock m_SendLock;
	u64 m_ClientId;
};

#endif
