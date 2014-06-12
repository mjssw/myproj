#include "IoSocketManager.h"
#include "IoSocketBase.h"

using namespace SGLib;

CIoSocketMgr::CIoSocketMgr(const CNetConfig &refNetConfig) : 
	m_refNetConfig(refNetConfig)
{
	//Init();
}
	
CIoSocketMgr::~CIoSocketMgr()
{
	Release();
}

void CIoSocketMgr::Release()
{
	for( s32 i=0; i<m_SocketMgr.m_nCount; ++i )
	{
		if( m_SocketMgr.m_IoSocketArray[i].m_pIoSocket )
		{
			SAFE_DELETE( m_SocketMgr.m_IoSocketArray[i].m_pIoSocket );
		}
	}
	SAFE_DELETE_ARRAY( m_SocketMgr.m_IoSocketArray );
	m_SocketMgr.m_nCount = 0;
}

CIoSocketBase* CIoSocketMgr::GetFreeIoSocket()
{
	m_Lock.Lock();

	s32 maxClientCount = m_refNetConfig.m_maxClientCount;
	s32 nFreeIdx = m_SocketMgr.m_nFirstFreeIoSocket;
	if( nFreeIdx >= maxClientCount )
	{
		m_Lock.UnLock();
		return NULL;
	}
        
    // 修改空闲队列,不关心prev指针
	m_SocketMgr.m_nFirstFreeIoSocket = m_SocketMgr.m_IoSocketArray[ nFreeIdx ].m_nNextIoSocket;
        
    // 修改使用队列
    m_SocketMgr.m_IoSocketArray[ nFreeIdx ].m_nNextIoSocket = m_SocketMgr.m_nInuseIoSocket;
    m_SocketMgr.m_IoSocketArray[ nFreeIdx ].m_nPrevIoSocket = maxClientCount;
    if( m_SocketMgr.m_nInuseIoSocket != maxClientCount )
    {
        m_SocketMgr.m_IoSocketArray[ m_SocketMgr.m_nInuseIoSocket ].m_nPrevIoSocket = nFreeIdx;
    }
    m_SocketMgr.m_nInuseIoSocket = nFreeIdx;

    ++m_SocketMgr.m_nInuseSize;

    m_Lock.UnLock();

	CIoSocketBase *pIoSocket = m_SocketMgr.m_IoSocketArray[ nFreeIdx ].m_pIoSocket;
	pIoSocket->Init();

	return pIoSocket;
}

CIoSocketBase* CIoSocketMgr::FindIoSocket(s32 nIoSocketId)
{
    if( nIoSocketId<0 || nIoSocketId>=m_refNetConfig.m_maxClientCount )
    {
        return NULL;
    }
	
	return m_SocketMgr.m_IoSocketArray[ nIoSocketId ].m_pIoSocket;
}

void CIoSocketMgr::PutIoSocket(CIoSocketBase &IoSocket)
{
    m_Lock.Lock();

	s32 nId = IoSocket.GetId();
	s32 maxClientCount = m_refNetConfig.m_maxClientCount;

    // 从使用队列中移除
    if( nId == m_SocketMgr.m_nInuseIoSocket )
    {
        m_SocketMgr.m_nInuseIoSocket = m_SocketMgr.m_IoSocketArray[ nId ].m_nNextIoSocket;
        if( m_SocketMgr.m_nInuseIoSocket != maxClientCount )
        {
            m_SocketMgr.m_IoSocketArray[ m_SocketMgr.m_nInuseIoSocket ].m_nPrevIoSocket = maxClientCount; 
        }
    }
    else
    {
        s32 nPrev = m_SocketMgr.m_IoSocketArray[ nId ].m_nPrevIoSocket;
        s32 nNext = m_SocketMgr.m_IoSocketArray[ nId ].m_nNextIoSocket;
        m_SocketMgr.m_IoSocketArray[ nPrev ].m_nNextIoSocket = nNext;
        if( nNext != maxClientCount )
        {
            m_SocketMgr.m_IoSocketArray[ nNext ].m_nPrevIoSocket = nPrev;
        }
    }

    // 修改空闲队列
	m_SocketMgr.m_IoSocketArray[ nId ].m_nNextIoSocket = m_SocketMgr.m_nFirstFreeIoSocket;
	m_SocketMgr.m_nFirstFreeIoSocket = nId;
        
    --m_SocketMgr.m_nInuseSize;

    m_Lock.UnLock();
}

void CIoSocketMgr::PrintFirstNFreeSocket(s32 nCount)
{
	s32 maxClientCount = m_refNetConfig.m_maxClientCount;
    if( nCount<0 || nCount>=maxClientCount )
    {
        nCount = maxClientCount;
    }

    printf( "FirstFree id=%d\n", m_SocketMgr.m_nFirstFreeIoSocket );
    for( s32 i=0, id=m_SocketMgr.m_nFirstFreeIoSocket; i<nCount; ++i )
    {
        if( id > maxClientCount )
        {
            break;
        }

        if( i % 25 == 0 ) printf( "\n" );
        printf( "%d->", id );

        id = m_SocketMgr.m_IoSocketArray[id].m_nNextIoSocket; 
    }
    printf( "\n\n" );
}