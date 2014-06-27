#ifndef _SG_TIMER_H_
#define _SG_TIMER_H_

#include "Define.h"
#include "List.h"
#include "Thread.h"
#include "Lock.h"
#include "SetupSocket.h"
#include "stacktrace.h"
#include "log.h"

namespace SGLib
{
    /*
     * just like timer in linux kernel
     */

    #define TVN_BITS    6
    #define TVR_BITS    8
    #define TVN_SIZE    (1 << TVN_BITS)
    #define TVR_SIZE    (1 << TVR_BITS)
    #define TVN_MASK    (TVN_SIZE - 1)
    #define TVR_MASK    (TVR_SIZE - 1)
    #define INDEX(N)    ((m_u64LastCheckTime >> (TVR_BITS + (N) * TVN_BITS)) & TVN_MASK)

    typedef void (*CallBackFunc)(void *pData, s32 nDataLen);

    template<s32 MaxTimerCount>
    class CTimer : public IRunnable
    {
    public:
        enum
        {
            E_Timer_UserDataLen = 1024,
            E_Invalid_TimerId   = -1,
        };

        CTimer();
        ~CTimer();

        bool  Start();
        void  Stop();
        s32   AddTimer( u32 uElapse, CallBackFunc Func, void *pUserData, s32 nUserDataLen, bool bPersist=false );
        void  DelTimer( s32 nTimerId );

        virtual void Run();

        static u64 GetCurMSec()
        {
        #ifdef _USE_WIN32
            return timeGetTime();
        #else
            struct timeval tv;
            gettimeofday( &tv, NULL );
            return ((u64)(tv.tv_sec) * 1000 + (tv.tv_usec/1000)); 
        #endif
        }

        void ShowTimer();

    private:
        u64         m_u64LastCheckTime;
        bool        m_bStop;
        CThread     m_WorkThread;
        class CTimerData
        {
        public:
            s32  m_nTimerId;
            u32  m_uElapse;
            u64  m_u64Expire;
            s32  m_UserDataLen;
            byte m_UserData[E_Timer_UserDataLen];
            s8	 m_nFlag;
            bool m_bIsRunning;    // if is calling timer-callback now
            CallBackFunc m_nCallBackFunc;
			
			CInUseList<CTimerData, MaxTimerCount> *m_pUseList;
            
            void SetFlag(bool bPersist)
            {
                m_nFlag = (bPersist==true) ? 1 : 0;
            }
       
            bool IsPersistTimer()
            { 
                return (m_nFlag==1);
            }

            void ClearPersistFlag()
            {
                m_nFlag = 0;
            }
        };
        CDataList<CTimerData, MaxTimerCount> m_Timers;

        CInUseList<CTimerData, MaxTimerCount> m_TimerList1[TVR_SIZE];
        CInUseList<CTimerData, MaxTimerCount> m_TimerList2[TVN_SIZE];
        CInUseList<CTimerData, MaxTimerCount> m_TimerList3[TVN_SIZE];
        CInUseList<CTimerData, MaxTimerCount> m_TimerList4[TVN_SIZE];
        CInUseList<CTimerData, MaxTimerCount> m_TimerList5[TVN_SIZE];
   
        CLock m_TimerListLock;

    private:
        void _InternalAddTimer(const CTimerData &Data);
        void _RunTimers();

        CInUseList<CTimerData, MaxTimerCount> *_FindInUserList(const CTimerData &Data)
        {
            u32 u32Idx = (u32)(Data.m_u64Expire - m_u64LastCheckTime);
            u32 u32Expires = (u32)Data.m_u64Expire;

            CInUseList<CTimerData, MaxTimerCount> *pList = NULL;
            if( u32Idx < TVR_SIZE )
            {
                s32 i = u32Expires & TVR_MASK;
                pList = &m_TimerList1[ i ];
            }
            else if( u32Idx < (1 << (TVR_BITS + TVN_BITS)) )
            {
                s32 i = (u32Expires >> TVR_BITS) & TVN_MASK;
                pList = &m_TimerList2[ i ];
            }
            else if( u32Idx < (1 << (TVR_BITS + 2 * TVN_BITS)) )
            {
                s32 i = (u32Expires >> (TVR_BITS + TVN_BITS)) & TVN_MASK;
                pList = &m_TimerList3[ i ];
            }
            else if( u32Idx < (1 << (TVR_BITS + 3 * TVN_BITS)) )
            {
                s32 i = (u32Expires >> (TVR_BITS + 2 * TVN_BITS)) & TVN_MASK;
                pList = &m_TimerList4[ i ];
            }
            else
            {
                s32 i = (u32Expires >> (TVR_BITS + 3 * TVN_BITS)) & TVN_MASK;
                pList = &m_TimerList5[ i ];
            }

            return pList;
        }

        void _SetExpire(u32 uElapse, u64 &u64Expire)
        {
            u64Expire = GetCurMSec() + uElapse;
        }

        u32 _Cascade(CInUseList<CTimerData, MaxTimerCount> List[], u32 u32Idx)
        {
            s32 nListHead = E_Invalid_ListNode_Id;
            List[ u32Idx ].ReplaceAndClear( nListHead );

            s32 nId = nListHead;
            while( nId != E_Invalid_ListNode_Id )
            {
                CTimerData *pData = m_Timers.GetData( nId );
                SG_ASSERT( pData != NULL );

				// 注意这里要先获取一下后一个timerid，
				// 否则下面的_InternalAddTimer 操作会修改掉当前timer对象的连接指针,
				// 导致后面的timer都无法正确的被放置到正确的队列中
				s32 nNextId = m_Timers.GetNextId( nId );

                _InternalAddTimer( *pData );

                nId = nNextId;
            }

            return u32Idx;
        }
        
        void _PrintTimerData(void *pData)
        {
            CTimerData *pTimer = (CTimerData*)pData;
            printf( "\tTimerId=%d\n", pTimer->m_nTimerId );
            printf( "\tTimerExprie=%llu\n", pTimer->m_u64Expire );
            printf( "\tTimerExprie=%llX\n", pTimer->m_u64Expire );
            printf( "\tTimerFlag=%d\n", pTimer->m_nFlag );
            printf( "\tTimerElapse=%u\n", pTimer->m_uElapse );
        }
    };

//////////////////////////////////////////////////////////////////////////////////////

    template<s32 MaxTimerCount>
    CTimer<MaxTimerCount>::CTimer() : 
        m_u64LastCheckTime(GetCurMSec()), 
        m_bStop(false),
        m_WorkThread(this, false),
        m_Timers()
    {
        for( s32 i=0; i<TVR_SIZE; ++i )
        {
            m_TimerList1[i].Init( m_Timers );
        }
       
        #define _INIT_TIMER_LIST(n) do{\
            for( s32 i=0; i<TVN_SIZE; ++i )\
            {\
                m_TimerList ## n[i].Init( m_Timers );\
            }\
        }while(0);

        _INIT_TIMER_LIST(2);
        _INIT_TIMER_LIST(3);
        _INIT_TIMER_LIST(4);
        _INIT_TIMER_LIST(5);

        #undef _INIT_TIMER_LIST
    }

    template<s32 MaxTimerCount>
    CTimer<MaxTimerCount>::~CTimer()
    {
        Stop();
    }

    template<s32 MaxTimerCount>
    bool CTimer<MaxTimerCount>::Start()
    {
        return m_WorkThread.Start();
    }
    
    template<s32 MaxTimerCount>
    void CTimer<MaxTimerCount>::Stop()
    {
        m_bStop = true;
        m_WorkThread.Stop();
    }
    
    template<s32 MaxTimerCount>
    s32 CTimer<MaxTimerCount>::AddTimer(u32 uElapse, CallBackFunc Func, void *pUserData, s32 nUserDataLen, bool bPersist)
    {
        if( uElapse == 0 )
        {
            return E_Invalid_TimerId;
        }

        CGuardLock<CLock> g( m_TimerListLock );

        s32 nId = m_Timers.Get();
        if( nId == E_Invalid_ListNode_Id )
        {
            return E_Invalid_TimerId;
        }

        CTimerData *pData = m_Timers.GetData( nId );
        SELF_ASSERT( pData != NULL, return E_Invalid_TimerId; );

        pData->m_nTimerId = nId;
        pData->m_uElapse = uElapse;
        _SetExpire( uElapse, pData->m_u64Expire );
        pData->SetFlag( bPersist );
        pData->m_nCallBackFunc = Func;
        pData->m_UserDataLen = (nUserDataLen>E_Timer_UserDataLen) ? E_Timer_UserDataLen : nUserDataLen;
        if( pData->m_UserDataLen > 0 )
        {
            memcpy( pData->m_UserData, pUserData, pData->m_UserDataLen );
        }
        pData->m_bIsRunning = false;

        _InternalAddTimer( *pData );

		CLog::DebugLog( "CTimer::AddTimer %d", nId );

        return nId;
    }
    
    template<s32 MaxTimerCount>
    void CTimer<MaxTimerCount>::DelTimer( s32 nTimerId )
    {
        CGuardLock<CLock> g( m_TimerListLock );

        bool bInUse = m_Timers.NodeIsInUse( nTimerId );
        if( bInUse == false )
        {
            return;
        }

        CTimerData *pData = m_Timers.GetData( nTimerId );
        SELF_ASSERT( pData != NULL, return; );

        if( pData->m_bIsRunning == false )
        {
			// 通过 _FindInUserList 找到时钟所在的队列有问题的:
			// 一个实际的情况：5秒间隔的时钟在时刻:270524913(0x101FE1F1)加入到时钟队列中, 其到期时间为:270529913(0x101FF579).
			// 加入的时钟队列在 m_TimerList2[53]里面(参加_FindInUserList函数)。
			// 此时时刻点(m_u64LastCheckTime)继续往前走，当m_u64LastCheckTime达到0x101FF500时，
			// 在  _RunTimers::_Cascade 函数才会将此时钟从m_TimerList2移动到m_TimerList1对应的位置上，
			// 但是如果在此时刻点之前例如270529702(0x101FF4A6)时删除此时钟，
			// 如果使用 _FindInUserList 查找时钟所在队列则会找到m_TimerList1中的一个队列中，导致删除时assert掉.

            //CInUseList<CTimerData, MaxTimerCount> *pList = _FindInUserList( *pData );
			CInUseList<CTimerData, MaxTimerCount> *pList = pData->m_pUseList;
            SELF_ASSERT( pList != NULL, return; );

            bool bRet = pList->Delete( nTimerId );
            SELF_ASSERT( bRet == true, ; );

            m_Timers.Put( nTimerId );
        }
        else
        {
            pData->ClearPersistFlag();
        }
		
		CLog::DebugLog( "CTimer::DelTimer %d", nTimerId );
    }

    template<s32 MaxTimerCount>
    void CTimer<MaxTimerCount>::Run()
    {
        CSetupSocket::Instance().Setup();
            
        struct timeval TimeOut;
        TimeOut.tv_sec = 0;
        TimeOut.tv_usec = 100;

        #ifdef _USE_WIN32
            SOCKET sock = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP );
        #endif

        while( m_bStop != true )
        {
            #ifdef _USE_WIN32
            fd_set fd;
            FD_ZERO( &fd );
			FD_SET( sock, &fd );
            s32 nRet = select( 0, NULL, NULL, &fd, &TimeOut );
            #else
            s32 nRet = select( 0, NULL, NULL, NULL, &TimeOut );
            #endif
            SG_ASSERT( nRet == 0 );

            u32 nCount = (u32)(GetCurMSec() - m_u64LastCheckTime);
            for( u32 i=0; i<nCount; ++i )
            {
                _RunTimers();
            }
        }
		
        #ifdef _USE_WIN32
            closesocket( sock );
        #endif
        CSetupSocket::Instance().Clean();
    }

    template<s32 MaxTimerCount>
    void CTimer<MaxTimerCount>::ShowTimer()
    {
        SGDEBUG( "---[LastCheckTime : %llu(%llX)]---\n", m_u64LastCheckTime ,m_u64LastCheckTime );

        for( s32 i=0; i<TVR_SIZE; ++i )
        {
            s32 nSize = m_TimerList1[i].Size();
            if( nSize > 0 )
            {
                SGDEBUG( "TimerList1[%d] has %d timers\n", i, nSize );
                s32 nId = m_TimerList1[i].GetHead();
                while( nId != E_Invalid_ListNode_Id )
                {
                    CTimerData *pData = m_Timers.GetData( nId );
                    SELF_ASSERT( pData != NULL, continue; );
                    _PrintTimerData( pData );
                    SGDEBUG( "\n" );
                    nId = m_Timers.GetNextId( nId );
                }
            }
        }
       
        #define _DISPLAY_TIMER_LIST(n) do{\
            for( s32 i=0; i<TVN_SIZE; ++i )\
            {\
                s32 nSize = m_TimerList ## n[i].Size();\
                if( nSize > 0 )\
                {\
                    SGDEBUG( "TimerList%d[%d] has %d timers\n", n, i, nSize );\
                    s32 nId = m_TimerList ## n[i].GetHead();\
                    while( nId != E_Invalid_ListNode_Id )\
                    {\
                        CTimerData *pData = m_Timers.GetData( nId );\
                        SELF_ASSERT( pData != NULL, continue; );\
                        _PrintTimerData( pData );\
                        SGDEBUG( "\n" );\
                        nId = m_Timers.GetNextId( nId );\
                    }\
                }\
            }\
        }while(0);

        _DISPLAY_TIMER_LIST(2);
        _DISPLAY_TIMER_LIST(3);
        _DISPLAY_TIMER_LIST(4);
        _DISPLAY_TIMER_LIST(5);

        #undef _DISPLAY_TIMER_LIST
    }
        
    template<s32 MaxTimerCount>
    void CTimer<MaxTimerCount>::_InternalAddTimer(const CTimerData &Data)
    {
        s32  nTimerId = Data.m_nTimerId;

        CInUseList<CTimerData, MaxTimerCount> *pList = _FindInUserList( Data );
        SELF_ASSERT( pList != NULL, return; );

        bool bRet = pList->Insert( nTimerId );
        SELF_ASSERT( bRet == true, return; );
       
		/*
        SGDEBUG( "-----AddTimer(%d)------\n", Data.m_nTimerId );
        SGDEBUG( "LastCheck : %llu\n", m_u64LastCheckTime );
        SGDEBUG( "ElapseTime: %lu\n", Data.m_uElapse );
        SGDEBUG( "ExpireTime: %llu\n", Data.m_u64Expire );
		//*/
    }
        
    template<s32 MaxTimerCount>
    void CTimer<MaxTimerCount>::_RunTimers()
    {
        CGuardLock<CLock> g( m_TimerListLock );

        m_u64LastCheckTime += 1;
        u32 u32Idx = ((u32)m_u64LastCheckTime) & TVR_MASK; 

        if( !u32Idx && 
            !_Cascade(m_TimerList2, INDEX(0)) &&
            !_Cascade(m_TimerList3, INDEX(1)) &&
            !_Cascade(m_TimerList4, INDEX(2)) 
          )
        {
            _Cascade( m_TimerList5, INDEX(3) );
        }
        
        s32 nListHead = E_Invalid_ListNode_Id;
        m_TimerList1[ u32Idx ].ReplaceAndClear( nListHead );

        s32 nId = nListHead;
        while( nId != E_Invalid_ListNode_Id )
        {
            CTimerData *pData = m_Timers.GetData( nId );
            SELF_ASSERT( pData != NULL, continue; );

            if( pData->m_nCallBackFunc )
            {
				//CLog::DebugLog( "CTimer::_RunTimers CallBack timerId=%d u32Idx=%u lastCheck=%llu Now=%llu Expire=%llu", 
                //   pData->m_nTimerId, u32Idx, m_u64LastCheckTime, GetCurMSec(), pData->m_u64Expire );
                
                pData->m_bIsRunning = true;
                pData->m_nCallBackFunc( pData->m_UserData, pData->m_UserDataLen );
                pData->m_bIsRunning = false;
            }

            s32 nOldId = nId;
            nId = m_Timers.GetNextId( nId );

            if( pData->IsPersistTimer() )
            {
                _SetExpire( pData->m_uElapse, pData->m_u64Expire );
                _InternalAddTimer( *pData );
            }
            else
            {
                m_Timers.Put( nOldId );
            }

        }
    }

};

#endif

