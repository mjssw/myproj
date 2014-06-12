#include "TemplateClient.h"
#include "msgid.pb.h"
using namespace std;

CTemplateClient::CTemplateClient(s32 nId) : CGateClientBase(nId)
{
}

void CTemplateClient::ProcPlayerMessage(u64 gateid, u64 clientid, s32 msgid, const byte *pkg, s32 len)
{
	map<int, ClientProtoProcFunc>::iterator it = m_mapProtoProc.find( msgid );
	if( it != m_mapProtoProc.end() )
	{
		(this->*it->second)(
			clientid,
			pkg, 
			len);
	}
}

void CTemplateClient::_RegisterClientProc(s32 id, ClientProtoProcFunc proc)
{
	if( proc != NULL )
	{
		m_mapProtoProc[ id ] = proc;
	}
}
