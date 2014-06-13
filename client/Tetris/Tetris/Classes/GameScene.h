#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "Define.h"
#include "cocos2d.h"

#define GAME_SCENE_COL_NUM 12
#define GAME_SCENE_ROW_NUM 21

class CBlock;
class CBlockElement
{
public:
	friend class CBlock;

	CBlockElement(s32 id);
	cocos2d::CCSprite* GetSprite(){ return m_pSprite; }
	s32 Row();
	s32 Col();
private:
	cocos2d::CCSprite *m_pSprite;
};

class CGameScene;
class CBlock
{
public:
	enum
	{
		E_SpriteCount_OneBlock = 4,
	};
	friend class CGameScene;
	CBlock(s32 id);
	s32 Id(){ return m_BlockId; }
	void AddScene(cocos2d::CCLayer *layer, s32 row, s32 col);
	void SetPosition(cocos2d::CCLayer *layer, s32 row, s32 col);
	void Down(cocos2d::CCLayer *layer);
	void Right(cocos2d::CCLayer *layer);
	void Left(cocos2d::CCLayer *layer);
	void RotateClockwise(s32 nid, std::vector<cocos2d::CCPoint> &vecPos, cocos2d::CCLayer *layer);	   // 顺时针旋转
	void RotateAntiClockwise(s32 nid, std::vector<cocos2d::CCPoint> &vecPos, cocos2d::CCLayer *layer); // 逆时针旋转
	CBlockElement* BlockAt(s32 idx);
	s32 CenterRow(){ return m_CenterRow; }
	s32 CenterCol(){ return m_CenterCol; }
	s32 Row();
	s32 Col();
	void GetRows(std::vector<s32> &rows);
	void MoveUp(s32 rowCount, cocos2d::CCLayer *layer);

	static s32 RotateClockwiseId(s32 id);
private:
	void _Move(CBlockElement *elem, cocos2d::CCPoint &endPoint);
	void _RemoveFromScene(cocos2d::CCLayer *layer);

private:
	s32 m_BlockId;
	s32 m_CenterRow;
	s32 m_CenterCol;
	CBlockElement *m_Block[E_SpriteCount_OneBlock]; 
};

class CGameScene
{
public:
	CGameScene();
	~CGameScene();

	void Init();
	bool PutBlock(CBlock *block, s32 column);
	bool TryDown(CBlock *block, cocos2d::CCLayer *layer);
	bool TryLeft(CBlock *block, cocos2d::CCLayer *layer);
	bool TryRight(CBlock *block, cocos2d::CCLayer *layer);
	s32  TryRotateClockwise(CBlock *block, cocos2d::CCLayer *layer);
	s32  TryRotateAntiClockwise(CBlock *block, cocos2d::CCLayer *layer);
	void CheckCleanUp(CBlock *block, cocos2d::CCLayer *layer);
	bool RiseBlock(int count, CBlock *block, cocos2d::CCLayer *layer);

private:
	bool _CheckRowFull(s32 row);
	void _DoClearBlock(std::vector<s32> &rows, cocos2d::CCLayer *layer);
	bool _CheckRowEmpty(s32 row);
	bool _CanDown(CBlock *block);
	void _MoveUpBlocks(s32 rowCount, cocos2d::CCLayer *layer);
	void _AddOtherBlocks(s32 rowCount, cocos2d::CCLayer *layer);

	void _DebugLogScene(s32 high);
private:
	//			  column
	//		0 1 2 3 4 5 6 7 8 9
	//		口口口口口口口口口口	0
	//		口口田口口口口口口口	1
	//		口口田口口口口口口口	2		<=== 1
	// row	口口田田口口口口口口	3
	//		口口口口口口口口口口	4
	//		口口口口口口口口口口	5
	//		口口口口口口口口口口	6
	//		口口口口口口口口口口	7
	//		口口口口口口口口口口	8
	//		口口口口口口口口口口	9
	//		口口口口口口口口口口	10
	//		口口口口口口口口口口	11
	//		口口口口口口口口口口	12
	//		口口口口口口口口口口	13
	//		口口口口囚囚囚口口口 14
	//		囚囚囚囚囚囚囚囚囚囚 15
	//		口口囚口口口囚口囚口	16		<=== 2
	//		囚囚囚囚囚囚囚囚囚囚 17
	//		囚囚囚囚囚囚囚囚囚囚 18
	//		口囚囚囚囚囚口口口口	19
	//
	//		1.   PutBlock( 1, 2 )  
	//		2.	 ClearBlock( [18,17,15], 3 )
	CBlockElement *m_Scene[GAME_SCENE_ROW_NUM][GAME_SCENE_COL_NUM];
	s32 m_MaxHigh;
	bool m_bHeadEmpty;
};

#endif
