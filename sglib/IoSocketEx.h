#ifndef _IO_SOCKET_EX_H_
#define _IO_SOCKET_EX_H_

#include "Define.h"
#include "IoObjectEx.h"
#include "Lock.h"
#include "CycleBuffer.h"
#include "IEventEx.h"

namespace SGLib
{
	class CIoServiceEx;
	class CIoSocketExMgr;
	class CAccepterEx;
	class CConnectorEx;
	class CIoSocketEx : public CIoObjectEx, public IEventEx
	{
	public:
		friend class CIoServiceEx;
		friend class CIoSocketExMgr;
		friend class CAccepterEx;
		friend class CConnectorEx;

		enum
		{
			E_PkgHead_Len = sizeof(s32),
			E_MaxPkg_Len = 4096,
		};

		CIoSocketEx(s32 id);
		virtual ~CIoSocketEx();

		virtual void HandleRead(s32 size, void *data);
		virtual void HandleWrite(s32 size, void *data);
		virtual void HandleError(void *data);
		
		virtual void HandleEvent(s32 paramLen, char *paramData){}

		virtual void OnAccept(){}
		virtual void OnConnect(){}
		virtual void OnConnectError(){}
		virtual void OnClose(){}
		virtual void OnRecv(const byte *pkg, s32 len){}
		virtual CIoSocketEx* Clone(s32 id); 

		virtual void Close();
		bool Send(const byte *data, s32 size);
		s32  GetSocket();
		s32  GetId();
		bool IsUsing(){ return m_isUsing; }
		
		virtual s32 GetObjectId(){ return GetId(); }
		virtual s32 GetObjectSocket(){ return GetSocket(); }

		void DebugShowInfo();

	protected:
		virtual void  _ProcessRecvData();
		void  _Init();
		bool  _Initialize(s32 sendSize, s32 recvSize);
		void  _RecvDataSize(s32 nDataSize);
		void  _SendDataSize(s32 nDataSize);
        void  _ProcRecvDataSize(s32 nDataSize);
		s32   _GetRecvFreeLen();
		byte* _GetRecvFreePtr();
		s32   _GetRecvDataLen();
		byte* _GetRecvDataPtr();
		s32   _GetSendFreeLen();
		byte* _GetSendFreePtr();
		s32   _GetSendDataLen();
		byte* _GetSendDataPtr();
        s32   _GetRecvDataSize();
        s32   _GetSendDataSize();
		byte* _GetRecvBuffer();
        byte* _GetSendBuffer();
		void  _SetSocket(s32 sock);
		void  _SetClientInfo(u32, u32, u16, u16);
		void  _SetIoService(CIoServiceEx &ioService);
		bool  _Close();
		void  _OnSend();
		bool  _DoSend();
		void  _SetUsing(bool isUsing){ m_isUsing = isUsing; }

	protected:
		s32 m_id;
		s32 m_socket;
		CLock m_socketLock;
		CIoServiceEx *m_ioService;
		bool m_isUsing;

		// 发送和接受数据缓冲区
		CLock m_recvBufferLock;
		CLock m_sendBufferLock;
		CCycleBuffer m_recvBuffer;
		CCycleBuffer m_sendBuffer;

        // 总收到的字节数
        u64 m_recvBytes;
        u64 m_sendBytes;

		// 连接过来的client信息
		u32	m_localIp;
		u32	m_remoteIp;
		u16	m_localPort;
		u16	m_remotePort;
	
	protected:
#ifdef _USE_WIN32
		CompletionKey_t	m_completionKey;
		Overlapped_t    m_overlappedSend;
		Overlapped_t    m_overlappedRecv;
		const CompletionKey_t& _GetCompletionKey(){ return m_completionKey; }
		bool _PostRecv();
		bool _PostSend();

		CLock m_sendingLock;
		bool m_isSending;
#else
		virtual s32 _DoHandleRead();
		virtual s32 _DoHandleWrite();
        bool _EpollSend();
#endif
	};
};

#endif
