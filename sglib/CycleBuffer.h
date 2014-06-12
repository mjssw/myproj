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

		// ��ȡ��Ч�������Ĵ�С������������������
		s32   GetSize();
		s32   GetCapacity();

		// ��ȡ�������л������ĳ��Ⱥ���ʼλ��
		s32   GetFreeLen();
		byte* GetFreePtr();

		// ��ȡ�����������������ĳ��Ⱥ���ʼλ��
		s32   GetDataLen();
		byte* GetDataPtr();

		// ��Ч���������Ƿ���������
		bool  DataIsContinue();

		// ���ӿ���������
		s32   AddFreeSize(s32 nFreeSize);

		// ������Ч���ݻ���
		s32   AddDataSize(s32 nDataSize, const byte *pData=NULL);

        // ԭʼ������λ��
        byte* GetBuffer(){ return m_Buffer; }

		// ��������, ���ݴ�����0��ʼ
		void  Arrange();

		// ���������Ϣ
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
