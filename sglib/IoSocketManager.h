#ifndef _SG_IOSOCKET_MANAGER_H_
#define _SG_IOSOCKET_MANAGER_H_

#include "Define.h"
#include "Lock.h"
#include "iosocketbase.h"
#include "Config.h"

namespace SGLib
{
	class CIoSocketBase;

	class CIoSocketMgr
	{
	public:

        enum
        {
            E_IoSocket_Free = 0,
            E_IoSocket_Inuse,
        };

		CIoSocketMgr(const CNetConfig &refNetConfig);
		~CIoSocketMgr();

		template<class T>
		s32 Init()
		{
			s32 maxClientCount = m_refNetConfig.m_maxClientCount;
			m_SocketMgr.m_nFirstFreeIoSocket = 0;
			m_SocketMgr.m_nInuseSize = 0;
			m_SocketMgr.m_nInuseIoSocket = maxClientCount;
			m_SocketMgr.m_nCount = maxClientCount;
			m_SocketMgr.m_IoSocketArray = new SSockListNode[m_SocketMgr.m_nInuseIoSocket];
			for( s32 i=0; i<maxClientCount; ++i )
			{
				m_SocketMgr.m_IoSocketArray[i].m_nNextIoSocket = (i+1);
				m_SocketMgr.m_IoSocketArray[i].m_nPrevIoSocket = ((i==0)?maxClientCount:i-1);
				m_SocketMgr.m_IoSocketArray[i].m_pIoSocket = new T( i );
				//SGDEBUG("sizeof(T)=%u\n", sizeof(*m_SocketMgr.m_IoSocketArray[i].m_pIoSocket));
				if( !m_SocketMgr.m_IoSocketArray[i].m_pIoSocket ||
				    !m_SocketMgr.m_IoSocketArray[i].m_pIoSocket->Initialize(
						m_refNetConfig.m_sendBufferSize, 
						m_refNetConfig.m_recvBufferSize) )
				{
					return E_Error_CreateIoSocketFailed;	
				}
				m_SocketMgr.m_IoSocketArray[i].m_pIoSocket->SetDispatchEvent(NULL);

			}

			return E_Error_Success;
		}

		void           Release();
		CIoSocketBase* GetFreeIoSocket();
		CIoSocketBase* FindIoSocket(s32 nIoSocketId);
		void           PutIoSocket(CIoSocketBase &IoSocket);

        void           PrintFirstNFreeSocket(s32 nCount);
	private:
		struct SSockListNode
		{
			CIoSocketBase  *m_pIoSocket;
			s32		        m_nNextIoSocket;
			s32		        m_nPrevIoSocket;
		};

		struct
		{
			s32 m_nFirstFreeIoSocket;  // 空闲队列头
			s32 m_nInuseIoSocket;      // 已使用的队列头
			s32 m_nInuseSize;          // 已使用的个数
			s32 m_nCount;
			SSockListNode *m_IoSocketArray;
		} m_SocketMgr;
			
        #ifdef _USE_WIN32
            CLock   m_Lock;
        #else
            CNoLock m_Lock;
        #endif

		const CNetConfig &m_refNetConfig;
	};
};

#endif
