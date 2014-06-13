#include "Block.h"
#include "cocos2d.h"

INIT_SIGNLETON_CLASS(CBlockManager);

#define MAX_NUMBER_2(num1, num2)				((num1)>(num2))?(num1):(num2)
#define MAX_NUMBER_3(num1, num2, num3)			MAX_NUMBER_2(num1, MAX_NUMBER_2(num2, num3))
#define MAX_NUMBER_4(num1, num2, num3, num4)	MAX_NUMBER_2(num1, MAX_NUMBER_3(num2, num3, num4))

extern cocos2d::CCPoint _BuildPoint(s32 row, s32 col);

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
	//SG_ASSERT( id > 0 && id <= CBlockBase::E_Block_Count && block!=NULL );
	m_Blocks[ id-1 ] = block;
}

//////////////////////////////////////////////////////////////////////////////

void CBlock1::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row+1, col ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+2, col ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+2, col+1 ) );
}
void CBlock1::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow-1, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow+1, centerCol) );
	vecPos.push_back( ccp(centerRow+1, centerCol+1) );
}

void CBlock2::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col + 1;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row+1, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row+1, col+1 ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+1, col+2 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row, col+2 ) );
}
void CBlock2::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow, centerCol-1) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol+1) );
	vecPos.push_back( ccp(centerRow-1, centerCol+1) );
}

void CBlock3::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col + 1;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row, col+1 ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+1, col+1 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+2, col+1 ) );
}
void CBlock3::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow-1, centerCol-1) );
	vecPos.push_back( ccp(centerRow-1, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow+1, centerCol) );
}

void CBlock4::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row;
	centerCol = col + 1;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row, col+1 ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row, col+2 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+1, col ) );
}
void CBlock4::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow, centerCol-1) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol+1) );
	vecPos.push_back( ccp(centerRow+1, centerCol-1) );
}

void CBlock5::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col + 1;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col+1 ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row+1, col+1 ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+2, col+1 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+2, col ) );
}
void CBlock5::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow-1, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow+1, centerCol) );
	vecPos.push_back( ccp(centerRow+1, centerCol-1) );
}

void CBlock6::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row;
	centerCol = col + 1;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row, col+1 ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row, col+2 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+1, col+2 ) );
}
void CBlock6::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow, centerCol-1) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol+1) );
	vecPos.push_back( ccp(centerRow+1, centerCol+1) );
}

void CBlock7::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row, col+1 ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+1, col ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+2, col ) );
}
void CBlock7::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow-1, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow+1, centerCol) );
	vecPos.push_back( ccp(centerRow-1, centerCol+1) );
}

void CBlock8::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col + 1;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row+1, col ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+1, col+1 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+1, col+2 ) );
}
void CBlock8::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow-1, centerCol-1) );
	vecPos.push_back( ccp(centerRow, centerCol-1) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol+1) );
}

void CBlock9::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col + 1;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col+1 ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row+1, col ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+1, col+1 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+1, col+2 ) );
}
void CBlock9::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow-1, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol-1) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol+1) );
}

void CBlock10::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col + 1;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col+1 ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row+1, col ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+1, col+1 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+2, col+1 ) );
}
void CBlock10::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow-1, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol-1) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow+1, centerCol) );
}

void CBlock11::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row;
	centerCol = col + 1;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row, col+1 ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row, col+2 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+1, col+1 ) );
}
void CBlock11::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow, centerCol-1) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol+1) );
	vecPos.push_back( ccp(centerRow+1, centerCol) );
}

void CBlock12::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row+1, col ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+2, col ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+1, col+1 ) );
}
void CBlock12::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow-1, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow+1, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol+1) );
}

static void Block13To16_SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row;
	centerCol = col;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row+1, col ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row, col+1 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+1, col+1 ) );
}
static void Block13To16_GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol+1) );
	vecPos.push_back( ccp(centerRow+1, centerCol) );
	vecPos.push_back( ccp(centerRow+1, centerCol+1) );
}
void CBlock13::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block13To16_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock13::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block13To16_GetPosition( vecPos, centerRow, centerCol );
}

void CBlock14::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block13To16_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock14::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block13To16_GetPosition( vecPos, centerRow, centerCol );
}

void CBlock15::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block13To16_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock15::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block13To16_GetPosition( vecPos, centerRow, centerCol );
}

void CBlock16::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block13To16_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock16::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block13To16_GetPosition( vecPos, centerRow, centerCol );
}

static void Block1719_SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row;
	centerCol = col + 2;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row, col+1 ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row, col+2 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row, col+3 ) );
}
static void Block1719_GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow, centerCol-2) );
	vecPos.push_back( ccp(centerRow, centerCol-1) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol+1) );
}
static void Block1820_SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row+1, col ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+2, col ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+3, col ) );
}
static void Block1820_GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow-1, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow+1, centerCol) );
	vecPos.push_back( ccp(centerRow+2, centerCol) );
}
void CBlock17::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block1719_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock17::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block1719_GetPosition( vecPos, centerRow, centerCol );
}

void CBlock18::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block1820_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock18::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block1820_GetPosition( vecPos, centerRow, centerCol );
}

void CBlock19::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block1719_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock19::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block1719_GetPosition( vecPos, centerRow, centerCol );
}

void CBlock20::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block1820_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock20::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block1820_GetPosition( vecPos, centerRow, centerCol );
}

static void Block2123_SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col + 1;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row, col+1 ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+1, col+1 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+1, col+2) );
}
static void Block2123_GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow-1, centerCol-1) );
	vecPos.push_back( ccp(centerRow-1, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol+1) );
}
static void Block2224_SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col + 1;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col+1 ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row+1, col ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+1, col+1 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+2, col) );
}
static void Block2224_GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow-1, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol-1) );
	vecPos.push_back( ccp(centerRow+1, centerCol-1) );
}
void CBlock21::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block2123_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock21::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block2123_GetPosition( vecPos, centerRow, centerCol );
}

void CBlock22::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block2224_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock22::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block2224_GetPosition( vecPos, centerRow, centerCol );
}

void CBlock23::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block2123_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock23::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block2123_GetPosition( vecPos, centerRow, centerCol );
}

void CBlock24::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block2224_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock24::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block2224_GetPosition( vecPos, centerRow, centerCol );
}

static void Block2527_SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col + 1;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col+1 ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row, col+2 ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+1, col ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+1, col+1) );
}
static void Block2527_GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow-1, centerCol) );
	vecPos.push_back( ccp(centerRow-1, centerCol+1) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow, centerCol-1) );
}
static void Block2628_SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	centerRow = row + 1;
	centerCol = col + 1;
	elem[0]->GetSprite()->setPosition( _BuildPoint( row, col ) );
	elem[1]->GetSprite()->setPosition( _BuildPoint( row+1, col ) );
	elem[2]->GetSprite()->setPosition( _BuildPoint( row+1, col+1 ) );
	elem[3]->GetSprite()->setPosition( _BuildPoint( row+2, col+1) );
}
static void Block2628_GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	vecPos.push_back( ccp(centerRow-1, centerCol-1) );
	vecPos.push_back( ccp(centerRow, centerCol-1) );
	vecPos.push_back( ccp(centerRow, centerCol) );
	vecPos.push_back( ccp(centerRow+1, centerCol) );
}
void CBlock25::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block2527_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock25::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block2527_GetPosition( vecPos, centerRow, centerCol );
}

void CBlock26::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block2628_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock26::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block2628_GetPosition( vecPos, centerRow, centerCol );
}

void CBlock27::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block2527_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock27::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block2527_GetPosition( vecPos, centerRow, centerCol );
}

void CBlock28::SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol)
{
	Block2628_SetPosition( elem, row, col, centerRow, centerCol );
}
void CBlock28::GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol)
{
	Block2628_GetPosition( vecPos, centerRow, centerCol );
}
