#ifndef _SG_LOCK_H_
#define _SG_LOCK_H_

#include "Define.h"
#include "NonCopyable.h"

namespace SGLib
{
#ifdef _USE_WIN32
    class CFastLock : private NonCopyable 
    {
    public:
        CFastLock();
        ~CFastLock();

        void Lock();   
        void UnLock();
        bool TryLock();

    private:
        #pragma pack(push, 8)
        LONG volatile m_Lock;
        #pragma pack(pop)
    };
#endif

    class CLock : private NonCopyable
    {
    public:
        CLock();
        ~CLock();

        void Lock();
        void UnLock();
        bool TryLock();

    private:
#ifdef _USE_WIN32
	CRITICAL_SECTION m_Lock;
#else
	pthread_mutex_t	 m_Lock;
#endif
    };

    class CNoLock
    {
    public:
        CNoLock(){}
        ~CNoLock(){}

        void Lock(){}
        void UnLock(){}
        bool TryLock(){ return true; }
    };

    template <class LockType>
    class CGuardLock : private NonCopyable
    {
    public:
        CGuardLock( LockType& );
        ~CGuardLock();
    private:
        LockType &m_RefLock;
    };

    template <class LockType>
    CGuardLock<LockType>::CGuardLock( LockType& lock ) : m_RefLock( lock )
    {
        m_RefLock.Lock();
    }

    template <class LockType>
    CGuardLock<LockType>::~CGuardLock()
    {
        m_RefLock.UnLock();
    }

};


#endif
