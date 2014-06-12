#include "IoSocketBase.h"
#include "IoService.h"

using namespace SGLib;

s32 CIoSocketBase::GetId()
{
	return m_nId;
}

void CIoSocketBase::SetDispatchEvent(CDispatchEvent *pDispatchEvent)
{
	m_pDispatchEvent = pDispatchEvent;
}

void CIoSocketBase::DisplayDetailInfo()
{
    printf( "IoSocket info as follows:\n" );
    printf( "Id:%d\tSocket:%d\n", m_nId, m_Socket );

    printf( "recv_buffer:(%d/%d)\tsend_buffer:(%d/%d)\n",
        GetRecvDataSize(),
        m_RecvBuffer.GetCapacity(),
        GetSendDataSize(),
        m_SendBuffer.GetCapacity() );

    printf( "recv_buffer ptr:%d-%d\n", 
		(s32)(GetRecvDataPtr()-GetRecvBuffer()),
		(s32)(GetRecvFreePtr()-GetRecvBuffer()) );
    m_RecvBuffer.DisplayInfo();
    
    printf( "send_buffer ptr:%d-%d\n", 
		(s32)(GetSendDataPtr()-GetSendBuffer()),
		(s32)(GetSendFreePtr()-GetSendBuffer()) );
    m_SendBuffer.DisplayInfo();
}

void CIoSocketBase::Dump(void *data)
{
	byte *pData = (byte*)data;
	*((s32*)pData) = m_nId;				pData += 4;
	*((s32*)pData) = m_Socket;			pData += 4;
    *((u64*)pData) = m_u64RecvBytes;	pData += 8;
	*((u64*)pData) = m_u64SendBytes;	pData += 8;
    m_RecvBufferLock.Lock();
    m_RecvBuffer.Dump( pData );			pData += 16;
	m_RecvBufferLock.UnLock();
    m_SendBuffer.Dump( pData );			pData += 16;
}

void  CIoSocketBase::DumpSendBufferSimple(char *info)
{ 
    CGuardLock<CLock> g( m_SendBufferLock );
	m_SendBuffer.DumpStr(info);
}

void  CIoSocketBase::DumpSendBuffer(char *info)
{ 
    CGuardLock<CLock> g( m_SendBufferLock );
	m_SendBuffer.DumpStrAll(info);
}

void CIoSocketBase::Init()
{
	m_RecvBuffer.Init();
	m_SendBuffer.Init();
    
    m_u64RecvBytes = 0;
    m_u64SendBytes = 0;
	m_bSending = false;
}

bool CIoSocketBase::Initialize(s32 nSendSize, s32 nRecvSize)
{
	return m_SendBuffer.Initialize( nSendSize ) &&
		   m_RecvBuffer.Initialize( nRecvSize );
}

void CIoSocketBase::SetSocket(s32 Socket)
{
	m_Socket = Socket;
}

s32 CIoSocketBase::GetSocket()
{
	return m_Socket;
}

void CIoSocketBase::SetClientInfo(u32 lIp, u32 rIp, u16 lPort, u16 rPort)
{
	m_uiLocalIp = lIp;
	m_uiRemoteIp = rIp;
	m_uiLocalPort = lPort;
	m_uiRemotePort = rPort;

	#ifdef _USE_WIN32
		m_CompletionKey.m_pData = this;
	#endif
}
		
void CIoSocketBase::Close()
{
    CGuardLock<CLock> g( m_SocketLock );
	if( m_Socket != INVALID_SOCKET )
	{
		#ifdef _USE_WIN32
            CancelIo( (HANDLE)m_Socket );
		    closesocket( m_Socket );
		#else
            s32 nRet = CBasicEpollOp::EpollEventDel( m_nEpollFd, m_Socket );
            SG_ASSERT( nRet == E_Error_Success );
		    close( m_Socket );
		#endif
		m_Socket = (s32)INVALID_SOCKET; 
        
        if( m_pDispatchEvent )
		{
			m_pDispatchEvent->PushEvent( CDispatchEvent::E_Event_Close, m_nId );
		}
	}
	else
	{
		CLog::ErrorLog( "socket[id:%d] already close", m_nId );
	}
}

/*
static bool DoSend(s32 hsock, const byte *pData, s32 nDataSize)
{
	char buf[MAX_RECV_PKG_LEN] = {0};
	s32 totalSize = nDataSize + SGLIB_PKG_HEAD_LEN;
	*((s32*)(buf)) = totalSize;
	memcpy( buf+SGLIB_PKG_HEAD_LEN, pData, nDataSize );

	int nSend = 0;
	char *pBuf = buf;
	int trySend = totalSize;
	while(true)
	{
		int len = send( hsock, pBuf, trySend, 0 );
		nSend += len;
		if( nSend < totalSize )
		{
			pBuf += len;
			trySend -= len;
		}
		else
		{
			break;
		}
	}
	return true;
}
//*/

bool CIoSocketBase::Send(const byte *pData, s32 nDataSize)
{
    CGuardLock<CLock> g( m_SendBufferLock );

	s32 totalSize = nDataSize + SGLIB_PKG_HEAD_LEN;
	s32 emptySize = m_SendBuffer.GetCapacity() - m_SendBuffer.GetSize();
	if( emptySize < totalSize )
	{
		CLog::ErrorLog( "Send failed, not enough send buf id=%d empty=%d send=%d",
			GetId(), emptySize, totalSize );
		return false;
	}

	//s32 nSendSize = m_SendBuffer.AddDataSize( SGLIB_PKG_HEAD_LEN, (const byte*)&nDataSize );
	s32 nSendHeadSize = m_SendBuffer.AddDataSize( SGLIB_PKG_HEAD_LEN, (const byte*)&totalSize );
	SG_ASSERT( nSendHeadSize == SGLIB_PKG_HEAD_LEN );
    s32 nSendBodySize = m_SendBuffer.AddDataSize( nDataSize, pData );
	SG_ASSERT( nSendBodySize == nDataSize );
	s32 nSendTotal = (nSendHeadSize + nSendBodySize);
    m_u64SendBytes += nSendTotal;
	SG_ASSERT( nSendTotal == totalSize );

#ifdef _USE_WIN32
    if( !PushSendEvent() )
#else
    if( !EpollSend() )
#endif
    {
		CLog::ErrorLog( "Do Send failed: id=%d", GetId() );
        return false;
    }
    
	return true;
}

void CIoSocketBase::PushEvent(s8 nEventType, s32 nIoSocketId)
{
	if( m_pDispatchEvent )
	{
		m_pDispatchEvent->PushEvent(nEventType, nIoSocketId);
	}
}

void CIoSocketBase::RecvDataSize(s32 nDataSize)
{
    CGuardLock<CLock> g( m_RecvBufferLock );
	m_RecvBuffer.AddDataSize( nDataSize );
    
    m_u64RecvBytes += nDataSize;
}

void CIoSocketBase::SendDataSize(s32 nDataSize)
{
    CGuardLock<CLock> g( m_SendBufferLock );
	m_SendBuffer.AddFreeSize( nDataSize );
}

void CIoSocketBase::ProcRecvDataSize(s32 nDataSize)
{
    CGuardLock<CLock> g( m_RecvBufferLock );
	m_RecvBuffer.AddFreeSize( nDataSize );
}

#ifdef _USE_WIN32
bool CIoSocketBase::PostRecv()
{
	Overlapped_t *pOverlapped = &GetRecvOverlapped();
	memset( &pOverlapped->m_sOverlapped, 0, sizeof(pOverlapped->m_sOverlapped) );

	pOverlapped->m_eEvtIocp = IOCP_RECV;
	{
		CGuardLock<CLock> g( m_RecvBufferLock );
		pOverlapped->m_sWsabuf.buf = (CHAR*)GetRecvFreePtr();
		pOverlapped->m_sWsabuf.len = GetRecvFreeLen();
	}

	if( pOverlapped->m_sWsabuf.len == 0 )
	{
		CLog::DebugLog( "CIoSocketBase::PostRecv client[%d,%d] PostRecv BUT len=0", 
            GetId(), GetSocket() );
	}

	DWORD dwBytesRecvd = 0;
	DWORD dwFlags = 0;
	if( WSARecv(
		GetSocket(), 
		&pOverlapped->m_sWsabuf,
		1, 
		&dwBytesRecvd, 
		&dwFlags, 
		&pOverlapped->m_sOverlapped,
		NULL) != 0 )
	{
		if( WSAGetLastError() != WSA_IO_PENDING )
		{
			CLog::ErrorLog( "CIoSocketBase::PostRecv client[%d,%d] PostRecv failed, err=%d", 
				GetId(), GetSocket(), (s32)WSAGetLastError() );

			return false;
		}
	}

	return true;
}

bool CIoSocketBase::PostSend()
{
	Overlapped_t *pOverlapped = &GetSendOverlapped();
	memset( &pOverlapped->m_sOverlapped, 0, sizeof(pOverlapped->m_sOverlapped) );

	pOverlapped->m_eEvtIocp = IOCP_SEND;
	{
		CGuardLock<CLock> g( m_SendBufferLock );
		pOverlapped->m_sWsabuf.buf = (CHAR*)GetSendDataPtr();
		pOverlapped->m_sWsabuf.len = GetSendDataLen();
	}

	if( pOverlapped->m_sWsabuf.len == 0 )
	{
		CLog::DebugLog( "CIoSocketBase::PostSend client[%d,%d] PostSend BUT len=0", 
            GetId(), GetSocket() );
	}

	DWORD dwBytesSend = 0;
	if( WSASend(
			GetSocket(), 
			&pOverlapped->m_sWsabuf,
			1, 
			&dwBytesSend, 
			0, 
			&pOverlapped->m_sOverlapped,
			NULL) != 0 )
	{
		int err = WSAGetLastError(); 
		if( err != WSA_IO_PENDING )
		{
			CLog::ErrorLog( "CIoSocketBase::PostSend client[%d,%d] PostSend failed, err=%d", 
				GetId(), GetSocket(), err );
			
			return false;
		}
	}

	return true;
}

bool CIoSocketBase::PushSendEvent()
{
	PushEvent( CDispatchEvent::E_Event_Send, GetId() );
	return true;
}

#else

bool CIoSocketBase::EpollSend()
{
    s32 nRet = CBasicEpollOp::EpollEventMod( m_nEpollFd, m_Socket, m_nId, true );
    if( nRet != E_Error_Success )
    {
        return false;
    }

    return true;
}

#endif

