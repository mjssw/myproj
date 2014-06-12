#include "CycleBuffer.h"
using namespace SGLib;

CCycleBuffer::CCycleBuffer() 
{
	m_nHead = 0;
	m_nTail = 0;
	m_nSize = 0;
	m_nCapacity = 0;
	m_Buffer = 0;
}

CCycleBuffer::~CCycleBuffer()
{
	SAFE_DELETE( m_Buffer );
}

void CCycleBuffer::Init()
{ 
	m_nHead = 0;
	m_nTail = 0;
	m_nSize = 0;
	memset( m_Buffer, 0, m_nCapacity );
}

bool CCycleBuffer::Initialize(s32 nSize)
{
	m_nCapacity = nSize;
	m_Buffer = new byte[m_nCapacity];
	return (m_Buffer != NULL);
}

s32 CCycleBuffer::GetSize()
{
	return m_nSize;
}

s32 CCycleBuffer::GetCapacity()
{
	return m_nCapacity;
}

// 获取连续的空闲区长度
s32 CCycleBuffer::GetFreeLen()
{
	int nLen = 0;
	if( m_nTail > m_nHead )
	{
		nLen = m_nCapacity - m_nTail;
	}
	else if( m_nTail < m_nHead )
	{
		nLen = m_nHead - m_nTail;
	}
	else
	{
		if( m_nSize == 0 )
		{
			nLen = m_nCapacity - m_nTail;
		}
	}

	return nLen;
}

byte* CCycleBuffer::GetFreePtr()
{
	byte *pPos = m_Buffer + m_nTail; 
	if( m_nTail==m_nHead && m_nSize>0 )
	{
		//pPos = NULL;
	}

	return pPos;
}

// 得到连续的数据区的长度
s32 CCycleBuffer::GetDataLen()
{
	int nLen = 0;
	if( m_nTail > m_nHead )
	{
		nLen = m_nTail - m_nHead;
	}
	else if( m_nTail < m_nHead )
	{
		nLen = m_nCapacity - m_nHead;
	}
	else
	{
		if( m_nSize > 0 )
		{
			nLen = m_nCapacity - m_nHead;
		}
	}

	return nLen;
}

byte* CCycleBuffer::GetDataPtr()
{
	byte *pPos = m_Buffer + m_nHead; 
	if( m_nTail==m_nHead && m_nSize==0 )
	{
		//pPos = NULL;
	}

	return pPos;
}

bool CCycleBuffer::DataIsContinue()
{
	if( m_nHead>m_nTail || (m_nHead==m_nTail && m_nSize>0) )
	{
		return false;
	}

	return true;
}

s32 CCycleBuffer::AddFreeSize(s32 nFreeSize)
{
	if( nFreeSize <= 0 )
	{
		return nFreeSize;
	}

	s32 nRealAdd = nFreeSize;
	if( m_nTail > m_nHead )
	{
		s32 nLen = m_nTail - m_nHead;
		if( nLen < nRealAdd )
		{
			nRealAdd = nLen;
		}
		m_nHead += nRealAdd;
		m_nSize -= nRealAdd;
	}
	else if( m_nTail<m_nHead || (m_nTail==m_nHead && m_nSize>0) )
	{
		s32 nLen1 = m_nCapacity - m_nHead;
		if( nLen1 >= nRealAdd )
		{
			m_nHead += nRealAdd;
			if( m_nHead >= m_nCapacity )
			{
				m_nHead = 0;
			}
			m_nSize -= nRealAdd;
		}
		else
		{
			/*
			s32 nNewFreeSize = nFreeSize - nLen1;
			m_nHead = 0;
			m_nSize -= nLen1;
			nRealAdd = nLen1 + AddFreeSize( nNewFreeSize );
			//*/

			s32 nNewFreeSize = nRealAdd - nLen1;
			s32 nLen2 = (m_nTail > nNewFreeSize) ?  nNewFreeSize : m_nTail;
			m_nHead = nLen2;
			nRealAdd = (nLen2 + nLen1);
			m_nSize -= nRealAdd;
		}
	}

	return nRealAdd;
}

s32 CCycleBuffer::AddDataSize(s32 nDataSize, const byte *pData)
{
	if( nDataSize <= 0 )
	{
		return 0;
	}

	s32 nRealAdd = nDataSize;
	if( m_nHead > m_nTail )
	{
		s32 nLen = m_nHead - m_nTail;
		if( nLen < nRealAdd )
		{
			nRealAdd = nLen;
		}

		if( pData )
		{
			memcpy( m_Buffer+m_nTail, pData, nRealAdd );
			pData += nRealAdd;
		}

		m_nTail += nRealAdd;
		m_nSize += nRealAdd;
	}
	else if( m_nTail>m_nHead || (m_nTail==m_nHead && m_nSize==0) )
	{
		s32 nLen1 = m_nCapacity - m_nTail;
		if( nLen1 >= nRealAdd )
		{
			if( pData )
			{
				memcpy( m_Buffer+m_nTail, pData, nRealAdd );
				pData += nRealAdd;
			}

			m_nTail += nRealAdd;
			if( m_nTail >= m_nCapacity )
			{
				m_nTail = 0;
			}
			m_nSize += nRealAdd;
		}
		else
		{
			if( pData )
			{
				memcpy( m_Buffer+m_nTail, pData, nLen1 );
				pData += nLen1;
			}

			/*
			m_nTail = 0;
			m_nSize += nLen1;
			s32 nNewDataSize = nDataSize - nLen1;
			nRealAdd = nLen1 + AddDataSize( nNewDataSize, pData );
			//*/
			
			s32 nNewDataSize = nRealAdd - nLen1;
			s32 nLen2 = (m_nHead > nNewDataSize) ?  nNewDataSize : m_nHead;
			m_nTail = nLen2;
			nRealAdd = (nLen2 + nLen1);
			m_nSize += nRealAdd;
			if( pData )
			{
				memcpy( m_Buffer, pData, nLen2 );
			}
		}
	}
	else
	{
		return 0;
	}

	return nRealAdd;
}

void CCycleBuffer::Arrange()
{
	// TODO
}

void CCycleBuffer::DisplayInfo()
{
	printf( "Capacity=%d Size=%d Head=%d Tail=%d\n",
		m_nCapacity, m_nSize, m_nHead, m_nTail );
}

void CCycleBuffer::Dump(void *data)
{
	byte *pData = (byte*)data;
	*((s32*)pData) = m_nCapacity;			pData += 4;
	*((s32*)pData) = m_nSize;				pData += 4;
	*((s32*)pData) = m_nHead;				pData += 4;
	*((s32*)pData) = m_nTail;				pData += 4;
}

void CCycleBuffer::DumpStr(char *info)
{
	sprintf( info, "cap=%d\tsz=%d\thead=%d\ttail=%d\t",
		m_nCapacity, 
		m_nSize,
		m_nHead,
		m_nTail );
}

void CCycleBuffer::DumpStrAll(char *info)
{
	int len = m_nCapacity* 4;
	char *data = new char[len];
	memset( data, 0, len );

	for(int i=0; i<m_nCapacity; ++i)
	{
		char _tmp[16] = {0};
		sprintf( _tmp, "%.2X ", m_Buffer[i] );
		strcat(data, _tmp);
	}

	sprintf( info, "cap=%d\tsz=%d\thead=%d\ttail=%d\t\n\tdat=%s",
		m_nCapacity, 
		m_nSize,
		m_nHead,
		m_nTail,
		data );

	delete [] data;
}
