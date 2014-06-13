#ifndef _MSG_ENEMY_PUT_BLOCK_H_
#define _MSG_ENEMY_PUT_BLOCK_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "TetrisScene.h"
#include "NetClient.h"
#include "GameManager.h"

class CMsgEnemyPutBlockNtf: public CMsgBase
{
public:
	CMsgEnemyPutBlockNtf(int blockid, int col) : m_BlockId(blockid), m_Col(col)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgEnemyPutBlockNtf" );
	}

private:
	int m_BlockId;
	int m_Col;
};

#endif