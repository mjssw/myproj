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
	//		�ڿڿڿڿڿڿڿڿڿ�	0
	//		�ڿ���ڿڿڿڿڿڿ�	1
	//		�ڿ���ڿڿڿڿڿڿ�	2		<=== 1
	// row	�ڿ�����ڿڿڿڿڿ�	3
	//		�ڿڿڿڿڿڿڿڿڿ�	4
	//		�ڿڿڿڿڿڿڿڿڿ�	5
	//		�ڿڿڿڿڿڿڿڿڿ�	6
	//		�ڿڿڿڿڿڿڿڿڿ�	7
	//		�ڿڿڿڿڿڿڿڿڿ�	8
	//		�ڿڿڿڿڿڿڿڿڿ�	9
	//		�ڿڿڿڿڿڿڿڿڿ�	10
	//		�ڿڿڿڿڿڿڿڿڿ�	11
	//		�ڿڿڿڿڿڿڿڿڿ�	12
	//		�ڿڿڿڿڿڿڿڿڿ�	13
	//		�ڿڿڿ��������ڿڿ� 14
	//		�������������������� 15
	//		�ڿ����ڿڿ���������	16		<=== 2
	//		�������������������� 17
	//		�������������������� 18
	//		�������������ڿڿڿ�	19
	//
	//		1.   PutBlock( 1, 2 )  
	//		2.	 ClearBlock( [18,17,15], 3 )
	s8 m_Scene[GAME_SCENE_ROW_NUM][GAME_SCENE_COL_NUM];
	s8 m_ColumnHigh[GAME_SCENE_COL_NUM];
	s32 m_MaxHigh;
};

#endif
