#include "IoSocketExManager.h"
#include "IoSocketEx.h"
#include "StackTrace.h"
using namespace SGLib;

CIoSocketExMgr::CIoSocketExMgr() : m_isInit(false)
{
}
	
CIoSocketExMgr::~CIoSocketExMgr()
{
	Fini();
}

bool CIoSocketExMgr::Init(s32 maxCount, s32 sendSize, s32 recvSize, CIoSocketEx &templateSocket)
{
	m_maxCount = maxCount;
	m_socketMgr.m_firstFreeIoSocket = 0;
	m_socketMgr.m_inuseSize = 0;
	m_socketMgr.m_inuseIoSocket = maxCount;
	m_socketMgr.m_count = maxCount;
	m_socketMgr.m_ioSocketArray = new SSockListNode[m_socketMgr.m_inuseIoSocket];
	SELF_ASSERT( m_socketMgr.m_ioSocketArray != NULL, return false; );
	for( s32 i=0; i<maxCount; ++i )
	{
		m_socketMgr.m_ioSocketArray[i].m_nextIoSocket = (i+1);
		m_socketMgr.m_ioSocketArray[i].m_prevIoSocket = ((i==0)?maxCount:i-1);
		m_socketMgr.m_ioSocketArray[i].m_ioSocket = templateSocket.Clone( i );
		if( !m_socketMgr.m_ioSocketArray[i].m_ioSocket ||
			!m_socketMgr.m_ioSocketArray[i].m_ioSocket->_Initialize(sendSize, recvSize) )
		{
			return false;	
		}
	}

	m_isInit = true;
	return true;
}

void CIoSocketExMgr::Fini()
{
	for( s32 i=0; i<m_socketMgr.m_count; ++i )
	{
		if( m_socketMgr.m_ioSocketArray[i].m_ioSocket )
		{
			SAFE_DELETE( m_socketMgr.m_ioSocketArray[i].m_ioSocket );
		}
	}
	SAFE_DELETE_ARRAY( m_socketMgr.m_ioSocketArray );
	m_socketMgr.m_count = 0;
	m_isInit = false;
}

CIoSocketEx* CIoSocketExMgr::GetFreeIoSocket()
{
	if( !m_isInit )
	{
		return NULL;
	}

	CGuardLock<CLock> g( m_lock );

	s32 maxClientCount = m_socketMgr.m_count;
	s32 freeIdx = m_socketMgr.m_firstFreeIoSocket;
	if( freeIdx >= maxClientCount )
	{
		return NULL;
	}
        
    // 修改空闲队列,不关心prev指针
	m_socketMgr.m_firstFreeIoSocket = m_socketMgr.m_ioSocketArray[ freeIdx ].m_nextIoSocket;
        
    // 修改使用队列
    m_socketMgr.m_ioSocketArray[ freeIdx ].m_nextIoSocket = m_socketMgr.m_inuseIoSocket;
    m_socketMgr.m_ioSocketArray[ freeIdx ].m_prevIoSocket = maxClientCount;
    if( m_socketMgr.m_inuseIoSocket != maxClientCount )
    {
        m_socketMgr.m_ioSocketArray[ m_socketMgr.m_inuseIoSocket ].m_prevIoSocket = freeIdx;
    }
    m_socketMgr.m_inuseIoSocket = freeIdx;
    
	++m_socketMgr.m_inuseSize;

	CIoSocketEx *ioSocket = m_socketMgr.m_ioSocketArray[ freeIdx ].m_ioSocket;
	ioSocket->_Init();
	ioSocket->_SetUsing( true );

	return ioSocket;
}

CIoSocketEx* CIoSocketExMgr::FindIoSocket(s32 ioSocketId)
{
	if( !m_isInit )
	{
		return NULL;
	}

    if( ioSocketId<0 || ioSocketId>=m_socketMgr.m_count )
    {
        return NULL;
    }
	
	return m_socketMgr.m_ioSocketArray[ ioSocketId ].m_ioSocket;
}

void CIoSocketExMgr::PutIoSocket(CIoSocketEx &IoSocket)
{
	if( !m_isInit )
	{
		return;
	}

	CGuardLock<CLock> g( m_lock );

	s32 id = IoSocket.GetId();
	s32 maxClientCount = m_socketMgr.m_count;
	IoSocket._SetUsing( false );

    // 从使用队列中移除
    if( id == m_socketMgr.m_inuseIoSocket )
    {
        m_socketMgr.m_inuseIoSocket = m_socketMgr.m_ioSocketArray[ id ].m_nextIoSocket;
        if( m_socketMgr.m_inuseIoSocket != maxClientCount )
        {
            m_socketMgr.m_ioSocketArray[ m_socketMgr.m_inuseIoSocket ].m_prevIoSocket = maxClientCount; 
        }
    }
    else
    {
        s32 prev = m_socketMgr.m_ioSocketArray[ id ].m_prevIoSocket;
        s32 next = m_socketMgr.m_ioSocketArray[ id ].m_nextIoSocket;
        m_socketMgr.m_ioSocketArray[ prev ].m_nextIoSocket = next;
        if( next != maxClientCount )
        {
            m_socketMgr.m_ioSocketArray[ next ].m_prevIoSocket = prev;
        }
    }

    // 修改空闲队列
	m_socketMgr.m_ioSocketArray[ id ].m_nextIoSocket = m_socketMgr.m_firstFreeIoSocket;
	m_socketMgr.m_firstFreeIoSocket = id;
        
    --m_socketMgr.m_inuseSize;
}

s32	CIoSocketExMgr::GetMaxCount()
{
	return m_maxCount;
}

void CIoSocketExMgr::PrintFirstNFreeSocket(s32 count)
{
	s32 maxClientCount = m_socketMgr.m_count;
    if( count<0 || count>=maxClientCount )
    {
        count = maxClientCount;
    }

    printf( "FirstFree id=%d\n", m_socketMgr.m_firstFreeIoSocket );
    for( s32 i=0, id=m_socketMgr.m_firstFreeIoSocket; i<count; ++i )
    {
        if( id >= maxClientCount )
        {
            break;
        }

        if( i % 25 == 0 ) printf( "\n" );
        printf( "%d->", id );

        id = m_socketMgr.m_ioSocketArray[id].m_nextIoSocket; 
    }
    printf( "\n\n" );
}

void CIoSocketExMgr::DebugShowUseInfo()
{
	printf( "InUse count=%d\n", m_socketMgr.m_inuseSize );
	printf( "FirstUse id=%d\n", m_socketMgr.m_inuseIoSocket );
    for( s32 i=0, id=m_socketMgr.m_inuseIoSocket; i<m_socketMgr.m_count; ++i )
    {
        if( id >= m_socketMgr.m_count )
        {
            break;
        }

        printf( "sock:%d info:\n", id );
		m_socketMgr.m_ioSocketArray[id].m_ioSocket->DebugShowInfo();

        id = m_socketMgr.m_ioSocketArray[id].m_nextIoSocket; 
        printf( "\n next sock:%d\n", id );
    }
    printf( "\n\n" );
}

void CIoSocketExMgr::DebugShowFreeInfo()
{
	s32 maxClientCount = m_socketMgr.m_count;
    s32 count = maxClientCount;

    printf( "FirstFree id=%d\n", m_socketMgr.m_firstFreeIoSocket );
    for( s32 i=0, id=m_socketMgr.m_firstFreeIoSocket; i<count; ++i )
    {
        if( id >= maxClientCount )
        {
            break;
        }

        if( i % 10 == 0 ) printf( "\n" );
        printf( "%d->", id );

        id = m_socketMgr.m_ioSocketArray[id].m_nextIoSocket; 
    }
    printf( "\n\n" );
}
