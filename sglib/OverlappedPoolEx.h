#ifndef _OVERLAPPED_POOL_EX_H_
#define _OVERLAPPED_POOL_EX_H_

#ifdef _USE_WIN32

#include "Queue.h"
#include "BufferPool.h"
#include "IoSocketEx.h"

namespace SGLib
{
	class COverlappedPoolEx
	{
	public:
		COverlappedPoolEx(s32 size);
		~COverlappedPoolEx();
		Overlapped_t* Get();
		void Put(Overlapped_t &ov);

	private:
		void _ReleaseOverlappeds();

	private:
		CSafeQueue<Overlapped_t*> m_pool;
		CBufferPool<CIoSocketEx::E_MaxPkg_Len> m_buffer;
	};
};

#else

#include "Define.h"

namespace SGLib
{
	class COverlappedPoolEx
	{
	public:
		COverlappedPoolEx(s32 size){}
		~COverlappedPoolEx(){}
	};
};

#endif

#endif
