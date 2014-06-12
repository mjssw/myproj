#ifndef _SG_BUFFER_POOL_H_
#define _SG_BUFFER_POOL_H_

#include "Define.h"
#include "NonCopyable.h"
#include "Queue.h"

namespace SGLib
{
    template <s32 BufferLen>
    class CBufferPool
    {
    public:
        CBufferPool( s32 nSize ) : m_FreeBuffer( nSize ) 
        {
			m_bufferLen = BufferLen;
		}

        ~CBufferPool()
        {
            Destroy();
        }

        byte* GetBuffer();
        void  PutBuffer( byte* );
		s32   GetBufferLen(){ return m_bufferLen; }

        void  Destroy();

    private:
        void  ReleaseBufferQueue( CSafeQueue<byte*, CLock>& );

    private:
        CSafeQueue< byte*, CLock > m_FreeBuffer;
		s32 m_bufferLen;
    };

    template <s32 BufferLen>
    byte* CBufferPool<BufferLen>::GetBuffer()
    {
        byte *pBuf = NULL;

        if( !m_FreeBuffer.Empty() )
        {
            m_FreeBuffer.Pop( pBuf );
        }

        if( !pBuf )
        {
            pBuf = new byte[ BufferLen ];
            if( !pBuf )
            {
                SGDEBUG( "ERROR: BufferPool::new buffer failed\n" );
                return 0;
            }
        }

        memset( pBuf, 0, BufferLen );

        return pBuf;
    }

    template <s32 BufferLen>
    void CBufferPool<BufferLen>::PutBuffer( byte *pBuf )
    {
        if( true != m_FreeBuffer.Push( pBuf ) )
        {
            SGDEBUG( "ERROR: BufferPool::put buffer failed. Than delete it.\n" );
            SAFE_DELETE( pBuf );
        }
    }

    template <s32 BufferLen>
    void CBufferPool<BufferLen>::Destroy()
    {
        ReleaseBufferQueue( m_FreeBuffer );
    }

    template <s32 BufferLen>
    void CBufferPool<BufferLen>::ReleaseBufferQueue( CSafeQueue<byte*, CLock> &BufferQueue )
    {
        while( !BufferQueue.Empty() )
        {
            byte *pBuf = NULL;
            if( true == BufferQueue.Pop( pBuf ) )
            {
                SAFE_DELETE_ARRAY( pBuf );
            }
            else
            {
                break;
            }
        }
    }
};

#endif
