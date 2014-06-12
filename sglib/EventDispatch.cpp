#include "EventDispatch.h"
#include "StackTrace.h"
#include "Log.h"
#include "IoSocketManager.h"
#include "IoSocketBase.h"
#include "Thread.h"

using namespace SGLib;

CDispatchEvent::CDispatchEvent(CIoSocketMgr &sockMgr, const CNetConfig &refNetConfig) : 
	m_refNetConfig(refNetConfig),
	m_QueueSignal(),
	m_EventQueue(m_refNetConfig.m_eventQueueSize),
	m_EventWorker(NULL),
	m_bStop(true),
	m_refSockMgr(sockMgr),
	m_tmpPkgDataBuffer(NULL)
{
}

CDispatchEvent::~CDispatchEvent()
{
	SAFE_DELETE_ARRAY( m_tmpPkgDataBuffer );
	SAFE_DELETE( m_EventWorker );
}

void CDispatchEvent::PushEvent(s8 nEventType, s32 nIoSocketId)
{
    bool bRet = m_EventQueue.Push( InitEvent(nEventType, nIoSocketId) );
    SG_ASSERT( bRet == true );
    m_QueueSignal.SetEvent();
}

bool CDispatchEvent::Start()
{
	m_tmpPkgDataBuffer = new byte[ m_refNetConfig.m_maxPackageLen ];
	if( m_tmpPkgDataBuffer == NULL )
	{
		return false;
	}

	m_EventWorker = new CThread(this);
	if( m_EventWorker != NULL )
	{
		if( m_EventWorker->Start() )
		{
			m_bStop = false;
			return true;
		}
	}
	return false;
}

void CDispatchEvent::Stop(s32 waitTime)
{
	if( !m_bStop && m_EventWorker )
	{
		m_bStop = true;
		m_EventWorker->Stop(waitTime);
	}
}

void CDispatchEvent::Run()
{
	CLog::DebugLog( "CDispatchEvent::Run Start" );

	while( !m_bStop )
	{
		m_QueueSignal.Wait( m_refNetConfig.m_waitEventTimeOut );

		if( m_bStop )
		{
			break;
		}

		while( !m_EventQueue.Empty() )
		{
			CDispatchEvent::UIoEvent Event;
			m_EventQueue.Pop( Event.nData );
			switch( Event.IoEvent.nEventType )
			{
			case CDispatchEvent::E_Event_Accept:
				_DoEventAccept( Event.IoEvent.nIoSocketId );
				break;
			case CDispatchEvent::E_Event_Connect:
				_DoEventConnect( Event.IoEvent.nIoSocketId );
				break;
			case CDispatchEvent::E_Event_Recv:
				_DoEventRecv( Event.IoEvent.nIoSocketId );
				break;
			case CDispatchEvent::E_Event_Close:
				_DoEventClose( Event.IoEvent.nIoSocketId );
				break;
			case CDispatchEvent::E_Event_OnSend:
				_DoEventOnSend( Event.IoEvent.nIoSocketId );
				break;
			case CDispatchEvent::E_Event_Send:
				_DoEventSend( Event.IoEvent.nIoSocketId );
				break;
			default:
				SG_ASSERT( false );
				break;
			};
		}
	}
	
	CLog::DebugLog( "CDispatchEvent::Run End" );
}

void CDispatchEvent::_DoEventAccept(s32 nIoSocketId)
{
	CLog::DebugLog( "CDispatchEvent::_DoEventAccept proc accept event:%d", nIoSocketId );
    CIoSocketBase *pIoSocketBase = m_refSockMgr.FindIoSocket( nIoSocketId );
    if( pIoSocketBase )
    {
        pIoSocketBase->OnAccept();
    }
}

void CDispatchEvent::_DoEventConnect(s32 nIoSocketId)
{
	CLog::DebugLog( "CDispatchEvent::_DoEventConnect proc connect event:%d", nIoSocketId );
    CIoSocketBase *pIoSocketBase = m_refSockMgr.FindIoSocket( nIoSocketId );
    if( pIoSocketBase )
    {
        pIoSocketBase->OnConnect();
    }
}
	
void CDispatchEvent::_DoEventRecv(s32 nIoSocketId)
{
    CIoSocketBase *pIoSocketBase = m_refSockMgr.FindIoSocket( nIoSocketId );
    if( pIoSocketBase )
    {
		// test
		char tmpDumpData[1024] = {0};
		pIoSocketBase->Dump( tmpDumpData );

		while(true)
		{
			s32 nSize = pIoSocketBase->GetRecvDataSize();
			if( nSize < (s32)SGLIB_PKG_HEAD_LEN )
			{
				return;
			}

			// ��������ֶ��ˣ��������İ����ݴ��ڴ�
			byte *pPkgData = m_tmpPkgDataBuffer;
			s32 nPkgLen = 0;
			s32 nBodyLen = 0;

			s32 nDataLen1 = pIoSocketBase->GetRecvDataLen();
			byte *pData1 = pIoSocketBase->GetRecvDataPtr();
			if( nDataLen1 >= (s32)SGLIB_PKG_HEAD_LEN )
			{
				// Headû�б��ֶ�, �ȼ���Ƿ���һ�����������ݰ�
				/*
				nBodyLen = *((s32*)pData1);
				nPkgLen = nBodyLen + SGLIB_PKG_HEAD_LEN;
				//*/
				nPkgLen = *((s32*)pData1);
				nBodyLen = nPkgLen - SGLIB_PKG_HEAD_LEN;

				if( nSize < nPkgLen )
				{
					return;
				}

				// Body���ܱ��ֶ�
				if( nDataLen1 >= nPkgLen )
				{
					// ������û�б��ֶ�
					pPkgData = pData1 + SGLIB_PKG_HEAD_LEN;
				}
				else
				{
					// �����ֶ���
					s32 nBodyLen1 = nDataLen1-SGLIB_PKG_HEAD_LEN;
					s32 nBodyLen2 = nBodyLen - nBodyLen1; 
					memcpy( pPkgData, pData1+SGLIB_PKG_HEAD_LEN, nBodyLen1 ); // �����ϰ벿��body����
					memcpy( pPkgData+nBodyLen1, pIoSocketBase->GetRecvBuffer(), nBodyLen2 ); // �����°벿��body����
				}
			}
			else
			{
				// Head���ֶ�
				s32 nPkgHeadLen2 = SGLIB_PKG_HEAD_LEN - nDataLen1; 
				memcpy( pPkgData, pData1, nDataLen1 ); // �����ϰ벿��head
				memcpy( pPkgData+nDataLen1, pIoSocketBase->GetRecvBuffer(), nPkgHeadLen2 ); // �����°벿��head
				/*
				nBodyLen = *((s32*)pPkgData);
				nPkgLen = nBodyLen + SGLIB_PKG_HEAD_LEN;
				//*/
				nPkgLen = *((s32*)pPkgData);
				nBodyLen = nPkgLen - SGLIB_PKG_HEAD_LEN;
                
				// ����Ƿ�һ��������
				if( nSize < nPkgLen )
				{
					return;
				}

				memcpy( pPkgData, 
					pIoSocketBase->GetRecvBuffer()+nPkgHeadLen2, 
					nBodyLen ); // ����body
			}
                    
			pIoSocketBase->OnRecv( pPkgData, nBodyLen );
			pIoSocketBase->ProcRecvDataSize( nPkgLen );
		}
    }
}

void CDispatchEvent::_DoEventClose(s32 nIoSocketId)
{
	CLog::DebugLog( "CDispatchEvent::_DoEventClose proc close event:%d", nIoSocketId );
    CIoSocketBase *pIoSocket = m_refSockMgr.FindIoSocket( nIoSocketId );
    if( pIoSocket )
    {
		CLog::DebugLog( "close socket. SockId=%d Iocket::GetSocket=%d", 
            pIoSocket->GetId(), pIoSocket->GetSocket() );
		CLog::DebugLog( "SockId=%d recvbytes=%llu sendbytes=%llu", 
            pIoSocket->GetId(), pIoSocket->m_u64RecvBytes, pIoSocket->m_u64SendBytes );
        if( pIoSocket->m_RecvBuffer.GetSize() > 0 )
        {
			CLog::DebugLog( "SockId=%d RecvBufferSize=%d", pIoSocket->GetId(), pIoSocket->m_RecvBuffer.GetSize() );
        }
        pIoSocket->OnClose();
        m_refSockMgr.PutIoSocket( *pIoSocket );
    }
}

void CDispatchEvent::_DoEventOnSend(s32 nIoSocketId)
{
#ifdef _USE_WIN32
    CIoSocketBase *pIoSocket = m_refSockMgr.FindIoSocket( nIoSocketId );
    if( pIoSocket )
    {
		if( pIoSocket->GetSendDataSize() > 0 )
		{
			if( !pIoSocket->PostSend() )
			{
				pIoSocket->Close();
			}
		}
		else
		{
			pIoSocket->m_bSending = false;
		}
	}
#endif
}

void CDispatchEvent::_DoEventSend(s32 nIoSocketId)
{
#ifdef _USE_WIN32
    CIoSocketBase *pIoSocket = m_refSockMgr.FindIoSocket( nIoSocketId );
    if( pIoSocket )
    {
		if( !pIoSocket->m_bSending )
		{
			pIoSocket->m_bSending = true;
			if( !pIoSocket->PostSend() )
			{
				pIoSocket->Close();
			}
		}
	}
#endif
}
