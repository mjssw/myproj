#ifndef _IO_SOCKET_EX_MANAGER_H_
#define _IO_SOCKET_EX_MANAGER_H_

#include "Define.h"
#include "Lock.h"
#include "IoSocketEx.h"

namespace SGLib
{
	class CIoSocketEx;
	class CIoSocketExMgr
	{
	public:
        enum
        {
            E_IoSocketEx_Free = 0,
            E_IoSocketEx_Inuse,
        };

		CIoSocketExMgr();
		~CIoSocketExMgr();

		bool Init(s32 maxCount, s32 sendSize, s32 recvSize, CIoSocketEx &templateSocket);
		void           Fini();
		CIoSocketEx*   GetFreeIoSocket();
		CIoSocketEx*   FindIoSocket(s32 ioSocketId);
		void           PutIoSocket(CIoSocketEx &ioSocket);
		s32			   GetMaxCount();
        void           PrintFirstNFreeSocket(s32 count);
		void		   DebugShowUseInfo();
		void		   DebugShowFreeInfo();
	private:
		struct SSockListNode
		{
			CIoSocketEx *m_ioSocket;
			s32		     m_nextIoSocket;
			s32		     m_prevIoSocket;
		};
		struct
		{
			s32 m_firstFreeIoSocket;  // ���ж���ͷ
			s32 m_inuseIoSocket;      // ��ʹ�õĶ���ͷ
			s32 m_inuseSize;          // ��ʹ�õĸ���
			s32 m_count;
			SSockListNode *m_ioSocketArray;
		} m_socketMgr;
        CLock   m_lock;
		bool m_isInit;
		s32 m_maxCount;
	};
};

#endif
