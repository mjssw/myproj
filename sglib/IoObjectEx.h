#ifndef _IO_OBJECT_EX_H_
#define _IO_OBJECT_EX_H_

#include "Define.h"

namespace SGLib
{
	struct SEventObj
	{
		enum
		{
			E_Event_AcceptClient = 101,
			E_Event_RecvData,
			E_Event_ClientClose,
			E_Event_ConnectSuccess,
			E_Event_ConnectError,
			E_Event_Timer,
		};

		s32 type;
		union
		{
			s32 param1;
			u32 param2;
			s64 param3;
			u64 param4;
			void *ptr;
		} param;
	};

    class CIoServiceEx;
	class CIoObjectEx
	{
	public:
        friend class CIoServiceEx;
		virtual ~CIoObjectEx(){}
		virtual void HandleRead(s32 size, void *data){}
		virtual void HandleWrite(s32 size, void *data){}
		virtual void HandleError(void *data){}
		virtual s32 GetObjectId(){ return 0; }
		virtual s32 GetObjectSocket(){ return 0; }

	protected:
#ifndef _USE_WIN32
		virtual s32 _DoHandleRead(){ return 0; }
		virtual s32 _DoHandleWrite(){ return 0; }
#endif
	};
};

#endif
