#ifndef _SG_QUEUE_H_
#define _SG_QUEUE_H_

#include "Define.h"
#include "NonCopyable.h"
#include "Lock.h"
#include <stdlib.h> 
#include <string.h> 

namespace SGLib
{
    template <class T>
    class CQueue : private NonCopyable
    {
    public:
        CQueue(s32 nSize);
        ~CQueue();

        bool  Useful();
        bool  Empty();
        s32   Size();
        bool  Push( T );
        bool  Pop( T& );
        void  Clear();

    protected:
        bool  _Extend();

    protected:
        T*     m_pBase;
        s32    m_Head;
        s32    m_Tail;
        s32    m_nSize;
    };

    template <class T>
    CQueue<T>::CQueue(s32 nSize) : 
        m_pBase( NULL )
      , m_Head( 0 )
      , m_Tail( 0 )
      , m_nSize( nSize )
    {
        if( m_nSize > 0 )
        {
            m_pBase = (T*)malloc( sizeof(T) * m_nSize );
        }
    }
         
    template <class T>
    CQueue<T>::~CQueue()
    {
        if( m_pBase )
        {
            free( m_pBase );
            m_pBase = NULL;
        }
    }

    template <class T>
    bool CQueue<T>::Useful()
    {
        return (bool)( m_pBase != NULL );
    }
     
    template <class T>
    bool CQueue<T>::Empty()
    {
        return (bool)( m_Head == m_Tail );
    }
         
    template <class T>
    s32 CQueue<T>::Size()
    {
        if( m_Head > m_Tail )
        {
            return ( m_Tail + m_nSize - m_Head + 1 );
        }
        else if(  m_Head < m_Tail )
        {
            return ( m_Tail - m_Head );
        }

        return 0;
    }
     
    template <class T>
    bool CQueue<T>::Push( T elem )
    {
        s32 pos = (m_Tail + 1) % m_nSize;
        if( m_Head == pos )
        {
            if( false == _Extend() )
            {
                return false;
            }
        }
            
        *(m_pBase + m_Tail) = elem;

        m_Tail = (m_Tail + 1) % m_nSize;

        return true;
    }
        
    template <class T>
    bool CQueue<T>::Pop( T& elem)
    {
        if( m_Head == m_Tail )
        {
            return false;
        }

        elem = *(m_pBase + m_Head);
        m_Head = (m_Head + 1) % m_nSize;

        return true;
    }

    template <class T>
    void CQueue<T>::Clear()
    {
        m_Head = 0;
        m_Tail = 0;
    }

    template <class T>
    bool CQueue<T>::_Extend()
    {
        s32 pos = m_nSize;
        m_nSize <<= 1;

        T *newBase = (T*)realloc( 
            (void*)m_pBase, 
            sizeof(T) * m_nSize );

	    if( !newBase )
        {
            return false;
        }

        m_pBase = newBase;

        if( m_Head != 0 )
        {
            // 拷贝内容并调整tail的位置
            memcpy( (void*)(m_pBase+pos), (void*)m_pBase, m_Tail * sizeof(T) );
            m_Tail += pos;
        }

        return true;
    }

    ////////////////////////////////////////////////////////

    template <class T, class LockType = CLock>
    class CSafeQueue : public CQueue<T>
    {
    public:
        CSafeQueue(s32 nSize);
        ~CSafeQueue();

        bool  Useful();
        bool  Empty();
        s32   Size();
        bool  Push( T );
        bool  Pop( T& );
        void  Clear();

    private:
        LockType m_Lock;

    };

    template <class T, class LockType>
    CSafeQueue<T, LockType>::CSafeQueue(s32 nSize) : 
        CQueue<T>( nSize )
    {
    }
        
    template <class T, class LockType>
    CSafeQueue<T, LockType>::~CSafeQueue()
    {
    }

    template <class T, class LockType>
    bool CSafeQueue<T, LockType>::Useful()
    {
        CGuardLock<LockType> g( m_Lock );
        return CQueue<T>::Useful();
    }

    template <class T, class LockType>
    bool CSafeQueue<T, LockType>::Empty()
    {
        CGuardLock<LockType> g( m_Lock );
        return CQueue<T>::Empty();
    }

    template <class T, class LockType>
    s32 CSafeQueue<T, LockType>::Size()
    {
        CGuardLock<LockType> g( m_Lock );
        return CQueue<T>::Size();
    }
        
    template <class T, class LockType>
    bool CSafeQueue<T, LockType>::Push( T elem )
    {
        CGuardLock<LockType> g( m_Lock );
        return CQueue<T>::Push( elem );
    }

    template <class T, class LockType>
    bool CSafeQueue<T, LockType>::Pop( T &elem )
    {
        CGuardLock<LockType> g( m_Lock );
        return CQueue<T>::Pop( elem );
    }

    template <class T, class LockType>
    void CSafeQueue<T, LockType>::Clear()
    {
        CGuardLock<LockType> g( m_Lock );
        CQueue<T>::Clear();
    }
};

#endif
