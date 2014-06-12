#ifndef _Template_Client_h_
#define _Template_Client_h_

#include "GateClientBase.h"
#include <map>

class CTemplateClient;
typedef void (CTemplateClient::*ClientProtoProcFunc)(u64 id, const byte *pkg, s32 len);

class CTemplateClient : public CGateClientBase
{
public:
	CTemplateClient(s32 nId);
	virtual void ProcPlayerMessage(u64 gateid, u64 clientid, s32 msgid, const byte *pkg, s32 len);

private:
	void _RegisterClientProc(s32 id, ClientProtoProcFunc proc);

private:
	std::map<s32, ClientProtoProcFunc> m_mapProtoProc;
};

#endif
