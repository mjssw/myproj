#ifndef _SG_EVENT_H_
#define _SG_EVENT_H_

#include "Define.h"
#include "NonCopyable.h"

namespace SGLib
{
    class CEvent : private NonCopyable
    {
    public:
        CEvent();
        ~CEvent();

        void Wait(s32 nWaitTime = -1);
        void SetEvent();
        void ResetEvent();

    private:
	#ifdef _USE_WIN32
        HANDLE m_hEvent;
	#else
		pthread_mutex_t m_Mutex;
		pthread_cond_t  m_Cond;
	#endif
    };
};

#endif
