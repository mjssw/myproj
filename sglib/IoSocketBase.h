#ifndef _SG_IO_SOCKET_BASE_H_
#define _SG_IO_SOCKET_BASE_H_

#include "Define.h"
#include "CycleBuffer.h"
#include "Lock.h"

namespace SGLib
{
    /*
     * Note! This lib use self-def protocol.
     *     pkg : head | user-date
     *             4       xxx
     */

    #define SGLIB_PKG_HEAD_LEN  (sizeof(s32))

    class CDispatchEvent;
	class CIoSocketBase
	{
	public:
		template<class TIoSocket>
		friend class CIoService;
        friend class CBasicEpollOp;
		friend class CIoSocketMgr;
		friend class CDispatchEvent;

		CIoSocketBase(s32 nId) : 
		  m_nId(nId),
		  m_SocketLock(),
		  m_Socket(0),
		  m_bSending(false),
		  m_RecvBufferLock(),
		  m_SendBufferLock(),
		  m_RecvBuffer(),
		  m_SendBuffer(),
          m_pDispatchEvent(NULL)
		{
		}

		virtual ~CIoSocketBase(){ /*Close();*/ };
		virtual void OnAccept(){};
		virtual void OnConnect(){};
		virtual void OnClose(){};
		virtual void OnRecv(const byte *pPkg, s32 nPkgLen){};
		virtual void OnConnectError(){};
		
		void  Close();
		bool  Send(const byte *pData, s32 nDataSize);
		void  PushEvent(s8 nEventType, s32 nIoSocketId);

		s32   GetId();
		void  SetDispatchEvent(CDispatchEvent *pDispatchEvent);


        void  DisplayDetailInfo();
		void  Dump(void *data);
		void  DumpSendBufferSimple(char *info);
		void  DumpSendBuffer(char *info);
	private:
		void  Init();
		bool  Initialize(s32 nSendSize, s32 nRecvSize);
        void  SetSocket(s32 Socket);
        s32   GetSocket();
		void  SetClientInfo(u32, u32, u16, u16);

		void  RecvDataSize(s32 nDataSize);
		void  SendDataSize(s32 nDataSize);
        void  ProcRecvDataSize(s32 nDataSize);

		s32   GetRecvFreeLen(){ return m_RecvBuffer.GetFreeLen(); }
		byte* GetRecvFreePtr(){ return m_RecvBuffer.GetFreePtr(); }
		s32   GetRecvDataLen(){ return m_RecvBuffer.GetDataLen(); }
		byte* GetRecvDataPtr(){ return m_RecvBuffer.GetDataPtr(); }

		s32   GetSendFreeLen(){ return m_SendBuffer.GetFreeLen(); }
		byte* GetSendFreePtr(){ return m_SendBuffer.GetFreePtr(); }
		s32   GetSendDataLen(){ return m_SendBuffer.GetDataLen(); }
		byte* GetSendDataPtr(){ return m_SendBuffer.GetDataPtr(); }

        s32   GetRecvDataSize(){ return m_RecvBuffer.GetSize(); }
        s32   GetSendDataSize()
		{ 
			CGuardLock<CLock> g( m_SendBufferLock );
			return m_SendBuffer.GetSize(); 
		}

        byte* GetRecvBuffer(){ return m_RecvBuffer.GetBuffer(); }
        byte* GetSendBuffer(){ return m_SendBuffer.GetBuffer(); }

		#ifdef _USE_WIN32
			CompletionKey_t& GetCompletionKey(){ return m_CompletionKey; }
			Overlapped_t& GetSendOverlapped(){ return m_OverlappedSend; }
			Overlapped_t& GetRecvOverlapped(){ return m_OverlappedRecv; }
			bool PostRecv();
			bool PostSend();
			bool PushSendEvent();
		#else
            void InitEpollFd(s32 nEpollFd){ m_nEpollFd=nEpollFd; }
            bool EpollSend();
		#endif

	protected:
        const s32  m_nId;

		// 客户端连接数据
		CLock   m_SocketLock;
		s32		m_Socket;
		u32		m_uiLocalIp;
		u32		m_uiRemoteIp;
		u16		m_uiLocalPort;
		u16		m_uiRemotePort;
		bool	m_bSending;

		#ifdef _USE_WIN32
			CompletionKey_t	m_CompletionKey;
			Overlapped_t    m_OverlappedSend;
			Overlapped_t    m_OverlappedRecv;
		#else
            s32 m_nEpollFd;
            
            union UEpollData
            {
                u64 u64Data;
                struct
                {
                    u64 n32Id : 32;
                    u64 n32Fd : 32;
                } sData;
            };
		#endif

		// 发送和接受数据缓冲区
		CLock m_RecvBufferLock;
		CLock m_SendBufferLock;
		CCycleBuffer m_RecvBuffer;
		CCycleBuffer m_SendBuffer;
        
        CDispatchEvent  *m_pDispatchEvent;

        // 总收到的字节数
        u64 m_u64RecvBytes;
        u64 m_u64SendBytes;
	};
};

#endif

