#include "OverlappedPoolEx.h"

#ifdef _USE_WIN32
#include "StackTrace.h"
using namespace SGLib;

COverlappedPoolEx::COverlappedPoolEx(s32 size) : 
	m_pool( size ),
	m_buffer( size )
{
}

COverlappedPoolEx::~COverlappedPoolEx()
{
	_ReleaseOverlappeds();
}

Overlapped_t* COverlappedPoolEx::Get()
{
	Overlapped_t *ov = NULL;
	if( !m_pool.Empty() )
	{
		bool ret = m_pool.Pop( ov );
		SELF_ASSERT( ret, ; );
	}

	if( ov == NULL )
	{
		ov = new Overlapped_t();
		SELF_ASSERT( ov!=NULL, return NULL; );
	}
	
	byte *buf = m_buffer.GetBuffer();
	if( buf != NULL )
	{
		ov->m_sWsabuf.buf = (CHAR*)buf;
		ov->m_sWsabuf.len = m_buffer.GetBufferLen();
	}
	else
	{
		bool ret = m_pool.Push( ov );
		SELF_ASSERT( ret, return NULL; );
	}

	return ov;
}

void COverlappedPoolEx::Put(Overlapped_t &ov)
{
	SELF_ASSERT( ov.m_sWsabuf.buf != NULL, return; );
	m_buffer.PutBuffer( (byte*)ov.m_sWsabuf.buf );
	bool ret = m_pool.Push( &ov );
	SELF_ASSERT( ret, return; );
}

void COverlappedPoolEx::_ReleaseOverlappeds()
{
	while( !m_pool.Empty() )
	{
		Overlapped_t *ov;
		if( true == m_pool.Pop( ov ) )
		{
			SAFE_DELETE( ov );
		}
		else
		{
			break;
		}
	}
}

#endif
