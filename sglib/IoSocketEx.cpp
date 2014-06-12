#include "IoSocketEx.h"
#include "IoSocketExManager.h"
#include "IoServiceEx.h"
#include "StackTrace.h"
#include "Log.h"
using namespace SGLib;

#ifdef _USE_WIN32
// 统计信息
static volatile LONG64 g_sendEventCount = 0;
static volatile LONG64 g_recvEventCount = 0;
static volatile LONG64 g_sendPkgCount = 0;
static volatile LONG64 g_procPkgCount = 0;
void _DisplayCountInfo()
{
	CLog::InfoLog( "sendEvent:%lld\trecvEvent:%lld\tsendPkg:%lld\tprocPkg:%lld",
		g_sendEventCount, g_recvEventCount, g_sendPkgCount, g_procPkgCount );
}
#else
void _DisplayCountInfo(){};
#endif

CIoSocketEx::CIoSocketEx(s32 id) :
	m_id( id ),
	m_socket( (s32)INVALID_SOCKET ),
	m_socketLock(),
	m_ioService( NULL ),
	m_isUsing( false ),
	m_recvBufferLock(),
	m_sendBufferLock(),
	m_recvBuffer(),
	m_sendBuffer(),
	m_recvBytes( 0 ),
	m_sendBytes( 0 ),
	m_localIp( 0 ),
	m_remoteIp( 0 ),
	m_localPort( 0 ),
	m_remotePort( 0 )
{
#ifdef _USE_WIN32
	m_completionKey.m_pData = this;
	m_isSending = false;
#endif
}

CIoSocketEx::~CIoSocketEx()
{
}

void CIoSocketEx::HandleRead(s32 size, void *data)
{
#ifdef _USE_WIN32
	SELF_ASSERT( data != NULL, return; );
	//::InterlockedIncrement64( &g_recvEventCount );

	Overlapped_t *overlapped = (Overlapped_t*)data;
	_RecvDataSize( size );
	if( size == 0 && overlapped->m_sWsabuf.len > 0 )
	{
		// 断开连接, 注意这里为了防止因为本身接受缓冲区已经满了，
		// 但是WSARecv出去的时候给的缓冲区的长度本身也是0，最后导致dwSize也是0
		CLog::DebugLog( "CIoSocketEx::HandleRead client[%d,%d] close", GetId(), m_socket );
		Close();
		return;
	}
#else
#endif

	_ProcessRecvData();

#ifdef _USE_WIN32
	if( _PostRecv() == false )
	{
		CLog::ErrorLog( "CIoSocketEx::HandleRead client[%d,%d] _PostRecv Error", GetId(), m_socket );
		Close();
	}
#else
#endif
}

void CIoSocketEx::HandleWrite(s32 size, void *data)
{
#ifdef _USE_WIN32
	//::InterlockedIncrement64( &g_sendEventCount );
	
	SELF_ASSERT( data != NULL, return; );
	Overlapped_t *overlapped = (Overlapped_t*)data;
	if( size == 0 && overlapped->m_sWsabuf.len > 0 )
	{
		// 断开连接
		CLog::DebugLog( "CIoSocketEx::HandleWrite client[%d,%d] close", GetId(), m_socket );
		Close();
	}
	else
	{
		_SendDataSize( size );
		CGuardLock<CLock> g( m_sendingLock );
		_OnSend();
	}
#else
#endif
}

void CIoSocketEx::HandleError(void *data)
{
#ifdef _USE_WIN32
	CLog::ErrorLog( "CIoSocketEx::HandleError client[%d,%d] close", GetId(), m_socket );
	//SELF_ASSERT( data != NULL, return; );
	//Overlapped_t *overlapped = (Overlapped_t*)data;
	Close();
#else
#endif
}

CIoSocketEx* CIoSocketEx::Clone(s32 id)
{
	//return new CIoSocketEx( id );
	return NULL;
}

void CIoSocketEx::Close()
{
	SELF_ASSERT( m_ioService!=NULL, return; );
	CGuardLock<CLock> g( m_socketLock );
	if( _Close() )
	{
		CLog::InfoLog( "CIoSocketEx::Close socket[%d:%d]", GetId(), GetSocket() );
		m_ioService->GetIoSocketExMgr().PutIoSocket( *this );
		OnClose();
	}
	else
	{
		CLog::DebugLog( "CIoSocketEx::Close socket[id:%d] already close", m_id );
	}
}

/*
static bool _directSend(s32 sock, const byte *data, s32 size)
{
	char msg[CIoSocketEx::E_MaxPkg_Len] = {0};
	*((s32*)msg) = size + CIoSocketEx::E_PkgHead_Len;
	memcpy( msg+CIoSocketEx::E_PkgHead_Len, data, size );
	const char *p = (const char *)msg;
	s32 len = size + CIoSocketEx::E_PkgHead_Len;
	while( 1 )
	{
		s32 _sz = send( sock, p, len, 0 );
		if( _sz != SOCKET_ERROR )
		{
			p += _sz;
			len -= _sz;
			if( len <= 0 ) break;
		}
		else
		{
#ifdef _USE_WIN32
            s32 _eno = (s32)GetLastError();
#else
            s32 _eno = errno;
#endif
			CLog::ErrorLog( "_directSend %d error:%d", sock, _eno );
			return false;
		}
	}
	return true;
}
//*/

bool CIoSocketEx::Send(const byte *data, s32 size)
{
	{
		CGuardLock<CLock> g( m_sendBufferLock );

		s32 totalSize = size + E_PkgHead_Len;
		s32 emptySize = m_sendBuffer.GetCapacity() - m_sendBuffer.GetSize();
		if( emptySize < totalSize )
		{
			CLog::ErrorLog( "CIoSocketEx::Send Error:not enough send buf, id:%d empty:%d trysend:%d", 
				GetId(), emptySize, totalSize );
			return false;
		}

		s32 sendHeadSize = m_sendBuffer.AddDataSize( E_PkgHead_Len, (const byte*)&totalSize );
		SELF_ASSERT( sendHeadSize == E_PkgHead_Len, return false; );
		s32 sendBodySize = m_sendBuffer.AddDataSize( size, data );
		SELF_ASSERT( sendBodySize == size, return false; );
		s32 sendTotal = (sendHeadSize + sendBodySize);
		m_sendBytes += sendTotal;
		SELF_ASSERT( sendTotal == totalSize, return false; );
	}

#ifdef _USE_WIN32
	//::InterlockedIncrement64( &g_sendPkgCount );
	CGuardLock<CLock> g( m_sendingLock );
	return _DoSend();
#else
    return _EpollSend();
#endif
}

s32 CIoSocketEx::GetSocket()
{ 
	return m_socket; 
}

s32 CIoSocketEx::GetId()
{ 
	return m_id; 
}

void CIoSocketEx::DebugShowInfo()
{
	CLog::InfoLog( "[%d]IoSocketEx:(%d,%d) sendBuffSize=%d recvBuffSize=%d", (IsUsing()?1:0),
		GetId(), GetSocket(), m_sendBuffer.GetSize(), m_recvBuffer.GetSize() );
}

void CIoSocketEx::_Init()
{
	m_recvBuffer.Init();
	m_sendBuffer.Init();
    
    m_recvBytes = 0;
    m_sendBytes = 0;

	m_ioService = NULL;
}

bool CIoSocketEx::_Initialize(s32 sendSize, s32 recvSize)
{
	return m_sendBuffer.Initialize( sendSize ) &&
		m_recvBuffer.Initialize( recvSize );
}

void CIoSocketEx::_RecvDataSize(s32 nDataSize)
{
    CGuardLock<CLock> g( m_recvBufferLock );
	m_recvBuffer.AddDataSize( nDataSize );
    
    m_recvBytes += nDataSize;
}

void CIoSocketEx::_SendDataSize(s32 nDataSize)
{
    CGuardLock<CLock> g( m_sendBufferLock );
	m_sendBuffer.AddFreeSize( nDataSize );
}

void CIoSocketEx::_ProcRecvDataSize(s32 nDataSize)
{
    CGuardLock<CLock> g( m_recvBufferLock );
	m_recvBuffer.AddFreeSize( nDataSize );
}

s32 CIoSocketEx::_GetRecvFreeLen()
{ 
	return m_recvBuffer.GetFreeLen(); 
}

byte* CIoSocketEx::_GetRecvFreePtr()
{ 
	return m_recvBuffer.GetFreePtr(); 
}

s32 CIoSocketEx::_GetRecvDataLen()
{ 
	return m_recvBuffer.GetDataLen(); 
}

byte* CIoSocketEx::_GetRecvDataPtr()
{ 
	return m_recvBuffer.GetDataPtr(); 
}

s32 CIoSocketEx::_GetSendFreeLen()
{ 
	return m_sendBuffer.GetFreeLen();
}

byte* CIoSocketEx::_GetSendFreePtr()
{ 
	return m_sendBuffer.GetFreePtr();
}

s32 CIoSocketEx::_GetSendDataLen()
{ 
	return m_sendBuffer.GetDataLen();
}

byte* CIoSocketEx::_GetSendDataPtr()
{ 
	return m_sendBuffer.GetDataPtr();
}

s32 CIoSocketEx::_GetRecvDataSize()
{ 
	return m_recvBuffer.GetSize();
}

s32 CIoSocketEx::_GetSendDataSize()
{ 
	CGuardLock<CLock> g( m_sendBufferLock );
	return m_sendBuffer.GetSize(); 
}

byte* CIoSocketEx::_GetRecvBuffer()
{ 
	return m_recvBuffer.GetBuffer();
}

byte* CIoSocketEx::_GetSendBuffer()
{ 
	return m_sendBuffer.GetBuffer();
}

void CIoSocketEx::_SetSocket(s32 sock)
{
	m_socket = sock;
}

void CIoSocketEx::_SetClientInfo(u32 lIp, u32 rIp, u16 lPort, u16 rPort)
{
	m_localIp = lIp;
	m_remoteIp = rIp;
	m_localPort = lPort;
	m_remotePort = rPort;
}

void CIoSocketEx::_ProcessRecvData()
{
	byte tmpPkgDataBuffer[E_MaxPkg_Len];// = {0};
	while(true)
	{
		s32 nSize = _GetRecvDataSize();
		if( nSize < (s32)E_PkgHead_Len )
		{
			return;
		}

		// 如果包被分段了，将完整的包数据存于此
		byte *pPkgData = tmpPkgDataBuffer;
		s32 nPkgLen = 0;
		s32 nBodyLen = 0;

		s32 nDataLen1 = _GetRecvDataLen();
		byte *pData1 = _GetRecvDataPtr();
		if( nDataLen1 >= (s32)E_PkgHead_Len )
		{
			// Head没有被分段, 先检查是否有一个完整的数据包
			/*
			nBodyLen = *((s32*)pData1);
			nPkgLen = nBodyLen + E_PkgHead_Len;
			//*/
			nPkgLen = *((s32*)pData1);
			nBodyLen = nPkgLen - E_PkgHead_Len;

			if( nSize < nPkgLen )
			{
				return;
			}

			// Body可能被分段
			if( nDataLen1 >= nPkgLen )
			{
				// 整个包没有被分段
				pPkgData = pData1 + E_PkgHead_Len;
			}
			else
			{
				// 包被分段了
				s32 nBodyLen1 = nDataLen1-E_PkgHead_Len;
				s32 nBodyLen2 = nBodyLen - nBodyLen1; 
				memcpy( pPkgData, pData1+E_PkgHead_Len, nBodyLen1 ); // 拷贝上半部分body数据
				memcpy( pPkgData+nBodyLen1, _GetRecvBuffer(), nBodyLen2 ); // 拷贝下半部分body数据
			}
		}
		else
		{
			// Head被分段
			s32 nPkgHeadLen2 = E_PkgHead_Len - nDataLen1; 
			memcpy( pPkgData, pData1, nDataLen1 ); // 拷贝上半部分head
			memcpy( pPkgData+nDataLen1, _GetRecvBuffer(), nPkgHeadLen2 ); // 拷贝下半部分head
			/*
			nBodyLen = *((s32*)pPkgData);
			nPkgLen = nBodyLen + E_PkgHead_Len;
			//*/
			nPkgLen = *((s32*)pPkgData);
			nBodyLen = nPkgLen - E_PkgHead_Len;

			// 检查是否够一个完整包
			if( nSize < nPkgLen )
			{
				return;
			}

			memcpy( pPkgData, 
				_GetRecvBuffer()+nPkgHeadLen2, 
				nBodyLen ); // 拷贝body
		}

#ifdef _USE_WIN32
		//::InterlockedIncrement64( &g_procPkgCount );
#endif
		OnRecv( pPkgData, nBodyLen );
		_ProcRecvDataSize( nPkgLen );
	}
}

void CIoSocketEx::_SetIoService(CIoServiceEx &ioService)
{
	m_ioService = &ioService;
}

bool CIoSocketEx::_Close()
{
	if( m_socket != INVALID_SOCKET )
	{
#ifdef _USE_WIN32
		CancelIo( (HANDLE)m_socket );
		closesocket( m_socket );
#else
		bool ret = m_ioService->DelEpollEvent( m_socket );
		SELF_ASSERT( ret, ; );
		close( m_socket );
#endif
		m_socket = (s32)INVALID_SOCKET;
		return true;
	}

	return false;
}

void CIoSocketEx::_OnSend()
{
#ifdef _USE_WIN32
	if( _GetSendDataSize() > 0 )
	{
		if( !_PostSend() )
		{
			CLog::ErrorLog( "CIoSocketEx::_OnSend client[%d,%d] _PostSend Error", GetId(), m_socket );
			Close();
		}
	}
	else
	{
		m_isSending = false;
	}
#else
#endif
}

bool CIoSocketEx::_DoSend()
{
#ifdef _USE_WIN32
	if( !m_isSending )
	{
		if( !_PostSend() )
		{
			CLog::ErrorLog( "CIoSocketEx::_DoSend client[%d,%d] _PostSend Error", GetId(), m_socket );
			//Close();
			return false;
		}
	}
	return true;
#else
	return true;
#endif
}

#ifdef _USE_WIN32
bool CIoSocketEx::_PostRecv()
{
	memset( &m_overlappedRecv.m_sOverlapped, 0, sizeof(m_overlappedRecv.m_sOverlapped) );
	m_overlappedRecv.m_eEvtIocp = IOCP_RECV;

	{
		CGuardLock<CLock> g( m_recvBufferLock );
		m_overlappedRecv.m_sWsabuf.buf = (CHAR*)_GetRecvFreePtr();
		m_overlappedRecv.m_sWsabuf.len = _GetRecvFreeLen();
	}

	if( m_overlappedRecv.m_sWsabuf.len == 0 )
	{
		CLog::DebugLog( "CIoSocketEx::_PostRecv client[%d,%d] _PostRecv BUT len=0", GetId(), m_socket );
	}

	DWORD dwBytesRecvd = 0;
	DWORD dwFlags = 0;
	if( WSARecv( m_socket, &m_overlappedRecv.m_sWsabuf, 1, 
			&dwBytesRecvd, &dwFlags, &m_overlappedRecv.m_sOverlapped, NULL) != 0 )
	{
		if( WSAGetLastError() != WSA_IO_PENDING )
		{
			CLog::ErrorLog( "CIoSocketEx::_PostRecv client[%d,%d] Error:%d", 
				GetId(), GetSocket(), (s32)WSAGetLastError() );
			return false;
		}
	}

	return true;
}

bool CIoSocketEx::_PostSend()
{
	memset( &m_overlappedSend.m_sOverlapped, 0, sizeof(m_overlappedSend.m_sOverlapped) );
	m_overlappedSend.m_eEvtIocp = IOCP_SEND;

	{
		CGuardLock<CLock> g( m_sendBufferLock );
		m_overlappedSend.m_sWsabuf.buf = (CHAR*)_GetSendDataPtr();
		m_overlappedSend.m_sWsabuf.len = _GetSendDataLen();
	}

	if( m_overlappedSend.m_sWsabuf.len == 0 )
	{
		CLog::DebugLog( "CIoSocketEx::_PostSend client[%d,%d] _PostSend BUT len=0", GetId(), m_socket );
	}

	DWORD dwBytesSend = 0;
	if( WSASend( m_socket, &m_overlappedSend.m_sWsabuf, 1,  &dwBytesSend, 0, 
		&m_overlappedSend.m_sOverlapped, NULL) != 0 )
	{
		int err = WSAGetLastError(); 
		if( err != WSA_IO_PENDING )
		{
			CLog::ErrorLog( "CIoSocketEx::_PostSend socket:[%d,%d] Error:%d", 
				GetId(), GetSocket(), (s32)WSAGetLastError() );
			return false;
		}
		else
		{
			CLog::DebugLog( "CIoSocketEx::_PostSend socket:[%d,%d] Pending:%d", 
				GetId(), GetSocket(), (s32)WSAGetLastError() );
		}
	}

	m_isSending = true;
	return true;
}

#else

s32 CIoSocketEx::_DoHandleRead()
{
	s64 readSize = 0;
	s32 len = _GetRecvFreeLen();
	byte *data = _GetRecvFreePtr();

	do
	{
		if( len == 0 )
		{
			// 没有空间了
			break;
		}

		s32 recvLen = recv( m_socket, data, len, 0 );
		if( recvLen==-1 && (errno==EAGAIN || errno==EWOULDBLOCK) )
		{
			break;
		}
		else if( recvLen == 0 )
		{
			Close();
			break;
		}
		else if( recvLen > 0 )
		{
			readSize += recvLen;
			_RecvDataSize( recvLen );
			if( recvLen == len )
			{
				len = _GetRecvFreeLen();
				data = _GetRecvFreePtr();
			}
			else
			{
				break;
			}
		}
		else
		{
			Close();
			break;
		}
	} while( 1 );

	return readSize;
}

s32 CIoSocketEx::_DoHandleWrite()
{
    SELF_ASSERT( m_ioService != NULL, return 0; );

	s64 totalSendSize = 0;
	s32 sendSize = 0;

	s32 sendLen = _GetSendDataLen();
	byte *sendData = _GetSendDataPtr();

	do
	{
		if( sendLen == 0 )
		{
			break;
		}

		s32 len = send( m_socket, sendData, sendLen, 0 );
		if( len == 0 )
		{
			break;
		}
		else if( len == -1 )
		{
			if( errno==EAGAIN || errno==EWOULDBLOCK )
			{
				break;
			}
			else
			{
				Close();
				return -1;
			}
		}

		sendSize += len;

		if( len < sendLen )
		{
			sendLen -= len;
			sendData += len;
            
            totalSendSize += len;
		}
		else
		{
			_SendDataSize( sendSize );
			sendSize = 0;

			sendLen = _GetSendDataLen();
			sendData = _GetSendDataPtr();
		}

	} while( 1 );

	if( sendSize > 0 )
	{
		_SendDataSize( sendSize );
	}

	// if all data is send, need to remove EPOLLOUT event 
	if( _GetSendDataSize() == 0 )
	{
		if( !m_ioService->ModEpollEvent( m_socket, *this, false ) )
		{
			Close();
		}
	}

	return totalSendSize;
}
        
bool CIoSocketEx::_EpollSend()
{
    SELF_ASSERT( m_ioService != NULL, return false; );
    return m_ioService->ModEpollEvent( m_socket, *this, true );
}

#endif
