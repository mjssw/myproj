#include "GroupClient.h"
#include "CommDef.h"
using namespace SGLib;

CGroupClient::CGroupClient(s32 id) : 
	CClientBase( id )
{
}

int CGroupClient::OnRecv(char *buf, int len)
{
	int totalProc = 0;
	char *pbuf = buf;
	while( true )
	{
		int procLen = 0;
		if( len < MSG_HEAD_LEN )
		{
			return totalProc;
		}

		MsgHeadType pkgLen = *((MsgHeadType*)pbuf);
		if( len < (int)pkgLen )
		{
			return totalProc;
		}

		int _id = *((int*)( pbuf + MSG_HEAD_LEN ));
		switch( _id )
		{
		default:
			break;
		}

		procLen = (pkgLen);
		pbuf += procLen;
		len -= procLen;
		totalProc += procLen;
	}

	return totalProc;
}

void CGroupClient::OnConnect()
{
}

void CGroupClient::OnClose()
{
}

void CGroupClient::OnConnectError()
{
}
