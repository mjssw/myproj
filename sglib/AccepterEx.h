#ifndef _ACCEPTER_EX_H_
#define _ACCEPTER_EX_H_

#include "Define.h"
#include "IoObjectEx.h"

namespace SGLib
{
	class CIoServiceEx;
	class CIoServiceExPool;
	class CAccepterEx : public CIoObjectEx
	{
	public:
		CAccepterEx(CIoServiceExPool &ioServicePool);
		virtual ~CAccepterEx();

		virtual void HandleRead(s32 size, void *data);
		virtual void HandleWrite(s32 size, void *data);
		virtual void HandleError(void *data);

		bool Start(s32 port);
		void Stop();
	
	protected:
#ifdef _USE_WIN32
		void _PostAcceptEx(Overlapped_t &overlapped);
		void _DoAcceptWindows(Overlapped_t &overlapped);
#else
		virtual s32 _DoHandleRead();
		virtual s32 _DoHandleWrite();
		void _DoAcceptLinux();
        s32  _EpollAcceptSocket();
        bool _SetSocketNonblock(s32 sock);
#endif
		bool _BindToIoService();
		void _Start();

	protected:
		s32 m_listenPort;
		s32 m_listenSock;
		CIoServiceExPool &m_ioServicePool;
        CIoServiceEx *m_ioService;
#ifdef _USE_WIN32
		CompletionKey_t				m_completionKey;
		Overlapped_t				m_overlappArray[MAX_ACCEPT_COUNT];
#endif
	};
};

#endif
