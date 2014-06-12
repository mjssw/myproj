#include "GameScene.h"
#include "Block.h"
#include "ServerManager.h"

CGameScene::CGameScene()
{
}

CGameScene::~CGameScene()
{
}

void CGameScene::Init()
{
	memset( m_Scene, SCENE_POSITION_EMPTY, GAME_SCENE_COL_NUM * GAME_SCENE_ROW_NUM );
	memset( m_ColumnHigh, 0, GAME_SCENE_COL_NUM );
	m_MaxHigh = 0;
}

bool CGameScene::PutBlock(s32 blockid, s32 column)
{
	CBlockBase *block = CBlockManager::Instance().GetBlock( blockid );
	if( block != NULL )
	{
		SG_ASSERT( blockid == block->Id() );
		return block->PutBlock( 
			m_Scene[0], 
			GAME_SCENE_COL_NUM,
			GAME_SCENE_ROW_NUM,
			m_ColumnHigh,
			column,
			m_MaxHigh );
	}
	else
	{
		SERVER_LOG_ERROR( "CGameScene::PutBlock failed. not found blockid:" << blockid );
		return false;
	}
}

void CGameScene::ClearBlock(const s32 *rows, s32 count)
{
	// 注意: rows 是一个已经排好序的数组
	_DoClearBlock( rows, count );
	_AdjustColumnHigh( count );
}

bool CGameScene::_CheckRowFull(s32 row)
{
	SG_ASSERT( row >= 0 && row < GAME_SCENE_ROW_NUM );
	bool ret = true;
	for( s32 col=0; col<GAME_SCENE_ROW_NUM; ++col )
	{
		if( m_Scene[col][row] == SCENE_POSITION_EMPTY )
		{
			ret = false;
			break;
		}
	}

	return ret;
}

bool CGameScene::_CheckRowEmpty(s32 row)
{
	SG_ASSERT( row >= 0 && row < GAME_SCENE_ROW_NUM );
	bool ret = true;
	for( s32 col=0; col<GAME_SCENE_COL_NUM; ++col )
	{
		if( m_Scene[row][col] == SCENE_POSITION_NOT_EMPTY )
		{
			ret = false;
			break;
		}
	}

	return ret;
}

void CGameScene::_DoClearBlock(const s32 *rows, s32 count)
{
	s32 n = count - 1;
	s32 empty_idx = rows[0];
	s32 row_size = GAME_SCENE_COL_NUM * sizeof(s8); 

	// 移动夹在删除行中间的内容
	for( s32 i=0; i<n; ++i )
	{
		for( s32 row=rows[i]-1; row>rows[i+1]; --row )
		{
			memcpy( m_Scene[empty_idx], m_Scene[row], row_size );
			--empty_idx; 
		}
	}

	// 移动最上层删除行以上的内容
	s32 maxhigh_idx = GAME_SCENE_ROW_NUM - m_MaxHigh;
	for( s32 row=rows[count-1]-1; row>=maxhigh_idx; --row )
	{
		memcpy( m_Scene[empty_idx], m_Scene[row], row_size );
		--empty_idx; 
	}

	// 清除旧数据
	for( s32 row=GAME_SCENE_ROW_NUM-m_MaxHigh-count; row<=maxhigh_idx; ++row )
	{
		memset( m_Scene[row], SCENE_POSITION_EMPTY, row_size );
	}
}

void CGameScene::_AdjustColumnHigh(s32 count)
{
	m_MaxHigh -= count;
	for( s32 col=0; col<GAME_SCENE_COL_NUM; ++col )
	{
		m_ColumnHigh[col] -= count;
		for( s32 row=GAME_SCENE_ROW_NUM-m_ColumnHigh[col]; row<GAME_SCENE_ROW_NUM; ++row )
		{
			if( m_Scene[row][col] == SCENE_POSITION_EMPTY )
			{
				--m_ColumnHigh[col];
			}
		}
	}
}

void CGameScene::DebugDisplayScene()
{
	for( s32 row=0; row<GAME_SCENE_ROW_NUM; ++row )
	{
		if( _CheckRowEmpty( row ) )
		{
			continue;
		}

		for( s32 col=0; col<GAME_SCENE_COL_NUM; ++col )
		{
			if( m_Scene[row][col] == SCENE_POSITION_EMPTY )
			{
				SGDEBUG(" ");
			}
			else
			{
				SGDEBUG("#");
			}
		}
		SGDEBUG("\n");
	}
	SGDEBUG("\n\n");
}
