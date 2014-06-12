#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "Define.h"

class CGameScene
{
public:
	enum
	{
		GAME_SCENE_COL_NUM = 12,
		GAME_SCENE_ROW_NUM = 21,

		SCENE_POSITION_EMPTY = 0,
		SCENE_POSITION_NOT_EMPTY = 1,
	};

	CGameScene();
	~CGameScene();

	void Init();
	bool PutBlock(s32 blockid, s32 column);
	void ClearBlock(const s32 *rows, s32 count);

	void DebugDisplayScene();

private:
	bool _CheckRowFull(s32 row);
	void _DoClearBlock(const s32 *rows, s32 count);
	void _AdjustColumnHigh(s32 count);
	bool _CheckRowEmpty(s32 row);

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
	s8 m_Scene[GAME_SCENE_ROW_NUM][GAME_SCENE_COL_NUM];
	s8 m_ColumnHigh[GAME_SCENE_COL_NUM];
	s32 m_MaxHigh;
};

#endif
