#ifndef _EVENT_DIAPATCH_H_
#define _EVENT_DIAPATCH_H_

#include "Define.h"
#include "Runnable.h"
#include "Queue.h"
#include "Event.h"
#include "Config.h"

namespace SGLib
{
	class CIoSocketMgr;
	class CThread;

	class CDispatchEvent : public IRunnable
    {
    public:
        enum
		{
			E_Event_Connect = 0,
			E_Event_Accept,
			E_Event_Recv,
            E_Event_Close,
			E_Event_OnSend,
			E_Event_Send,

			E_Event_Count,
		};

        union UIoEvent
		{
			s64 nData;
			struct
			{
				s8  nEventType;
				s32 nIoSocketId;
			} IoEvent;
		};

        CDispatchEvent(CIoSocketMgr &sockMgr, const CNetConfig &refNetConfig);
		~CDispatchEvent();

		static s64 InitEvent(s8 nEventType, s32 nIoSocketId)
        {
            UIoEvent IoEvent;
			IoEvent.nData = 0;
            IoEvent.IoEvent.nEventType = nEventType;
            IoEvent.IoEvent.nIoSocketId = nIoSocketId;
            return IoEvent.nData;
        }

        void PushEvent(s8 nEventType, s32 nIoSocketId);
		bool Start();
		void Stop(s32 waitTime);
		void Run();

	private:
		void  _DoEventAccept(s32 nIoSocketId);
        void  _DoEventConnect(s32 nIoSocketId);
        void  _DoEventRecv(s32 nIoSocketId);
        void  _DoEventClose(s32 nIoSocketId);
		void  _DoEventOnSend(s32 nIoSocketId);
		void  _DoEventSend(s32 nIoSocketId);

	private:	
		const CNetConfig& m_refNetConfig;
        CEvent		    m_QueueSignal;
		CSafeQueue<s64>	m_EventQueue;
		CThread			*m_EventWorker;
		bool			m_bStop;
		CIoSocketMgr&	m_refSockMgr;
		byte			*m_tmpPkgDataBuffer;
    };
}

#endif
