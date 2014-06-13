#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "utils.h"
#include "Define.h"
#include "GameScene.h"
#include "cocos2d.h"


class CBlockBase
{
public:
	enum
	{
		E_VeryCount_OneBlock = 4,	// 1个方块的变化数
		E_Block_Count = 28,			// 所有形状的方块个数
	};

	CBlockBase(s32 id) : m_Id(id)
	{}
	s32 Id(){ return m_Id; }
	virtual ~CBlockBase(){}

	// putrow, putcol 方块左上角的行列
	virtual void SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol){}
	virtual void GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol){}
protected:
	s32 m_Id;
};

class CBlockManager
{
	DEFINE_SINGLETON_CLASS(CBlockManager);

public:
	void Init();
	CBlockBase* GetBlock(s32 id);

private:
	void _RegisterBlock(s32 id, CBlockBase *block);

private:
	CBlockBase *m_Blocks[CBlockBase::E_Block_Count]; 
};


#define _DEFINE_BLOCK_BY_ID(id)	\
class CBlock ## id : public CBlockBase\
{\
public:\
	CBlock ## id() : CBlockBase(id){}\
	virtual void SetPosition(CBlockElement *elem[], s32 row, s32 col, s32 &centerRow, s32 &centerCol);\
	virtual void GetPosition(std::vector<cocos2d::CCPoint> &vecPos, s32 centerRow, s32 centerCol);\
};

/////////////////////////////////////////////////////////////////////


// Block1~4   
//     口		    口		口口		口口口
//     口		口口口		  口		口
//     口口					  口
_DEFINE_BLOCK_BY_ID(1);
_DEFINE_BLOCK_BY_ID(2);
_DEFINE_BLOCK_BY_ID(3);
_DEFINE_BLOCK_BY_ID(4);

// Block5~8
//		  口					  口口		口		    		
//		  口		口口口		  口			口口口
//		口口			口		  口
_DEFINE_BLOCK_BY_ID(5);
_DEFINE_BLOCK_BY_ID(6);
_DEFINE_BLOCK_BY_ID(7);
_DEFINE_BLOCK_BY_ID(8);

// Block9~12
//		  口			  口			口口口		口
//		口口口		口口			  口			口口
//					  口						口
_DEFINE_BLOCK_BY_ID(9);
_DEFINE_BLOCK_BY_ID(10);
_DEFINE_BLOCK_BY_ID(11);
_DEFINE_BLOCK_BY_ID(12);

// Block13~16
//		口口
//		口口			...
//
_DEFINE_BLOCK_BY_ID(13);
_DEFINE_BLOCK_BY_ID(14);
_DEFINE_BLOCK_BY_ID(15);
_DEFINE_BLOCK_BY_ID(16);

// Block17~20
//					口
//		口口口口		口		...
//					口
//					口
_DEFINE_BLOCK_BY_ID(17);
_DEFINE_BLOCK_BY_ID(18);
_DEFINE_BLOCK_BY_ID(19);
_DEFINE_BLOCK_BY_ID(20);

// Block21~24
//		口口			  口
//		  口口		口口		...
//					口
_DEFINE_BLOCK_BY_ID(21);
_DEFINE_BLOCK_BY_ID(22);
_DEFINE_BLOCK_BY_ID(23);
_DEFINE_BLOCK_BY_ID(24);

// Block25~28
//		  口口		口
//		口口			口口		...
//					  口
_DEFINE_BLOCK_BY_ID(25);
_DEFINE_BLOCK_BY_ID(26);
_DEFINE_BLOCK_BY_ID(27);
_DEFINE_BLOCK_BY_ID(28);

#undef _DEFINE_BLOCK_BY_ID

#endif
