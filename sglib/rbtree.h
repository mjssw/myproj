#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include <deque>

namespace SGLib
{
	template<typename T, typename U>
	class CRBNode
	{
	public:
		enum
		{
			E_Color_Black,
			E_Color_Red,
		};

		CRBNode() : m_parent(NULL), m_left(NULL), m_right(NULL), m_color(E_Color_Black)
		{}

		char Color(){ return m_color; }
		void SetColor(char color){ m_color = color; }
		bool IsBlack(){ return (Color() == E_Color_Black); }
		bool IsRed(){ return (Color() == E_Color_Red); }
		void SetBlack(){ SetColor(E_Color_Black); }
		void SetRed(){ SetColor(E_Color_Red); }

		CRBNode* Parent(){ return m_parent; }
		CRBNode* Left(){ return m_left; }
		CRBNode* Right(){ return m_right; }
		CRBNode* GrandParent(){ return ((m_parent != NULL) ? m_parent->GrandParent() : NULL); }
		CRBNode* Uncle()
		{
			CRBNode *gparent = GrandParent();
			if( gparent )
			{
				if( gparent->Left() == Parent() )
				{
					return Parent()->Right();
				}
				else
				{
					return Parent()->Left();
				}
			}
			return NULL;
		}

		void SetParent(CRBNode<T, U> *parent){ m_parent = parent; }
		void SetLeft(CRBNode<T, U> *left){ m_left = left; }
		void SetRight(CRBNode<T, U> *right){ m_right = right; }

		T& Key(){ return m_key; }
		U& Data(){ return m_data; }
		void SetKey(T &key){ m_key = key; }
		void SetData(U &data){ m_data = data; }

	private:
		CRBNode *m_parent;
		CRBNode *m_left;
		CRBNode *m_right;
		char m_color;
		T m_key;
		U m_data;
	};
	
	template<typename T, typename U>
	class CRBTree
	{
	public:
		CRBTree();
		~CRBTree();

		bool Insert(T &key, U &data);
		bool Delete(T &key);
		int Hight();
		U* TryGet(T &key);

		void Display();

	private:
		void _Destory(CRBNode<T, U> *node);
		void _Release(CRBNode<T, U> *node);
		void _RotateLeft(CRBNode<T, U> &node);
		void _RotateRight(CRBNode<T, U> &node);
		CRBNode<T, U>* _FindInsertPos(T &key);
		void _InsertFixup(CRBNode<T, U> &node);
		int _Hight(CRBNode<T, U> *node);
		CRBNode<T, U>* _Search(T &key);
		void _Delete(CRBNode<T, U> &node);
		void _DeleteFixup(CRBNode<T, U> *node, CRBNode<T, U> *parent);

	private:
		CRBNode<T, U> *m_root;
	};

	template<typename T, typename U>
	CRBTree<T, U>::CRBTree() : m_root(NULL)
	{
	}

	template<typename T, typename U>
	CRBTree<T, U>::~CRBTree()
	{
		if( m_root != NULL )
		{
			_Destory( m_root );
		}
	}
		
	template<typename T, typename U>
	bool CRBTree<T, U>::Insert(T &key, U &data)
	{
		if( m_root == NULL )
		{
			CRBNode<T, U> *node = new CRBNode<T, U>();
			if( !node )
			{
				return false;
			}
			node->SetKey( key );
			node->SetData( data );
			node->SetBlack();
			m_root = node;
			return true;
		}

		CRBNode<T, U> *pos = _FindInsertPos( key );
		if( !pos || pos->Key() == key )
		{
			return false;
		}

		CRBNode<T, U> *node = new CRBNode<T, U>();
		if( !node )
		{
			return false;
		}
		node->SetKey( key );
		node->SetData( data );
		node->SetRed();
		node->SetParent( pos );

		if( pos->Key() < node->Key() )
		{
			pos->SetRight( node );
		}
		else
		{
			pos->SetLeft( node );
		}

		_InsertFixup( *node );

		return true;
	}

	template<typename T, typename U>
	bool CRBTree<T, U>::Delete(T &key)
	{
		CRBNode<T, U> *node = _Search( key );
		if( !node )
		{
			return false;
		}

		_Delete( *node );
		return true;
	}

	template<typename T, typename U>
	int CRBTree<T, U>::Hight()
	{
		return _Hight( m_root );
	}

	template<typename T, typename U>
	U* CRBTree<T, U>::TryGet(T &key)
	{
		CRBNode<T, U> *node = _Search( key );
		if( !node )
		{
			return NULL;
		}

		return &node->Data();
	}

	template<typename T, typename U>
	void CRBTree<T, U>::_Destory(CRBNode<T, U> *node)
	{
		if( node )
		{
			_Destory( node->Left() );
			_Destory( node->Right() );
			_Release( node );	
		}
	}

	template<typename T, typename U>
	void CRBTree<T, U>::_Release(CRBNode<T,U> *node)
	{
		if( node )
		{
			delete node;
			node = NULL;
		}
	}

	/* 
	*      px                              px
	*     /                               /
	*    x                               y                
	*   /  \      --(左旋)-->           / \                
	*  lx   y                          x  ry     
	*     /   \                       /  \
	*    ly   ry                     lx  ly  
	*/
	template<typename T, typename U>
	void CRBTree<T, U>::_RotateLeft(CRBNode<T,U> &node)
	{
		// param node is x
		CRBNode<T, U> *y = node.Right();
		if( !y )
		{
			return;
		}
		CRBNode<T, U> *ly = y->Left();

		CRBNode<T, U> *px = node.Parent();
		if( px )
		{
			if( px->Left() == &node )
			{
				px->SetLeft( y );
			}
			else
			{
				px->SetRight( y );
			}
		}
		else
		{
			m_root = y;
		}
		y->SetParent( px );
		y->SetLeft( &node );

		node.SetParent( y );
		node.SetRight( ly );

		if( ly )
		{
			ly->SetParent( &node );
		}
	}

	/* 
	*            py                               py
	*           /                                /
	*          y                                x                  
	*         /  \      --(右旋)-->            /  \                     
	*        x   ry                           lx   y  
	*       / \                                   / \                   
	*      lx  rx                                rx  ry
	*/
	template<typename T, typename U>
	void CRBTree<T, U>::_RotateRight(CRBNode<T,U> &node)
	{
		// param node is y
		CRBNode<T, U> *x = node.Left();
		if( !x )
		{
			return;
		}
		CRBNode<T, U> *rx = x->Right();

		CRBNode<T, U> *py = node.Parent();
		if( py )
		{
			if( py->Left() == &node )
			{
				py->SetLeft( x );
			}
			else
			{
				py->SetRight( x );
			}
		}
		else
		{
			m_root = x;
		}
		x->SetParent( py );
		x->SetRight( &node );

		node.SetParent( x );
		node.SetLeft( rx );

		if( rx )
		{
			rx->SetParent( &node );
		}
	}
		
	template<typename T, typename U>
	CRBNode<T, U>* CRBTree<T, U>::_FindInsertPos(T &key)
	{
		if( !m_root )
		{
			return NULL;
		}
	
		CRBNode<T, U> *p = m_root; 
		CRBNode<T, U> *gp = NULL;

		while( p )
		{
			gp = p;
			if( p->Key() == key )
			{
				break;
			}
			else if( key < p->Key() )
			{
				p = p->Left();
			}
			else
			{
				p = p->Right();
			}
		}

		return gp;
	}

	template<typename T, typename U>
	void CRBTree<T, U>::_InsertFixup(CRBNode<T, U> &node)
	{
		CRBNode<T, U> *parent, *gparent, *uncle, *pnode=&node;
		while( (parent=pnode->Parent()) && parent->IsRed() )
		{
			// 父亲节点存在，且是红色节点，则其必然有祖父节点
			gparent = parent->Parent();
			if( gparent->Left() == parent )
			{
				uncle = gparent->Right();
				if( uncle && uncle->IsRed() )
				{
					// case 1 颜色反转
					parent->SetBlack();
					uncle->SetBlack();
					gparent->SetRed();
					pnode = gparent;
				}
				else
				{
					// case 2
					if( parent->Right() == pnode )
					{
						_RotateLeft( *parent );
						CRBNode<T, U> *tmp = pnode;
						pnode = parent;
						parent = tmp;

						//Display();
					}

					// case 3
					parent->SetBlack();
					gparent->SetRed();
					_RotateRight( *gparent ); 
						
					//Display();
				}
			}
			else
			{
				uncle = gparent->Left();
				if( uncle && uncle->IsRed() )
				{
					// case 1 颜色反转
					parent->SetBlack();
					uncle->SetBlack();
					gparent->SetRed();
					pnode = gparent;
				}
				else
				{
					// case 2
					if( parent->Left() == pnode )
					{
						_RotateRight( *parent );
						CRBNode<T, U> *tmp = pnode;
						pnode = parent;
						parent = tmp;
						
						//Display();
					}

					// case 3
					parent->SetBlack();
					gparent->SetRed();
					_RotateLeft( *gparent ); 
						
					//Display();
				}
			}
		}

		m_root->SetBlack();
	}
		
	template<typename T, typename U>
	int CRBTree<T, U>::_Hight(CRBNode<T, U> *node)
	{
		if( !node )
		{
			return 0;
		}
		
		int lh = _Hight( node->Left() );
		int rh = _Hight( node->Right() );
		return ((lh > rh ? lh: rh) + 1);
	}

	template<typename T, typename U>
	CRBNode<T, U>* CRBTree<T, U>::_Search(T &key)
	{
		CRBNode<T, U> *node = m_root;
		while( node )
		{
			if( node->Key() == key )
			{
				break;
			}

			if( key < node->Key() )
			{
				node = node->Left();
			}
			else
			{
				node = node->Right();
			}
		}

		return node;
	}

	template<typename T, typename U>
	void CRBTree<T, U>::_Delete(CRBNode<T, U> &node)
	{
		char color = node.Color();
		CRBNode<T, U> *child=NULL, *parent=NULL;
		if( node.Left() != NULL && node.Right() != NULL )
		{
			// 找右子树的最左节点 succ
			CRBNode<T, U> *successor = node.Right();
			while( successor->Left() )
			{
				successor = successor->Left();
			}
	
			// 下面使用上述的 succ 节点替换 node
			// 先设置 node 的父节点的chlid指向目标
			if( node.Parent() )
			{
				if( node.Parent()->Left() == &node )
				{
					node.Parent()->SetLeft( successor );
				}
				else
				{
					node.Parent()->SetRight( successor );
				}
			}
			else
			{
				m_root = successor; 
			}

			child = successor->Right();
			parent = successor->Parent();
			color = successor->Color();

			if( parent == &node )
			{
				parent = successor;
			}
			else
			{
				if( child )
				{
					child->SetParent( parent );
				}
				parent->SetLeft( child );

				successor->SetRight( node.Right() );
				node.Right()->SetParent( successor );
			}

			successor->SetParent( node.Parent() );
			successor->SetLeft( node.Left() );
			successor->SetColor( node.Color() );
			node.Left()->SetParent( successor );
		}
		else
		{
			if( node.Left() == NULL )
			{
				child = node.Right();
			}
			else
			{
				child = node.Left();
			}
			parent = node.Parent();
			color = node.Color();
			if( child )
			{
				child->SetParent( parent );
			}
			if( parent )
			{
				if( parent->Left() == &node )
				{
					parent->SetLeft( child );
				}
				else
				{
					parent->SetRight( child );
				}
			}
			else
			{
				m_root = child;
			}
		}

		if( color == CRBNode<T, U>::E_Color_Black )
		{
			//Display();

			_DeleteFixup( child, parent );
		}

		_Release( &node );
	}

	template<typename T, typename U>
	void CRBTree<T, U>::_DeleteFixup(CRBNode<T, U> *node, CRBNode<T, U> *parent)
	{
		CRBNode<T, U> *other;

		while( (!node || node->IsBlack()) && node != m_root )
		{
			if( parent->Left() == node )
			{
				other = parent->Right();
				if( other->IsRed() )
				{
					// Case 1: x的兄弟w是红色的  
					other->SetBlack();
					parent->SetRed();
					_RotateLeft( *parent );
					other = parent->Right();
				}
				if( (!other->Left() || other->Left()->IsBlack()) &&
					(!other->Right() || other->Right()->IsBlack()) )
				{
					// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
					other->SetRed();
					node = parent;
					parent = node->Parent();
				}
				else
				{
					if( !other->Right() || other->Right()->IsBlack() )
					{
						// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
						other->Left()->SetBlack();
						other->SetRed();
						_RotateRight( *other );
						other = parent->Right();
					}
					// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
					other->SetColor( parent->Color() );
					parent->SetBlack();
					other->Right()->SetBlack();
					_RotateLeft( *parent );
					node = m_root;
					break;
				}
			}
			else
			{
				other = parent->Left();
				if( other->IsRed() )
				{
					// Case 1: x的兄弟w是红色的  
					other->SetBlack();
					parent->SetRed();
					_RotateRight( *parent );
					other = parent->Left();
				}
				if( (!other->Left() || other->Left()->IsBlack()) &&
					(!other->Right() || other->Right()->IsBlack()) )
				{
					// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
					other->SetRed();
					node = parent;
					parent = node->Parent();
				}
				else
				{
					if( !other->Left() || other->Left()->IsBlack() )
					{
						// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
						other->Right()->SetBlack();
						other->SetRed();
						_RotateLeft( *other );
						other = parent->Left();

						//Display();
					}
					// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
					other->SetColor( parent->Color() );
					parent->SetBlack();
					other->Left()->SetBlack();
					_RotateRight( *parent );
					node = m_root;
						
					//Display();
					break;
				}
			}
		}

		if( node )
		{
			node->SetBlack();
		}
	}

	template<typename T, typename U>
	void CRBTree<T, U>::Display()
	{
		printf( "---------------- trees ------------------\n" );
		if( !m_root )
		{
			return;
		}

		int high = Hight();
		CRBNode<T, U> nil;
		nil.SetBlack();

		std::deque< CRBNode<T, U>* > curNodes;
		std::deque< CRBNode<T, U>* > nextNodes;
		curNodes.push_back( m_root );
		
		int count = 0;
		while( 1 )
		{
			++count;
			nextNodes.clear();
			while( !curNodes.empty() )
			{
				CRBNode<T, U> *node = curNodes.front();
				curNodes.pop_front();

				if( node == &nil )
				{
					printf( "[nil(-)]" );
					nextNodes.push_back( &nil );
					nextNodes.push_back( &nil );
				}
				else
				{
					printf( "[%.3d(%c)]", (int)node->Key(), 
						((node->Color()==CRBNode<T, U>::E_Color_Black)?'B':'R') );
					
					if( node->Left() )
					{
						nextNodes.push_back( node->Left() );
					}
					else
					{
						nextNodes.push_back( &nil );
					}
					if( node->Right() )
					{
						nextNodes.push_back( node->Right() );
					}
					else
					{
						nextNodes.push_back( &nil );
					}
				}
			}

			printf( "\n" );
			if( count >= high )
			{
				break;
			}
			curNodes = nextNodes;
		}
			
		printf( "\n" );
	}
};

#endif
