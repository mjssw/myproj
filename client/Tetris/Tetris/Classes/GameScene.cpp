#include "GameScene.h"
#include "Block.h"
#include "NetManager.h"
using namespace cocos2d;
#include <vector>
#include <algorithm>
using namespace std;

CCPoint _BuildPoint(s32 row, s32 col)
{
	return CCPoint( col*BLOCK_W+BATTLE_AREA_X, (GAME_SCENE_ROW_NUM-1-row)*BLOCK_H+BATTLE_AREA_Y);
}

CBlockElement::CBlockElement(s32 id)
{
	CCRect rc;
	if( id <= 0 )
	{
		// 上涨的方块
		rc.setRect( 0, BLOCK_H, BLOCK_W, BLOCK_H );
	}
	else
	{
		// 普通方块
		rc.setRect( ((id-1)/CBlockBase::E_VeryCount_OneBlock)*BLOCK_W, 0, BLOCK_W, BLOCK_H );
	}
	m_pSprite = CCSprite::create( PATCH_RES_DIR("tetris_brick.png"), rc );
	m_pSprite->setAnchorPoint( ccp(0, 0) );
}

s32 CBlockElement::Row()
{
	return (GAME_SCENE_ROW_NUM - ((m_pSprite->getPositionY() - BATTLE_AREA_Y) / BLOCK_H) - 1);
}

s32 CBlockElement::Col()
{
	return ((m_pSprite->getPositionX() - BATTLE_AREA_X) / BLOCK_W);
}

CBlock::CBlock(s32 id) : m_BlockId(id)
{
	for( int i=0; i<E_SpriteCount_OneBlock; ++i )
	{
		m_Block[i] = new CBlockElement( id );
	}
}

void CBlock::AddScene(cocos2d::CCLayer *layer, s32 row, s32 col)
{
	SetPosition( layer, row, col );
	for( int i=0; i<E_SpriteCount_OneBlock; ++i )
	{
		layer->addChild( m_Block[i]->GetSprite() );
	}
}

void CBlock::SetPosition(cocos2d::CCLayer *layer, s32 row, s32 col)
{
	CBlockBase *block = CBlockManager::Instance().GetBlock( m_BlockId );
	if( !block )
	{
		return;
	}

	block->SetPosition( m_Block, row, col, m_CenterRow, m_CenterCol );
}

void CBlock::Down(cocos2d::CCLayer *layer)
{
	for( int i=0; i<E_SpriteCount_OneBlock; ++i )
	{
		CCPoint pt = m_Block[i]->GetSprite()->getPosition();
		pt.y -= BLOCK_H;
		_Move( m_Block[i], pt );
	}
	m_CenterRow += 1;
}

void CBlock::Right(cocos2d::CCLayer *layer)
{
	for( int i=0; i<E_SpriteCount_OneBlock; ++i )
	{
		CCPoint pt = m_Block[i]->GetSprite()->getPosition();
		pt.x += BLOCK_W;
		_Move( m_Block[i], pt );
	}
	m_CenterCol += 1;
}

void CBlock::Left(cocos2d::CCLayer *layer)
{
	for( int i=0; i<E_SpriteCount_OneBlock; ++i )
	{
		CCPoint pt = m_Block[i]->GetSprite()->getPosition();
		pt.x -= BLOCK_W;
		_Move( m_Block[i], pt );
	}
	m_CenterCol -= 1;
}

void CBlock::RotateClockwise(s32 nid, vector<CCPoint> &vecPos, cocos2d::CCLayer *layer)
{
	if( vecPos.size() != E_SpriteCount_OneBlock )
	{
		CCLog( "[ERROR] RotateClockwise" );
		return;
	}

	m_BlockId = nid;

	for( int i=0; i<E_SpriteCount_OneBlock; ++i )
	{
		CCPoint pt = _BuildPoint( (s32)vecPos[i].x, (s32)vecPos[i].y );
		_Move( m_Block[i], pt );
	}
}

void CBlock::RotateAntiClockwise(s32 nid, vector<CCPoint> &vecPos, cocos2d::CCLayer *layer)
{
}

CBlockElement* CBlock::BlockAt(s32 idx)
{
	if( idx < 0 || idx >= CBlock::E_SpriteCount_OneBlock )
	{
		return NULL;
	}

	return m_Block[idx];
}

s32 CBlock::Row()
{
	s32 minRow = GAME_SCENE_ROW_NUM;
	for( int i=0; i<E_SpriteCount_OneBlock; ++i )
	{
		s32 row = m_Block[i]->Row();
		if( row < minRow )
		{
			minRow = row;
		}
	}
	return minRow;
}

s32 CBlock::Col()
{
	s32 minCol = GAME_SCENE_COL_NUM;
	for( int i=0; i<E_SpriteCount_OneBlock; ++i )
	{
		s32 col = m_Block[i]->Col();
		if( col < minCol )
		{
			minCol = col;
		}
	}
	return minCol;
}

void CBlock::GetRows(std::vector<s32> &rows)
{
	// 取不重复的所有行
	s32 flag[GAME_SCENE_ROW_NUM] = {0};
	memset( flag, 0, sizeof(s32) * GAME_SCENE_ROW_NUM );
	for( int i=0; i<E_SpriteCount_OneBlock; ++i )
	{
		s32 row = m_Block[i]->Row(); 
		if( flag[row] == 0 )
		{
			rows.push_back( row );
			++flag[row];
		}
	}
}

void CBlock::MoveUp(s32 rowCount, cocos2d::CCLayer *layer)
{
	for( int i=0; i<E_SpriteCount_OneBlock; ++i )
	{
		s32 row = m_Block[i]->Row();
		s32 col = m_Block[i]->Col();
		CCPoint pt = _BuildPoint( row-rowCount, col );
		_Move( m_Block[i], pt );
	}
}

void CBlock::_Move(CBlockElement *elem, cocos2d::CCPoint &endPoint)
{		
	/*
	double moveTime = 0.01;
	CCMoveTo *moveAct = CCMoveTo::create( moveTime, endPoint );

	CCFiniteTimeAction *action= CCSequence::create( moveAct, NULL );  
	elem->GetSprite()->stopAllActions(); 
	elem->GetSprite()->runAction( action );
	//*/
	if( elem )
	{
		elem->GetSprite()->setPosition( endPoint );
	}
}


s32 CBlock::RotateClockwiseId(s32 id)
{
	s32 _id = id - 1;
	if( _id == 0 ) 
	{
		return CBlockBase::E_VeryCount_OneBlock;
	}

	s32 gid = _id / CBlockBase::E_VeryCount_OneBlock;
	s32 nid = _id - 1;
	if( nid / CBlockBase::E_VeryCount_OneBlock != gid )
	{
		nid = (gid+1) * CBlockBase::E_VeryCount_OneBlock - 1;
	}
	return (nid+1);
}

///////////////////////////////////////////////////////////////////////////////

CGameScene::CGameScene()
{
}

CGameScene::~CGameScene()
{
}

void CGameScene::Init()
{
	memset( m_Scene, NULL, GAME_SCENE_COL_NUM * GAME_SCENE_ROW_NUM * sizeof(CBlockElement*) );
	m_MaxHigh = 0;
	m_bHeadEmpty = true;
}

bool CGameScene::PutBlock(CBlock *block, s32 column)
{
	for( int i=0; i<CBlock::E_SpriteCount_OneBlock; ++i )
	{
		CBlockElement *elem = block->BlockAt(i);
		if( elem )
		{
			s32 row = elem->Row();
			s32 col = elem->Col();
			s32 high = GAME_SCENE_ROW_NUM - row;
			if( high > m_MaxHigh )
			{
				m_MaxHigh = high;
			}

			if( m_Scene[ row ][ col ] )
			{
				return false;
			}
			
			m_Scene[ row ][ col ] = elem;
		}
	}

	CCLog( "put %d, maxHigh=%d", block->Id(), m_MaxHigh );
	return true;
}

bool CGameScene::TryDown(CBlock *block, cocos2d::CCLayer *layer)
{
	if( _CanDown(block) )
	{
		block->Down( layer );
		return true;
	}

	return false;
}

bool CGameScene::TryLeft(CBlock *block, cocos2d::CCLayer *layer)
{
	for( int i=0; i<CBlock::E_SpriteCount_OneBlock; ++i )
	{
		CBlockElement *elem = block->BlockAt(i);
		if( elem )
		{
			s32 row = elem->Row();
			s32 col = elem->Col();
			if( col - 1 < 0 )
			{
				return false;
			}
			if( m_Scene[ row ][ col - 1 ] != NULL )
			{
				return false;
			}
		}
	}

	block->Left( layer );

	return true;
}

bool CGameScene::TryRight(CBlock *block, cocos2d::CCLayer *layer)
{
	for( int i=0; i<CBlock::E_SpriteCount_OneBlock; ++i )
	{
		CBlockElement *elem = block->BlockAt(i);
		if( elem )
		{
			s32 row = elem->Row();
			s32 col = elem->Col();
			if( col + 1 >= GAME_SCENE_COL_NUM )
			{
				return false;
			}
			if( m_Scene[ row ][ col + 1 ] != NULL )
			{
				return false;
			}
		}
	}

	block->Right( layer );

	return true;
}

s32 CGameScene::TryRotateClockwise(CBlock *block, cocos2d::CCLayer *layer)
{
	s32 nid = CBlock::RotateClockwiseId( block->Id() );
	CBlockBase *bk = CBlockManager::Instance().GetBlock( nid );
	if( !bk )
	{
		return block->Id();
	}

	vector<CCPoint> vecPos;
	bk->GetPosition( vecPos, block->CenterRow(), block->CenterCol() );
	bool ret = true;
	vector<CCPoint>::iterator it = vecPos.begin();
	for( ; it != vecPos.end(); ++it )
	{
		s32 row = (s32)it->x;
		s32 col = (s32)it->y;
		if( row < 0 || row >= GAME_SCENE_ROW_NUM ||
			col < 0 || col >= GAME_SCENE_COL_NUM )
		{
			return block->Id();
		}

		if( m_Scene[ row ][ col ] != NULL )
		{
			return block->Id();
		}
	}

	block->RotateClockwise( nid, vecPos, layer );

	return block->Id();
}

s32 CGameScene::TryRotateAntiClockwise(CBlock *block, cocos2d::CCLayer *layer)
{
	return block->Id();
}

static bool CompairRow(s32 row1, s32 row2)
{  
    return (row1 > row2);
}

void CGameScene::CheckCleanUp(CBlock *block, cocos2d::CCLayer *layer)
{
	// 注意: rows 是一个已经排好序的数组
	if( !block || !layer )
	{
		return;
	}

	vector<s32> rows;
	block->GetRows( rows );
	sort( rows.begin(), rows.end(), CompairRow );

	vector<s32>::iterator it = rows.begin();
	for( ; it != rows.end(); )
	{
		if( !_CheckRowFull( *it ) )
		{
			it = rows.erase( it );
		}
		else
		{
			++it;
		}
	}

	_DoClearBlock( rows, layer );
	s32 oldHigh = m_MaxHigh; 
	m_MaxHigh -= rows.size();
	if( rows.size() > 0 )
	{
		CCLog( "clear, maxHigh=%d", m_MaxHigh );
		_DebugLogScene( oldHigh );
	}

	// 网络消息
	if( rows.size() > 0 )
	{
		CNetManager::Instance().ClearBlock( rows );
	}
}

bool CGameScene::_CheckRowFull(s32 row)
{
	bool ret = true;
	for( s32 col=0; col<GAME_SCENE_COL_NUM; ++col )
	{
		if( m_Scene[row][col] == NULL )
		{
			ret = false;
			break;
		}
	}

	return ret;
}

bool CGameScene::_CheckRowEmpty(s32 row)
{
	//SG_ASSERT( row >= 0 && row < GAME_SCENE_ROW_NUM );
	bool ret = true;
	for( s32 col=0; col<GAME_SCENE_COL_NUM; ++col )
	{
		if( m_Scene[row][col] != NULL )
		{
			ret = false;
			break;
		}
	}

	return ret;
}

bool CGameScene::_CanDown(CBlock *block)
{
	if( !block )
	{
		return false;
	}
	for( int i=0; i<CBlock::E_SpriteCount_OneBlock; ++i )
	{
		CBlockElement *elem = block->BlockAt(i);
		if( elem )
		{
			s32 row = elem->Row();
			s32 col = elem->Col();
			if( row + 1 >= GAME_SCENE_ROW_NUM )
			{
				return false;
			}
			if( m_Scene[ row + 1 ][ col ] != NULL )
			{
				return false;
			}
		}
	}

	return true;
}

void CGameScene::_DoClearBlock(vector<s32> &rows, cocos2d::CCLayer *layer)
{
	s32 count = rows.size();
	if( count == 0 )
	{
		return;
	}

	s32 n = count - 1;
	s32 empty_idx = rows[0];

	// 删除行
	for( s32 i=0; i<count; ++i )
	{
		for( s32 col=0; col<GAME_SCENE_COL_NUM; ++col )
		{
			layer->removeChild( m_Scene[rows[i]][col]->GetSprite() );
			m_Scene[rows[i]][col] = NULL;
		}
	}

	// 移动夹在删除行中间的内容
	for( s32 i=0; i<n; ++i )
	{
		for( s32 row=rows[i]-1; row>rows[i+1]; --row )
		{
			for( s32 col=0; col<GAME_SCENE_COL_NUM; ++col )
			{
				//layer->removeChild( m_Scene[empty_idx][col]->GetSprite() );
				if( m_Scene[row][col] )
				{
					m_Scene[row][col]->GetSprite()->setPosition( _BuildPoint(empty_idx, col) );
				}

				m_Scene[empty_idx][col] = m_Scene[row][col];
				m_Scene[row][col] = NULL;
			}

			--empty_idx; 
		}
	}

	// 移动最上层删除行以上的内容
	s32 maxhigh_idx = GAME_SCENE_ROW_NUM - m_MaxHigh;
	for( s32 row=rows[count-1]-1; row>=maxhigh_idx; --row )
	{
		for( s32 col=0; col<GAME_SCENE_COL_NUM; ++col )
		{
			if( m_Scene[row][col] )
			{
				m_Scene[row][col]->GetSprite()->setPosition( _BuildPoint(empty_idx, col) );
			}

			m_Scene[empty_idx][col] = m_Scene[row][col];
			m_Scene[row][col] = NULL;
		}

		--empty_idx; 
	}
}

bool CGameScene::RiseBlock(int count, CBlock *block, cocos2d::CCLayer *layer)
{
	if( !block || !layer )
	{
		return false;
	}

	if( !_CanDown( block ) )
	{
		s32 row = block->Row();
		if( row - count < 0 )
		{
			return false;
		}

		// 当前方块不能在往下掉了，但是涨方块,
		// 当前方块上移count行
		block->MoveUp( (s32)count, layer );
	}

	// 当前场景内所有方块统一上移count行
	_MoveUpBlocks( (s32)count, layer );

	// 增加额外方块
	_AddOtherBlocks( (s32)count, layer );

	m_MaxHigh += count;

	return true;
}

void CGameScene::_MoveUpBlocks(s32 rowCount, cocos2d::CCLayer *layer)
{
	for( s32 row=(GAME_SCENE_ROW_NUM-m_MaxHigh); row<GAME_SCENE_ROW_NUM; ++row )
	{
		s32 targetRow = row - rowCount;
		for( s32 col=0; col<GAME_SCENE_COL_NUM; ++col )
		{
			if( m_Scene[row][col] )
			{
				m_Scene[row][col]->GetSprite()->setPosition( _BuildPoint(targetRow, col) );
				m_Scene[targetRow][col] = m_Scene[row][col];
				m_Scene[row][col] = NULL;
			}
		}
	}
}

void CGameScene::_AddOtherBlocks(s32 rowCount, cocos2d::CCLayer *layer)
{
	for( s32 row=GAME_SCENE_ROW_NUM-rowCount; row<GAME_SCENE_ROW_NUM; ++row )
	{
		s32 col = 0;
		if( m_bHeadEmpty )
		{
			col += 1;
		}
		m_bHeadEmpty = !m_bHeadEmpty; 
		for( ; col<GAME_SCENE_COL_NUM; col+=2 )
		{
			CBlockElement *elem = new CBlockElement( 0 );
			if( !elem )
			{
				return;
			}

			elem->GetSprite()->setPosition( _BuildPoint(row, col) );
			m_Scene[row][col] = elem; 
			layer->addChild( elem->GetSprite() );
		}
	}
}

void CGameScene::_DebugLogScene(s32 high)
{
	s32 row = GAME_SCENE_ROW_NUM - high;
	for( ; row < GAME_SCENE_ROW_NUM; ++row )
	{
		for( s32 col=0; col<GAME_SCENE_COL_NUM; ++col )
		{
			if( m_Scene[row][col] == NULL )
			{
				OutputDebugString( L"o" );
			}
			else
			{
				OutputDebugString( L"#" );
			}
		}
		OutputDebugString( L"\n" );
	}
}
