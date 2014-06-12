#ifndef _IO_SERVICE_EX_H_
#define _IO_SERVICE_EX_H_

#include "Thread.h"
#include "IoObjectEx.h"
#include "Queue.h"

namespace SGLib
{
	class CIoSocketExMgr;
	class CEventWorkerExPool;
	class IEventEx;
	class CIoServiceEx : public IRunnable
	{
	public:
		enum
		{
			E_Max_IoThread_Count = 32,
			E_Max_EpollEvent_Count = 4096,
		};

		CIoServiceEx(CIoSocketExMgr &ioSocketExMgr, CEventWorkerExPool *eventWorkerPool=NULL);
		~CIoServiceEx();

		bool Start();
		void Stop();
		bool IsRun();
		CIoSocketExMgr& GetIoSocketExMgr(){ return m_ioSocketExMgr; }
		bool TryPushEvent(s32 worker, const SEventObj &eventObj, IEventEx *obj);

		virtual void Run();

#ifdef _USE_WIN32
		bool BindIoObject(s32 sock, const CompletionKey_t &key);
#else
		bool BindIoObject(s32 sock, const CIoObjectEx &ioObj);
        bool AddEpollEvent(s32 sock, u32 event, const CIoObjectEx &ioObj);
        bool DelEpollEvent(s32 sock);
        bool ModEpollEvent(s32 sock, const CIoObjectEx &ioObj, bool isAddOutEvent);
#endif

	private:
		void _SetRun(bool isRun);
		bool _Init();
		void _IoRunWindows();
		void _IoRunLinux();
		void _InterruptIoThread();

	private:
		bool m_isRun;
		s32 m_ioThreadCount;
        CThread	*m_ioThread[E_Max_IoThread_Count];
		CIoSocketExMgr &m_ioSocketExMgr;
		CEventWorkerExPool *m_eventWorkerPool;
#ifdef _USE_WIN32
		HANDLE m_completionHandle;
#else
		s32 m_epollFd;
		struct epoll_event m_epollEvents[E_Max_EpollEvent_Count];

        class CEpoolWaitInterrupter : public CIoObjectEx
        {
        public:
            CEpoolWaitInterrupter();
            ~CEpoolWaitInterrupter();
            bool Init(s32 epollfd);
            void Fini(s32 epollfd);
            void Interrupt(s32 epollfd);

        public:
            virtual void HandleRead(s32 size, void *data){}
            virtual void HandleWrite(s32 size, void *data){}
            virtual void HandleError(void *data){}
        protected:
            virtual s32 _DoHandleRead(){ return 0; }
            virtual s32 _DoHandleWrite(){ return 0; }
        private:
            s32 m_fdRead;
            s32 m_fdWrite;
        } m_interrupter;
#endif
	};
};

#endif
