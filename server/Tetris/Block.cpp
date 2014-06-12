#include "Block.h"
#include "StackTrace.h"
#include "GameScene.h"

INIT_SIGNLETON_CLASS(CBlockManager);

#define MAX_NUMBER_2(num1, num2)				((num1)>(num2))?(num1):(num2)
#define MAX_NUMBER_3(num1, num2, num3)			MAX_NUMBER_2(num1, MAX_NUMBER_2(num2, num3))
#define MAX_NUMBER_4(num1, num2, num3, num4)	MAX_NUMBER_2(num1, MAX_NUMBER_3(num2, num3, num4))

void CBlockManager::Init()
{
#define _REG_BLOCK(id)	_RegisterBlock( id, new CBlock ## id() );

	_REG_BLOCK(1);
	_REG_BLOCK(2);
	_REG_BLOCK(3);
	_REG_BLOCK(4);
	_REG_BLOCK(5);
	_REG_BLOCK(6);
	_REG_BLOCK(7);
	_REG_BLOCK(8);
	_REG_BLOCK(9);
	_REG_BLOCK(10);
	_REG_BLOCK(11);
	_REG_BLOCK(12);
	_REG_BLOCK(13);
	_REG_BLOCK(14);
	_REG_BLOCK(15);
	_REG_BLOCK(16);
	_REG_BLOCK(17);
	_REG_BLOCK(18);
	_REG_BLOCK(19);
	_REG_BLOCK(20);
	_REG_BLOCK(21);
	_REG_BLOCK(22);
	_REG_BLOCK(23);
	_REG_BLOCK(24);
	_REG_BLOCK(25);
	_REG_BLOCK(26);
	_REG_BLOCK(27);
	_REG_BLOCK(28);

#undef _REG_BLOCK
}

CBlockBase* CBlockManager::GetBlock(s32 id)
{
	if( id > 0 && id <= CBlockBase::E_Block_Count )
	{
		return m_Blocks[id-1];
	}

	return NULL;
}

void CBlockManager::_RegisterBlock(s32 id, CBlockBase *block)
{
	SG_ASSERT( id > 0 && id <= CBlockBase::E_Block_Count && block!=NULL );
	m_Blocks[ id-1 ] = block;
}

//////////////////////////////////////////////////////////////////////////////


bool CBlock1::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 2 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 3) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 1) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_2( high[putcol], high[putcol+1] );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-1)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-2)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - (row_from-2);
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - row_from;

	cur_max_high = MAX_NUMBER_3( high[putcol], high[putcol + 1], cur_max_high );

	return true;
}

bool CBlock2::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 3 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+2] + 2) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_3( high[putcol], high[putcol+1], high[putcol+2] );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 2) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 2) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 2] = CGameScene::GAME_SCENE_ROW_NUM - (row_from-1);

	cur_max_high = MAX_NUMBER_4( high[putcol], high[putcol + 1], high[putcol + 2], cur_max_high );

	return true;
}

bool CBlock3::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 2 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 3) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_2( high[putcol]+2, high[putcol+1] );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+2)*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - row_from;

	cur_max_high = MAX_NUMBER_3( high[putcol], high[putcol + 1], cur_max_high );

	return true;
}

bool CBlock4::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 3 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 2) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+2] + 1) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_3( high[putcol], high[putcol+1]+1, high[putcol+2]+1 );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 2) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+1)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 2] = CGameScene::GAME_SCENE_ROW_NUM - row_from;

	cur_max_high = MAX_NUMBER_4( high[putcol], high[putcol + 1], high[putcol + 2], cur_max_high );

	return true;
}

bool CBlock5::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 2 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 3) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_2( high[putcol], high[putcol+1] );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-2)*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - (row_from-2);

	cur_max_high = MAX_NUMBER_3( high[putcol], high[putcol + 1], cur_max_high );

	return true;
}

bool CBlock6::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 3 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+2] + 2) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_3( high[putcol]+1, high[putcol+1]+1, high[putcol+2] );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 2) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 2) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 2] = CGameScene::GAME_SCENE_ROW_NUM - row_from;

	cur_max_high = MAX_NUMBER_4( high[putcol], high[putcol + 1], high[putcol + 2], cur_max_high );

	return true;
}

bool CBlock7::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 2 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 3) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 1) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_2( high[putcol], high[putcol+1]+2 );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+1)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+2)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - row_from;

	cur_max_high = MAX_NUMBER_3( high[putcol], high[putcol + 1], cur_max_high );

	return true;
}

bool CBlock8::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 3 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 2) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+2] + 1) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_3( high[putcol], high[putcol+1], high[putcol+2] );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 2) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-1)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - (row_from-1);
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 2] = CGameScene::GAME_SCENE_ROW_NUM - row_from;

	cur_max_high = MAX_NUMBER_4( high[putcol], high[putcol + 1], high[putcol + 2], cur_max_high );

	return true;
}

bool CBlock9::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 3 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 2) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+2] + 1) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_3( high[putcol], high[putcol+1], high[putcol+2] );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 2) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - (row_from-1);
	high[putcol + 2] = CGameScene::GAME_SCENE_ROW_NUM - row_from;

	cur_max_high = MAX_NUMBER_2( high[putcol + 1], cur_max_high );

	return true;
}

bool CBlock10::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 2 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 3) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_2( high[putcol]+1, high[putcol+1] );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - (row_from-1);

	cur_max_high = MAX_NUMBER_2( high[putcol + 1], cur_max_high );

	return true;
}

bool CBlock11::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 3 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 2) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+2] + 1) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_3( high[putcol]+1, high[putcol+1], high[putcol+2]+1 );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 2) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 2] = CGameScene::GAME_SCENE_ROW_NUM - row_from;

	cur_max_high = MAX_NUMBER_2( high[putcol + 1], cur_max_high );

	return true;
}

bool CBlock12::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 2 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 3) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 1) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_2( high[putcol], high[putcol+1]+1 );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-1)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+1)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - (row_from-1);
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - row_from;

	cur_max_high = MAX_NUMBER_2( high[putcol], cur_max_high );

	return true;
}

static bool _PutBlock13to16(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 2 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 2) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 2) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_2( high[putcol], high[putcol+1] );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-1)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - (row_from-1);
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - (row_from-1);

	cur_max_high = MAX_NUMBER_2( high[putcol], cur_max_high );

	return true;
}

bool CBlock13::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock13to16( scene, col, row, high, putcol, cur_max_high );
}

bool CBlock14::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock13to16( scene, col, row, high, putcol, cur_max_high );
}

bool CBlock15::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock13to16( scene, col, row, high, putcol, cur_max_high );
}

bool CBlock16::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock13to16( scene, col, row, high, putcol, cur_max_high );
}


static bool _PutBlock17and19(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 4 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+2] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+3] + 1) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_4( high[putcol], high[putcol+1], high[putcol+2], high[putcol+3] );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 2) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 3) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 2] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 3] = CGameScene::GAME_SCENE_ROW_NUM - row_from;

	cur_max_high = MAX_NUMBER_2( high[putcol], cur_max_high );

	return true;
}

static bool _PutBlock18and20(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 1 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 4) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = high[putcol];
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-1)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-2)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-3)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - (row_from-3);

	cur_max_high = MAX_NUMBER_2( high[putcol], cur_max_high );

	return true;
}

bool CBlock17::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock17and19( scene, col, row, high, putcol, cur_max_high );
}

bool CBlock18::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock18and20( scene, col, row, high, putcol, cur_max_high );
}

bool CBlock19::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock17and19( scene, col, row, high, putcol, cur_max_high );
}

bool CBlock20::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock18and20( scene, col, row, high, putcol, cur_max_high );
}

static bool _PutBlock21and23(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 3 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 2) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+2] + 1) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_3( high[putcol]+1, high[putcol+1], high[putcol+2] );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 2) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 2] = CGameScene::GAME_SCENE_ROW_NUM - (row_from+1);

	cur_max_high = MAX_NUMBER_2( high[putcol], cur_max_high );

	return true;
}

static bool _PutBlock22and24(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 2 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 2) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 2) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_2( high[putcol], high[putcol+1]+1 );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+1)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - (row_from-1);

	cur_max_high = MAX_NUMBER_2( high[putcol + 1], cur_max_high );

	return true;
}

bool CBlock21::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock21and23( scene, col, row, high, putcol, cur_max_high );
}

bool CBlock22::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock22and24( scene, col, row, high, putcol, cur_max_high );
}

bool CBlock23::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock21and23( scene, col, row, high, putcol, cur_max_high );
}

bool CBlock24::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock22and24( scene, col, row, high, putcol, cur_max_high );
}

static bool _PutBlock25and27(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 3 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 1) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 2) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+2] + 1) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_3( high[putcol], high[putcol+1], high[putcol+2]+1 );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 2) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+1)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - (row_from+1);
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - row_from;
	high[putcol + 2] = CGameScene::GAME_SCENE_ROW_NUM - row_from;

	cur_max_high = MAX_NUMBER_2( high[putcol + 1], cur_max_high );

	return true;
}

static bool _PutBlock26and28(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	if( putcol > CGameScene::GAME_SCENE_COL_NUM - 2 ||
		putcol < 0 )
	{
		return false;
	}

	if( (high[putcol] + 2) > CGameScene::GAME_SCENE_ROW_NUM ||
		(high[putcol+1] + 2) > CGameScene::GAME_SCENE_ROW_NUM )
	{
		return false;
	}

	s32 max_high = MAX_NUMBER_2( high[putcol]+1, high[putcol+1] );
	s32 row_from = CGameScene::GAME_SCENE_ROW_NUM - max_high - 1;

	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + row_from*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from-1)*CGameScene::GAME_SCENE_COL_NUM + putcol) = CGameScene::SCENE_POSITION_NOT_EMPTY;
	*(scene + (row_from+1)*CGameScene::GAME_SCENE_COL_NUM + putcol + 1) = CGameScene::SCENE_POSITION_NOT_EMPTY;

	high[putcol] = CGameScene::GAME_SCENE_ROW_NUM - (row_from-1);
	high[putcol + 1] = CGameScene::GAME_SCENE_ROW_NUM - row_from;

	cur_max_high = MAX_NUMBER_2( high[putcol], cur_max_high );

	return true;
}

bool CBlock25::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock25and27( scene, col, row, high, putcol, cur_max_high );
}

bool CBlock26::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock26and28( scene, col, row, high, putcol, cur_max_high );
}

bool CBlock27::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock25and27( scene, col, row, high, putcol, cur_max_high );
}

bool CBlock28::PutBlock(s8 *scene, s32 col, s32 row, s8 *high, s32 putcol, s32 &cur_max_high)
{
	return _PutBlock26and28( scene, col, row, high, putcol, cur_max_high);
}

