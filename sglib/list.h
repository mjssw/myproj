#ifndef _SG_LIST_H_
#define _SG_LIST_H_

#include "Define.h"
#include "StackTrace.h"

namespace SGLib
{
    enum
    {
        E_Invalid_ListNode_Id = -1,
    };

    typedef void (*PrintNodeDataFunc)(void *pData);

    template<class DataType>
    class CListNode
    {
    public:
        CListNode() : 
            m_nNext(E_Invalid_ListNode_Id),  
            m_nPrev(E_Invalid_ListNode_Id), 
            m_bInUse(false)
        {}

        s32         m_nNext;
        s32         m_nPrev;
        bool        m_bInUse;
        DataType    m_Data;
    };

    template<class DataType, s32 MaxNodeCount>
    class CDataList
    {
    public:
        template<class Type, s32 Count>
        friend class CInUseList;

        CDataList();
        ~CDataList(){};

        s32         Get();
        void        Put(s32 nId);

        DataType*   GetData(s32 nId);
        s32         GetNextId(s32 nCurId);
        s32         GetPrevId(s32 nCurId);
        bool        NodeIsInUse(s32 nId);

        void        DisplayInfo();
    private:
        CListNode<DataType>* GetNode(s32 nId);
    private:
        s32                 m_nFirstFreeNode;
        CListNode<DataType> m_Nodes[MaxNodeCount];
    };
   
    template<class DataType, s32 MaxNodeCount>
    class CInUseList
    {
    public:

        CInUseList() : 
            m_bInit(false),
            m_nListHeadNode(E_Invalid_ListNode_Id),
            m_nSize(0),
            m_pDataList(NULL)
        {}

        CInUseList(CDataList<DataType, MaxNodeCount> &DataList) : 
            m_bInit(true),
            m_nListHeadNode(E_Invalid_ListNode_Id),
            m_nSize(0),
            m_pDataList(&DataList)
        {}
    
        void  Init(CDataList<DataType, MaxNodeCount> &DataList);
        bool  Insert(s32 nId);
        bool  Delete(s32 nId);
        s32   Size(){ return m_nSize; }
        s32   GetHead(){ return m_nListHeadNode; }
        void  ReplaceAndClear(s32 &nListHead);
        void  DisplayInfo(PrintNodeDataFunc PrintData);
    private:
        bool                                m_bInit;
        s32                                 m_nListHeadNode;
        s32                                 m_nSize;
        CDataList<DataType, MaxNodeCount>  *m_pDataList;
    };

    //////////////////////////////////////////////////////////////////////////

    template<class DataType, s32 MaxNodeCount>
    CDataList<DataType, MaxNodeCount>::CDataList()
    {
        m_nFirstFreeNode = 0;
        for( s32 i=0; i<MaxNodeCount; ++i )
        {
            m_Nodes[i].m_nNext = (i==MaxNodeCount-1) ? E_Invalid_ListNode_Id : (i+1);
            m_Nodes[i].m_nPrev = (i==0) ? E_Invalid_ListNode_Id : (i-1);
            m_Nodes[i].m_bInUse = false;
			m_Nodes[i].m_Data.m_pUseList = NULL;
        }
    }
    
    template<class DataType, s32 MaxNodeCount>
    s32 CDataList<DataType, MaxNodeCount>::Get()
    {
        if( m_nFirstFreeNode == E_Invalid_ListNode_Id )
        {
            return E_Invalid_ListNode_Id;
        }

        s32 nId = m_nFirstFreeNode;
        m_Nodes[nId].m_bInUse = true;
        m_nFirstFreeNode = m_Nodes[nId].m_nNext; 
        return nId;
    }
    
    template<class DataType, s32 MaxNodeCount>
    void CDataList<DataType, MaxNodeCount>::Put(s32 nId)
    {
        if( nId<0 || nId>=MaxNodeCount || m_Nodes[nId].m_bInUse==false )
        {
            return;
        }

        m_Nodes[nId].m_nNext = m_nFirstFreeNode;
        m_Nodes[nId].m_bInUse = false;
		m_Nodes[nId].m_Data.m_pUseList = NULL;
        m_nFirstFreeNode = nId;
    }
    
    template<class DataType, s32 MaxNodeCount>
    DataType* CDataList<DataType, MaxNodeCount>::GetData(s32 nId)
    {
        CListNode<DataType> *pNode = GetNode( nId );
        if( !pNode || pNode->m_bInUse==false )
        {
            return NULL;
        }

        return &pNode->m_Data;
    }
    
    template<class DataType, s32 MaxNodeCount>
    s32 CDataList<DataType, MaxNodeCount>::GetNextId(s32 nCurId)
    {
        CListNode<DataType> *pNode = GetNode( nCurId );
        if( !pNode )
        {
            return E_Invalid_ListNode_Id;
        }

        return pNode->m_nNext;
    }

    template<class DataType, s32 MaxNodeCount>
    s32 CDataList<DataType, MaxNodeCount>::GetPrevId(s32 nCurId)
    {
        CListNode<DataType> *pNode = GetNode( nCurId );
        if( !pNode )
        {
            return E_Invalid_ListNode_Id;
        }

        return pNode->m_nPrev;
    }

    template<class DataType, s32 MaxNodeCount>
    bool CDataList<DataType, MaxNodeCount>::NodeIsInUse(s32 nId)
    {
        CListNode<DataType> *pNode = GetNode( nId );
        if( !pNode || pNode->m_bInUse==false )
        {
            return false;
        }

        return true;
    }

    template<class DataType, s32 MaxNodeCount>
    void CDataList<DataType, MaxNodeCount>::DisplayInfo()
    {
        for( s32 nId=m_nFirstFreeNode; nId!=E_Invalid_ListNode_Id; )
        {
            printf( "FreeNode : %d\n", nId );
            nId = m_Nodes[ nId ].m_nNext;
        }
    }

    template<class DataType, s32 MaxNodeCount>
    CListNode<DataType>* CDataList<DataType, MaxNodeCount>::GetNode(s32 nId)
    {
        if( nId<0 || nId>=MaxNodeCount )
        {
            return NULL;
        }
        
        return &m_Nodes[nId];
    }
   
    template<class DataType, s32 MaxNodeCount>
    void CInUseList<DataType, MaxNodeCount>::Init(CDataList<DataType, MaxNodeCount> &DataList)
    {
        if( m_bInit != true )
        {
            m_pDataList = &DataList;
            m_bInit = true;
        }
    }

    template<class DataType, s32 MaxNodeCount>
    bool CInUseList<DataType, MaxNodeCount>::Insert(s32 nId)
    {
        if( m_bInit != true )
        {
            return m_bInit;
        }

        SG_ASSERT( m_pDataList != NULL );

        CListNode<DataType> *pNode = m_pDataList->GetNode( nId );
        if( !pNode || pNode->m_bInUse==false )
        {
            return false;
        }

        pNode->m_nPrev = E_Invalid_ListNode_Id;
        pNode->m_nNext = m_nListHeadNode;

        if( m_nListHeadNode != E_Invalid_ListNode_Id )
        {
            CListNode<DataType> *pHead = m_pDataList->GetNode( m_nListHeadNode );
            SG_ASSERT( pHead != NULL );
            pHead->m_nPrev = nId;
        }

        m_nListHeadNode = nId;
        ++m_nSize;

		pNode->m_Data.m_pUseList = this;

        return true;
    }
    
    template<class DataType, s32 MaxNodeCount>
    bool CInUseList<DataType, MaxNodeCount>::Delete(s32 nId)
    {
        if( m_bInit != true )
        {
            return m_bInit;
        }

        SG_ASSERT( m_pDataList != NULL );

        CListNode<DataType> *pNode = m_pDataList->GetNode( nId );
        if( !pNode || pNode->m_bInUse==false )
        {
            return false;
        }

        if( nId == m_nListHeadNode )
        {
			SG_ASSERT( pNode->m_nPrev == E_Invalid_ListNode_Id );
            m_nListHeadNode = pNode->m_nNext;
            if( m_nListHeadNode != E_Invalid_ListNode_Id )
            {
                CListNode<DataType> *pNode = m_pDataList->GetNode( m_nListHeadNode );
			    SG_ASSERT( pNode != NULL );
                pNode->m_nPrev = E_Invalid_ListNode_Id;
            }
        }
        else
        {
			SG_ASSERT( pNode->m_nPrev != E_Invalid_ListNode_Id );  
            s32 nPrev = pNode->m_nPrev;
            s32 nNext = pNode->m_nNext;

            CListNode<DataType> *pPrevNode = m_pDataList->GetNode( nPrev );
			SG_ASSERT( pPrevNode != NULL );  
            pPrevNode->m_nNext = nNext;

            if( nNext != E_Invalid_ListNode_Id )
            {
                CListNode<DataType> *pNextNode = m_pDataList->GetNode( nNext );
			    SG_ASSERT( pNextNode != NULL );
                pNextNode->m_nPrev = nPrev;
            }
        }

        --m_nSize;
		
		pNode->m_Data.m_pUseList = NULL;

        return true;
    }
        
    template<class DataType, s32 MaxNodeCount>
    void CInUseList<DataType, MaxNodeCount>::ReplaceAndClear(s32 &nListHead)
    {
        nListHead = m_nListHeadNode;
        m_nListHeadNode = E_Invalid_ListNode_Id;
        m_nSize = 0;
    }

    template<class DataType, s32 MaxNodeCount>
    void CInUseList<DataType, MaxNodeCount>::DisplayInfo(PrintNodeDataFunc PrintData)
    {
        s32 nId = m_nListHeadNode;
        printf( "InUseList Size=%d\n", m_nSize );
        while( nId != E_Invalid_ListNode_Id )
        {
            CListNode<DataType> *pNode = m_pDataList->GetNode( nId );
            if( !pNode )
            {
                break;
            }
    
            printf( "Id=%d Next=%d Prev=%d Inuse=%d\n", nId, pNode->m_nNext, pNode->m_nPrev, pNode->m_bInUse );
            if( PrintData )
            {
                printf( "\t" );
                PrintData( (void*)(&pNode->m_Data) );
            }
            nId = pNode->m_nNext;
        }
    }
};

#endif

