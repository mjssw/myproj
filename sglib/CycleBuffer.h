#ifndef _SG_CYCLE_BUFFER_H_
#define _SG_CYCLE_BUFFER_H_

#include "Define.h"
#include <string.h>

namespace SGLib
{
	class CCycleBuffer
	{
	public:
		CCycleBuffer();
		~CCycleBuffer();

		void  Init();
		bool  Initialize(s32 nSize);

		// 获取有效数据区的大小和整个缓冲区的容量
		s32   GetSize();
		s32   GetCapacity();

		// 获取连续空闲缓冲区的长度和起始位置
		s32   GetFreeLen();
		byte* GetFreePtr();

		// 获取连续数据区缓冲区的长度和起始位置
		s32   GetDataLen();
		byte* GetDataPtr();

		// 有效的数据区是否是连续的
		bool  DataIsContinue();

		// 增加空闲区长度
		s32   AddFreeSize(s32 nFreeSize);

		// 增加有效数据缓冲
		s32   AddDataSize(s32 nDataSize, const byte *pData=NULL);

        // 原始缓冲区位置
        byte* GetBuffer(){ return m_Buffer; }

		// 整理缓冲区, 数据从索引0开始
		void  Arrange();

		// 调试输出信息
		void  DisplayInfo();
		void  Dump(void *data);
		void  DumpStr(char *info);
		void  DumpStrAll(char *info);

	private:
		byte *m_Buffer;
		s32	 m_nHead;
		s32	 m_nTail;
		s32	 m_nSize;
		s32	 m_nCapacity;
	};
};

#endif
